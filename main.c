#include "raylib.h"
#include "npuzzle.h"
#include <stdio.h>

//#include "devel.h"

#define SCREEN_WIDTH 360
#define SCREEN_HEIGHT 640

#define TOUCH_RADIUS 5
#define MAX_DIFFICULTY 5

#if defined (__ANDROID__)
    #define SCREEN_DOWN (GetTouchPointCount() > 0) && (!check_touch_position(current_position))
#else
    #define SCREEN_DOWN IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
#endif

int i, j;
char buffer[3];
bool is_mute = false;
Vector2 current_position;

typedef struct
{
    char item[3];
    Vector2 position;
}cell;

bool check_touch_position(Vector2 current_position)
{
    Vector2 touch_position = GetTouchPosition(0);
    
    if
    (
        (touch_position.x-TOUCH_RADIUS < current_position.x) &&
        (touch_position.y-TOUCH_RADIUS < current_position.y) &&
        (touch_position.x+TOUCH_RADIUS > current_position.x) &&
        (touch_position.y+TOUCH_RADIUS > current_position.y)
    )
    {
        return true;
    }  
    else
    {
        return false;
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
    int pos_x, pos_y, def_xy;
    
    def_xy = ((9.0-n)/2) * 40;
    pos_x = def_xy;
    pos_y = def_xy + 40;
    
    Sound move_sound = LoadSound("assets/move.ogg");
    Sound win_sound = LoadSound("assets/win.ogg");
    
    create_puzzle(vals, n, n);
    
    for(i = 1; i <= n*n; i++)
    {
        sprintf(puzzle[i-1].item, "%d", vals[i-1]);
        
        puzzle[i-1].position.x = pos_x;
        puzzle[i-1].position.y = pos_y;
        
        if(i%n == 0)
        {
            pos_x = def_xy;
            pos_y += 40;
        }
        else
        {
            pos_x += 40;
        }
    }
    
    while(true)
    {      
        if(atoi(puzzle[n*n-1].item) == 0)
        {
            for(i = 1; i < n*n-1; i++)
            {
                if(atoi(puzzle[i].item) < atoi(puzzle[i-1].item))
                {
                    win = false;
                    break;
                }
                else
                {
                    win = true;
                }
            }
        }
        
        if(SCREEN_DOWN)
        {
            current_position = GetMousePosition();
            
            // Sound is_mute & BACK to main
            if(current_position.x >= 290 && current_position.y <= 40)
            {
                is_mute = !is_mute;
            }
            
            // If BACK button pressed, close current game
            if
            (
                (current_position.y >= 555 && current_position.y <= 595) &&
                (current_position.x >= 85 && current_position.x <= 275)
            )
            {
                if(!is_mute)
                {
                    PlaySound(click_sound);
                }
                
                break;
            }
            
            if(!win)
            {
                for(i = 0; i < n*n; i++)
                {
                    if
                    (
                        (current_position.x > puzzle[i].position.x) &&
                        (current_position.x < puzzle[i].position.x+40) &&
                        (current_position.y > puzzle[i].position.y) &&
                        (current_position.y < puzzle[i].position.y+40)
                    )
                    {
                        j = i;
                        break;
                    }
                    else 
                    {
                        j = -1;
                    }
                }
                
                if(j != -1)
                {
                    if(((j+1)%n != 0) && (atoi(puzzle[j+1].item) == 0))
                    {
                        if(!is_mute)
                        {
                            PlaySound(move_sound);
                        }
                        
                        tmp = atoi(puzzle[j].item);
                        
                        sprintf(puzzle[j+1].item, "%d", tmp);
                        sprintf(puzzle[j].item, "%d", 0);
                    }
                    
                    else if((j%n != 0) && (atoi(puzzle[j-1].item) == 0))
                    {
                        if(!is_mute)
                        {
                            PlaySound(move_sound);
                        }
                        
                        tmp = atoi(puzzle[j].item);
                        
                        sprintf(puzzle[j-1].item, "%d", tmp);
                        sprintf(puzzle[j].item, "%d", 0);
                    }
                    
                    else if(((j+n) < n*n) && (atoi(puzzle[j+n].item) == 0))
                    {
                        if(!is_mute)
                        {
                            PlaySound(move_sound);
                        }
                        
                        tmp = atoi(puzzle[j].item);
                        
                        sprintf(puzzle[j+n].item, "%d", tmp);
                        sprintf(puzzle[j].item, "%d", 0);
                    }
                    
                    else if(((j-n) > -1) && (atoi(puzzle[j-n].item) == 0))
                    {
                        if(!is_mute)
                        {
                            PlaySound(move_sound);
                        }
                        
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
                DrawTexture(texture, puzzle[i].position.x, puzzle[i].position.y, WHITE);
                if(atoi(puzzle[i].item) != 0)
                {
                    DrawText(puzzle[i].item, puzzle[i].position.x+10, puzzle[i].position.y+10, 20, BLUE);
                }
            }
            
            DrawText("BACK", 150, 570, 20, DARKGRAY);
            
            if(is_mute)
            {
                DrawText("sound", 290, 10, 20, RED);
            }
            else
            {
                DrawText("sound", 290, 10, 20, BLUE);
            }
            
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
    Color diff_button_color[MAX_DIFFICULTY];
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "npuzzle");
    
    InitAudioDevice();
    SetTargetFPS(60);
    
    const Texture2D logo = LoadTexture("assets/logo.png");
    const Texture2D cell = LoadTexture("assets/cell.png");
    
    Sound click_sound = LoadSound("assets/click.ogg");
    
    current_position.x = -1; current_position.y = -1;
    
    while(!WindowShouldClose())
    {        
        if(SCREEN_DOWN)
        {
            current_position = GetMousePosition();
            
            //Change difficulty
            if(current_position.y >= 430 && current_position.y <= 470)
            {
                if(current_position.x >= 85 && current_position.x <= 115)
                {
                    difficulty = 3;
                    
                    if(!is_mute)
                    {
                        PlaySound(click_sound);
                    }
                }
                else if(current_position.x >= 125 && current_position.x <= 155)
                {
                    difficulty = 4;
                    
                    if(!is_mute)
                    {
                        PlaySound(click_sound);
                    }
                }
                else if(current_position.x >= 165 && current_position.x <= 195)
                {
                    difficulty = 5; 
                    
                    if(!is_mute)
                    {
                        PlaySound(click_sound);
                    }
                }
                else if(current_position.x >= 205 && current_position.x <= 235)
                {
                    difficulty = 6;
                    if(!is_mute)
                    {
                        PlaySound(click_sound);
                    }
                }
                else if(current_position.x >= 245 && current_position.x <= 275)
                {
                    difficulty = 7;
                    if(!is_mute)
                    {
                        PlaySound(click_sound);
                    }
                }
            }
            
            //Start Game
            else if
            (
                (current_position.y >= 495 && current_position.y <= 545) &&
                (current_position.x >= 85 && current_position.x <= 275)
            )
            {
                if(!is_mute)
                {
                    PlaySound(click_sound);
                }
                
                npuzzle(difficulty, cell, click_sound);
            }
            
            //Sound is mute
            else if(current_position.x >= 290 && current_position.y <= 40)
            {
                is_mute = !is_mute;
            }
        }
        
        for(i = 0; i < 5; i++)
        {
            diff_button_color[i] = BLUE;
        }
        
        diff_button_color[difficulty-3] = GREEN;
        
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            
            draw_logo(logo);
            
            DrawText("Select difficulty", 110, 400, 18, DARKGRAY);
            
            DrawText("Touch to start", 101, 510, 20, DARKGRAY);
            
            if(is_mute)
            {
                DrawText("sound", 290, 10, 20, RED);
            }
            else
            {
                DrawText("sound", 290, 10, 20, BLUE);
            }
            
            for(i = 0; i < MAX_DIFFICULTY; i++)
            {
                //itoa(i+3, buffer, 10);
                sprintf(buffer, "%d", i+3);
                DrawText(buffer, 90+i*40, 435, 30, diff_button_color[i]);
            }
            
            //draw_scheme(); // For the developer
            
        EndDrawing();
    }
    
    UnloadSound(click_sound);
    
    UnloadTexture(cell);
    UnloadTexture(logo);
    
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}