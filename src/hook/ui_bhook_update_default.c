#include "ui_win.h" // For ui_box_t, ui_win_t, SDL_Event
#include "ui_box.h" // For ui_bhook_fire, BOX_CLICKED

void ui_bhook_update_default(ui_box_t* box, SDL_Event *e, void *data)
{
	ui_box_t *current = box->list;
	box->flags &= ~BOX_CLICKED;
	while(current) {
		if (current->update) {
			ui_bhook_fire(current->update, current, e, data);
		}
		current = current->next;
	}
}