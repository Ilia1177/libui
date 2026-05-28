#include "ui_box.h"

// ui_box_t* ui_box_focused(ui_box_t* boxes, SDL_Point *p)
// {
// 	if ( !boxes || (boxes->flags & BOX_HIDDEN))
// 		return NULL;
// 	ui_box_t* selected = NULL;
// 	ui_box_t* curr = boxes;
// 	while(curr) {
// 		if(SDL_PointInRect(p, &curr->area)) {
// 			selected = curr;
// 		}
// 		ui_box_t *child = ui_box_focused(curr->childs, p);
// 		if (child) {
// 			selected = child;
// 		}
// 		curr = curr->next;
// 	}
// 	return selected;
// }
void ui_box_swap(ui_box_t *a, ui_box_t *b)
{
    // Just swap their list pointers, not the area positions
    ui_box_t *parent = a->parent;
    ui_box_t *curr   = parent->childs;
    ui_box_t *prev_a = NULL, *prev_b = NULL;

    while (curr) {
        if (curr->next == a) prev_a = curr;
        if (curr->next == b) prev_b = curr;
        curr = curr->next;
    }

    // Swap next pointers
    if (prev_a) prev_a->next = b; else parent->childs = b;
    if (prev_b) prev_b->next = a; else parent->childs = a;

    ui_box_t *tmp = a->next;
    a->next = b->next;
    b->next = tmp;
}

void ui_box_bring_to_front(ui_box_t *b)
{
    ui_box_t *parent = b->parent;
    if (!parent || parent->childs == b) return; // already first

    // Unlink b from its current position
    ui_box_t *curr = parent->childs;
    while (curr && curr->next != b)
        curr = curr->next;
    if (!curr) return;
    curr->next = b->next;

    // Relink b at the front
    b->next = parent->childs;
    parent->childs = b;
}
ui_box_t* ui_box_hovered(ui_box_t* boxes, SDL_Point* p)
{
    if (!boxes || (boxes->flags & BOX_HIDDEN))
        return NULL;
    ui_box_t* selected = NULL;
    ui_box_t* curr = boxes;
    while (curr) {
        if (SDL_PointInRect(p, &curr->area)) {
            selected = curr;
        }
        ui_box_t* child = ui_box_hovered(curr->childs, p);
        if (child) {
            selected = child;
        }
        curr = curr->next;
    }
    return selected;
}

void ui_box_flags(ui_box_t* b, short flag, bool add, bool all)
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
    int       n = 0;
    ui_box_t* curr = head;
    while (curr) {
        n++;
        n += ui_box_count_all(curr->childs);
        curr = curr->next;
    }
    return n;
}

int ui_box_count_prev(ui_box_t* boxes)
{
    int n = 0;
    while (boxes) {
        n++;
        boxes = boxes->prev;
    }
    return n;
}

int ui_box_count_next(ui_box_t* boxes)
{
    int n = 0;
    while (boxes) {
        n++;
        boxes = boxes->next;
    }
    return n;
}

ui_box_t* ui_box_last(ui_box_t* boxes)
{
    ui_box_t* curr = boxes;
    ui_box_t* last = NULL;
    ;
    while (curr) {
        last = curr;
        curr = curr->next;
    }
    return last;
}
