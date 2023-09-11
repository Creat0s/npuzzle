#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 360
#define SCREEN_HEIGHT 640

#define RADIUS 5
#define MAX_DIFFICULTY 5

#if defined (__ANDROID__)
    #define GET_POSITION GetTouchPosition(0)
    #define PRESSED (GetTouchPointCount() > 0) && (!check_position(position))
#else
    #define GET_POSITION GetMousePosition()
    #define PRESSED IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
#endif

int i, j;               // Counter
char buffer[3];
bool is_mute = false;
Vector2 position;

typedef struct
{
    Vector2 pos;
    char item[3];
}   cell;

bool check_position(Vector2 position)
{
    Vector2 pos = GetTouchPosition(0);
    
    if((pos.x-RADIUS < position.x) && (pos.y-RADIUS < position.y) &&
       (pos.x+RADIUS > position.x) && (pos.y+RADIUS > position.y)) return true;
       
    else return false;
}

// Подсчет инверсий в пятнашках
int count_inversions(int total_tiles, int vals[])
{
    int inversions = 0;

    for(int i = 0; i < total_tiles-1; i++)
    {
        for(int j = i+1; j < total_tiles; j++)
        {
            if(vals[i] && vals[j] && vals[i] > vals[j]) inversions++;
        }
    }

    return inversions;
}

// Проверки, является ли пятнашка решаемой
bool is_solvable(int vals[], int n, int m)
{
    int total_tiles, inversions;
    
    total_tiles = n*m;
    inversions = count_inversions(total_tiles, vals);

    if(n%2 == 1) return inversions % 2 == 0;
    
    else
    {
        int emptyRow = 0;
        
        for (int i = 0; i < total_tiles; i++)
        {
            if (vals[i] == 0) {emptyRow = i/m; break;}
        }
        
        return (emptyRow%2 == 0) ? (inversions%2 == 1) : (inversions%2 == 0);
    }
}

void draw_logo(Texture2D logo)
{    
    DrawTexture(logo, 80, 120, WHITE);
    return;
}

void npuzzle(int n, Texture2D texture, Sound click_sound)
{
    bool wSd = true;
    bool win = false;
    cell puzzle[n*n];
    int tmp, vals[n*n];
    int posx, posy, defxy;
    
    defxy = ((9.0-n)/2) * 40;
    posx = defxy; posy = defxy + 40;
    
    Sound move_sound = LoadSound("assets/move.ogg");
    Sound win_sound = LoadSound("assets/win.ogg");
    
    // Заполняем список числами
    do
    {
        for(i = 0; i < n*n; i++)
        {
            do
            {
                tmp = rand()%(n*n);
                vals[i] = tmp;
                
                for(j = 0; j < i; j++)
                {
                    if(vals[j] == tmp) break;
                }
            }
            while(j < i);
        }
    }
    while(!is_solvable(vals, n, n));
    
    //Заполняем игровое поле
    for(i = 1; i <= n*n; i++)
    {
        sprintf(puzzle[i-1].item, "%d", vals[i-1]);
        
        puzzle[i-1].pos.x = posx;
        puzzle[i-1].pos.y = posy;
        
        if(i%n == 0)
        {
            posx = defxy;
            posy += 40;
        }
        else posx+=40;
    }
    
    while(true)
    {      
        if(atoi(puzzle[n*n-1].item) == 0)
        {
            for(i = 1; i < n*n-1; i++)
            {
                if(atoi(puzzle[i].item) < atoi(puzzle[i-1].item)) {win = false; break;}
                win = true;
            }
        }
        
        if(PRESSED)
        {
            position = GET_POSITION;
            
            // Sound is_mute & BACK to main
            if(position.x >= 290 && position.y <= 40) is_mute = !is_mute;
            if((position.y >= 555 && position.y <= 595) && (position.x >= 85 && position.x <= 275)) {if(!is_mute) {PlaySound(click_sound);} break;}
            
            if(win != true)
            {
                for(i = 0; i < n*n; i++)
                {
                    if((position.x > puzzle[i].pos.x) &&
                       (position.x < puzzle[i].pos.x+40) &&
                       (position.y > puzzle[i].pos.y) &&
                       (position.y < puzzle[i].pos.y+40))
                                {j = i; break;}
                                
                    else j = -1;
                }
                
                if(j != -1)
                {
                    if(((j+1)%n != 0) && (atoi(puzzle[j+1].item) == 0))
                    {
                        if(!is_mute) PlaySound(move_sound);
                        tmp = atoi(puzzle[j].item);
                        sprintf(puzzle[j+1].item, "%d", tmp);
                        sprintf(puzzle[j].item, "%d", 0);
                    }
                    
                    else if((j%n != 0) && (atoi(puzzle[j-1].item) == 0))
                    {
                        if(!is_mute) PlaySound(move_sound);
                        tmp = atoi(puzzle[j].item);                    
                        sprintf(puzzle[j-1].item, "%d", tmp);
                        sprintf(puzzle[j].item, "%d", 0);
                    }
                    
                    else if(((j+n) < n*n) && (atoi(puzzle[j+n].item) == 0))
                    {
                        if(!is_mute) PlaySound(move_sound);
                        tmp = atoi(puzzle[j].item);
                        sprintf(puzzle[j+n].item, "%d", tmp);
                        sprintf(puzzle[j].item, "%d", 0);
                    }
                    
                    else if(((j-n) > -1) && (atoi(puzzle[j-n].item) == 0))
                    {
                        if(!is_mute) PlaySound(move_sound);
                        tmp = atoi(puzzle[j].item);
                        sprintf(puzzle[j-n].item, "%d", tmp);
                        sprintf(puzzle[j].item, "%d", 0);
                    }
                }
            }
        }
        
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            
            for(i = 0; i < n*n; i++)
            {
                DrawTexture(texture, puzzle[i].pos.x, puzzle[i].pos.y, WHITE);
                if(atoi(puzzle[i].item) != 0) DrawText(puzzle[i].item, puzzle[i].pos.x+10, puzzle[i].pos.y+10, 20, BLUE);
            }
            
            DrawText("BACK", 150, 570, 20, DARKGRAY);
            
            if(is_mute) DrawText("sound", 290, 10, 20, RED);
            else DrawText("sound", 290, 10, 20, BLUE);
            
            if(win == true)
            {
                if(wSd) {if(!is_mute) {PlaySound(win_sound);} wSd = false;}
                DrawText("You win!", 136, 510, 20, DARKPURPLE);
            }
            
        EndDrawing();
    }
    
    UnloadSound(win_sound);
    UnloadSound(move_sound);
    
    return;
}

int main(void)
{
    int difficulty = 3;
    Color diffBClr[MAX_DIFFICULTY];
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib:npuzzle");
    InitAudioDevice();
    SetTargetFPS(60);
    
    const Texture2D logo = LoadTexture("assets/logo.png");
    const Texture2D cell = LoadTexture("assets/cell.png");
    
    Sound click_sound = LoadSound("assets/click.ogg");
    
    position.x = -1; position.y = -1;
    
    while (!WindowShouldClose())
    {        
        if(PRESSED)
        {
            position = GET_POSITION;
            
            //Change difficulty
            if(position.y >= 430 && position.y <= 470)
            {
                if(position.x >= 85 && position.x <= 115) {difficulty = 3; if(!is_mute) PlaySound(click_sound);}
                else if(position.x >= 125 && position.x <= 155) {difficulty = 4; if(!is_mute) PlaySound(click_sound);}
                else if(position.x >= 165 && position.x <= 195) {difficulty = 5; if(!is_mute) PlaySound(click_sound);}
                else if(position.x >= 205 && position.x <= 235) {difficulty = 6; if(!is_mute) PlaySound(click_sound);}
                else if(position.x >= 245 && position.x <= 275) {difficulty = 7; if(!is_mute) PlaySound(click_sound);}
            }
            
            //Start Game
            else if((position.y >= 495 && position.y <= 545) && (position.x >= 85 && position.x <= 275))
            {
                if(!is_mute) PlaySound(click_sound);
                npuzzle(difficulty, cell, click_sound);
            }
            
            //Sound is_mute
            else if(position.x >= 290 && position.y <= 40) is_mute = !is_mute;
        }
        
        for(i = 0; i < 5; i++) diffBClr[i] = BLUE;
        diffBClr[difficulty-3] = GREEN;
        
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            
            draw_logo(logo);
            
            DrawText("Select difficulty", 110, 400, 18, DARKGRAY);
            DrawText("Touch to start", 101, 510, 20, DARKGRAY);
            
            if(is_mute) DrawText("sound", 290, 10, 20, RED);
            else DrawText("sound", 290, 10, 20, BLUE);
            
            for(i = 0; i < MAX_DIFFICULTY; i++)
            {
                //itoa(i+3, buffer, 10);
                sprintf(buffer, "%d", i+3);
                DrawText(buffer, 90+i*40, 435, 30, diffBClr[i]);
            }
            
            //drawScheme();
            
        EndDrawing();
    }
    
    UnloadSound(click_sound);
    
    UnloadTexture(cell);
    UnloadTexture(logo);
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}