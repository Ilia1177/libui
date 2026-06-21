
#include "ui_box.h"

void ui_lhook_destroy_default(ui_layer_t* l, SDL_Event* e, void *d)
{
	(void)d;
	(void)e;
	if (l->texture){
		SDL_DestroyTexture(l->texture);
	}
	ui_lhook_clean(&l->destroy);
	free(l);
	layer_nb--;
}

void ui_lhook_prepend(ui_layhandler_t **list, ui_lhook_fn_t fn) {
    if (!fn) return;
    ui_layhandler_t *handler = calloc(1, sizeof(ui_layhandler_t));
    handler->fn = fn;
    handler->next = *list;
    *list = handler;
}

void ui_lhook_append(ui_layhandler_t **list, ui_lhook_fn_t fn) 
{
	ui_layhandler_t *handler;
	ui_layhandler_t *curr;

	if (!fn || !list) 
		return;
	handler = calloc(1, sizeof(ui_layhandler_t));
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

void ui_lhook_fire(ui_layhandler_t *list, ui_layer_t *b, SDL_Event *e, void* d) 
{
	ui_layhandler_t* handler;
	ui_layhandler_t* next;

	handler = list;
    while (handler) {
		next = handler->next;
        if (handler->fn) {
            handler->fn(b, e, d);
		}
        handler = next;
    }
}

void ui_lhook_clean(ui_layhandler_t **list) 
{
	if (!list || !*list) 
		return;
	ui_layhandler_t *curr = *list;
	while(curr) {
		ui_layhandler_t *next = curr->next;
		free(curr);
		curr = next;
	}

    *list = NULL;
}

void ui_lhook_remove(ui_layhandler_t **list, ui_lhook_fn_t fn)
{
    if (!list || !*list || !fn)
        return;

    ui_layhandler_t *curr = *list;
    ui_layhandler_t *prev = NULL;

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

void ui_lhook_replace(ui_layhandler_t *handler, ui_lhook_fn_t old, ui_lhook_fn_t new)
{
	ui_layhandler_t * curr;
	ui_layhandler_t *next;

	curr = handler;
	while(curr) {
		next = curr->next;
		if (curr->fn == old)
			curr->fn = new;
		curr = next;
	}
}
