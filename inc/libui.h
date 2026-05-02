#ifndef LIBUI_H
# define LIBUI_H

# include <stdbool.h> // New: Include stdbool.h for 'bool' type
# include <SDL2/SDL_image.h>
# include <SDL2/SDL.h>

typedef struct ui_box_s ui_box_t;
typedef struct ui_win_s ui_win_t;

typedef struct ui_scale_s {
    float x;
    float y;
} ui_scale_t;

typedef struct ui_pos_s {
	int x;
	int y;
} ui_pos_t;

typedef struct ui_rgba_s {
	int r;
	int g;
	int b;
	int a;
} ui_rgba_t;

bool ui_is_mouse_in(ui_box_t*);
int ui_init();
int ui_quit();

# include "ui_win.h"
# include "ui_global.h"
#endif
