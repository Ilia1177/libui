#include "libui.h"

void ui_box_update_default(ui_box_t* box) {
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
			current->update(current);
		current = current->next;
	}
}
