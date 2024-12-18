#include <stdio.h>
#include "board.h"

int main()
{
    Board board;
    board.white_pieces.bishops = 1;
    if (board.white_pieces.bishops == 1)
    {
        printf("Success\n");
    }
    else
    {
        printf("Failure\n");
    }
    printf("Hello, World!\n");
    return 0;
}