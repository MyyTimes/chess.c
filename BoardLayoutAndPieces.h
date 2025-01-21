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
/*
void ChangePositionOfPiece(struct chessPiece *piece, int newPosition)
{
    piece->instantPosition[0] = newPosition / 10;
    piece->instantPosition[1] = newPosition % 10;
}
*/
void LocatePieceOnBoard(struct chessPiece *piece, char board[8][8])
{
    board[piece->instantPosition[0]][piece->instantPosition[1]] = piece->symbol;
}

void MovePieceAndSetBoard(struct chessPiece **piece, char chessBoard[8][8], int nextPosition, char defaultSymbol)
{
    chessBoard[(*piece)->instantPosition[0]][(*piece)->instantPosition[1]] = defaultSymbol; //Clear old position
    //ChangePositionOfPiece(piece, nextPosition); 
    //Save new position in struct
    (*piece)->instantPosition[0] = nextPosition / 10;
    (*piece)->instantPosition[1] = nextPosition % 10;
    chessBoard[(*piece)->instantPosition[0]][(*piece)->instantPosition[1]] = (*piece)->symbol;
}

int PawnMotion(struct chessPiece **pawn, struct chessPiece **takenPiece, int nextPosition)
{
    int nextColumn = nextPosition % 10;
    int nextRow = nextPosition / 10;
    int stepDirection = (*pawn)->isWhite ? 1 : -1;

    if(((*pawn)->instantPosition[0] - nextRow == stepDirection) && ((*pawn)->instantPosition[1] == nextColumn))
    {
        if((*pawn)->firstMove == 1)
        {
            (*pawn)->firstMove = 0;
        }

        return 1;
    }
    
    if(((*pawn)->instantPosition[0] - nextRow == 2 * stepDirection) && ((*pawn)->instantPosition[1] == nextColumn) && (*pawn)->firstMove)
    {
        (*pawn)->firstMove = 0;

        return 1;
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
                return 1;
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

int Rok()
{
    //Check between king and rook
    //Check first move, both 
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