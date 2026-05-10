#include "ui_box.h"

void ui_box_flags(ui_box_t *b, short flag, bool add)
{
    while (b) {
        if (add)
            b->flags |= flag;
        else
            b->flags &= ~flag;
        b = b->next;
    }
}

int ui_box_count_all(ui_box_t* head)
{
	int n = 0;
	ui_box_t* curr = head;
	while(curr) {
		n++;
		n += ui_box_count_all(curr->list);
		curr = curr->next;
	}
	return n;
}

int ui_box_count_prev(ui_box_t *boxes)
{
	int n = 0;
	while(boxes) {
		n++;
		boxes = boxes->prev;
	}
	return n;
}

int ui_box_count_next(ui_box_t *boxes)
{
	int n = 0;
	while(boxes) {
		n++;
		boxes = boxes->next;
	}
	return n;
}

ui_box_t* ui_box_last(ui_box_t* boxes)
{
	ui_box_t *curr = boxes;
	ui_box_t* last = NULL;;
	while (curr) {
		last = curr;
		curr = curr->next;
	}
	return last;
}


