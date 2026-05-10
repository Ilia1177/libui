#include "ui_win.h"

void ui_whook_add(ui_winhandler_t **list, void (*fn)(ui_win_t*, SDL_Event*, void*)) {
    if (!fn) return;
    ui_winhandler_t *handler = calloc(1, sizeof(ui_winhandler_t));
    handler->fn = fn;
    handler->next = *list;  // prepend
    *list = handler;
}

// void ui_whook_fire(ui_winhandler_t *list, ui_win_t *win, SDL_Event *e, void* data) {
//     while (list) {
//         if (list->fn)
//             list->fn(win, e, data);
//         list = list->next;
//     }
// }

void ui_whook_clean(ui_winhandler_t **list) {
	if (!list || !*list) return;
	ui_winhandler_t *curr = *list;
	while(curr) {
		ui_winhandler_t *next = curr->next;
		free(curr);
		curr = next;
	}
    *list = NULL;
}
