#include <stdio.h>
#include "libui.h" // Now includes guimp.h, which includes libui.h
#include <SDL2/SDL.h> // Required for SDL_GetKeyName and SDL_KeyboardEvent
# include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void handler(ui_box_t*b, SDL_Event* e, void*data) {
	(void)e;
	(void)data;
	if(b->flags & BOX_CLICKED) {
		printf("OPTION ACTIVATED\n");
		b->parent_window->background_color.r += 10;
	}
}

void load_image_handler(ui_box_t *box, SDL_Event *e, void *data)
{
    (void)e;
	(void)data;
	ui_globalApp_t *ref = box->parent_window->global;
	if((box->flags & BOX_CLICKED && !ref->loading)) {
		ui_whook_add(&ref->actions, ui_whook_loadimage);
		printf("load image attached\n");
	}
}

void	handler_canvas_dimensions(ui_box_t* b, SDL_Event *e, void* data)
{
	ui_win_t* win;
	ui_box_t* menu;
	(void)e;
	(void)data;

	win = b->parent_window;
	menu = win->menu;
	switch(win->menu->type) {
		case UI_VERTICAL_MENU:
			b->area.w = win->area.w - menu->area.w;
			b->area.h = win->area.h;
			break;
		case UI_HORIZONTAL_MENU:
			b->area.h = win->area.h - menu->area.h;
			b->area.y = menu->area.h;
			b->area.w = win->area.w;
			break;
		default :
			break;
	}
}

void new_window(ui_box_t* box, SDL_Event *e, void* data)
{
	(void)data;
	(void)e;
	if(box->flags & BOX_CLICKED ) {
		ui_welem_message(box->parent_window, "hello,\n i am a new window");
	}
}

ui_win_t* make_simple_window(ui_globalApp_t* app, char* name)
{
	ui_win_t* win;

	win = ui_win_create(app, (SDL_Rect){0, 0, 0, 0}, name);
	ui_win_add(&app->windows, win);
	ui_box_t* menu = ui_belem_menu_navbar(win, UI_VERTICAL_MENU);
	ui_box_t* file_menu = 	ui_belem_menu_list(menu, "file");
	ui_box_t* image_menu = 	ui_belem_menu_list(menu, "image");
							ui_belem_menu_list(menu, "about");
	ui_belem_menu_item(image_menu, "size", handler);
	ui_belem_menu_item(file_menu, "load", load_image_handler);
	ui_belem_menu_item(file_menu, "new btn", new_window);
	ui_belem_menu_item(file_menu, "EXIT", ui_bhook_winclose);
	win->canvas = ui_box_create(ui_area(
				menu->area.x + menu->area.w, 
				menu->area.y, 
				win->area.w - menu->area.w, 
				menu->area.h), win->colors[3], win);
	ui_bhook_add(&win->canvas->on_window_event, handler_canvas_dimensions);
	ui_bhook_add(&win->canvas->update, ui_bhook_movelayer);
	return win;
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	ui_globalApp_t* app = ui_global_init("GuImP");
	if (!app)
		return -1;

	make_simple_window(app, "GuImP");
	
	// creation of window 2
	ui_win_t *win2 = ui_win_create(app, (SDL_Rect){10, 20, 0, 0}, "window 2: horizontal menu");
	win2->menu = ui_belem_menu_navbar(win2, UI_HORIZONTAL_MENU);
	ui_box_t* menu1win2 = 	ui_belem_menu_list(win2->menu, "option 1");
	ui_box_t* menu2win2 = 	ui_belem_menu_list(win2->menu, "option 2");
	ui_box_t* menu3win2 = 	ui_belem_menu_list(win2->menu, "option 3");
	ui_box_t* menu4win2 = 	ui_belem_menu_list(win2->menu, "option 4");
	ui_box_t* menu5win2 = 	ui_belem_menu_list(win2->menu, "option 5");
	for (int i = 0; i < 5; i++) {
		ui_belem_menu_item(menu1win2, "click me", handler);
		ui_belem_menu_item(menu2win2, "click me", handler);
		ui_belem_menu_item(menu3win2, "click me", handler);
		ui_belem_menu_item(menu4win2, "click me", handler);
		ui_belem_menu_item(menu5win2, "click me", handler);
	}
	ui_win_add(&app->windows, win2);

	// Third window
	ui_win_t *win3 = ui_win_create(app, (SDL_Rect){200, 400, 0, 0}, "window 3: full window menu");
	win3->menu = ui_belem_menu_navbar(win3, UI_FULLWINDOW_MENU);
	for (int i = 0; i < 6; i++) {
		ui_box_t* btn = ui_belem_menu_item(win3->menu, "click this", handler);
		btn->flags &= ~BOX_HIDDEN;
	}
	ui_win_add(&app->windows, win3);

	app->start(app);
	ui_global_free(app);
	return 0;
}
