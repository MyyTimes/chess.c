#ifndef PIECES_STRUCTURE_H
#define PIECES_STRUCTURE_H

#include "HeaderFiles.h"

struct chessPiece
{
    int startingPosition[2];
    int instantPosition[2];
    char symbol;
    int firstMove; //at the beginning: 1
    int firstMovePosition; //at the beginning = startPosition !The position reached in the first movement
    int isWhite;
    int isTaken; //at the beginning: 0
};

void SetIsTaken(struct chessPiece **piece, int value)
{
    (*piece)->isTaken = ((*piece)->symbol == 'K' || (*piece)->symbol == 'k') ? 0 : value;
}

void CreatePawns(struct chessPiece *piece, int yxPosition, char pawnSymbol)
{
    piece->symbol = pawnSymbol;
    piece->startingPosition[0] = yxPosition / 10;
    piece->startingPosition[1] = yxPosition % 10;
    piece->instantPosition[0] = piece->startingPosition[0];
    piece->instantPosition[1] = piece->startingPosition[1];
    piece->firstMove = 1;
    piece->firstMovePosition = piece->startingPosition[0] * 10 + piece->startingPosition[1];
    piece->isTaken = 0;
}

void LocatePieceOnBoard(struct chessPiece *piece, char board[8][8])
{
    board[piece->instantPosition[0]][piece->instantPosition[1]] = piece->symbol;
}

void MovePieceAndSetBoard(struct chessPiece **piece, char chessBoard[8][8], int nextPosition)
{
    chessBoard[(*piece)->instantPosition[0]][(*piece)->instantPosition[1]] = BOARD_DEFAULT_SYMBOL; //Clear old position
    //Save new position in struct
    (*piece)->instantPosition[0] = nextPosition / 10;
    (*piece)->instantPosition[1] = nextPosition % 10;
    chessBoard[(*piece)->instantPosition[0]][(*piece)->instantPosition[1]] = (*piece)->symbol;
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

void FindSelectedPiece(struct chessPiece pawns[], struct chessPiece majorPieces[], int selectedPos, struct chessPiece **selectedPiece)
{
    for(int i = 0; i < 8; i++)
    {
        if(pawns != NULL && pawns[i].isTaken == 0 && ((pawns[i].instantPosition[0]) == selectedPos / 10) && ((pawns[i].instantPosition[1]) == (selectedPos % 10)))
        {
            *selectedPiece = &pawns[i];
            return;
        }

        if(majorPieces != NULL && majorPieces[i].isTaken == 0 && (majorPieces[i].instantPosition[0] == selectedPos / 10) && (majorPieces[i].instantPosition[1] == selectedPos % 10))
        {
            *selectedPiece = &majorPieces[i];
            return;
        }
    }
}

void ResetTempBoard(char tempBoard[8][8], char board[8][8])
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            tempBoard[i][j] = board[i][j];
        }
    }
}

void CreateChessBoard(char board[8][8])
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = BOARD_DEFAULT_SYMBOL;
        }
    }
}

//----------------------------------------------------------------------------------------------------
#endif // PIECES_STRUCTURE_H
//----------------------------------------------------------------------------------------------------
