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
ui_box_t* ui_belem_message(ui_win_t* win, const char* msg);
ui_win_t	*ui_welem_input(ui_globalApp_t *app);
ui_win_t	*ui_welem_message(ui_globalApp_t* app, const char *message);
ui_box_t	*ui_belem_input(ui_win_t *win, int max_len);
ui_box_t	*ui_belem_button(ui_win_t* win, const char* label);
ui_box_t	*ui_belem_canvas(ui_win_t* win);
ui_box_t	*ui_belem_menu_make(ui_win_t *win, menutype_e type);
ui_box_t	*ui_belem_menu_item(ui_box_t *list, const char* label, ui_bhook_fn_t fn);
ui_box_t	*ui_belem_menu_list(ui_box_t *menu, const char* label) ;
ui_win_t	*ui_belem_win_input(ui_win_t *win);
ui_box_t	*ui_belem_input(ui_win_t *win, int max_len);
#endif
