#include "libui.h"

static int ui_whook_quitkey(ui_win_t* win, SDL_Event *e, void *data)
{
	(void)data;

	if (!e || !win) {
		return 1;
	} else if (e->type == SDL_KEYDOWN) {
		switch (e->key.keysym.sym) {
			case SDLK_ESCAPE:
				win->state |= WIN_QUIT;
				return 0;

			default:
				break;
		}
	}
	return 1;
} 

static void	ui_bhook_maxsize(ui_box_t *box, SDL_Event *e, void *data) 
{
	(void)e;
	(void)data;
	ui_win_t *win = box->parent_window;
	if(win->boxes && win->boxes != box) {
		box->area.y = BOX_MENU_H + (2 * MENU_GAP_Y) + win->boxes->border;
		box->area.h = win->area.h - (BOX_MENU_H + (2 * MENU_GAP_Y) - win->boxes->border);
	}
	box->area.h = win->area.h;
	box->area.w = win->area.w;
}

ui_win_t *ui_welem_message(ui_globalApp_t *ref, const char *message)
{
	SDL_Rect area = {-1, -1, 0, 100};
	TTF_SizeText(ref->windows->font, message, &area.w, NULL);
	area.w = (int)area.w / ref->scale_x;
    ui_win_t *popup = ui_win_create(ref, area, "pop up", 0);
	ui_whook_prepend(&popup->on_key_down, ui_whook_quitkey);
	// ui_box_t *menu = ui_menu_init(popup);
	ui_box_t *menu = ui_box_create(popup, 0, DEFAULT_BOX_COLOR);
    ui_bhook_append(&menu->on_window_event, ui_bhook_maxsize);
	ui_whook_remove(&popup->on_click_down, ui_whook_clickdown_default);
	ui_whook_remove(&popup->on_mouse_motion, ui_whook_mousemotion_default);
	ui_box_t* msg = ui_belem_message(popup, message);
    ui_box_t *btn = ui_belem_button(popup,  ui_tex_str(popup, "ok", COLOR_WHITE));
	ui_bhook_append(&btn->on_key_down, ui_bhook_winclose);
	ui_bhook_append(&btn->on_click_down, ui_bhook_winclose);
    ui_bhook_wincenter(btn, NULL, &(SDL_Rect){0, 40, 0, 0});
    ui_bhook_wincenter(msg, NULL, &(SDL_Rect){0, -80, 0, 0});
    ui_box_add_child(menu, btn);
    ui_box_add_child(menu, msg);
	// ui_menu_build(menu, UI_NONE);
    return popup;
}
