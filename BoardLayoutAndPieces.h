#include <stdlib.h>
#include <math.h>

struct chessPiece
{
    //int isWhite; //Is the piece white or black?
    int startingPosition[2];
    int instantPosition[2];
    char symbol;
    int firstMove;

    //char pieceName[15];
};

void CreatePawns(struct chessPiece *piece, int yxPosition)
{
    piece->symbol = 'P';
    piece->startingPosition[0] = yxPosition / 10;
    piece->startingPosition[1] = yxPosition % 10;
    piece->instantPosition[0] = piece->startingPosition[0];
    piece->instantPosition[1] = piece->startingPosition[1];
}

void ChangePositionOfPiece(struct chessPiece *piece, int newPosition)
{
    piece->instantPosition[0] = newPosition / 10;
    piece->instantPosition[1] = newPosition % 10;
}

void LocatePieceOnBoard(struct chessPiece *piece, char board[8][8])
{
    board[piece->instantPosition[0]][piece->instantPosition[1]] = piece->symbol;
}

void MovePieceAndSetBoard(struct chessPiece *piece, char chessBoard[8][8], int nextPosition, char defaultSymbol)
{
    chessBoard[piece->instantPosition[0]][piece->instantPosition[1]] = defaultSymbol; //Clear old position
    ChangePositionOfPiece(piece, nextPosition); //Save new position in struct
    chessBoard[piece->instantPosition[0]][piece->instantPosition[1]] = piece->symbol;
}

int PawnMotion(struct chessPiece *pawn, int nextPosition)
{
    if((pawn->instantPosition[0] - nextPosition / 10 == 1)&& (pawn->instantPosition[1] == nextPosition % 10))
    {
        if(pawn->firstMove == 1)
        {
            pawn->firstMove = 0;
        }

        return 1;
    }
    else if((pawn->instantPosition[0] - nextPosition / 10 == 2) && (pawn->instantPosition[1] == nextPosition % 10) && pawn->firstMove)
    {
        if(pawn->firstMove == 1)
        {
            pawn->firstMove = 0;
        }

        return 1;
    }

    return 0;
}

int RookMotion(struct chessPiece *rook, char chessBoard[8][8], int nextPos, char defaultSymbol)
{   
    int nextPosition[] = {nextPos / 10, nextPos % 10};

    if(nextPosition[0] == rook->instantPosition[0])
    {
        int step = (nextPosition[1] - rook->instantPosition[1]);

        for(int i =  1 * abs(step) / step; i != step + abs(step) / step; i += abs(step) / step)
        {
            if(chessBoard[rook->instantPosition[0]][rook->instantPosition[1] + i] != defaultSymbol)
                return 0;
        }
    }
    else if(nextPosition[1] == rook->instantPosition[1])
    {
        int step = (nextPosition[0] - rook->instantPosition[0]);

        for(int i =  1 * abs(step) / step; i != step + abs(step) / step; i += abs(step) / step)
        {
            if(chessBoard[rook->instantPosition[0] + i][rook->instantPosition[1]] != defaultSymbol)
                return 0;
        }
    }
    else
    {
        return 0;
    }
    
    if(rook->firstMove == 1)
    {
        rook->firstMove = 0;
    }
    
    return 1;
}

int HorseMotion(struct chessPiece *horse, char chessBoard[8][8], int nextPos, char defaultSymbol)
{
    double distance = sqrt(pow(horse->instantPosition[0] - (nextPos / 10), 2) + pow(horse->instantPosition[1] - (nextPos % 10), 2));

    if(distance == sqrt(5) && (chessBoard[nextPos / 10][nextPos % 10] == defaultSymbol))
    {
        if(horse->firstMove == 1)
        {
            horse->firstMove = 0;
        }

        return 1;
    }

    return 0;
}

int BishopMotion(struct chessPiece *bishop, char chessBoard[8][8], int nextPos, char defaultSymbol)
{
    //Think in reverse for the y-axis
    int iX = 1, iY = 1;
    int tempBishopPositionY = bishop->instantPosition[0];
    int tempBishopPositionX = bishop->instantPosition[1];

    if(bishop->instantPosition[0] > nextPos / 10)
    {
        iY = -1;
    }

    if(bishop->instantPosition[1] > nextPos % 10)
    {
        iX = -1;
    }

    while(chessBoard[tempBishopPositionY + iY][tempBishopPositionX + iX] == defaultSymbol)
    {
        tempBishopPositionX += iX;
        tempBishopPositionY += iY;

        if((tempBishopPositionY == nextPos / 10) && (tempBishopPositionX == nextPos % 10))
        {
            if(bishop->firstMove == 1)
            {
                bishop->firstMove = 0;
            }

            return 1;
        }     
    }

    return 0;
}

int KingMotion(struct chessPiece *king, char chessBoard[8][8], int nextPos, char defaultSymbol)
{
    double distance = sqrt(pow(king->instantPosition[0] - (nextPos / 10), 2) + pow(king->instantPosition[1] - (nextPos % 10), 2));

    if(distance <= sqrt(2) && (chessBoard[nextPos / 10][nextPos % 10] == defaultSymbol))
    {
        if(king->firstMove == 1)
        {
            king->firstMove = 0;
        }

        return 1;
    }
    else
        return 0;
}