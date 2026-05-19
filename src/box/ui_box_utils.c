#include "ui_box.h"

ui_box_t* ui_box_hovered(ui_box_t* boxes, SDL_Point *p)
{
	if ( !boxes || (boxes->flags & BOX_HIDDEN))
		return NULL;
	ui_box_t* selected = NULL;
	ui_box_t* curr = boxes;
	while(curr) {
		if(SDL_PointInRect(p, &curr->area)) {
			selected = curr;
		}
		ui_box_t *child = ui_box_hovered(curr->childs, p);
		if (child) {
			selected = child;
		}
		curr = curr->next;
	}
	return selected;
}

void ui_box_flags(ui_box_t *b, short flag, bool add, bool all)
{
    while (b) {
        if (add)
            b->flags |= flag;
        else
            b->flags &= ~flag;
		if (b->childs && all)
			ui_box_flags(b->childs, flag, add, all);
        b = b->next;
    }
}

int ui_box_count_all(ui_box_t* head)
{
	int n = 0;
	ui_box_t* curr = head;
	while(curr) {
		n++;
		n += ui_box_count_all(curr->childs);
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


