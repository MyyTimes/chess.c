#ifndef PAT_SYSTEM_H
#define PAT_SYSTEM_H

#include "HeaderFiles.h"

int IsRivalPiece(int isWhite, char rivalPiece)
{
    if(rivalPiece >= 'a' && isWhite)
        return 1;
    else if(rivalPiece < 'a' && !isWhite)
        return 1;
    else
        return 0;
}

void PawnRange(struct chessPiece *pawn, struct chessPiece *enPassant, char tempBoard[8][8])
{
    int direction = pawn->isWhite == 1 ? -1 : 1;
    int row = pawn->instantPosition[0] + direction;
    int column = pawn->instantPosition[1];

    if(tempBoard[row][column] == BOARD_DEFAULT_SYMBOL)
    {
        tempBoard[row][column] = BOARD_MARKED_SYMBOL;

        if(pawn->firstMove && (tempBoard[pawn->instantPosition[0] + direction * 2][column] == BOARD_DEFAULT_SYMBOL))
        {
            tempBoard[pawn->instantPosition[0] + direction * 2][column] = BOARD_MARKED_SYMBOL;
        }
    }

    //Take rival piece
    if(column + 1 <= BOARD_SIZE && tempBoard[row][column + 1] != BOARD_DEFAULT_SYMBOL)
    {
        if(IsRivalPiece(pawn->isWhite, tempBoard[row][column + 1]))
        {
            tempBoard[row][column + 1] = BOARD_MARKED_SYMBOL;
        }
    }
    else if(column - 1 >= 0 && tempBoard[row][column - 1] != BOARD_DEFAULT_SYMBOL)
    {
        if(IsRivalPiece(pawn->isWhite, tempBoard[row][column - 1]))
        {
            tempBoard[row][column - 1] = BOARD_MARKED_SYMBOL;
        }
    }
    
    //If en passant is possible
    if(enPassant != NULL)
    {
        if(pawn->instantPosition[0] == enPassant->instantPosition[0] && abs(pawn->instantPosition[1] - enPassant->instantPosition[1]) == 1)
        {
            tempBoard[enPassant->instantPosition[0] + direction][enPassant->instantPosition[1]] = BOARD_MARKED_SYMBOL;
        }
    }
}

//For rook and bishop
void MarkLinearRange(struct chessPiece *piece, char tempBoard[8][8], int directionX, int directionY)
{
    int row;
    int column;

    for (int i = 1; ; i++)
    {
        row = piece->instantPosition[0] + i * directionX;
        column = piece->instantPosition[1] + i * directionY;

        if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE)
            break;

        if (tempBoard[row][column] == BOARD_DEFAULT_SYMBOL)
        {
            tempBoard[row][column] = BOARD_MARKED_SYMBOL;
        }
        else
        {
            if (IsRivalPiece(piece->isWhite, tempBoard[row][column]))
                tempBoard[row][column] = BOARD_MARKED_SYMBOL;

            break;
        }
    }
}

void RookRange(struct chessPiece *rook, char tempBoard[8][8])
{
    MarkLinearRange(rook, tempBoard, 1, 0);
    MarkLinearRange(rook, tempBoard, 0, -1);
    MarkLinearRange(rook, tempBoard, -1, 0);
    MarkLinearRange(rook, tempBoard, 0, 1);
}

void BishopRange(struct chessPiece *bishop, char tempBoard[8][8])
{
    MarkLinearRange(bishop, tempBoard, 1, 1);
    MarkLinearRange(bishop, tempBoard, 1, -1);
    MarkLinearRange(bishop, tempBoard, -1, 1);
    MarkLinearRange(bishop, tempBoard, -1, -1);
}

void HorseRange(struct chessPiece *horse, char tempBoard[8][8])
{
    int row;
    int column;

    for(int i = -2; i <= 2; i++)
    {
        for(int j = -2; j <= 2; j++)
        {
            row = horse->instantPosition[0] + i;
            column = horse->instantPosition[1] + j;

            if(row >= 0 && row < BOARD_SIZE && column >= 0 && column < BOARD_SIZE)
            {   
                if(pow(i, 2) + pow(j, 2) == 5)
                {
                    if(tempBoard[row][column] == BOARD_DEFAULT_SYMBOL || IsRivalPiece(horse->isWhite, tempBoard[row][column]))
                        tempBoard[row][column] = BOARD_MARKED_SYMBOL;
                }
            }
        }
    }
}

void QueenRange(struct chessPiece *queen, char tempBoard[8][8])
{
    RookRange(queen, tempBoard);
    BishopRange(queen, tempBoard);
}

void KingRange(struct chessPiece *king, char tempBoard[8][8])
{
    int row;
    int column;

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            row = king->instantPosition[0] + i;
            column = king->instantPosition[1] + j;

            if(row >= 0 && row < BOARD_SIZE && column >= 0 && column < BOARD_SIZE)
            {   
                if(tempBoard[row][column] == BOARD_DEFAULT_SYMBOL || IsRivalPiece(king->isWhite, tempBoard[row][column]))
                    tempBoard[row][column] = BOARD_MARKED_SYMBOL;
            }
        }
    }
}

void MarkCastling(struct chessPiece *king, struct chessPiece *castlingRook, int castlingPos,char tempBoard[8][8])
{
    if(castlingRook->firstMove)
    {
        int direction = king->instantPosition[1] > castlingRook->instantPosition[1] ? -1 : 1;

        for(int i = king->instantPosition[1] + direction; i > 0 && i < BOARD_SIZE - 1; i += direction)
        {
            if(tempBoard[king->instantPosition[0]][i] != BOARD_DEFAULT_SYMBOL)
                return;
        }

        tempBoard[castlingRook->instantPosition[0]][castlingRook->instantPosition[1]] = BOARD_MARKED_SYMBOL;
    }
}

void FindFunctionBySymbol(struct chessPiece *piece, struct chessPiece *enPassant, char tempBoard[8][8])
{
    switch (piece->symbol)
    {
        case 'p':
        case 'P':
            PawnRange(piece, enPassant, tempBoard);
            break;

        case 'r':
        case 'R':
            RookRange(piece, tempBoard);
            break;

        case 'b':
        case 'B':
            BishopRange(piece, tempBoard);
            break;

        case 'h':
        case 'H':
            HorseRange(piece, tempBoard);
            break;

        case 'q':
        case 'Q':
            QueenRange(piece, tempBoard);
            break;

        case 'k':
        case 'K':
            KingRange(piece, tempBoard);
            break;
        
        default:
            break;
    }
}

int IsKingThreatened(struct chessPiece pawns[], struct chessPiece majorPieces[], struct chessPiece *king, char chessBoard[8][8])
{
    char checkBoard[8][8]; 

    for(int i = 0; i < BOARD_SIZE; i++)
    {
        ResetTempBoard(checkBoard, chessBoard);

        FindFunctionBySymbol(&pawns[i], NULL, checkBoard);
        if(checkBoard[king->instantPosition[0]][king->instantPosition[1]] == BOARD_MARKED_SYMBOL)
            return 1;
            
        ResetTempBoard(checkBoard, chessBoard);

        FindFunctionBySymbol(&majorPieces[i], NULL, checkBoard);
        if(checkBoard[king->instantPosition[0]][king->instantPosition[1]] == BOARD_MARKED_SYMBOL)
            return 1;
    }

    return 0;
}

int CheckKings(struct chessPiece wP[], struct chessPiece bP[], struct chessPiece wM[], struct chessPiece bM[], int *isWhiteTurn, int *isCheck, char board[8][8])
{
    if(*isWhiteTurn == 0)
    {
        if(IsKingThreatened(bP, bM, &wM[KING_INDEX], board))
        {
            printf(RED "Illegal Move!\n" RESET);

            if(Undo(board))
            {
                *isWhiteTurn = !(*isWhiteTurn);
                printf(GREEN "The move was undone.\n" RESET);
            }
            else
            {
                printf(RED "Undoing the move was unsuccessful!\n" RESET);
            }

            return 2; //ILLEGAL MOVE
        }   
        if(IsKingThreatened(wP, wM, &bM[KING_INDEX], board))
        {
            printf(GREEN "Check!\n" RESET);
            *isCheck = 1;
            return 1; //CHECK
        }      
    }
    else
    {
        if(IsKingThreatened(wP, wM, &bM[KING_INDEX], board))
        {
            printf(RED "Illegal Move!\n" RESET);

            if(Undo(board))
            {
                *isWhiteTurn = !(*isWhiteTurn);
                printf(GREEN "The move was undone.\n" RESET);
            }
            else
            {
                printf(RED "Undoing the move was unsuccessful!\n" RESET);
            }

            return 2; //ILLEGAL MOVE
        }   
        if(IsKingThreatened(bP, bM, &wM[KING_INDEX], board))
        {
            printf(GREEN "Check!\n" RESET);
            *isCheck = 1;
            return 1; //CHECK
        } 
    }

    return 0;
}

int CheckmateBoardControl(char board[8][8])
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(board[i][j] == BOARD_MARKED_SYMBOL)
            {
                return 1;
            }
        }
    }

    return 0;
}

int CheckmateControl(struct chessPiece pawns[], struct chessPiece majorPieces[], char chessBoard[8][8])
{
    char mateBoard[8][8]; 

    for(int i = 0; i < BOARD_SIZE; i++)
    {
        ResetTempBoard(mateBoard, chessBoard);

        FindFunctionBySymbol(&pawns[i], NULL, mateBoard);
        if(CheckmateBoardControl(mateBoard))
            return 0;
        
        ResetTempBoard(mateBoard, chessBoard);

        FindFunctionBySymbol(&majorPieces[i], NULL, mateBoard);
        if(CheckmateBoardControl(mateBoard))
            return 0;
    }

    return 1;
}

#endif // PAT_SYSTEM_H