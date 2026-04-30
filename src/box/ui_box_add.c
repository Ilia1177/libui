#include "libui.h"

void ui_box_add(ui_box_t **list, ui_box_t *new) {
	if (!*list) {
		*list = new;
		return;
	}
	ui_box_t *curr = *list;
	while(curr->next) {
		curr = curr->next;
	}
	curr->next = new;
}

