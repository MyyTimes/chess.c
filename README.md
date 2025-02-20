# chess.c

[UNDER DEVELOP]

A simple chess game implementation in C.

# Main Function:
The main function is located in the `MainChess.c` file, which is the entry point of the program. Other header files are also included via `HeaderFiles.h`.

# Features:
- All chess pieces and their moves are implemented.
- Ability to play chess with a friend locally.
- Automatically saves game steps to a text file at the end of the game.
- Some chess dynamics such as en passant and pawn promotion are implemented.
- Undo moves feature

# How to Play:
Starting with White, players take turns inputting the position of the piece they want to move and the position they want to move it to. If the move is valid, it is executed; otherwise, the player is prompted again. Positions are selected according to the game board, where rows are numbered vertically, and columns are labeled with letters horizontally. When entering a move, the row comes first, followed by the column (e.g., 7A, 3C). Additionally, at the beginning of the game, other game-related commands are displayed on the screen.

# Contact:
For any inquiries or issues, feel free to reach out to me at [muhammedyusufy@gmail.com].
