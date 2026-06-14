#include "ui_win.h"

void ui_filter_prepend(ui_filhandler_t **list, ui_filter_fn_t fn, void* d) {
    if (!fn) return;
    ui_filhandler_t *handler = calloc(1, sizeof(ui_filhandler_t));
    handler->fn = fn;
	handler->data = d;
    handler->next = *list;  // point to current head
    *list = handler;        // become new head
}

void ui_filter_append(ui_filhandler_t **list, ui_filter_fn_t fn, void *d) 
{
	ui_filhandler_t *handler;
	ui_filhandler_t *curr;

	if (!fn) 
		return;
	handler = calloc(1, sizeof(ui_filhandler_t));
	handler->data = d;
	handler->fn = fn;
	handler->next = NULL;
	if (!*list) {
		*list = handler;
		ui_log("Filter append: append filter");
		return;
	}
	curr = *list;
	while (curr->next)
		curr = curr->next;
	curr->next = handler;
	ui_log("Filter append: append filter");
}

void ui_filter_fire(ui_filhandler_t *list, ui_layer_t *l, SDL_Event *e, void* d) 
{
	ui_filhandler_t* handler;
	ui_filhandler_t* next;
	(void)d;

	// ui_log("box hook fire");
	handler = list;
    while (handler) {
		ui_log("handler");
		next = handler->next;
        if (handler->fn) {
            handler->fn(l, e, handler->data);
		}
        handler = next;
    }
}

void ui_filter_clean(ui_filhandler_t **list) 
{
	if (!list || !*list) 
		return;
	ui_filhandler_t *curr = *list;
	while(curr) {
		ui_filhandler_t *next = curr->next;
		free(curr);
		curr = next;
	}

    *list = NULL;
}

void ui_filter_remove(ui_filhandler_t **list, ui_filter_fn_t fn)
{
    if (!list || !*list || !fn)
        return;

    ui_filhandler_t *curr = *list;
    ui_filhandler_t *prev = NULL;

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

void ui_filter_replace(ui_filhandler_t *handler, ui_filter_fn_t old, ui_filter_fn_t new)
{
	ui_filhandler_t *curr;
	ui_filhandler_t *next;

	curr = handler;
	while(curr) {
		next = curr->next;
		if (curr->fn == old)
			curr->fn = new;
		curr = next;
	}
}
