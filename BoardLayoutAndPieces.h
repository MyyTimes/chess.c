#include <stdlib.h>
#include <math.h>

struct chessPiece
{
    int startingPosition[2];
    int instantPosition[2];
    char symbol;
    int firstMove; //at the beginning: 1
    int isWhite;
    int isTaken; //at the beginning: 0
};

void CreatePawns(struct chessPiece *piece, int yxPosition)
{
    piece->symbol = 'P';
    piece->startingPosition[0] = yxPosition / 10;
    piece->startingPosition[1] = yxPosition % 10;
    piece->instantPosition[0] = piece->startingPosition[0];
    piece->instantPosition[1] = piece->startingPosition[1];
}

void LocatePieceOnBoard(struct chessPiece *piece, char board[8][8])
{
    board[piece->instantPosition[0]][piece->instantPosition[1]] = piece->symbol;
}

void MovePieceAndSetBoard(struct chessPiece **piece, char chessBoard[8][8], int nextPosition, char defaultSymbol)
{
    chessBoard[(*piece)->instantPosition[0]][(*piece)->instantPosition[1]] = defaultSymbol; //Clear old position
    //Save new position in struct
    (*piece)->instantPosition[0] = nextPosition / 10;
    (*piece)->instantPosition[1] = nextPosition % 10;
    chessBoard[(*piece)->instantPosition[0]][(*piece)->instantPosition[1]] = (*piece)->symbol;
}

int PawnMotion(struct chessPiece **pawn, struct chessPiece **takenPiece, int nextPosition, char board[8][8], char defaultSymbol)
{
    int nextColumn = nextPosition % 10;
    int nextRow = nextPosition / 10;
    int stepDirection = (*pawn)->isWhite ? 1 : -1;

    if(board[nextRow][nextColumn] == defaultSymbol)
    {
        if(((*pawn)->instantPosition[0] - nextRow == stepDirection) && ((*pawn)->instantPosition[1] == nextColumn))
        {
            if((*pawn)->firstMove == 1)
            {
                (*pawn)->firstMove = 0;
            }

            return 1;
        }
        
        //Moving 2 step forward
        if(((*pawn)->instantPosition[0] - nextRow == 2 * stepDirection) && ((*pawn)->instantPosition[1] == nextColumn) && (*pawn)->firstMove)
        {
            if(board[nextRow + stepDirection][nextColumn] == defaultSymbol)
            {
                (*pawn)->firstMove = 0;
                return 2;
            }
        }
    }

    //Taking rival piece
    //nextPosition == takenPiece position
    if((*takenPiece) != NULL)
    {
        if((*takenPiece)->instantPosition[0] == nextRow && (*takenPiece)->instantPosition[1] == nextColumn)
        {
            //Is nextPosition suitable for taking it
            if(((*pawn)->instantPosition[0] - nextRow == stepDirection) && abs((*pawn)->instantPosition[1] - nextColumn) == 1)
            {
                if((*pawn)->firstMove == 1)
                {
                    (*pawn)->firstMove = 0;
                }
                (*takenPiece)->isTaken = 1;
                return 3;
            }
        }
    }

    return 0;
}

int RookMotion(struct chessPiece **rook, struct chessPiece **takenPiece, char chessBoard[8][8], int nextPos, char defaultSymbol)
{   
    int nextPosition[] = {nextPos / 10, nextPos % 10};

    if(nextPosition[0] == (*rook)->instantPosition[0])
    {
        int step = (nextPosition[1] - (*rook)->instantPosition[1]);

        //Check UNTIL last step (nextPosition)
        for(int i = abs(step) / step; i != step; i += abs(step) / step)
        {
            if(chessBoard[(*rook)->instantPosition[0]][(*rook)->instantPosition[1] + i] != defaultSymbol)
                return 0;
        }
    }
    else if(nextPosition[1] == (*rook)->instantPosition[1])
    {
        int step = (nextPosition[0] - (*rook)->instantPosition[0]);

        for(int i =  1 * abs(step) / step; i != step; i += abs(step) / step)
        {
            if(chessBoard[(*rook)->instantPosition[0] + i][(*rook)->instantPosition[1]] != defaultSymbol)
                return 0;
        }
    }
    else
    {
        return 0;
    }

    //Check last step
    //Is possible to take piece
    if((*takenPiece) != NULL)
    {
        if((*takenPiece)->instantPosition[0] == nextPosition[0] && (*takenPiece)->instantPosition[1] == nextPosition[1])
        {
            (*takenPiece)->isTaken = 1;
        }
    }
    else if(chessBoard[nextPosition[0]][nextPosition[1]] != defaultSymbol) //So this piece is my piece, not rival
    {
        return 0;
    }

    if((*rook)->firstMove == 1)
    {
        (*rook)->firstMove = 0;
    }
    
    return 1;
}

int HorseMotion(struct chessPiece **horse, struct chessPiece **takenPiece, char chessBoard[8][8], int nextPos, char defaultSymbol)
{
    double distance = sqrt(pow((*horse)->instantPosition[0] - (nextPos / 10), 2) + pow((*horse)->instantPosition[1] - (nextPos % 10), 2));

    if(distance == sqrt(5))
    {
        if((*takenPiece) != NULL)
        {
            if((*takenPiece)->instantPosition[0] == nextPos / 10 && (*takenPiece)->instantPosition[1] == nextPos % 10)
            {
                (*takenPiece)->isTaken = 1;
            }
        }
        else if(chessBoard[nextPos / 10][nextPos % 10] != defaultSymbol) //So this piece is my piece, not rival
        {
            return 0;
        }

        if((*horse)->firstMove == 1)
        {
            (*horse)->firstMove = 0;
        }

        return 1;
    }

    return 0;
}

int BishopMotion(struct chessPiece **bishop, struct chessPiece **takenPiece, char chessBoard[8][8], int nextPos, char defaultSymbol)
{
    //Think in reverse for the y-axis
    int iX = 1, iY = 1;
    int tempBishopPositionY = (*bishop)->instantPosition[0];
    int tempBishopPositionX = (*bishop)->instantPosition[1];

    if((*bishop)->instantPosition[0] > nextPos / 10)
    {
        iY = -1;
    }

    if((*bishop)->instantPosition[1] > nextPos % 10)
    {
        iX = -1;
    }

    while(chessBoard[tempBishopPositionY + iY][tempBishopPositionX + iX] == defaultSymbol)
    {
        tempBishopPositionX += iX;
        tempBishopPositionY += iY;

        if((tempBishopPositionY + iY == nextPos / 10) && (tempBishopPositionX + iX == nextPos % 10))
        {
            if((*takenPiece) != NULL)
            {
                if((*takenPiece)->instantPosition[0] == nextPos / 10 && (*takenPiece)->instantPosition[1] == nextPos % 10)
                {
                    (*takenPiece)->isTaken = 1;
                }
            }
            else if(chessBoard[nextPos / 10][nextPos % 10] != defaultSymbol) //So this piece is my piece, not rival
            {
                return 0;
            }

            if((*bishop)->firstMove == 1)
            {
                (*bishop)->firstMove = 0;
            }

            return 1;
        }     
    }

    return 0;
}

int KingMotion(struct chessPiece **king, struct chessPiece **takenPiece, char chessBoard[8][8], int nextPos, char defaultSymbol)
{
    double distance = sqrt(pow((*king)->instantPosition[0] - (nextPos / 10), 2) + pow((*king)->instantPosition[1] - (nextPos % 10), 2));

    if(distance <= sqrt(2))
    {
        if((*takenPiece) != NULL)
        {
            if((*takenPiece)->instantPosition[0] == nextPos / 10 && (*takenPiece)->instantPosition[1] == nextPos % 10)
            {
                (*takenPiece)->isTaken = 1;
            }
        }
        else if(chessBoard[nextPos / 10][nextPos % 10] != defaultSymbol) //So this piece is my piece, not rival
        {
            return 0;
        }

        if((*king)->firstMove == 1)
        {
            (*king)->firstMove = 0;
        }

        return 1;
    }
    
    return 0;
}

int Rok(struct chessPiece **king, struct chessPiece **rook, char board[8][8], char defaultSymbol)
{
    int distance = abs((*king)->instantPosition[1] - (*rook)->instantPosition[1]);
    int step = (*king)->instantPosition[1] - (*rook)->instantPosition[1] > 0 ? -1 : 1; //Left or right

    for(int i = 1 * step; i * step < distance; i += 1 * step)
    {
        if(board[(*king)->instantPosition[0]][(*king)->instantPosition[1] + i] != defaultSymbol)
        {
            return 0;
        }
    }

    int kingNewXPos;
    int rookNewXPos;
    if(step == 1)
    {
        kingNewXPos = 6;
        rookNewXPos = 5;
    }
    else if(step == -1)
    {
        kingNewXPos = 2;
        rookNewXPos = 3;
    }

    board[(*king)->instantPosition[0]][(*king)->instantPosition[1]] = defaultSymbol;
    board[(*rook)->instantPosition[0]][(*rook)->instantPosition[1]] = defaultSymbol;
    (*king)->instantPosition[1] = kingNewXPos;
    (*rook)->instantPosition[1] = rookNewXPos;
    board[(*king)->instantPosition[0]][(*king)->instantPosition[1]] = (*king)->symbol;
    board[(*rook)->instantPosition[0]][(*rook)->instantPosition[1]] = (*rook)->symbol;

    (*king)->firstMove = 0;
    (*rook)->firstMove = 0;
    
    return 1;
}

void PawnPromotion(struct chessPiece **pawn)
{
    char c;
    char newPiece;

    printf("Input what you want to promote the pawn to (Q: Queen / H: Horse / B: Bishop / R: Rook): ");

    while (1) {
        scanf(" %c", &newPiece);

        if (newPiece == 'Q' || newPiece == 'H' || newPiece == 'B' || newPiece == 'R')
            break;
        else 
        {
            printf("Please input valid piece!: ");
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    (*pawn)->symbol = newPiece;
}


//----------------------------------------------------------------------------------------------------
#ifndef SaveToText_H    // If B_H is not defined
#define SaveToText_H
#include "SaveToText.h"
//----------------------------------------------------------------------------------------------------

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
void SaveDataToText(char defaultSymbol)
{
    printf("Starting save...\n");

    if(StartSaving())
        return;

    struct savedMove *temp = NULL; 
    temp = lastMove;

    while(temp != NULL)
    {
        if(temp->takenPiece == NULL)
            WriteMovesToText(temp->movedPiece->isWhite, temp->startPosition, temp->endPosition, temp->movedPiece->symbol, defaultSymbol, defaultSymbol);
        else
            WriteMovesToText(temp->movedPiece->isWhite, temp->startPosition, temp->endPosition, temp->movedPiece->symbol, temp->takenPiece->symbol, defaultSymbol);

        temp = temp->previousMove;
    }

    CloseSaving();

    printf("Saving completed!\n");
}

//Save all moves at the end of the game to text BUT REVERSE
void SaveDataReverseToText(struct savedMove *temp, char defaultSymbol) 
{ 
    if (temp == NULL) 
    {
        return;
    }
    
    SaveDataReverseToText(temp->previousMove, defaultSymbol);
    
    if(temp->takenPiece == NULL)
        WriteMovesToText(temp->movedPiece->isWhite, temp->startPosition, temp->endPosition, temp->movedPiece->symbol, defaultSymbol, defaultSymbol);
    else
        WriteMovesToText(temp->movedPiece->isWhite, temp->startPosition, temp->endPosition, temp->movedPiece->symbol, temp->takenPiece->symbol, defaultSymbol);
}

//----------------------------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------------------------