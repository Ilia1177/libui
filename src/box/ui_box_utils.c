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
//

void ui_box_move(ui_box_t* b, SDL_Rect move)
{
	ui_box_t* curr;

	if (!b)
		return;

	b->area.x += move.x;
	b->area.y += move.y;
	b->area.w += move.w;
	b->area.h += move.h;
	ui_box_center_layers(b, NULL);
	curr = b->childs;
	while(curr)
	{
		ui_box_move(curr->childs, move);
		curr = curr->next;
	}
	
}

void destroy_one_box(ui_box_t* b, SDL_Event *e, void* data);

int ui_box_remove(ui_box_t **list, ui_box_t* box)
{
    if(!list || !*list)
        return 0;

    ui_box_t* prev = NULL;
    ui_box_t* curr = *list;

    while(curr) {
        ui_box_t* next = curr->next; // Cache next pointer
		if (ui_box_remove(&curr->childs, box))
			return 1;
        if(curr == box) {
            if (prev) {
                prev->next = next; 
                if (next) {
                    next->prev = prev;
                }
            } else {
                *list = next;
                if (next) {
                    next->prev = NULL;
                }
            }
			destroy_one_box(curr, NULL, NULL);
            return 1;
        }
        prev = curr;
        curr = next;
    }
	return 0;
}

ui_box_t* ui_get_box_by_label(ui_box_t* box, const char* name) {
	ui_box_t* curr = box;
	ui_box_t* found;

	found = NULL;
	while(curr) {
		if (curr->label && !strncmp(name, curr->label, strlen(curr->label))) {
			return curr;
		}
		found = ui_get_box_by_label(curr->childs, name);
		if (found)
			return found;
		curr = curr->next;
	}
	return NULL;
}

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
