#include "ui_box.h"

ui_box_t* ui_box_iter(ui_box_t *b, int n) {
	ui_box_t *curr = b;
	while(n > 0 && curr) {
		curr = curr->next;
		n--;
	}
	return curr;
}
