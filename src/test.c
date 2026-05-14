#include <stdio.h>
#include "libui.h" // Now includes guimp.h, which includes libui.h
#include <SDL2/SDL.h> // Required for SDL_GetKeyName and SDL_KeyboardEvent
# include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


// int ui_whook_new_layer(ui_win_t *win, SDL_Event* e, void* data);
// void new_file(ui_box_t* b, SDL_Event *e, void* data) 
// {
// 	(void)e;
// 	(void)data;
// 	if(!b || !(b->flags & BOX_CLICKED))
// 		return;
// 	ui_whook_add(&b->parent_window->global->actions, ui_whook_new_layer);
// }

void set_move_layer(ui_box_t*b, SDL_Event* e, void*data) {
	(void)e;
	(void)data;
	if(!b || !(b->flags & BOX_CLICKED))
		return;
	if (!(b->flags & BOX_FOCUSED))
		b->parent_window->global->rawtool = NULL;
	else
		b->parent_window->global->rawtool = ui_bhook_movelayer;
}

void load_image_handler(ui_box_t *box, SDL_Event *e, void *data)
{
    (void)e;
	(void)data;
	ui_globalApp_t *ref = box->parent_window->global;
	if((box->flags & BOX_CLICKED && !ref->loading)) {
		ui_whook_add(&ref->actions, ui_whook_loadimage);
	}
}

void	handler_canvas_dimensions(ui_box_t* b, SDL_Event *e, void* data)
{
	ui_win_t* win;
	ui_box_t* menu;
	(void)e;
	(void)data;

	win = b->parent_window;
	menu = win->boxes;
	if (!menu) {
		b->area = ui_area(0, 0, win->area.w, win->area.h);
		return;
	}
	switch(win->boxes->type) {
		case UI_VERTICAL_MENU:
			b->area = ui_area(menu->area.w, 0, 
					win->area.w - menu->area.w, win->area.h);
			break;
		case UI_HORIZONTAL_MENU:
			b->area = ui_area(0, menu->area.h, 
					win->area.w, win->area.h - menu->area.h);
		default :
			break;
	}
}

void new_window(ui_box_t* box, SDL_Event *e, void* data)
{
	(void)data;
	(void)e;
	ui_globalApp_t* app;

	app = box->parent_window->global;
	if(box->flags & BOX_CLICKED ) {
		ui_welem_message(app, "hello,\n i am a new window");
	}
}

ui_win_t* main_window(ui_globalApp_t* app, char* name, menutype_e type)
{
	ui_win_t* win;

	win = ui_win_create(app, (SDL_Rect){0, 0, 1200, 800}, name, SDL_WINDOW_RESIZABLE);
	ui_box_t* menu = ui_belem_menu_make(win, type);
	ui_box_t* file_menu = 	ui_belem_menu_list(menu, "file");
	ui_belem_menu_list(menu, "image");
	ui_belem_menu_list(menu, "filtres");
	// ui_belem_menu_item(file_menu, "new", new_file);
	ui_belem_menu_item(file_menu, "load", load_image_handler);
	ui_belem_menu_item(file_menu, "EXIT", ui_bhook_winclose);
	win->canvas = ui_belem_canvas(win);
	ui_bhook_add(&win->canvas->on_window_event, handler_canvas_dimensions);
	return win;
}

ui_win_t* palette_window(ui_globalApp_t* app, char* name)
{
	ui_win_t* win;

	win = ui_win_create(app, (SDL_Rect){800, 400, 200, 800}, name, 0);
	ui_box_t* menu = ui_belem_menu_make(win, UI_FULLWINDOW_MENU);
	ui_belem_menu_item(menu, "A", set_move_layer);
	ui_box_flags(menu->list, BOX_HIDDEN, false);
	return win;
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	ui_globalApp_t* app = ui_global_init("GuImP");
	if (!app)
		return -1;

	main_window(app, "GuImP", UI_HORIZONTAL_MENU);
	palette_window(app, "tools");
	app->start(app);
	ui_global_free(app);
	return 0;
}
