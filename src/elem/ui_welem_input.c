#include "libui.h"

void transfert_all_input(ui_globalApp_t* app, ui_box_t* box)
{
	static int box_nb = 0;
	if (!box) {
		return;
	} else if (box->data) {
		printf("Input is: %s\n", (char*)box->data);
		fflush(stdout);
		int n = app->input_nb;
		app->inputs = ui_realloc(app->inputs,
			(app->input_nb + 1) * sizeof(char*),  //0ld
			(app->input_nb + 2) * sizeof(char*));
		app->inputs[n]     = box->data;		// store current input
		app->inputs[n + 1] = NULL;			// NULL terminate like argv
		app->input_nb++;
		printf("input nb: %d\n", app->input_nb);
		fflush(stdout);
		box->data = NULL;
		box->state &= ~BOX_FOCUSED;
	}
    transfert_all_input(app, box->childs);
    transfert_all_input(app, box->next);
	box_nb++;
	printf("Box number: %d\n", box_nb);
	fflush(stdout);
}

void ui_bhook_valid_input(ui_box_t* b, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_globalApp_t *app;

	if (!(b->state & BOX_CLICKED))
		return;
	app = b->parent_window->global;
	printf("hook: valid input\n");
	fflush(stdout);
	transfert_all_input(app, b->parent_window->boxes);
	app->windows->state |= WIN_DIRTY;
	b->parent_window->state |= WIN_QUIT;
}

ui_win_t *ui_welem_input(ui_globalApp_t *app)//, char *message)
{
	SDL_Rect area = {-1, -1, 230, 150};
    ui_win_t *popup;

	popup = ui_win_create(app, area, "pop up", 0);
	ui_box_t* menu = ui_box_create(popup, UI_LAYOUT_FILL_X | UI_LAYOUT_FILL_Y, DEFAULT_BOX_COLOR);
	ui_box_t* msg = ui_belem_message(popup, "Select the image path");
    ui_box_t *input = ui_belem_input(popup, 64);
	ui_box_t* valid = ui_belem_button(popup, ui_tex_str(popup, "load", DEFAULT_TEXT_COLOR));
    ui_bhook_wincenter(msg, NULL, &(SDL_Rect){0, -40, 0, 0});
    ui_bhook_wincenter(input, NULL, &(SDL_Rect){0, 40, 0, 0});
    ui_bhook_wincenter(valid, NULL, &(SDL_Rect){0, 100, 0, 0});
    ui_box_add_child(menu, input);
    ui_box_add_child(menu, valid);
    ui_box_add_child(menu, msg);
	ui_bhook_append(&valid->on_click_down, ui_bhook_valid_input);
	ui_bhook_append(&valid->on_key_down, ui_bhook_valid_input);
	// ui_menu_build(menu, UI_NONE);
	ui_box_add_root(&popup->boxes, menu);
    return popup;
}
