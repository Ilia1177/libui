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

ui_win_t* main_window(ui_globalApp_t* app, char* name)
{
	ui_win_t* win;

	win = ui_win_create(app, (SDL_Rect){0, 0, 1200, 800}, name);
	ui_win_add(&app->windows, win);
	ui_box_t* menu = ui_belem_menu_area(win, UI_VERTICAL_MENU);
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

ui_win_t* palette_window(ui_globalApp_t* app, char* name)
{
	ui_win_t* win;

	win = ui_win_create(app, (SDL_Rect){0, 0, 0, 0}, name);
	ui_win_add(&app->windows, win);
	ui_box_t* menu = ui_belem_menu_area(win, UI_FULLWINDOW_MENU);
	ui_belem_menu_item(menu, "A", handler);
	ui_belem_menu_item(menu, "B", load_image_handler);
	ui_belem_menu_item(menu, "C", new_window);
	ui_belem_menu_item(menu, "D", ui_bhook_winclose);
	return win;
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	ui_globalApp_t* app = ui_global_init("GuImP");
	if (!app)
		return -1;

	main_window(app, "GuImP");
	palette_window(app, "tools");
	app->start(app);
	ui_global_free(app);
	return 0;
}
