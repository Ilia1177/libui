#include "ui_win.h" // For ui_box_t, ui_win_t, SDL_Event
#include "ui_box.h" // For ui_bhook_fire, BOX_CLICKED


void ui_layer_update(ui_layer_t* layer, float zoom) 
{
    int cx = layer->area.x + layer->area.w / 2;
    int cy = layer->area.y + layer->area.h / 2;
	layer->area.w = (int)(layer->area.w * zoom);
    layer->area.h = (int)(layer->area.h * zoom);
    layer->area.x = cx - layer->area.w / 2;
    layer->area.y = cy - layer->area.h / 2;
}

void ui_bhook_update_default(ui_box_t* box, SDL_Event *e, void *data)
{
	ui_box_t *current = box->childs;
	box->flags &= ~BOX_CLICKED;
	while(current) {
		if (current->update) {
			ui_bhook_fire(current->update, current, e, data);
		}
		current = current->next;
	}
}
