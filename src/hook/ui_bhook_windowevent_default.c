#include "ui_win.h" // For ui_box_t, ui_win_t, SDL_Event
#include "ui_box.h" // For ui_bhook_fire

void	ui_bhook_windowevent_default(ui_box_t *b, SDL_Event* e, void* data)
{
	ui_box_t *curr = b->list;
	while(curr) {
		if(curr->on_window_event)
            ui_bhook_fire(curr->on_window_event, curr, e, data);
		curr = curr->next;
	}
}