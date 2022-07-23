#include <string>
#include "raylib.h"
#define BUFFER_LIMIT 100000000
#define CURSOR "_"

int FONT_SIZE = 20;
float MARGIN = 10;
int FONT_SPACING = MARGIN / 2;
std::string filename;

struct {  // memory time
    int start = 0;
    std::string Text = "";
    int end = 0;

    bool isFull() {
        return Text.size() > BUFFER_LIMIT ? true : false;
    }

    bool insertText(std::string text, int from = 0, int to = 0) {
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

    void eraseText() {
        Text = Text.substr(0, Text.size() - 1);
    }

    int count(char chr) {  // counts the occurance of a char
        int count = 0;
        for (char& c : Text)
            if (c == chr)
                count++;
        return count;
    }
} Memory;

void loadFile(std::string& fname) {
    char* text = LoadFileText(fname.c_str());
    Memory.insertText(text);
    UnloadFileText(text);
    delete[] text;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        filename = argv[1];
        if (FileExists(filename.c_str()))
            loadFile(filename);
        else
            SaveFileText(filename.c_str(), Memory.Text.data());
    }

    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Dev");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    ClearBackground(CLITERAL(Color){17, 17, 17, 255});

    while (!WindowShouldClose()) {  // detect window closure
        char x[] = {(char)GetCharPressed(), '\0'};
        // const float currentTime = GetFrameTime();

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

        if (IsKeyPressed(KEY_TAB)) {
            Memory.insertText("\t");
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            SaveFileText(filename.c_str(), Memory.Text.data());
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) {
            CloseWindow();
        }

        BeginDrawing();
        ClearBackground(CLITERAL(Color){17, 17, 17, 255});
        DrawTextPro(GetFontDefault(), (Memory.Text + CURSOR).data(),
                    Vector2{MARGIN, MARGIN}, Vector2{0, 0}, 0, FONT_SIZE,
                    FONT_SPACING, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}