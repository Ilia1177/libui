#include "ui_win.h"

void ui_bhook_prepend(ui_boxhandler_t **list, ui_bhook_fn_t fn) {
    if (!fn) return;
    ui_boxhandler_t *handler = calloc(1, sizeof(ui_boxhandler_t));
    handler->fn = fn;
    handler->next = *list;  // point to current head
    *list = handler;        // become new head
}

void ui_bhook_append(ui_boxhandler_t **list, ui_bhook_fn_t fn) 
{
	ui_boxhandler_t *handler;
	ui_boxhandler_t *curr;

	if (!fn || !list) 
		return;
	handler = calloc(1, sizeof(ui_boxhandler_t));
	handler->fn = fn;
	handler->next = NULL;
	if (!*list) {
		*list = handler;
		return;
	}
	curr = *list;
	while (curr->next)
		curr = curr->next;
	curr->next = handler;
}

void ui_bhook_fire(ui_boxhandler_t *list, ui_box_t *b, SDL_Event *e, void* d) 
{
	ui_boxhandler_t* handler;
	ui_boxhandler_t* next;


	// ui_log("box hook fire");
	handler = list;
    while (handler) {
		next = handler->next;
		if (handler->fn == ui_bhook_clickdown_default) {printf("click down: %p\n", handler);}
        if (handler->fn) {
            handler->fn(b, e, d);
		}
        handler = next;
    }
}

void ui_bhook_clean(ui_boxhandler_t **list) 
{
	if (!list || !*list) 
		return;
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

    while (curr) {
        if (curr->fn == fn) {
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

void ui_bhook_replace(ui_boxhandler_t *handler, ui_bhook_fn_t old, ui_bhook_fn_t new)
{
	ui_boxhandler_t * curr;
	ui_boxhandler_t *next;

	curr = handler;
	while(curr) {
		next = curr->next;
		if (curr->fn == old)
			curr->fn = new;
		curr = next;
	}
}
