#include <stdio.h>
#include "libui.h" // Now includes guimp.h, which includes libui.h
#include <SDL2/SDL.h> // Required for SDL_GetKeyName and SDL_KeyboardEvent
# include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

ui_win_t* main_window(ui_globalApp_t* app, char* name, boxtype_e type)
{
	ui_win_t* win;

	win = ui_win_create(app, (SDL_Rect){0, 0, 1200, 800}, name, SDL_WINDOW_RESIZABLE);
	ui_box_t* menu = ui_menu_init(win);
	ui_box_t* file = ui_belem_button(win, ui_texture_text(win, "file", COLOR_WHITE));
	ui_box_t* image = ui_belem_button(win, ui_texture_text(win, "image", COLOR_WHITE));
	ui_box_t* windows = ui_belem_button(win, ui_texture_text(win, "windows", COLOR_WHITE));

	ui_box_t* load_btn = ui_belem_button(win, ui_texture_text(win, "load", COLOR_WHITE));
	ui_box_t* exit_btn = ui_belem_button(win, ui_texture_text(win, "exit", COLOR_WHITE));
	ui_bhook_prepend(&load_btn->update, load_image_handler);
	ui_bhook_prepend(&exit_btn->update, ui_bhook_winclose);

	ui_box_t* factice_btn1 = ui_belem_button(win, ui_texture_text(win, "factice", COLOR_WHITE));
	ui_box_t* factice_btn2 = ui_belem_button(win, ui_texture_text(win, "factice", COLOR_WHITE));
	ui_box_t* factice_btn3 = ui_belem_button(win, ui_texture_text(win, "factice", COLOR_WHITE));
	ui_box_t* factice_btn4 = ui_belem_button(win, ui_texture_text(win, "factice", COLOR_WHITE));
	ui_box_add_child(image, factice_btn1);
	ui_box_add_child(image, factice_btn2);
	ui_box_add_child(image, factice_btn3);
	ui_box_add_child(image, factice_btn4);


	ui_box_add_child(menu, file);
	ui_box_add_child(menu, image);
	ui_box_add_child(menu, windows);
	ui_box_add_child(file, load_btn);
	ui_box_add_child(file, exit_btn);
	ui_menu_build(menu, type);
	return win;
}

void change_canvas_color(ui_box_t* slider, SDL_Event* e, void*data)
{
	(void)e;
	(void)data;

	ui_slider_data_t* slider_data = (ui_slider_data_t*)slider->data;
	ui_box_t* cnv = slider->parent_window->global->windows->canvas;
	cnv->color.r = (int)slider_data->current_value;
	// printf("current value: %f\n", slider_data->current_value);
}

ui_win_t *new_menu_window(ui_globalApp_t*app) {

	// SDL_Rect area = {0, 0, 0, 0};
	ui_win_t *win = ui_win_create(app, (SDL_Rect){0, 0, 600, 800}, "test", SDL_WINDOW_RESIZABLE);

	ui_box_t* menu = ui_box_create(win,(SDL_Rect){0, 0, 0, 0}, win->colors[1]);
	
	ui_box_t* list1 = ui_belem_button(win, ui_texture_text(win, "list 1", COLOR_WHITE));
	ui_box_t* list2 = ui_belem_button(win, ui_texture_text(win, "list 2", COLOR_WHITE));
	ui_box_t* list3 = ui_belem_button(win, ui_texture_text(win, "list 3", COLOR_WHITE));
	ui_box_add_child(menu, list1);
	ui_box_add_child(menu, list2);
	ui_box_add_child(menu, list3);

	ui_box_t* btn1 = ui_belem_button(win, ui_texture_text(win, "load", COLOR_WHITE));
	ui_box_t* btn2 = ui_belem_button(win, ui_texture_text(win, "EXIT", COLOR_WHITE));
	ui_box_add_child(list1, btn1);
	ui_box_add_child(list1, btn2);

	ui_box_t* option1 = ui_belem_button(win, ui_texture_text(win, "option 1", COLOR_WHITE));
	ui_box_t* option2 = ui_belem_button(win, ui_texture_text(win, "option 2", COLOR_WHITE));
	ui_box_add_child(btn2, option1);
	ui_box_add_child(btn2, option2);
	ui_menu_build(menu, UI_VERTICAL_MENU);
	ui_box_add_root(&win->boxes, menu);
	win->canvas = ui_belem_canvas(win);
	return win;
}

ui_win_t* palette_window(ui_globalApp_t* app, char* name)
{
	ui_win_t* win;

	win = ui_win_create(app, (SDL_Rect){800, 400, 200, 800}, name, 0);
	ui_box_t* menu = ui_menu_init(win);
	ui_box_t* tool_move_layer = ui_belem_button(win, ui_texture_text(win, "A", COLOR_WHITE));
	ui_bhook_prepend(&tool_move_layer->update, set_move_layer);
	ui_box_t* slider = ui_belem_slider(win, ui_slider_data(0.0f, 255.0f, 0.5f));
	ui_bhook_prepend(&slider->update, change_canvas_color);
	ui_box_add_child(menu, slider);
	ui_box_add_child(menu, tool_move_layer);
	ui_menu_build(menu, UI_FULLWINDOW_MENU);
	return win;
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	ui_globalApp_t* app = ui_global_init("GuImP");
	if (!app)
		return -1;

	// horizontal menu
	main_window(app, "GuImP", UI_HORIZONTAL_MENU);
	// full window menu
	palette_window(app, "tools");
	// vertical menu
	new_menu_window(app);
	app->start(app);
	ui_global_free(app);
	return 0;
}
