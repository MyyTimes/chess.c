#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include "HeaderFiles.h"

struct savedMove
{
    struct chessPiece *movedPiece;
    struct chessPiece *takenPiece;
    int startPosition;
    int endPosition;

    struct savedMove *previousMove;
};
struct savedMove *lastMove = NULL;

void CreateNode(struct chessPiece *movedPiece, struct chessPiece *takenPiece, int startPos, int endPos)
{
    struct savedMove *newMove = (struct savedMove*)malloc(sizeof(struct savedMove));
    newMove->movedPiece = movedPiece;
    newMove->takenPiece = takenPiece;
    newMove->startPosition = startPos;
    newMove->endPosition = endPos;
    newMove->previousMove = NULL;

    if(lastMove != NULL)
    {
        newMove->previousMove = lastMove;
        lastMove = newMove;
    }
    else
    {
        lastMove = newMove;
    }
}

//----------------------------------------------------------------------------------------------------
//Save all moves at the end of the game to text
void SaveDataToText()
{
    printf("Starting save...\n");

    if(StartSaving())
        return;

    struct savedMove *temp = NULL; 
    temp = lastMove;

    while(temp != NULL)
    {
        if(temp->takenPiece == NULL)
            WriteMovesToText(temp->movedPiece->isWhite, temp->startPosition, temp->endPosition, temp->movedPiece->symbol, BOARD_DEFAULT_SYMBOL);
        else
            WriteMovesToText(temp->movedPiece->isWhite, temp->startPosition, temp->endPosition, temp->movedPiece->symbol, temp->takenPiece->symbol);

        temp = temp->previousMove;
    }

    CloseSaving();

    printf("Saving completed!\n");
}

//Save all moves at the end of the game to text BUT REVERSE
void SaveDataReverseToText(struct savedMove *temp) 
{ 
    if (temp == NULL) 
    {
        return;
    }
    
    SaveDataReverseToText(temp->previousMove);
    
    if(temp->takenPiece == NULL)
        WriteMovesToText(temp->movedPiece->isWhite, temp->startPosition, temp->endPosition, temp->movedPiece->symbol, BOARD_DEFAULT_SYMBOL);
    else
        WriteMovesToText(temp->movedPiece->isWhite, temp->startPosition, temp->endPosition, temp->movedPiece->symbol, temp->takenPiece->symbol);
}

int Undo(char board[8][8])
{
    if(lastMove != NULL && lastMove->movedPiece != NULL)
    {
        board[lastMove->movedPiece->instantPosition[0]][lastMove->movedPiece->instantPosition[1]] = BOARD_DEFAULT_SYMBOL;

        lastMove->movedPiece->instantPosition[0] = lastMove->startPosition / 10;
        lastMove->movedPiece->instantPosition[1] = lastMove->startPosition % 10;
        board[lastMove->startPosition / 10][lastMove->startPosition % 10] = lastMove->movedPiece->symbol;

        if(lastMove->takenPiece != NULL)
        {
            board[lastMove->takenPiece->instantPosition[0]][lastMove->takenPiece->instantPosition[1]] = BOARD_DEFAULT_SYMBOL;

            //If it is ROK
            if((lastMove->movedPiece->symbol == 'K' && lastMove->takenPiece->symbol == 'R') || (lastMove->movedPiece->symbol == 'k' && lastMove->takenPiece->symbol == 'r'))
            {
                //Set ROOK
                lastMove->takenPiece->instantPosition[0] = lastMove->endPosition / 10;
                lastMove->takenPiece->instantPosition[1] = lastMove->endPosition % 10; 
                board[lastMove->endPosition / 10][lastMove->endPosition % 10] = lastMove->takenPiece->symbol;

                lastMove->movedPiece->firstMove = 1;
                lastMove->takenPiece->firstMove = 1;
            }

            //A piece was taken
            else if(lastMove->takenPiece->symbol != BOARD_DEFAULT_SYMBOL)
            {
                SetIsTaken(&lastMove->takenPiece, 0);
                //lastMove->takenPiece->isTaken = 0;
                
                board[lastMove->takenPiece->instantPosition[0]][lastMove->takenPiece->instantPosition[1]] = lastMove->takenPiece->symbol;

                if(lastMove->endPosition == lastMove->takenPiece->firstMovePosition && lastMove->takenPiece->startingPosition[0] == lastMove->startPosition / 10 && lastMove->takenPiece->startingPosition[1] == lastMove->startPosition % 10)
                {
                    lastMove->takenPiece->firstMovePosition = lastMove->takenPiece->startingPosition[0] * 10 + lastMove->takenPiece->startingPosition[1];
                    lastMove->takenPiece->firstMove = 1;
                }
            }
        }

        if(lastMove->endPosition == lastMove->movedPiece->firstMovePosition && lastMove->movedPiece->startingPosition[0] == lastMove->startPosition / 10 && lastMove->movedPiece->startingPosition[1] == lastMove->startPosition % 10)
        {
            lastMove->movedPiece->firstMovePosition = lastMove->movedPiece->startingPosition[0] * 10 + lastMove->movedPiece->startingPosition[1];
            lastMove->movedPiece->firstMove = 1;
        }

        struct savedMove *temp = lastMove; 
        lastMove = lastMove->previousMove; 
        free(temp);  

        return 1;
    }
    else
    {
        printf("Undo operation failed: Pointer is NULL!\n");
        return 0;
    }
}

#endif //SAVE_SYSTEM_H
