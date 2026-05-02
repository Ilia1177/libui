#include "ui_win.h"

void ui_win_update_default(ui_win_t* win, SDL_Event*e, void* data)
{
	ui_box_t *current = win->menu;
	while (current) {
		if (current->update)
			current->update->fn(current, e, data);
		current = current->next;
	}
}

