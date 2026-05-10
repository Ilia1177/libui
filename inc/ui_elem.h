#ifndef UI_ELEM_H
# define UI_ELEM_H
# include "libui.h"

# include "ui_box.h"   // gets ui_bhook_fn_t, menutype_e
typedef enum {
    UI_HORIZONTAL_MENU,
    UI_VERTICAL_MENU,
    UI_FULLWINDOW_MENU,
    UI_NONE
} menutype_e;
// typedef ui_bhook_fn_s ui_bhook_fn_t;
//
ui_win_t *ui_welem_input(ui_win_t *win);
ui_win_t *ui_welem_message(ui_win_t *win, char *message);
ui_box_t *ui_belem_input(ui_win_t *win, int max_len);
ui_box_t*	ui_belem_button(ui_win_t* win, SDL_Rect area, char* label);

ui_box_t *ui_belem_menu_navbar(ui_win_t *win, menutype_e type);
ui_box_t* ui_belem_menu_item(ui_box_t *list, char* label, ui_bhook_fn_t fn);
ui_box_t *ui_belem_menu_list(ui_box_t *menu, char* label) ;
ui_win_t *ui_belem_win_input(ui_win_t *win);
ui_box_t*	ui_belem_button(ui_win_t* win, SDL_Rect area, char* label);
ui_box_t *ui_belem_input(ui_win_t *win, int max_len);
#endif
