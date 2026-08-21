#include "libui.h" // Now includes guimp.h, which includes libui.h


void new_window(ui_box_t* box, SDL_Event *e, void* data)
{
	(void)data;
	(void)e;
	ui_globalApp_t* app;

	app = box->win->global;
	if(box->state & BOX_CLICKED ) {
		ui_welem_message(app, "hello,\n i am a new window");
	}
}

ui_box_t* menu_1(ui_win_t* win)
{
	ui_box_t* menu;

	menu = ui_box_create(win, UI_LAYOUT_FILL_X, DEFAULT_BOX_COLOR);
	menu->area.h = 50;
	ui_box_t* file_menu = ui_belem_button(win, ui_tex_str(win, "button 1", DEFAULT_TEXT_COLOR));
	ui_box_t* opt1 = ui_belem_button(win, ui_tex_str(win, "1", DEFAULT_TEXT_COLOR));
	ui_box_add_child(file_menu, opt1);
	ui_box_add_child(menu, file_menu);
    ui_layout_menu(menu);
	return menu;
}

ui_box_t* menu_2(ui_win_t* win) 
{
	ui_box_t* menu;

	menu = ui_box_create(win, UI_LAYOUT_FILL_X, DEFAULT_BOX_COLOR);
	menu->area.h = 50;
	ui_box_t* btn_a = ui_belem_button(win, ui_tex_str(win, "btn 2", DEFAULT_TEXT_COLOR));
	ui_box_t* btn_b = ui_belem_button(win, ui_tex_str(win, "btn 2", DEFAULT_TEXT_COLOR));
	ui_box_t* btn_c = ui_belem_button(win, ui_tex_str(win, "btn 3", DEFAULT_TEXT_COLOR));
	ui_box_add_child(btn_a, menu);
	ui_box_add_child(btn_b, menu);
	ui_box_add_child(btn_c, menu);
    ui_layout_menu(menu);
	return menu;
}

ui_box_t* menu_3(ui_win_t* win) 
{
	ui_box_t* menu;

	menu = ui_box_create(win, UI_LAYOUT_FILL_X, DEFAULT_BOX_COLOR);
	menu->area.h = 50;
	ui_box_t* btn_a = ui_belem_button(win, ui_tex_str(win, "btn 2", DEFAULT_TEXT_COLOR));
	ui_box_t* btn_b = ui_belem_button(win, ui_tex_str(win, "btn 2", DEFAULT_TEXT_COLOR));
	ui_box_t* btn_c = ui_belem_button(win, ui_tex_str(win, "btn 3", DEFAULT_TEXT_COLOR));
	ui_box_add_child(btn_a, menu);
	ui_box_add_child(btn_b, menu);
	ui_box_add_child(btn_c, menu);
    ui_layout_menu(menu);
	return menu;
}


ui_win_t* main_window(ui_globalApp_t* app, char* name)
{
	ui_win_t* win;

	win = ui_win_create(app, (SDL_Rect){0, 0, 1200, 800}, name, SDL_WINDOW_RESIZABLE);

	if (!win) {
		printf("window init fail\n");
		return NULL;
	} else {
		printf("window created\n");
	}

	ui_box_t* container;
	uint32_t layout = UI_LAYOUT_FILL_X | UI_LAYOUT_FILL_Y | UI_LAYOUT_DIR_COL;
	container = ui_box_create(win, layout);
	ui_box_add_root(&win->boxes, container);
	ui_box_add_child(container, menu_1(win));
	ui_box_add_child(container, menu_2(win));
	ui_box_add_child(container, menu_3(win));
	return win;
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	printf("init\n");
	ui_globalApp_t* app = ui_global_init("GuImP", NULL);
	if (!app)
		return -1;

	printf("main windows creation\n");
	main_window(app, "GuImP");
	printf("start\n");
	app->start(app);
	ui_global_free(app);
	return 0;
}
