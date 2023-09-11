#include "npuzzle.h"

int count_inversions(int total_tiles, int *vals)
{
    int inversions = 0;
    
    for(int i = 0; i < total_tiles-1; i++)
    {
        for(int j = i+1; j < total_tiles; j++)
        {
            if(vals[i] && vals[j] && vals[i] > vals[j])
            {
                inversions++;
            }
        }
    }
    
    return inversions;
}

bool is_solvable(int *vals, int n, int m)
{
    int total_tiles = n*m;
    int inversions = count_inversions(total_tiles, vals);
    
    if(n%2 == 1)
    {
        return inversions%2 == 0;
    }
    else
    {
        int empty_row = 0;
        
        for(int i = 0; i < total_tiles; i++)
        {
            if(vals[i] == 0)
            {
                empty_row = i/m;
                break;
            }
        }
        
        return (empty_row%2 == 0) ? (inversions%2 == 1) : (inversions%2 == 0);
    }
}

void create_puzzle(int *vals, int n, int m)
{
    int i, j, tmp;
    
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
                    if(vals[j] == tmp) 
                    {
                        break;
                    }
                }
            }
            while(j < i);
        }
    }
    while(!is_solvable(vals, n, n));
}