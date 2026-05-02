#include "ui_win.h"

void ui_win_update_default(ui_win_t* win)
{
	printf("window %d update\n", win->id);
	fflush(stdout);
	if (win->texture) {
		// SDL_UpdateTexture(win->texture, NULL, data, win->w * 4);
	}
	ui_box_t *current = win->menu;
	while (current) {
		if (current->update)
			current->update(current);
		current = current->next;
	}
}

