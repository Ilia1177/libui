

typedef struct ui_box_s {
	SDL_Rectangle rect;
	ui_rgba_t color;
} ui_box_t;

void ui_windraw_box(ui_win_t *win, ui_box_t box);
