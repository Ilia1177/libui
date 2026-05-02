#include "ui_box.h"

void ui_box_update_default(ui_box_t* box, SDL_Event *e, void *data) {
	//update current ....
	// if(box->on_hover_enter) {
	// 	box->on_mouse_motion(box);
	// }
	//update childs
	printf("box: update\n");
	fflush(stdout);
	ui_box_t *current = box->child_boxes;
	while(current) {
		if (current->update)
			ui_box_event_fire(current->update, current, e, data);
		current = current->next;
	}
}
