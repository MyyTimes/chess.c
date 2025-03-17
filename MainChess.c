#include "HeaderFiles.h"

int SetActiveRotativeBoard();
void PrintChessBoard(char[8][8], int, int);
int TakeInputFromGamer(char[]);
void PrintInfo();
void ClearTerminal();

int main()
{    
    PrintInfo();

    int gameResult = 0; // 1-> White 2-> Black 3-> Stalemate
    int isGameOver = 0;
    int isCheck = 0;
    char chessBoard[8][8];
    char tempBoard[8][8];
    int isWhiteTurn = 1;
    int kingsIndex = 7;
    
    int isRotationEnable = SetActiveRotativeBoard();

    //Prepare the default board
    CreateChessBoard(chessBoard);
    CreateChessBoard(tempBoard);

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
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        LocatePieceOnBoard(&whiteMajorPieces[i], chessBoard);
        LocatePieceOnBoard(&blackMajorPieces[i], chessBoard);
    }

    //Create Pawns
    for(int i = 0; i < BOARD_SIZE; i++)
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

    struct chessPiece *enPassantPawn = (struct chessPiece *)malloc(sizeof(struct chessPiece));
    enPassantPawn = NULL;
    
    //LOAD GAME -- CHANGE IT LATER
    if(!LoadGameFromTextFile(whitePawns, blackPawns, whiteMajorPieces, blackMajorPieces, chessBoard))
    {
        printf(RED "There is a problem with the file!\n" RESET);
    }

    while(isGameOver == 0)
    {
        //Setting free other pointers
        selectedPiece = NULL;
        takenPiece = NULL;

        ResetTempBoard(tempBoard, chessBoard); // tempBoard = chessBoard
        PrintChessBoard(chessBoard, isWhiteTurn, isRotationEnable); 

        // CHECK KING - ILLEGAL MOVE OR CHECK
        CheckKings(whitePawns, blackPawns, whiteMajorPieces, blackMajorPieces, &isWhiteTurn, &isCheck, chessBoard);

        // Checkmate - Stalemate
        if(isWhiteTurn == 0)
        {
            if(CheckmateControl(blackPawns, blackMajorPieces, chessBoard))
            {
                if(isCheck)
                {
                    printf(BGREEN "WHITE wins the game!\n" RESET);
                    gameResult = 1;
                    isGameOver = 1;
                    continue;
                }
                else //Stalemate
                {
                    printf(BWHITE "It's a stalemate!\n" RESET);
                    gameResult = 3;
                    isGameOver = 1;
                    continue;
                }     
            }
        }
        else
        {
            if(CheckmateControl(whitePawns, whiteMajorPieces, chessBoard))
            {
                if(isCheck)
                {
                    printf(BGREEN "BLACK wins the game!\n" RESET);
                    gameResult = 2;
                    isGameOver = 1;
                    continue;
                }
                else //Stalemate
                {
                    printf(BWHITE "It's a stalemate!\n" RESET);
                    gameResult = 3;
                    isGameOver = 1;
                    continue;
                }
            }
        }

        if(isWhiteTurn)
            printf(YELLOW "Turn of WHITE\n" RESET);
        else
            printf(YELLOW "Turn of BLACK\n" RESET);

        //Take input from user to play
        //Select piece to move from-
        selectedPosition = TakeInputFromGamer("Select your piece: ");

        switch (selectedPosition)
        {
            case 373: //Entering "UN" -> (int)373
                if(Undo(chessBoard))
                {
                    isWhiteTurn = !isWhiteTurn;
                    printf(GREEN "Undoing the move was successful.\n" RESET);
                }
                else
                {
                    printf(RED "Undoing the move was unsuccessful!\n" RESET);
                }
                continue;

            //Entering "WW" -> (int)402 | "BW" -> (int)192 | "SM" -> (int)352
            case 403:
            case 192:
            case 352:
                gameResult = (selectedPosition == 403) ? 1 : (selectedPosition == 192) ? 2 : 3;
                isGameOver = 1;
                continue;

            case 253: 
                PrintInfo();
                continue;

            default:
                break;
        }

        //Find selected piece
        if(isWhiteTurn)
        {
            FindSelectedPiece(whitePawns, whiteMajorPieces, selectedPosition, &selectedPiece);
        }
        else
        {
            FindSelectedPiece(blackPawns, blackMajorPieces, selectedPosition, &selectedPiece);
        }

        if(selectedPiece == NULL)
        {
            ClearTerminal();
            printf(RED "There is no piece in that location!\n" RESET);
            continue;
        }

        //CASTLING CONTROL
        if((selectedPiece->symbol == 'k' || selectedPiece->symbol == 'K') && selectedPiece->firstMove)
        {
            if(CheckKings(whitePawns, blackPawns, whiteMajorPieces, blackMajorPieces, &isWhiteTurn, &isCheck, chessBoard) == 0)
            {
                int castlingPos = selectedPiece->instantPosition[0] * 10 + BOARD_SIZE - 1;
                MarkCastling(selectedPiece, selectedPiece->isWhite ? &whiteMajorPieces[0] : &blackMajorPieces[0], castlingPos, tempBoard); // RIGHT ROOK
                castlingPos = selectedPiece->instantPosition[0] * 10 + 0;
                MarkCastling(selectedPiece, selectedPiece->isWhite ? &whiteMajorPieces[1] : &blackMajorPieces[1], castlingPos, tempBoard); // LEFT ROOK
            }
        }

        FindFunctionBySymbol(selectedPiece, enPassantPawn, tempBoard);

        PrintChessBoard(tempBoard, isWhiteTurn, isRotationEnable);

        //Select piece to move to XX
        nextPosition = TakeInputFromGamer("Input its next position: ");

        //Find what piece is in next position -> it will be taken if it is possible
        //NOTE: I send rival pieces, so there's no need to check whether they are takeable due to rivalry.
        FindSelectedPiece((isWhiteTurn ? blackPawns : whitePawns), (isWhiteTurn ? blackMajorPieces : whiteMajorPieces), nextPosition, &takenPiece);
        
        if(takenPiece == NULL)
        {
            printf("No target piece\n");
        }
        else
        {
            printf("Target piece symbol: %c\n", takenPiece->symbol);
        }

        //VALID MOVE - MAKE THE MOVE
        if(tempBoard[nextPosition / 10][nextPosition % 10] == BOARD_MARKED_SYMBOL)
        {       
            //CASTLING
            if((selectedPiece->symbol == 'k' || selectedPiece->symbol == 'K') && selectedPiece->firstMove)
            {    
                if(nextPosition / 10 == selectedPiece->startingPosition[0] && ((nextPosition % 10 == 0) || (nextPosition % 10 == 7)))
                {
                    // Find rook
                    FindSelectedPiece(NULL, isWhiteTurn ? whiteMajorPieces : blackMajorPieces, nextPosition, &takenPiece);

                    int kingNewXPos = selectedPiece->instantPosition[1] > takenPiece->instantPosition[1] ? 2 : 6;
                    int rookNewXPos = selectedPiece->instantPosition[1] > takenPiece->instantPosition[1] ? 3 : 5;
                    MovePieceAndSetBoard(&selectedPiece, chessBoard, selectedPiece->instantPosition[0] * 10 + kingNewXPos);
                    MovePieceAndSetBoard(&takenPiece, chessBoard, takenPiece->instantPosition[0] * 10 + rookNewXPos);

                    selectedPiece->firstMove = 0;
                    takenPiece->firstMove = 0;
                    selectedPiece->firstMovePosition = selectedPiece->instantPosition[0] * 10 + selectedPiece->instantPosition[1];
                    takenPiece->firstMovePosition = takenPiece->instantPosition[0] * 10 + takenPiece->instantPosition[1];

                    isWhiteTurn = !isWhiteTurn;
                    CreateNode(selectedPiece, takenPiece, selectedPosition, nextPosition);

                    ClearTerminal();
                    continue;    
                }
            }
            
            if(enPassantPawn != NULL)
            {
                if(nextPosition / 10 + (enPassantPawn->isWhite == 1 ? -1 : 1) == enPassantPawn->instantPosition[0] && nextPosition % 10 == enPassantPawn->instantPosition[1])
                {       
                    //TAKE EN PASSANT
                    SetIsTaken(&enPassantPawn, 1);
                    chessBoard[enPassantPawn->instantPosition[0]][enPassantPawn->instantPosition[1]] = BOARD_DEFAULT_SYMBOL;
                    
                    takenPiece = enPassantPawn;
                }
                
                if(enPassantPawn->isWhite == !isWhiteTurn)
                   enPassantPawn = NULL; //RESET en passant pawn
            }            

            MovePieceAndSetBoard(&selectedPiece, chessBoard, nextPosition);

            //En Passant
            if((selectedPiece->symbol == 'P' || selectedPiece->symbol == 'p'))
            {
                if((abs(selectedPosition / 10 - nextPosition / 10) == 2))
                    enPassantPawn = selectedPiece;

                if((selectedPiece->isWhite == 1 && nextPosition / 10 == 0) || ((selectedPiece->isWhite == 0 && nextPosition / 10 == BOARD_SIZE - 1)))
                {
                    //Pawn is located on the zone of last rival line
                    PawnPromotion(&selectedPiece);
                }
            }

            if(takenPiece != NULL && takenPiece->isWhite != isWhiteTurn) // Second condition is for CASTLING
            {
                if(nextPosition / 10 == takenPiece->instantPosition[0] && nextPosition % 10 == takenPiece->instantPosition[1])
                {
                    SetIsTaken(&takenPiece, 1);
                }
            }
            
            if(selectedPiece->firstMove == 1)
            {
                selectedPiece->firstMove = 0;
                selectedPiece->firstMovePosition = nextPosition;
            }
            
            isWhiteTurn = !isWhiteTurn;

            CreateNode(selectedPiece, takenPiece, selectedPosition, nextPosition);
            ClearTerminal();
        }
        else
        {
            ClearTerminal();
            printf(RED "INVALID MOVE\n" RESET);
        }
    }

    if(StartSaving())
        return 0; 
    //SaveDataToText(defaultSymbol);
    SaveDataReverseToText(lastMove);
    WriteResultToText(gameResult);
    CloseSaving();
    printf("Saving completed!\n");
    
    //Set free - malloc
    free(selectedPiece);
    free(takenPiece);
    free(enPassantPawn);

    return 0;
}

int SetActiveRotativeBoard()
{
    printf("Return to the board every round? (%sY%s/%sN%s)", GREEN, RESET, RED, RESET);

    char responce;

    scanf(" %c", &responce);

    while (getchar() != '\n');

    responce = (responce >= 'a' && responce <= 'z') ? responce - 'a' + 'A' : responce;

    if (responce == 'Y') 
        return 1;
    else if (responce == 'N')
        return 0;
    else 
    {
        printf("Invalid response. Please enter Y or N.\n");
        return SetActiveRotativeBoard();
    }
}

void PrintChessBoard(char board[8][8], int isWhiteTurn, int isRotationEnabled)
{
    int startPosY = 0;
    int endPosY = BOARD_SIZE;

    if(isWhiteTurn != 1 && isRotationEnabled == 1)
    {
        startPosY = BOARD_SIZE - 1;
        endPosY = -1;

        printf(BRED "  H G F E D C B A\n" RESET);
    }
    else
        printf(BRED "  A B C D E F G H\n" RESET);

    for(int i = startPosY; i != endPosY; i += ((isWhiteTurn != 1 && isRotationEnabled == 1) ? -1 : 1))
    {
        printf(BRED "%d " RESET, i + 1);
        
        for(int j = startPosY; j != endPosY; j += ((isWhiteTurn != 1 && isRotationEnabled == 1) ? -1 : 1))
        {
            if(board[i][j] == BOARD_MARKED_SYMBOL)
            {
                if((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0))
                    printf(BackGround GREEN "%c " RESET, board[i][j]);
                else
                    printf(GREEN "%c " RESET, board[i][j]); 
            }
            else if(board[i][j] != BOARD_DEFAULT_SYMBOL)
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
    printf(BRED "---------- I ----------\n" RESET);
    printf(BYELLOW "Welcome\n" RESET);
    printf("To castle, enter the position of the rook on that side for the next move.\n");
    printf("To undo, enter \"%sUN%s\".\n", YELLOW, RESET);
    printf("Entering \"%sWW%s\": White wins | \"%sBW%s\": Black wins || \"%sSM%s\": Stalemate\n", YELLOW, RESET, YELLOW, RESET, YELLOW, RESET);
    printf("Enter \"%sIN%s\" to see this panel again! \n", YELLOW, RESET);
    printf(BackGround "by MyyTimes\n" RESET);
    printf(BRED "---------- ^ ----------\n" RESET);
}

void ClearTerminal()
{
    //printf("%s", CLEAR_SCREEN);
    system("cls"); //Clear terminal for Windows
}