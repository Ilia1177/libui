#include "ui_global.h" // Includes ui_globalApp_t
#include <SDL2/SDL.h> // For SDL_GetWindowFromID, SDL_GetMouseState, etc.
#include <stdio.h> // For printf debugging
#include <stdlib.h> // For malloc, free
#include "ui_win.h"

// Helper to find ui_win_t from SDL_WindowID
ui_win_t* find_window_by_id(ui_globalApp_t* app, uint32_t window_id) {
    ui_win_t* current_win = app->windows; 
    while (current_win) {
        if (current_win->id == window_id) {
            return current_win;
        }
        current_win = current_win->next; // Assuming a 'next' pointer in ui_win_t
    }
    return NULL;
}

// ui_pos_t ui_get_mouse() {
// 	SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
// }

void reset_state_and_input(ui_globalApp_t* app, bool* running)
{
	if (running)
		*running = false; 
	app->state &= APP_LOADING;
	for (int i = 0; i < app->input_nb; i++)
		free(app->inputs[i]);
	free(app->inputs);
	app->inputs      = calloc(1, sizeof(char*));
	app->inputs[0]   = NULL;
	app->input_nb = 0;
}

ui_globalApp_t* ui_global_init(char* name)
{
	ui_globalApp_t *app;
	(void)name;

	if (ui_init())
		return NULL;
    app = (ui_globalApp_t*)calloc(1, sizeof(ui_globalApp_t));
    if (!app) {
        return NULL;
    }
	app->loading = false;
    app->state = 0;
	app->actions = NULL;
	app->focused_box = NULL;
	app->inputs = calloc(1, sizeof(char*));
	app->inputs[0] = NULL;
	app->input_nb = 0;
    app->windows = NULL; // Initialize list of windows_list
	// app->windows = ui_win_create(app, (SDL_Rect){0, 0, 0, 0}, name);
	// app->windows->canvas = ui_box_create(app->windows->area, TEAL, app->windows);
	// app->windows->canvas->area.x = 0;
	// app->windows->canvas->area.y = 0;
	// ui_bhook_add(&app->windows->canvas->update, ui_bhook_fullwidth);
	// ui_bhook_add(&app->windows->canvas->update, ui_bhook_fullheight);
	// ui_bhook_add(&app->windows->canvas->update, ui_bhook_movelayer);
	SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
	// app->selected_window = app->windows;
    app->start = ui_start; // Set the start function pointer
 	// app->menu_color_1 = (SDL_Color){124, 56, 210, 255};
 	// app->menu_color_2 = (SDL_Color){138, 46, 2, 255};

	// app->button_area = (SDL_Rect){MENU_OFFSET_X, MENU_OFFSET_Y, BOX_MENU_W, BOX_MENU_H};
    return app;
}

// ui_global_free implementation
void ui_global_free(ui_globalApp_t* app) {
	printf("Quitting guimp...\n");
	fflush(stdout);
    if (app) {
        // Free all windows managed by the app
        ui_win_t* current = app->windows;
        while (current) {
            ui_win_t* next = current->next;
            ui_whook_destroy_default(current, NULL, NULL);
            current = next;
        }
		while(app->input_nb > 0) {
			free(app->inputs[app->input_nb]);
			app->input_nb--;
		}
		free(app->inputs);
		if(app->actions)
			ui_whook_clean(&app->actions);
        ui_quit();
        free(app);
    }
}

void ui_win_remove(ui_win_t** windows, ui_win_t* toremove)
{
	if(!windows || !*windows || !toremove)
		return;
	printf("  remove window %d\n", toremove->id);
    ui_win_t *curr = *windows;
    ui_win_t *prev = NULL;
    while (curr) {
        if (curr == toremove) {
            if (prev)
                prev->next = curr->next;
            else
                *windows = curr->next;
            ui_whook_destroy_default(curr, NULL, NULL);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

ui_win_t* ui_global_keyboard(ui_globalApp_t *app, SDL_Event* e) 
{
	ui_win_t* win = find_window_by_id(app, e->key.windowID);
	if (!win)
		return NULL;
	switch(e->type) {
		case SDL_KEYDOWN:
			ui_whook_fire(&win->on_key_down, win, e, NULL);
			break;
		case SDL_KEYUP:
			ui_whook_fire(&win->on_key_up, win, e, NULL);
	}
	return win;
}

ui_win_t* ui_global_mousewheel(ui_globalApp_t *app, SDL_Event* e)
{
	ui_win_t *win = find_window_by_id(app, e->wheel.windowID);
	if (!win)
		return NULL;
	ui_whook_fire(&win->on_mouse_wheel, win, e, NULL);
	return win;
}

ui_win_t *ui_global_mousemotion(ui_globalApp_t* app, SDL_Event *e)
{
	ui_win_t *win = find_window_by_id(app, e->motion.windowID);
	if (!win)
		return NULL;
	ui_whook_fire(&win->on_mouse_motion, win, e, NULL);
	return win;
}

ui_win_t *ui_global_mouseclick(ui_globalApp_t* app, SDL_Event *e) 
{
	ui_win_t *win;

	win = find_window_by_id(app, e->button.windowID);
	if (!win)
		return NULL;
	SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
	switch (e->type) 
	{
		case SDL_MOUSEBUTTONDOWN:
			ui_whook_fire(&win->on_click_down, win, e, NULL);
			break;
		case SDL_MOUSEBUTTONUP:
			ui_whook_fire(&win->on_click_up, win, e, NULL);
			break;
	}
	return win;
}

ui_win_t* ui_global_windowevent(ui_globalApp_t* app, SDL_Event* e)
{
	ui_win_t *win;

	win = find_window_by_id(app, e->window.windowID);
	if (!win)
		return NULL;
	ui_whook_fire(&win->on_window_event, win, e, NULL);
	return win;
}

int	ui_check_dead_window(ui_globalApp_t *app)
{
	ui_win_t *curr;
	int windows_closed;

	windows_closed = 0;
	curr = app->windows;
	while (curr) {
		ui_win_t *next = curr->next;
		if (curr->state & WIN_QUIT) {
			if (curr->id == 1) {
				app->state |= APP_QUIT;
				return -1;
			} else {
				ui_win_remove(&app->windows, curr);
				windows_closed++;
			}
		}
		curr = next;
	}
	return windows_closed;
}

void ui_action_update_and_render(ui_globalApp_t* app, ui_win_t* win, SDL_Event* e)
{
		if (app->actions) {
			ui_whook_fire(&app->actions, app->windows, e, (void*)app->inputs);
		}
		if (win) {
			ui_whook_fire(&win->update, win, e, NULL);
			ui_whook_fire(&win->render, win, e, NULL);
		}
}

ui_win_t* ui_dispatch_event(ui_globalApp_t* app, SDL_Event *e)
{
	ui_win_t* win;

	win = NULL;
	while (SDL_PollEvent(e)) {
		// printf("event is: %d\n", e->type);
		switch (e->type) {
			case SDL_QUIT:
				printf("SDL_QUIT\n");
				app->state |= APP_QUIT; return NULL;
			case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
				// printf("SDL_MOUSE_CLICK\n");
				win = ui_global_mouseclick(app, e); break;
			case SDL_MOUSEMOTION:
				// printf("SDL_MOUSE_MOTION\n");
				win = ui_global_mousemotion(app, e); break;
			case SDL_MOUSEWHEEL:
				printf("SDL_MOUSE_WHEEL\n");
				win = ui_global_mousewheel(app, e); break;
			case SDL_KEYUP: case SDL_KEYDOWN: //case SDL_TEXTINPUT:
				// printf("SDL_KEYBOARD\n");
				win = ui_global_keyboard(app, e); break;
			case SDL_WINDOWEVENT:
				// printf("SDL_WINDOW_EVENT\n");
				win = ui_global_windowevent(app, e); break;
		}
		ui_action_update_and_render(app, win, e);
	}
	return win;
}

// Main loop
// 1. dispatch event to windows and boxes;
// 2. update & render all dirty windows;
void ui_start(ui_globalApp_t *app)
{
	ui_win_t *curr;
	SDL_Event e;

	printf("1. start\n");
    while (!(app->state & APP_QUIT)) {
		// printf("%s - 2. check dead win\n", ui_get_time());
		ui_check_dead_window(app);
		// printf("%s - 3. dispatch events\n", ui_get_time());
		ui_dispatch_event(app, &e);

		// ui_bhook_fire(app->tool, app->windows->canvas, NULL, NULL);
		if (app->rawtool)
			app->rawtool(app->windows->canvas, &e, NULL);
		curr = app->windows;
		while(curr) {
			// printf("%s - 4. update & render\n", ui_get_time());
			ui_win_t* next = curr->next;
			ui_whook_fire(&curr->update, curr, &e, NULL);
			ui_whook_fire(&curr->render, curr, &e, NULL);
			curr = next;
		}
	}
	SDL_Delay(1);
}
