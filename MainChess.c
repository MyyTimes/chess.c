#include <stdio.h>
#include <stdlib.h>
#include "BoardLayoutAndPieces.h"

void CreateChessBoard(char[8][8], int, char);
void PrintChessBoard(char[8][8], int);
void ClearTerminal();

int main()
{
    char chessBoard[8][8];
    int lengthBoard = sizeof(chessBoard) / sizeof(chessBoard[0]); //Length -> x = y
    char defaultSymbol = 'O';
    int numberPieces = 16;

    //Prepare the board
    CreateChessBoard(chessBoard, lengthBoard, defaultSymbol);

    struct chessPiece pawn[8];

    /*
    struct chessPiece rightRook = {{7, 0}, {7, 0}, 'R'}; LocatePieceOnBoard(&rightRook, chessBoard);
    struct chessPiece leftRook = {{7, 7}, {7, 7}, 'R'}; LocatePieceOnBoard(&leftRook, chessBoard);
    struct chessPiece rightHorse = {{7, 1}, {7, 1}, 'H'}; LocatePieceOnBoard(&rightHorse, chessBoard);
    struct chessPiece leftHorse = (struct chessPiece){{7, 6}, {7, 6}, 'H'}; LocatePieceOnBoard(&leftHorse, chessBoard);
    struct chessPiece rightBishop = (struct chessPiece){{7, 2}, {7, 2}, 'B'}; LocatePieceOnBoard(&rightBishop, chessBoard);
    struct chessPiece leftBishop = (struct chessPiece){{7, 5}, {7, 5}, 'B'}; LocatePieceOnBoard(&leftBishop, chessBoard);
    struct chessPiece queen = (struct chessPiece){{7, 3}, {7, 3}, 'Q'}; LocatePieceOnBoard(&queen, chessBoard);
    struct chessPiece king = (struct chessPiece){{7, 4}, {7, 4}, 'K'}; LocatePieceOnBoard(&king, chessBoard);
    */

    struct chessPiece majorPieces[] = 
    {
        {{7, 0}, {7, 0}, 'R', 1}, //rightRook
        {{7, 7}, {7, 7}, 'R', 1}, //leftRook
        {{7, 1}, {7, 1}, 'H', 1}, //rightHorse
        {{7, 6}, {7, 6}, 'H', 1}, //leftHorse
        {{7, 2}, {7, 2}, 'B', 1}, //rightBishop
        {{7, 5}, {7, 5}, 'B', 1}, //leftBishop
        {{7, 3}, {7, 3}, 'Q', 1}, //queen
        {{7, 4}, {7, 4}, 'K', 1}  //king
    };

    //Locate all major pieces on chessBoard
    for(int i = 0; i < lengthBoard; i++)
    {
        LocatePieceOnBoard(&majorPieces[i], chessBoard);
    }

    //Create Pawns
    for(int i = 0; i < lengthBoard; i++)
    {
        CreatePawns(&pawn[i], 60 + i);
        LocatePieceOnBoard(&pawn[i], chessBoard);
        pawn[i].firstMove = 1;
    }
 
    PrintChessBoard(chessBoard, lengthBoard); 

    int selectedPosition;
    int nextPosition;
    while(1)
    {
        //Take input from user to play
        printf("Select your piece: ");
        scanf("%d", &selectedPosition);
        printf("Input its next position: ");
        scanf("%d", &nextPosition);

        char selectedPiece = chessBoard[selectedPosition / 10][selectedPosition % 10];
        printf("%c\n", selectedPiece);

        switch (selectedPiece)
        {
            case 'P':
                for(int i = 0; i < 8; i++)
                {
                    if((pawn[i].instantPosition[0] == selectedPosition / 10) && (pawn[i].instantPosition[1] == selectedPosition % 10) && PawnMotion(&pawn[i], nextPosition))
                    {
                        chessBoard[pawn[i].instantPosition[0]][pawn[i].instantPosition[1]] = defaultSymbol; //Clear old position
                        ChangePositionOfPiece(&pawn[i], nextPosition);
                        chessBoard[pawn[i].instantPosition[0]][pawn[i].instantPosition[1]] = pawn[i].symbol;
                    } 
                }
                break;

            case 'R':         
                //If it is right rook:
                if((majorPieces[0].instantPosition[0] == selectedPosition / 10) && (majorPieces[0].instantPosition[1] == selectedPosition % 10))
                {
                    if(RookMotion(&majorPieces[0], chessBoard, nextPosition, defaultSymbol))
                    {
                        MovePieceAndSetBoard(&majorPieces[0], chessBoard, nextPosition, defaultSymbol);
                    }
                }
                //If it is left rook:
                else if((majorPieces[1].instantPosition[0] == selectedPosition / 10) && (majorPieces[1].instantPosition[1] == selectedPosition % 10))
                {
                    if(RookMotion(&majorPieces[1], chessBoard, nextPosition, defaultSymbol))
                    {
                        MovePieceAndSetBoard(&majorPieces[1], chessBoard, nextPosition, defaultSymbol);
                    }
                }
                break;

            case 'H':
                //If it is right horse
                if((majorPieces[2].instantPosition[0] == selectedPosition / 10) && (majorPieces[2].instantPosition[1] == selectedPosition % 10))
                {
                    if(HorseMotion(&majorPieces[2], chessBoard, nextPosition, defaultSymbol))
                    {
                        MovePieceAndSetBoard(&majorPieces[2], chessBoard, nextPosition, defaultSymbol);
                    }
                }
                //If it is left horse
                else if((majorPieces[3].instantPosition[0] == selectedPosition / 10) && (majorPieces[3].instantPosition[1] == selectedPosition % 10))
                {
                    if(HorseMotion(&majorPieces[3], chessBoard, nextPosition, defaultSymbol))
                    {
                        MovePieceAndSetBoard(&majorPieces[3], chessBoard, nextPosition, defaultSymbol);
                    }
                }
                break;
            
            case 'B':
                //If it is right bishop
                if((majorPieces[4].instantPosition[0] == selectedPosition / 10) && (majorPieces[4].instantPosition[1] == selectedPosition % 10))
                {
                    if(BishopMotion(&majorPieces[4], chessBoard, nextPosition, defaultSymbol))
                    {
                        MovePieceAndSetBoard(&majorPieces[4], chessBoard, nextPosition, defaultSymbol);
                    }
                }
                //If it is left bishop
                else if((majorPieces[5].instantPosition[0] == selectedPosition / 10) && (majorPieces[5].instantPosition[1] == selectedPosition % 10))
                {
                    if(BishopMotion(&majorPieces[5], chessBoard, nextPosition, defaultSymbol))
                    {
                        MovePieceAndSetBoard(&majorPieces[5], chessBoard, nextPosition, defaultSymbol);
                    }
                }
                break;

            case 'Q':
                //Calling horse and bishop functions
                if(BishopMotion(&majorPieces[6], chessBoard, nextPosition, defaultSymbol) || RookMotion(&majorPieces[6], chessBoard, nextPosition, defaultSymbol))
                {
                    MovePieceAndSetBoard(&majorPieces[6], chessBoard, nextPosition, defaultSymbol);
                }

            case 'K':
                if(KingMotion(&majorPieces[7], chessBoard, nextPosition, defaultSymbol))
                {
                    MovePieceAndSetBoard(&majorPieces[7], chessBoard, nextPosition, defaultSymbol);
                }
            
            //OTHER CASES


            default:
                break;
        }

        PrintChessBoard(chessBoard, lengthBoard); 
    }
    
        

    return 0;
}

void CreateChessBoard(char board[8][8], int length, char defaultSymbol)
{
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < length; j++)
        {
            board[i][j] = defaultSymbol;
        }
    }
}

void PrintChessBoard(char board[8][8], int length)
{
    //ClearTerminal();

    printf("  A B C D E F G H\n");

    for(int i = 0; i < length; i++)
    {
        printf("%d ", i + 1);
        
        for(int j = 0; j < length; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void ClearTerminal()
{
    system("cls"); //Clear terminal for Windows
}

