#include "ui_win.h"

int ui_whook_add(ui_winhandler_t **list, ui_whook_fn_t fn) {
    if (!fn) return -1;
    ui_winhandler_t *handler = calloc(1, sizeof(ui_winhandler_t));
    handler->fn = fn;
    handler->next = *list;  // prepend
    *list = handler;
	return 0;
}

int ui_whook_remove(ui_winhandler_t **list, ui_whook_fn_t fn) {
    if (!list || !*list || !fn) return -1;
    ui_winhandler_t *curr = *list;
    ui_winhandler_t *prev = NULL;
    while (curr) {
        if (curr->fn == fn) {
            if (prev)
                prev->next = curr->next;
            else
                *list = curr->next;  // was head
            free(curr);
            return 0;  // remove first match only
        }
        prev = curr;
        curr = curr->next;
    }
	return 0;
}

int ui_whook_fire(ui_winhandler_t **list, ui_win_t *win, SDL_Event *e, void *data) {
    ui_winhandler_t *curr = *list;
    while (curr) {
        ui_winhandler_t *next = curr->next;
        if (curr->fn) {
            int ret = curr->fn(win, e, data);
            if (ret <= 0) {
                ui_whook_remove(list, curr->fn);  // remove from real list
			}
        }
        curr = next;
    }
    return 0;
}
// int ui_whook_fire(ui_winhandler_t **list, ui_win_t *win, SDL_Event *e, void* data) {
//     while (list) {
// 		ui_winhandler_t *next = list->next;
//         if (list->fn && list->fn(win, e, data) == 0)
// 			ui_whook_remove(&list, list->fn);
//
//         list = next;
//     }
// 	return 0;
// }

int ui_whook_clean(ui_winhandler_t **list) {
	if (!list || !*list) return -1;
	ui_winhandler_t *curr = *list;
	while(curr) {
		ui_winhandler_t *next = curr->next;
		free(curr);
		curr = next;
	}
    *list = NULL;
	return 0;
}
