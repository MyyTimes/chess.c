#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

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

                lastMove->movedPiece->firstMovePosition = lastMove->movedPiece->startingPosition[0] * 10 + lastMove->movedPiece->startingPosition[1];
                lastMove->takenPiece->firstMovePosition = lastMove->takenPiece->startingPosition[0] * 10 + lastMove->takenPiece->startingPosition[1];
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

//----------------------------------------------------------------------------------------------------
//LOAD SYSTEM

char CharToInt(int value)
{
    char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    return letters[value];
}

int LoadGameFromTextFile(struct chessPiece wPawns[], struct chessPiece bPawns[], struct chessPiece wMajors[], struct chessPiece bMajors[], char chessBoard[8][8])
{
    char fileName[100];
    printf("Enter the name of the text file to be uploaded: ");
    scanf(" %s", fileName);

    FILE *loadFile = fopen(fileName, "r");

    if(loadFile != NULL)
    {
        int isWhite = 1;
        char startPos[3];
        char endPos[3];
        int startPosValue;
        int endPosValue;

        struct chessPiece *selectedPiece = (struct chessPiece *)malloc(sizeof(struct chessPiece));
        struct chessPiece *takenPiece = (struct chessPiece *)malloc(sizeof(struct chessPiece));
        selectedPiece = NULL;
        takenPiece = NULL;

        printf(GREEN "File is loading...\n" RESET);

        while(fscanf(loadFile, "%2s", startPos) != EOF)
        {   
            if(fscanf(loadFile, "%2s", endPos) > 0)
            {
                startPosValue = (startPos[0] - '1') * 10 + (startPos[1] - 'A');
                endPosValue = (endPos[0] - '1') * 10 + (endPos[1] - 'A');

                //printf("%d %d\n", startPosValue, endPosValue);

                if(isWhite)
                {
                    FindSelectedPiece(wPawns, wMajors, startPosValue, &selectedPiece);
                    FindSelectedPiece(bPawns, bMajors, endPosValue, &takenPiece);
                }
                else
                {
                    FindSelectedPiece(bPawns, bMajors, startPosValue, &selectedPiece);
                    FindSelectedPiece(wPawns, wMajors, endPosValue, &takenPiece);
                }

                if(selectedPiece != NULL)
                {
                    selectedPiece->instantPosition[0] = startPosValue / 10;
                    selectedPiece->instantPosition[1] = startPosValue % 10;

                    if(selectedPiece->firstMove)
                    {
                        selectedPiece->firstMove = 0;
                        selectedPiece->firstMovePosition = endPosValue;
                    }

                    MovePieceAndSetBoard(&selectedPiece,chessBoard, endPosValue);

                    if(takenPiece != NULL)
                    {
                        SetIsTaken(&takenPiece, 1);
                    }
                }
                else
                {
                    return 0;
                }

                //Create NODE to be able to UNDO
                CreateNode(selectedPiece, takenPiece, startPosValue, endPosValue);

                isWhite = !isWhite;
            }
            else
            {
                return 0;
            }     
        }

        //free(selectedPiece);
        //free(takenPiece);
        
        return 1;
    }
    else
    {
        return 0;
    }
}


#endif //SAVE_SYSTEM_H
