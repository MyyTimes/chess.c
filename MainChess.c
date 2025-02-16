#include <stdio.h>
#include <stdlib.h>
#include "BoardLayoutAndPieces.h"
#include "ColorPaletteForTerminal.h"

int CheckControl(struct chessPiece[], struct chessPiece[], struct chessPiece[], struct chessPiece[], int, int *, char [8][8], char);
int SendPiecesToControlCheckKing(struct chessPiece[], struct chessPiece[], struct chessPiece*, char[8][8], char);
int ControlCheckKing(struct chessPiece *, struct chessPiece *, char [8][8], char);
void CheckKing();

int PieceSwitchFunction(struct chessPiece **, struct chessPiece **, struct chessPiece **, int, char ,char [8][8]);
void FindSelectedPiece(struct chessPiece[], struct chessPiece[], int, struct chessPiece **);
void CreateChessBoard(char[8][8], int, char);
void PrintChessBoard(char[8][8], int, char);
int TakeInputFromGamer(char[]);
void PrintInfo();
void ClearTerminal();

int main()
{
    PrintInfo();
    
    int isGameOver = 0;
    char chessBoard[8][8];
    int lengthBoard = sizeof(chessBoard) / sizeof(chessBoard[0]); //Length -> x = y
    char defaultSymbol = 'O';
    int isWhiteTurn = 1;
    int kingsIndex = 7;

    //Prepare the default board
    CreateChessBoard(chessBoard, lengthBoard, defaultSymbol);

    //WHITE PIECES
    struct chessPiece whitePawns[8];

    struct chessPiece whiteMajorPieces[] = 
    {
        {{7, 0}, {7, 0}, 'R', 1, 70, 1, 0}, //rightRook
        {{7, 7}, {7, 7}, 'R', 1, 77, 1, 0}, //leftRook
        {{7, 1}, {7, 1}, 'H', 1, 71, 1, 0}, //rightHorse
        {{7, 6}, {7, 6}, 'H', 1, 76, 1, 0}, //leftHorse
        {{7, 2}, {7, 2}, 'B', 1, 72, 1, 0}, //rightBishop
        {{7, 5}, {7, 5}, 'B', 1, 75, 1, 0}, //leftBishop
        {{7, 3}, {7, 3}, 'Q', 1, 73, 1, 0}, //queen
        {{7, 4}, {7, 4}, 'K', 1, 74, 1, 0}  //king
    };

    //BLACK PIECES
    struct chessPiece blackPawns[8];

    struct chessPiece blackMajorPieces[] = 
    {
        {{0, 0}, {0, 0}, 'r', 1, 00, 0, 0}, //rightRook
        {{0, 7}, {0, 7}, 'r', 1, 07, 0, 0}, //leftRook
        {{0, 1}, {0, 1}, 'h', 1, 01, 0, 0}, //rightHorse
        {{0, 6}, {0, 6}, 'h', 1, 06, 0, 0}, //leftHorse
        {{0, 2}, {0, 2}, 'b', 1, 02, 0, 0}, //rightBishop
        {{0, 5}, {0, 5}, 'b', 1, 05, 0, 0}, //leftBishop
        {{0, 3}, {0, 3}, 'q', 1, 03, 0, 0}, //queen
        {{0, 4}, {0, 4}, 'k', 1, 04, 0, 0}   //king
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
        CreatePawns(&whitePawns[i], 60 + i, 'P');
        LocatePieceOnBoard(&whitePawns[i], chessBoard);

        blackPawns[i].isWhite = 0;
        CreatePawns(&blackPawns[i], 10 + i, 'p');
        LocatePieceOnBoard(&blackPawns[i], chessBoard);
    }
 
    int selectedPosition;
    int nextPosition;

    struct chessPiece *selectedPiece = (struct chessPiece *)malloc(sizeof(struct chessPiece));
    struct chessPiece *takenPiece = (struct chessPiece *)malloc(sizeof(struct chessPiece));
    selectedPiece = NULL;
    takenPiece = NULL;

    struct chessPiece *enPassantPawn = (struct chessPiece *)malloc(sizeof(struct chessPiece));
    enPassantPawn = NULL;

    while(isGameOver == 0)
    {
        PrintChessBoard(chessBoard, lengthBoard, defaultSymbol); 

        if(isWhiteTurn)
            printf(YELLOW "Turn of WHITE\n" RESET);
        else
            printf(YELLOW "Turn of BLACK\n" RESET);

        //Take input from user to play
        //Select piece to move from-
        selectedPosition = TakeInputFromGamer("Select your piece: ");

        //UNDO
        if(selectedPosition == 373) //Entering "UN" -> (int)373
        {
            if(Undo(chessBoard, defaultSymbol))
            {
                isWhiteTurn = !isWhiteTurn;
                printf(GREEN "Undoing the move was successful.\n" RESET);

                continue;
            }
            else
            {
                printf(RED "Undoing the move was unsuccessful!\n" RESET);
                continue;
            }
        }

        //Select piece to move to-
        nextPosition = TakeInputFromGamer("Input its next position: ");

        //Setting free other pointers
        selectedPiece = NULL;
        takenPiece = NULL;

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

        if(selectedPiece == NULL)
        {
            printf(RED "There is no piece in that location!\n" RESET);
            continue;
        }

        //ROK      
        if((selectedPiece->symbol == 'K' || selectedPiece->symbol == 'k') && selectedPiece->firstMove == 1)
        {
            //To castling, the king must not be threatened
            if(CheckControl(whitePawns, blackPawns, whiteMajorPieces, blackMajorPieces, kingsIndex, &isWhiteTurn, chessBoard, defaultSymbol) != 2)
            {
                if(isWhiteTurn)  //FIND ROOK FOR WHITE
                    FindSelectedPiece(whitePawns, whiteMajorPieces, nextPosition, &takenPiece);
                else //FIND ROOK FOR BLACK
                    FindSelectedPiece(blackPawns, blackMajorPieces, nextPosition, &takenPiece);

                if(takenPiece != NULL && (takenPiece->symbol == 'R' || takenPiece->symbol == 'r') && takenPiece->firstMove)
                {
                    if(nextPosition / 10 == selectedPiece->startingPosition[0] && ((nextPosition % 10 == 0) || (nextPosition % 10 == 7)))
                    {
                        if(Rok(&selectedPiece, &takenPiece, chessBoard, defaultSymbol))
                        {
                            isWhiteTurn = !isWhiteTurn;

                            CreateNode(selectedPiece, takenPiece, selectedPosition, nextPosition);

                            continue;
                        }
                    }
                }
            }
        }
    
        if(PieceSwitchFunction(&selectedPiece, &takenPiece, &enPassantPawn, nextPosition, defaultSymbol, chessBoard))
        {
            MovePieceAndSetBoard(&selectedPiece, chessBoard, nextPosition, defaultSymbol);

            isWhiteTurn = !isWhiteTurn;

            if(enPassantPawn != NULL && (enPassantPawn->isWhite == isWhiteTurn))
                enPassantPawn = NULL; //RESET en passant pawn

            CreateNode(selectedPiece, takenPiece, selectedPosition, nextPosition);
            
            if(CheckControl(whitePawns, blackPawns, whiteMajorPieces, blackMajorPieces, kingsIndex, &isWhiteTurn, chessBoard, defaultSymbol));
                continue; //ILLEGAL MOVE
        }
        else
        {
            printf(RED "ILLEGAL MOVE!\n" RESET);
        }
    }

    if(StartSaving())
        return 0; 
    //SaveDataToText(defaultSymbol);
    SaveDataReverseToText(lastMove, defaultSymbol);
    CloseSaving();
    printf("Saving completed!\n");
    
    //Set free - malloc
    free(selectedPiece);
    free(takenPiece);
    free(enPassantPawn);

    return 0;
}

int CheckControl(struct chessPiece wPawns[], struct chessPiece bPawns[], struct chessPiece wMajors[], struct chessPiece bMajors[], int kingsIndex, int *isWhiteTurn, char board[8][8], char defaultSymbol)
{
    if(*isWhiteTurn == 0)
    {
        //First control is for "ILLEGAL MOVE"

        if(SendPiecesToControlCheckKing(bPawns, bMajors, &wMajors[kingsIndex], board, defaultSymbol))
        {
            printf(RED "ILLEGAL MOVE\n" RESET);

            if(Undo(board, defaultSymbol))
            {
                *isWhiteTurn = !(*isWhiteTurn);
                printf(GREEN "Undoing the move was successful.\n" RESET);
                
                return 1; //continue;
            }
            else
            {
                printf(RED "Undoing the move was unsuccessful!\n" RESET);

                return 1; //continue;
            }
        }
        else if(SendPiecesToControlCheckKing(wPawns, wMajors, &bMajors[kingsIndex], board, defaultSymbol))
        {
            CheckKing();
            return 2; //Check
        }      
    }
    else
    {
        if(SendPiecesToControlCheckKing(wPawns, wMajors, &bMajors[kingsIndex], board, defaultSymbol))
        {
            printf(RED "ILLEGAL MOVE\n" RESET);

            if(Undo(board, defaultSymbol))
            {
                *isWhiteTurn = !(*isWhiteTurn);
                printf(GREEN "Undoing the move was successful.\n" RESET);

                return 1; //continue;
            }
            else
            {
                printf(RED "Undoing the move was unsuccessful!\n" RESET);

                return 1; //continue;
            }
        }
        else if(SendPiecesToControlCheckKing(bPawns, bMajors, &wMajors[kingsIndex], board, defaultSymbol))
        {
            CheckKing();
            return 2; //Check
        }
    }

    return 0;
}

int SendPiecesToControlCheckKing(struct chessPiece pawns[], struct chessPiece majors[], struct chessPiece *king, char board[8][8], char defaultSymbol)
{
    for(int i = 0; i < 8; i++)
    {            
        if(pawns[i].isTaken == 0 && ControlCheckKing(&pawns[i], king, board, defaultSymbol))
        {
            return 1;
        }
        if(majors[i].isTaken == 0 && ControlCheckKing(&majors[i], king, board, defaultSymbol) && (majors[i].symbol != 'k' && majors[i].symbol != 'K'))
        {
            return 1;
        }
    }

    return 0;
}

int ControlCheckKing(struct chessPiece *lastMovedPiece, struct chessPiece *king, char board[8][8], char defaultSymbol)
{
    int kingPosition = king->instantPosition[0] * 10 + king->instantPosition[1];

    if(lastMovedPiece->symbol == 'P' || lastMovedPiece->symbol == 'p')
    {
        if(PawnMotion(&lastMovedPiece, &king, kingPosition, board, defaultSymbol) == 3)
        {
            return 1;
        }
    }
    else if(PieceSwitchFunction(&lastMovedPiece, &king, NULL, kingPosition, defaultSymbol, board))
    {
        return 1;
    }

    return 0;
}

void CheckKing()
{
    printf(GREEN "CHECK!\n" RESET);
}

int PieceSwitchFunction(struct chessPiece **selectedPiece, struct chessPiece **takenPiece, struct chessPiece **enPassantPawn, int nextPosition, char defaultSymbol, char chessBoard[8][8])
{
    switch ((*selectedPiece)->symbol)
    {
        case 'P':
        case 'p':
            int pawnMotion = PawnMotion(selectedPiece, takenPiece, nextPosition, chessBoard, defaultSymbol);
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
                            *takenPiece = *enPassantPawn;
                            return 1;
                        }
                    }
                }
            }

            break;
        
        case 'R':  
        case 'r':       
            if(RookMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                return 1;
            }

            break;

        case 'H':
        case 'h':
            if(HorseMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                return 1;
            }

            break;
        
        case 'B':
        case 'b':
            if(BishopMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                return 1;
            }
    
            break;

        case 'Q':
        case 'q':
            //Calling horse and bishop functions
            if(BishopMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol) || RookMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
                return 1;
            }

            break;

        case 'K':
        case 'k':
            if(KingMotion(selectedPiece, takenPiece, chessBoard, nextPosition, defaultSymbol))
            {
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

    printf(BRED "  A B C D E F G H\n" RESET);

    for(int i = 0; i < length; i++)
    {
        printf(BRED "%d " RESET, i + 1);
        
        for(int j = 0; j < length; j++)
        {
            if(board[i][j] != defaultSymbol)
            {                    
                if((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0))
                    printf("%s%c " RESET, board[i][j] >= 'a' ? BackGround MAGENTA : BackGround CYAN, board[i][j]);
                else
                    printf("%s%c " RESET, board[i][j] >= 'a' ? MAGENTA : CYAN, board[i][j]);  
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
    printf("To undo, enter \"UN\".\n");
    printf("by MyyTimes\n");
    printf("---------- ^ ----------\n");
}

void ClearTerminal()
{
    system("cls"); //Clear terminal for Windows
}
