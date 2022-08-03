#include "raylib.h"
#include "stdio.h"
#include "string.h"  //string related
#define BUFFER_LIMIT 100000000

int FONT_SIZE = 20;
float MARGIN = 10;
int FONT_SPACING = 2;

char filenames[100][100];
char files[1][BUFFER_LIMIT];
int FileCount = 0;
int FileFocused = 0;

Vector2 cursor = {0, 0};
size_t cursorIndex = 0;

enum STATES { FIRST, EDIT, SEARCH };
int STATE = FIRST;

void eraseText(size_t amount) {
    size_t index = strlen(files[FileFocused]) - 1;
    for (size_t i = 0; i < amount || (index - i < 0); i++) {
        files[FileFocused][index - i] = 0;
    }
}

void inputChar(char inp) {
    if (!inp)
        return;
    char* TextInput = &inp;
    strncat(files[FileFocused], TextInput, 1);
}

void inputText(const char* text) {
    strncat(files[FileFocused], text, strlen(text));
}

size_t count(const char text[], char a) {
    size_t count = 0;
    size_t i = 0;
    while (text[i] != '\0') {
        if (text[i] == a)
            count++;
        i++;
    }
    return count;
}

Vector2 getLocation(size_t location) {
    char text[location];
    for (size_t i = 0; i < location; i++)
        text[i] = files[FileFocused][i];

    const char* lastNL = strrchr(text, '\n');
    if (lastNL == NULL)
        lastNL = text;

    return (Vector2){MARGIN + MeasureText(lastNL, FONT_SIZE) + FONT_SPACING,
                     (count(text, '\n') * (FONT_SIZE + MARGIN)) + MARGIN};
}

void eofCursor() {
    const char* lastNL = strrchr(files[FileFocused], '\n');
    if (lastNL == NULL)
        lastNL = files[FileFocused];

    cursor.x = MARGIN + MeasureText(lastNL, FONT_SIZE) + FONT_SPACING;
    cursor.y =
        (count(files[FileFocused], '\n') * (FONT_SIZE + MARGIN)) + MARGIN;
    cursorIndex = strlen(files[FileFocused]);

    // cursor = getLocation(strlen(files[FileFocused]));
}

void placeCursor(size_t at) {
    cursor = getLocation(at);
    cursorIndex = at;
}
int main(int argc, const char* argv[]) {
    if (argc > 1) {
        if (FileExists(argv[1])) {
            strcpy(filenames[FileCount], argv[1]);
            strcpy(files[FileCount], LoadFileText(argv[1]));
            FileFocused = FileCount++;
        } else
            SaveFileText(argv[1], "");
    }

    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Dev");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {  // detect window closure
        char inp = (char)GetCharPressed();
        inputChar(inp);

        if (IsKeyPressed(KEY_BACKSPACE))
            eraseText(1);

        if (IsKeyPressed(KEY_ENTER))
            inputChar('\n');

        if (IsKeyPressed(KEY_TAB))
            inputChar('\t');

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) &&
            IsKeyPressed(KEY_Q)) {
            // TODO save last state
            exit(1);
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V)) {
            const char* clipboard = GetClipboardText();
            if (clipboard)
                inputText(clipboard);
        }

        if (IsKeyPressed(KEY_LEFT))
            placeCursor(cursorIndex - 1);
        if (IsKeyPressed(KEY_RIGHT))
            placeCursor(cursorIndex + 1);

        BeginDrawing();

        ClearBackground(CLITERAL(Color){17, 17, 17, 255});
        // DrawText(files[FileFocused], MARGIN, MARGIN, FONT_SIZE, WHITE);
        DrawTextEx(GetFontDefault(), files[FileFocused],
                   (Vector2){MARGIN, MARGIN}, FONT_SIZE, FONT_SPACING, WHITE);

        if (STATE == FIRST) {
            eofCursor();
            STATE = EDIT;
        }

        DrawRectangle(cursor.x, cursor.y, 1, FONT_SIZE, WHITE);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
