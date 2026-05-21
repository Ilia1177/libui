#include "ui_global.h" // Includes ui_globalApp_t
#include <SDL2/SDL.h> // For SDL_GetWindowFromID, SDL_GetMouseState, etc.
#include <stdio.h> // For printf debugging
#include <stdlib.h> // For malloc, free
#include "ui_win.h"
#include "ui_forward.h"

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

void reset_state_and_input(ui_globalApp_t* app, bool* running)
{
	printf("reset app states !\n");
	fflush(stdout);
	if (running) {
		*running = false;
	}
	app->state &= ~APP_LOADING;
	for (int i = 0; i < app->input_nb; i++) {
		free(app->inputs[i]);
	}
	free(app->inputs);
	app->inputs		= calloc(1, sizeof(char*));
	app->inputs[0]	= NULL;
	app->input_nb	= 0;
}

ui_globalApp_t* ui_global_init(char* name, void* env)
{
	ui_globalApp_t *app;
	(void)name;

	if (ui_init())
		return NULL;
    app = (ui_globalApp_t*)calloc(1, sizeof(ui_globalApp_t));
    if (!app) {
        return NULL;
    }
	app->env = env;
	app->loading = false;
    app->state = 0;
	app->actions = NULL;
	app->focused_box = NULL;
	app->inputs = calloc(1, sizeof(char*));
	app->inputs[0] = NULL;
	app->input_nb = 0;
    app->windows = NULL; // Initialize list of windows_list
	app->scale_x = 1.0f;
	app->scale_y = 1.0f;
	SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
    app->start = ui_start; // Set the start function pointer
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
		case SDL_KEYDOWN: case SDL_TEXTINPUT:
			ui_whook_fire(&win->on_key_down, win, e, NULL);
			ui_box_event_forward(win, e, NULL);
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
	printf("on mouse wheel fire\n");
	fflush(stdout);
	ui_whook_fire(&win->on_mouse_wheel, win, e, NULL);
	ui_box_event_forward(win, e, NULL);
	return win;
}

ui_win_t *ui_global_mousemotion(ui_globalApp_t* app, SDL_Event *e)
{
	ui_win_t *win = find_window_by_id(app, e->motion.windowID);
	if (!win)
		return NULL;
	ui_whook_fire(&win->on_mouse_motion, win, e, NULL);
	ui_box_event_forward(win, e, NULL);
	return win;
}

ui_win_t *ui_global_mouseclick(ui_globalApp_t* app, SDL_Event *e) 
{
	ui_win_t *win;

	win = find_window_by_id(app, e->button.windowID);
	if (!win)
		return NULL;
	SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
	switch (e->type) {
		case SDL_MOUSEBUTTONDOWN:
			ui_whook_fire(&win->on_click_down, win, e, NULL);
			ui_box_event_forward(win, e, NULL);
			break;
		case SDL_MOUSEBUTTONUP:
			ui_whook_fire(&win->on_click_up, win, e, NULL);
			ui_box_event_forward(win, e, NULL);
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
	ui_box_event_forward(win, e, NULL);
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
				ui_log("Quit app");
				app->state |= APP_QUIT;
				return -1;
			} else {
				ui_log("Remove window");
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
	(void)win;
	if (app->actions) {
		ui_whook_fire(&app->actions, app->windows, e, app->inputs);
	}
	// if (win && win->state & WIN_DIRTY) {
	// 	ui_whook_fire(&win->update, win, e, NULL);
	// 	ui_bhook_forward(win, e, NULL, UI_FORWARD_UPDATE);
	// 	ui_whook_fire(&win->render, win, e, NULL);
	// 	ui_bhook_forward(win, e, NULL, UI_FORWARD_RENDER);
	// }
}

ui_win_t* ui_dispatch_event(ui_globalApp_t* app, SDL_Event *e)
{
	ui_win_t* win;

	win = NULL;
	while (SDL_PollEvent(e)) {
		ui_log_eventstr(e);
		switch (e->type) {
			case SDL_QUIT:
				app->state |= APP_QUIT; return NULL;
			case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
				win = ui_global_mouseclick(app, e); break;
			case SDL_MOUSEMOTION:
				win = ui_global_mousemotion(app, e); break;
			case SDL_MOUSEWHEEL:
				win = ui_global_mousewheel(app, e); break;
			case SDL_KEYUP: case SDL_KEYDOWN: case SDL_TEXTINPUT:
				win = ui_global_keyboard(app, e); break;
			case SDL_WINDOWEVENT:
				win = ui_global_windowevent(app, e); break;
		}
		// ui_whook_update_default(win, e, NULL);
		// ui_whook_render_default(win, e, NULL);
		// ui_action_update_and_render(app, win, e);
	}
	return win;
}


// Main loop
// 1. dispatch event to windows and forward event to boxes;
// 2. fire action hook if any (action are menu button)
// 3. update all windows and boxes
// 4  render dirty windows + boxes only;
void ui_start(ui_globalApp_t *app)
{
	ui_win_t *curr;
	SDL_Event e;

	ui_scale_t scale1;
	ui_scale_t scale2 = ui_win_get_scale(app->windows);
	app->scale_x = scale2.x;
	app->scale_y = scale2.y;
	SDL_RenderGetScale(app->windows->renderer, &scale1.x, &scale1.y);
	printf("scale1 x: %f scale1 y: %f, scale2 x: %f, scale2 y: %f\n", scale1.x, scale1.y, scale2.x, scale2.y);
	ui_log("1. start");
	// ui_global_get_scale(app);
	// ui_win_t* win = app->windows;
	// printf("canvas area: %d", app->windows->canvas->color.a);
	// printf("canvas area: %d %d %d %d\n", win->canvas->area.x, win->canvas->area.y, win->canvas->area.w, win->canvas->area.h);
    while (!(app->state & APP_QUIT)) {
		ui_check_dead_window(app);
		ui_dispatch_event(app, &e);
		ui_whook_fire(&app->actions, app->windows, &e, app->inputs);
		curr = app->windows;
		while(curr) {
			if (curr->state & WIN_DIRTY) {
				// ui_log("update and render");
				ui_whook_fire(&curr->update, curr, &e, NULL);
				ui_box_update_forward(curr, &e, NULL);
				ui_whook_fire(&curr->render, curr, &e, NULL);
			}
			curr = curr->next;
		}
	}
	SDL_Delay(1);
}
