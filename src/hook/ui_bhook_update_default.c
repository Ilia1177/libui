#include "libui.h"
#include "ui_win.h"
#include "ui_box.h"

void ui_bhook_update_default(ui_box_t* box, SDL_Event *e, void *data)
{
	(void)data;
	ui_layer_t* curr = box->layers;
	// if (!(box->flags & BOX_STALE))
	// 	return;
	while(curr) {
		if(curr->state& LAYER_STALE && curr->filters) {
		printf("window: %d, update box layer\n", box->parent_window->id);
			ui_filter_fire(curr->filters, curr, e, curr->filters_data);
		}
		curr->state &= ~LAYER_STALE;
		curr = curr->next;
	}
}
