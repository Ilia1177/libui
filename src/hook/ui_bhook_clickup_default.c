#include "ui_win.h" // For ui_box_t, ui_win_t, SDL_Event
#include "ui_box.h" // For ui_bhook_fire

void	ui_bhook_clickup_default(ui_box_t *b, SDL_Event* e, void* data)
{
    b->flags &= ~BOX_PRESSED;
	ui_box_t *curr = b->childs;
	while(curr) {
		if(curr->on_click_up)
			ui_bhook_fire(curr->on_click_up, curr, e, data);
		curr = curr->next;
	}
}
