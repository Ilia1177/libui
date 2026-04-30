#include "libui.h"
void ui_win_add(ui_win_t **list, ui_win_t *new) {
    if (!*list) {
        *list = new;          // empty list — new node is the head
        return;
    }
    ui_win_t *curr = *list;
    while (curr->next)
        curr = curr->next;
    curr->next = new;
}
// void ui_win_add(ui_win_t* list, ui_win_t *new) {
// 	ui_win_t *curr = list;
// 	while(curr) {
// 		curr = curr->next;
// 	}
// 	curr = new;
// }
