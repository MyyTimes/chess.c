#include <stdio.h>
#include <stdlib.h>
#include "BoardLayoutAndPieces.h"

void PieceSwitchFunction(struct chessPiece **, struct chessPiece **, int, int, char ,char [8][8]);
void FindSelectedPiece(struct chessPiece[], struct chessPiece[], int, struct chessPiece **);
void CreateChessBoard(char[8][8], int, char);
void PrintChessBoard(char[8][8], int);
void ClearTerminal();

int main()
{
    char chessBoard[8][8];
    int lengthBoard = sizeof(chessBoard) / sizeof(chessBoard[0]); //Length -> x = y
    char defaultSymbol = 'O';
    int isWhiteTurn = 1;

    //Prepare the board
    CreateChessBoard(chessBoard, lengthBoard, defaultSymbol);

    //WHITE PIECES
    struct chessPiece whitePawns[8];

    struct chessPiece whiteMajorPieces[] = 
    {
        {{7, 0}, {7, 0}, 'R', 1, 1, 0}, //rightRook
        {{7, 7}, {7, 7}, 'R', 1, 1, 0}, //leftRook
        {{7, 1}, {7, 1}, 'H', 1, 1, 0}, //rightHorse
        {{7, 6}, {7, 6}, 'H', 1, 1, 0}, //leftHorse
        {{7, 2}, {7, 2}, 'B', 1, 1, 0}, //rightBishop
        {{7, 5}, {7, 5}, 'B', 1, 1, 0}, //leftBishop
        {{7, 3}, {7, 3}, 'Q', 1, 1, 0}, //queen
        {{7, 4}, {7, 4}, 'K', 1, 1, 0}  //king
    };

    //BLACK PIECES
    struct chessPiece blackPawns[8];

    struct chessPiece blackMajorPieces[] = 
    {
        {{0, 0}, {0, 0}, 'R', 1, 0, 0}, //rightRook
        {{0, 7}, {0, 7}, 'R', 1, 0, 0}, //leftRook
        {{0, 1}, {0, 1}, 'H', 1, 0, 0}, //rightHorse
        {{0, 6}, {0, 6}, 'H', 1, 0, 0}, //leftHorse
        {{0, 2}, {0, 2}, 'B', 1, 0, 0}, //rightBishop
        {{0, 5}, {0, 5}, 'B', 1, 0, 0}, //leftBishop
        {{0, 3}, {0, 3}, 'Q', 1, 0, 0}, //queen
        {{0, 4}, {0, 4}, 'K', 1, 0, 0}   //king
    };

    //Locate all major pieces on chessBoard
    for(int i = 0; i < lengthBoard; i++)
    {
        LocatePieceOnBoard(&whiteMajorPieces[i], chessBoard);
        LocatePieceOnBoard(&blackMajorPieces[i], chessBoard);
    }

    //Create Pawns
    for(int i = 0; i < lengthBoard; i++)
    {
        whitePawns[i].isWhite = 1;
        CreatePawns(&whitePawns[i], 60 + i);
        LocatePieceOnBoard(&whitePawns[i], chessBoard);
        whitePawns[i].firstMove = 1;
        whitePawns[i].isTaken = 0;

        blackPawns[i].isWhite = 0;
        CreatePawns(&blackPawns[i], 10 + i);
        LocatePieceOnBoard(&blackPawns[i], chessBoard);
        blackPawns[i].firstMove = 1;
        blackPawns[i].isTaken = 0;
    }
 
    PrintChessBoard(chessBoard, lengthBoard); 

    int selectedPosition;
    int nextPosition;

    struct chessPiece *selectedPiece = (struct chessPiece *)malloc(sizeof(struct chessPiece));
    struct chessPiece *takenPiece = (struct chessPiece *)malloc(sizeof(struct chessPiece));
    selectedPiece = NULL;
    takenPiece = NULL;

    while(1)
    {
        if(isWhiteTurn)
            printf("Turn of WHITE\n");
        else
            printf("Turn of BLACK\n");

        //Take input from user to play
        printf("Select your piece: ");
        scanf("%d", &selectedPosition);
        printf("Input its next position: ");
        scanf("%d", &nextPosition);

        //Check the turn and find selected piece
        if(isWhiteTurn)
        {
            FindSelectedPiece(whitePawns, whiteMajorPieces, selectedPosition, &selectedPiece);
            //Find what piece is in next position -> it will be taken if it is possible
            //NOTE: I send rival pieces, so there's no need to check whether they are takeable due to rivalry.
            FindSelectedPiece(blackPawns, blackMajorPieces, nextPosition, &takenPiece);
        }
        else
        {
            FindSelectedPiece(blackPawns, blackMajorPieces, selectedPosition, &selectedPiece);
            //Find what piece is in next position -> it will be taken if it is possible
            //NOTE: I send rival pieces, so there's no need to check whether they are takeable due to rivalry.
            FindSelectedPiece(whitePawns, whiteMajorPieces, nextPosition, &takenPiece);
        }

        if(takenPiece == NULL)
        {
            printf("No target piece\n");
        }
        else
        {
            printf("Target piece symbol: %c\n", takenPiece->symbol);
        }

        PieceSwitchFunction(&selectedPiece, &takenPiece, selectedPosition, nextPosition, defaultSymbol, chessBoard);

        isWhiteTurn = !isWhiteTurn;

        PrintChessBoard(chessBoard, lengthBoard); 
    }
    
    return 0;
}

void PieceSwitchFunction(struct chessPiece **selectedPiece, struct chessPiece **takenPiece, int selectedPosition, int nextPosition, char defaultSymbol, char chessBoard[8][8])
{
    switch ((*selectedPiece)->symbol)
    {
        case 'P':
            if (PawnMotion(selectedPiece, takenPiece, nextPosition))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
            }

            break;
        
        case 'R':         
            if(RookMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
            }

            break;

        case 'H':
            if(HorseMotion(selectedPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
            }

            break;
        
        case 'B':
            if(BishopMotion(selectedPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
            }
    
            break;

        case 'Q':
            //Calling horse and bishop functions
            if(BishopMotion(selectedPiece, chessBoard, nextPosition, defaultSymbol) || RookMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
            }

        case 'K':
            if(KingMotion(selectedPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
            }
        
        default:
            break;
    }
}

void FindSelectedPiece(struct chessPiece pawns[], struct chessPiece majorPieces[], int selectedPos, struct chessPiece **selectedPiece)
{
    for(int i = 0; i < 8; i++)
    {
        if(pawns[i].isTaken == 0 && ((pawns[i].instantPosition[0]) == selectedPos / 10) && ((pawns[i].instantPosition[1]) == (selectedPos % 10)))
        {
            *selectedPiece = &pawns[i];
            return;
        }

        if(majorPieces[i].isTaken == 0 && (majorPieces[i].instantPosition[0] == selectedPos / 10) && (majorPieces[i].instantPosition[1] == selectedPos % 10))
        {
            *selectedPiece = &majorPieces[i];
            return;
        }
    }
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

/*
void TakeRivalPiece(struct chessPiece wP[], struct chessPiece wM[], struct chessPiece bP[], struct chessPiece bM[], int takenPiecePos, int isWhiteTurn)
{
    struct chessPiece *takenPiece = (struct chessPiece *)malloc(sizeof(struct chessPiece));

    if(isWhiteTurn)
    {
        FindSelectedPiece(wP, wM, takenPiecePos, &takenPiece);
    }
    else
    {
        FindSelectedPiece(bP, bM, takenPiecePos, &takenPiece);
    }

    if(takenPiece == NULL)
        printf("Null pointer"); 
    else
        printf("%c ", takenPiece->symbol);

    free(takenPiece);
    takenPiece = NULL;
}
*/