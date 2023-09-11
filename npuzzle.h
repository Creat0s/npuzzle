int countInversions(cell puzzle[], int n)
{
    int inversions = 0;
    
    for(int i = 0; i < n-1; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            if((atoi(puzzle[i].item) > 0) && (atoi(puzzle[j].item) > 0)
            && (atoi(puzzle[i].item) > atoi(puzzle[j].item))) inversions++;
        }
    }
    return inversions;
}

bool checkSolvable(cell puzzle[], int N, int M)
{
    int inversions, emptyRow = 0;
    
    inversions = countInversions(puzzle, N*M);
    
    for(int i = 0, k = 0; i < N*M; i++)
    {
        if(i%N == 0) k = 0;
        if(atoi(puzzle[i].item) == 0) {emptyRow = k; break;}
        k++;
    }
    
    if(N%2 == 1)
    {
        if(inversions%2 == 0) return true;
        else return false;
    }
    
    else if((inversions + emptyRow)%2 == 0) return true;
    else return false;
}
