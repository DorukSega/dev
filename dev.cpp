#include <string>
#include "raylib.h"
#define BUFFER_LIMIT 100000000
#define CURSOR "|"

int FONT_SIZE = 20;
float MARGIN = 10;
int FONT_SPACING = 2;
std::string filename;

struct {  // memory time
    std::string Text = "";

    size_t cursor = 0;

    bool isFull() {
        return Text.size() > BUFFER_LIMIT ? true : false;
    }

    void placeCursor(size_t at) {
        if (Text.size() <= at || at < 0)
            return;
        Text.erase(cursor, 1);
        Text.insert(at, CURSOR);
        cursor = at;
    }
    void placeCursorEnd() {
        Text.append(CURSOR);
        cursor = Text.size() - 1;
    }
    void writeText(std::string text) {
        insertText(text, cursor);
        cursor += text.size();
    }
    void insertText(std::string text, size_t from = 0) {
        if (isFull())
            return;
        // TODO calculate the location of the cursor
        Text.insert(from, text);
    }
    void removeText(size_t from, size_t amount = 1) {
        if (from + amount > Text.size() - 1 || from < 0)
            return;

        // TODO calculate the location of the cursor
        Text.erase(from, amount);
    }
    void eraseText() {
        if (!cursor)
            return;
        removeText(cursor - 1);
        cursor--;
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
    Memory.placeCursorEnd();
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
            Memory.writeText(TextInput);

        if (IsKeyPressed(KEY_BACKSPACE)) {
            Memory.eraseText();
        }

        if (IsKeyPressed(KEY_ENTER)) {
            Memory.writeText("\n");
        }

        if (IsKeyPressed(KEY_TAB)) {
            Memory.writeText("\t");
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            Memory.Text.erase(Memory.cursor, 1);
            SaveFileText(filename.c_str(), Memory.Text.data());
            Memory.Text.insert(Memory.cursor, CURSOR);
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) {
            CloseWindow();
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V)) {
            Memory.writeText(GetClipboardText());
        }

        if (IsKeyPressed(KEY_LEFT))
            Memory.placeCursor(Memory.cursor - 1);
        if (IsKeyPressed(KEY_RIGHT))
            Memory.placeCursor(Memory.cursor + 1);

        BeginDrawing();
        ClearBackground(CLITERAL(Color){17, 17, 17, 255});
        DrawTextPro(GetFontDefault(), Memory.Text.data(),
                    Vector2{MARGIN, MARGIN}, Vector2{0, 0}, 0, FONT_SIZE,
                    FONT_SPACING, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}