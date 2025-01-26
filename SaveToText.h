#include <stdio.h>

FILE *saveText;

char IntToChar(int value)
{
    char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    return letters[value];
}

void WriteMovesToText(int isWhite, int startPos, int endPos, char movedPiece, char takenPiece, char defaultSymbol)
{
    if(saveText != NULL)
    {
        fprintf(saveText, "Player: %s: %c: ", isWhite == 1 ? "White" : "Black", movedPiece);
        fprintf(saveText, "%d%c -> %d%c ", startPos / 10 + 1, IntToChar(startPos % 10), endPos / 10 + 1, IntToChar(endPos % 10));

        if(takenPiece != defaultSymbol)
            fprintf(saveText, "| %c was taken.", takenPiece);

        fprintf(saveText, "\n");
    }
    else
    {
        printf("Error: Unable to open file.\n");
        return;
    }
}

int StartSaving()
{
    if(ftell(saveText) == -1)
    {
        printf("Starting save...\n");

        saveText = fopen("SaveMove.txt", "w");

        if (saveText == NULL) 
        {
            printf("Error: Unable to open file.\n");
            return 1;
        }
    }

    return 0;
}

void CloseSaving()
{
    if(saveText != NULL)
    {
        fclose(saveText);
        saveText = NULL;
    }
}