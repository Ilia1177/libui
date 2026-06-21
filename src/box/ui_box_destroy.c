#include "ui_box.h"

void	ui_bhook_destroy_default(ui_box_t*box, SDL_Event*e, void* d) 
{
	(void)e;
	(void)d;
	// ui_box_t *current_child;
	if (!box)
		return;
	ui_layer_destroy_all(&box->layers);
    ui_bhook_clean(&box->render);
    ui_bhook_clean(&box->update);
    ui_bhook_clean(&box->destroy);
    ui_bhook_clean(&box->on_window_event);
    ui_bhook_clean(&box->on_key_down);
    ui_bhook_clean(&box->on_click_down);
    ui_bhook_clean(&box->on_click_up);
    ui_bhook_clean(&box->on_mouse_motion);
    ui_bhook_clean(&box->on_mouse_wheel);
	free(box);
	box_nb--;
}
