#include "ui_box.h"

void	ui_box_destroy_default(ui_box_t*box, SDL_Event*e, void* data) {
	static int n = 0;
	ui_box_t *current_child = box->child_boxes;
	while(current_child) {
		ui_box_t *next = current_child->next;
		ui_box_destroy_default(current_child, e, data);
		current_child = next;
	}
	    // free handler lists
    ui_box_handler_clean(&box->render);
    ui_box_handler_clean(&box->update);
    ui_box_handler_clean(&box->destroy);
    ui_box_handler_clean(&box->on_click_down);
    ui_box_handler_clean(&box->on_click_up);
    ui_box_handler_clean(&box->on_mouse_motion);
	free(box);
	printf("destroy box %d\n", ++n);
}
