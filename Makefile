r: main.c termbox.h # run
	clang main.c -O1 -Werror -Wall -Wextra -o bin/dev && clear && ./bin/dev

b: main.c termbox.h # build
	clang main.c -O1 -Werror -Wall -Wextra -o bin/dev

