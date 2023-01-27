#define TB_IMPL
#include "termbox.h"
#include <stdio.h>

#define FILE_SIZE 1024

typedef struct {
    unsigned int x, y;
} cursor;

typedef struct {
    char data[FILE_SIZE];
    cursor cur;
} efile;

void draw_screen(efile *ses);

int write2cur(efile *ses, char *text, size_t size);

int main() {
    struct tb_event ev;
    efile base = (efile) {0};
    tb_init();
        unsigned char live = 1, change = 0; // boolmongus

        tb_printf(0, 0, TB_GREEN, 0, "hello from dev");
        tb_printf(0, 1, 0, 0, "width=%d height=%d", tb_width(), tb_height());
        tb_present();

        while (live) {
            tb_poll_event(&ev);

            switch(ev.type) {
                case TB_EVENT_KEY:
                    if (ev.key){
                        switch(ev.key){
                            case TB_KEY_ESC:
                                live = 0;
                                break;
                        }
                    } else if (ev.ch){
                        switch(ev.ch){
                            default:
                                base.data[0] = ev.ch;
                                change = 1;
                        }
                    }
                    break;
                case TB_EVENT_RESIZE:
                    break;
                case TB_EVENT_MOUSE:
                    break;
                default:
                    tb_shutdown();
                    fprintf(stderr, "%s:%d ERROR: invalid event type.\n", __FILE__, __LINE__);
                    exit(1);
            } // end switch(ev.type)

            if (change) {
                change = 0;
                draw_screen(&base);
            }
        } // end while

   
    tb_shutdown();

    return 0;
}

// draws screen
void draw_screen(efile *ses){
    tb_clear();
    int xnum = 0;
    int line = 0;
    for (int i = 0; i < FILE_SIZE; i++) {
        const char ch = ses->data[i];
        if (ch == 0)
            break;
        if (ch== '\n') {
            line++;
            xnum = 0;
            continue;
        }

        tb_printf(tb_width()-10, tb_height()-1, 0, 0, "%d:%d:%d", xnum, line, i);
        tb_print(xnum, line, 0, 0, &ch);
        tb_present();
        xnum++;
    }
    tb_set_cursor(ses->cur.x, ses->cur.y);
}

int write2cur(efile *ses, char *text, size_t size) {
    if (size < 1) {
        tb_shutdown();
        fprintf(stderr, "%s:%d ERROR: invalid write size.\n", __FILE__, __LINE__);
        exit(1);
    }
    unsigned int nline = ses->cur.y;

    for (int i = 0; i < FILE_SIZE; ++i) {
        const char ch = ses->data[i];
        
        if (ch == '\n' && nline)
            nline--;
        else if (!nline)
             
        else if (!ch)

    }
}