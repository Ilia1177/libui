#include "ui_win.h"
void ui_bhook_add(ui_boxhandler_t **list, ui_bhook_fn_t fn) {
    if (!fn) return;
    ui_boxhandler_t *handler = calloc(1, sizeof(ui_boxhandler_t));
    handler->fn = fn;
    handler->next = *list;  // point to current head
    *list = handler;        // become new head
}

void ui_bhook_fire(ui_boxhandler_t *list, ui_box_t *box, SDL_Event *e, void* data) {
    while (list) {
		ui_boxhandler_t *next = list->next;
        if (list->fn) {
            list->fn(box, e, data);
		}
        list = next;
    }
}

void ui_bhook_clean(ui_boxhandler_t **list) {
	if (!list || !*list) return;
	ui_boxhandler_t *curr = *list;
	while(curr) {
		ui_boxhandler_t *next = curr->next;
		free(curr);
		curr = next;
	}

    *list = NULL;
}

void ui_bhook_remove(ui_boxhandler_t **list, ui_bhook_fn_t fn)
{
    if (!list || !*list || !fn)
        return;

    ui_boxhandler_t *curr = *list;
    ui_boxhandler_t *prev = NULL;

    while (curr)
    {
        if (curr->fn == fn)
        {
            if (prev)
                prev->next = curr->next;
            else
                *list = curr->next;

            free(curr);
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}
