#include <string>
#include "raylib.h"

struct {  // memory time
    int start = 0;
    std::string Text = "";
    int end = 0;

    bool isFull() { return Text.size() > 100000000 ? true : false; }

    bool insertText(std::string text, int from = NULL, int to = NULL) {
        if (isFull())
            return false;
        if (!from && !to)
            Text += text;
        else if (from > start && to < end)
            Text = Text.substr(0, from) + text + Text.substr(to, Text.npos);
        else
            return false;

        return true;
    }
    void eraseText() { Text = Text.substr(0, Text.size() - 1); }

} Memory;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Dev");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    ClearBackground(CLITERAL(Color){17, 17, 17, 255});

    while (!WindowShouldClose()) {  // detect window closure
        char x[] = {GetCharPressed(), '\0'};
        const double currentTime = GetTime();

        std::string TextInput = x;
        // if (key )
        //     Memory.insertText(std::to_string((char)GetKeyPressed()));
        if (TextInput != "")
            Memory.insertText(TextInput);

        if (IsKeyPressed(KEY_BACKSPACE)) {
            Memory.eraseText();
        }

        if (IsKeyPressed(KEY_ENTER)) {
            Memory.insertText("\n");
        }

        BeginDrawing();
        ClearBackground(CLITERAL(Color){17, 17, 17, 255});

        DrawText(Memory.Text.c_str(), 10, 10, 20, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}