#include "ui_box.h"

void ui_box_add_root(ui_box_t **list, ui_box_t *new)
{
	if (!list || !new)
		return;
    if (!*list) {
        *list = new;
        return;
    }
	ui_box_t *curr = *list;
    while (curr->next)
        curr = curr->next;
	curr->next = new;
	new->prev = curr;
}

void ui_box_add_child(ui_box_t *parent, ui_box_t *child) {
    if (!parent || !child) 
		return;
    child->parent = parent;
    ui_box_add_root(&parent->childs, child);
}
