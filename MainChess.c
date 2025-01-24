#include <stdio.h>
#include <stdlib.h>
#include "BoardLayoutAndPieces.h"
#include "ColorPaletteForTerminal.h"

int PieceSwitchFunction(struct chessPiece **, struct chessPiece **, struct chessPiece **, int, int, char ,char [8][8]);
void FindSelectedPiece(struct chessPiece[], struct chessPiece[], int, struct chessPiece **);
void CreateChessBoard(char[8][8], int, char);
void PrintChessBoard(char[8][8], int, char);
int TakeInputFromGamer(char[]);
void PrintInfo();
void ClearTerminal();

int main()
{
    PrintInfo();
    
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
 
    //PrintChessBoard(chessBoard, lengthBoard, defaultSymbol); 

    int selectedPosition;
    int nextPosition;

    struct chessPiece *selectedPiece = (struct chessPiece *)malloc(sizeof(struct chessPiece));
    struct chessPiece *takenPiece = (struct chessPiece *)malloc(sizeof(struct chessPiece));
    selectedPiece = NULL;
    takenPiece = NULL;

    struct chessPiece *enPassantPawn = (struct chessPiece *)malloc(sizeof(struct chessPiece));
    enPassantPawn = NULL;

    while(1)
    {
        PrintChessBoard(chessBoard, lengthBoard, defaultSymbol); 

        if(isWhiteTurn)
            printf(YELLOW "Turn of WHITE\n" RESET);
        else
            printf(YELLOW "Turn of BLACK\n" RESET);

        //Take input from user to play
        //Select piece to move from-
        selectedPosition = TakeInputFromGamer("Select your piece: ");
        //Select piece to move to-
        nextPosition = TakeInputFromGamer("Input its next position: ");

        //Check the turn and find selected piece
        if(isWhiteTurn)
        {
            FindSelectedPiece(whitePawns, whiteMajorPieces, selectedPosition, &selectedPiece);

            //ROK
            if(selectedPiece->symbol == 'K' && selectedPiece->firstMove == 1)
            {
                if(nextPosition / 10 == selectedPiece->startingPosition[0] && ((nextPosition % 10 == 0) || (nextPosition % 10 == 7)))
                {
                    FindSelectedPiece(whitePawns, whiteMajorPieces, nextPosition, &takenPiece);
                    if(takenPiece != NULL)
                    {
                        if(takenPiece->symbol == 'R' && takenPiece->firstMove)
                        {
                            if(Rok(&selectedPiece, &takenPiece, chessBoard, defaultSymbol))
                            {
                                isWhiteTurn = !isWhiteTurn;
                                continue;
                            }
                        } 
                    }
                }
            }

            //Find what piece is in next position -> it will be taken if it is possible
            //NOTE: I send rival pieces, so there's no need to check whether they are takeable due to rivalry.
            FindSelectedPiece(blackPawns, blackMajorPieces, nextPosition, &takenPiece);
        }
        else
        {
            FindSelectedPiece(blackPawns, blackMajorPieces, selectedPosition, &selectedPiece);

            //ROK
            if(selectedPiece->symbol == 'K' && selectedPiece->firstMove == 1)
            {
                if(nextPosition / 10 == selectedPiece->startingPosition[0] && ((nextPosition % 10 == 0) || (nextPosition % 10 == 7)))
                {
                    FindSelectedPiece(whitePawns, whiteMajorPieces, nextPosition, &takenPiece);
                    if(takenPiece != NULL)
                    {
                        if(takenPiece->symbol == 'R' && takenPiece->firstMove)
                        {
                            if(Rok(&selectedPiece, &takenPiece, chessBoard, defaultSymbol))
                            {
                                isWhiteTurn = !isWhiteTurn;
                                continue;
                            }
                        } 
                    }
                }
            }

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

        if(PieceSwitchFunction(&selectedPiece, &takenPiece, &enPassantPawn,selectedPosition, nextPosition, defaultSymbol, chessBoard))
        {
            isWhiteTurn = !isWhiteTurn;

            if(enPassantPawn != NULL && (enPassantPawn->isWhite == isWhiteTurn))
                enPassantPawn = NULL; //RESET en passant pawn
        }
        else
        {
            printf(RED "ILLEGAL MOVE!\n" RESET);
        }
        

        //PrintChessBoard(chessBoard, lengthBoard, defaultSymbol); 

        //Setting free other pointers
        selectedPiece = NULL;
        takenPiece = NULL;
    }
    
    return 0;
}

int PieceSwitchFunction(struct chessPiece **selectedPiece, struct chessPiece **takenPiece, struct chessPiece **enPassantPawn,int selectedPosition, int nextPosition, char defaultSymbol, char chessBoard[8][8])
{
    switch ((*selectedPiece)->symbol)
    {
        case 'P':
            int pawnMotion = PawnMotion(selectedPiece, takenPiece, nextPosition);
            if (pawnMotion)
            {
                if(((*selectedPiece)->isWhite == 1 && nextPosition / 10 == 0) || (((*selectedPiece)->isWhite == 0 && nextPosition / 10 == 7)))
                {
                    //Pawn is located on the zone of last rival line
                    PawnPromotion(selectedPiece);
                }
                else if(pawnMotion == 2) //This pawn started by moving two squares forward
                {
                    *enPassantPawn = *selectedPiece;
                }

                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);

                return 1;
            }
            else //pawnMotion is zero but En Passant is possible so we will check it
            {
                if((*enPassantPawn) != NULL)
                {
                    //if distance is sqrt(2)
                    if(sqrt(pow(nextPosition / 10 - (*selectedPiece)->instantPosition[0], 2) + pow(nextPosition % 10 - (*selectedPiece)->instantPosition[1], 2)) == sqrt(2))
                    {
                        int checkingMultiple = (*selectedPiece)->isWhite == 1 ? 1 : -1; 

                        if((nextPosition / 10) * checkingMultiple < ((*enPassantPawn)->instantPosition[0]) * checkingMultiple && (nextPosition / 10) * checkingMultiple > ((*enPassantPawn)->startingPosition[0]) * checkingMultiple)
                        {
                            MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
                            chessBoard[(*enPassantPawn)->instantPosition[0]][(*enPassantPawn)->instantPosition[1]] = defaultSymbol;
                            (*enPassantPawn)->isTaken = 1;
                            printf(YELLOW "EN PASSANT: %c\n" RESET, (*enPassantPawn)->symbol);
                            return 1;
                        }
                    }
                }
            }

            break;
        
        case 'R':         
            if(RookMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
                return 1;
            }

            break;

        case 'H':
            if(HorseMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
                return 1;
            }

            break;
        
        case 'B':
            if(BishopMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
                return 1;
            }
    
            break;

        case 'Q':
            //Calling horse and bishop functions
            if(BishopMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol) || RookMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
                return 1;
            }

            break;

        case 'K':
            if(KingMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                MovePieceAndSetBoard(selectedPiece, chessBoard, nextPosition, defaultSymbol);
                return 1;
            }

            break;
        
        default:
            return 0;
            break;

    }

    return 0;
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

void PrintChessBoard(char board[8][8], int length, char defaultSymbol)
{
    //ClearTerminal();

    printf(RED "  A B C D E F G H\n" RESET);

    for(int i = 0; i < length; i++)
    {
        printf(RED "%d " RESET, i + 1);
        
        for(int j = 0; j < length; j++)
        {
            if(board[i][j] != defaultSymbol)
            {
                if((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0))
                    printf(BackGround CYAN "%c " RESET, board[i][j]);
                else
                    printf(CYAN "%c " RESET, board[i][j]);
            }
            else
            {
                if((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0))
                    printf(BackGround "%c " RESET, board[i][j]);
                else
                    printf("%c ", board[i][j]);    
            }    
        }
        printf("\n");
    }

}

int TakeInputFromGamer(char questionText[])
{
    char inputPos[3]; //Last term is '\0'

    printf("%s\n", questionText);
    scanf("%2s", inputPos);

    return ((int)inputPos[0] - (int)'1') * 10 + ((int)inputPos[1] - (int)'A');
}

void PrintInfo()
{
    printf("---------- I ----------\n");
    printf("Welcome\n");
    printf("To castle, enter the position of the rook on that side for the next move.\n");
    printf("by MyyTimes\n");
    printf("---------- ^ ----------\n");
}


void ClearTerminal()
{
    system("cls"); //Clear terminal for Windows
}
