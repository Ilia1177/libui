#include "ui_box.h"

void	ui_bhook_destroy_default(ui_box_t*box, SDL_Event*e, void* data) {
	static int n = 0;
	ui_box_t *current_child = box->list;
	while(current_child) {
		ui_box_t *next = current_child->next;
		ui_bhook_destroy_default(current_child, e, data);
		current_child = next;
	}
	ui_layer_destroy(&box->layers);
    ui_bhook_clean(&box->render);
    ui_bhook_clean(&box->update);
    ui_bhook_clean(&box->destroy);
    ui_bhook_clean(&box->on_click_down);
    ui_bhook_clean(&box->on_click_up);
    ui_bhook_clean(&box->on_mouse_motion);
	free(box);
	printf("destroy box %d\n", ++n);
}
