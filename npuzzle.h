#ifndef NPUZZLE_H
#define NPUZZLE_H
#include <stdlib.h>
#include <stdbool.h>

// Counting inversions in puzzle
int count_inversions(int total_tiles, int *vals);

// Checks if the puzzle is solvable
bool is_solvable(int *vals, int n, int m);

// Create solvable puzzle
void create_puzzle(int *vals, int n, int m);

#endif