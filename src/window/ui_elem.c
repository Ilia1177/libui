#include "ui_win.h"

ui_box_t*	ui_elem_button(ui_win_t* win) {
		ui_globalApp_t *ref = win->global;
		ui_box_t* button = ui_box_create(ref->button_area, ref->menu_color_2, win);
		// ui_box_t* button = ui_box_create(
		// 		ui_tool_rectcenter(win->area, ref->button_area), 
		// 		ref->menu_color_2, win);
		ui_bhook_add(&button->update, ui_bhook_label2texture);
		return button;
}

void	ui_bhook_wincenter(ui_box_t* box, SDL_Event* e, void* data) {
	(void)e;
	(void)data;
	ui_win_t *win = box->parent_window;
	ui_globalApp_t *ref = win->global;
	// box->area = ui_tool_rectcenter(win->area, ref->button_area);
	box->area = ui_tool_rectcenter(
		(SDL_Rect){0, 0, win->area.w, win->area.h},
		(SDL_Rect){0, 0, ref->button_area.w, ref->button_area.h});
}

ui_win_t	*ui_win_popup(ui_win_t* win)
{
		ui_globalApp_t *ref = win->global;
		ui_win_t* popup = ui_win_create(200, 200, ref, "pop up");
		ui_bhook_destroy_default(popup->canvas, NULL, NULL);
		ui_box_t* menu = ui_box_create((SDL_Rect){0, 0, popup->area.w, popup->area.h}, ref->menu_color_1, popup);
		ui_bhook_add(&menu->update, ui_bhook_maxsize);
		ui_box_t* btn = ui_elem_button(popup);
		ui_bhook_add(&btn->update, ui_bhook_wincenter);
		ui_bhook_add(&btn->update, ui_bhook_winclose);
		ui_box_add(&menu->list, btn);
		// ui_elem_menubar(popup, NULL);
		// ui_bhook_destroy_default(popup->canvas, NULL, NULL);
		// ui_bhook_add(&popup->menu->update, ui_bhook_maxsize);
		// ui_box_t* btn = ui_elem_menubutton(popup->menu, ui_bhook_winclose);
		// ui_bhook_add(&btn->update, ui_bhook_winclose);
		// ui_bhook_add(&btn->update, ui_bhook_wincenter);
		// btn->label = "OKKkkk";
		// btn->area = ui_tool_rectcenter(
		// 	(SDL_Rect){0, 0, win->area.w, win->area.h},
		// 	(SDL_Rect){0, 0, ref->button_area.w, ref->button_area.h});
		ui_win_add(&ref->windows, popup);
		return popup;
}

