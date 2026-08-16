#include "ui_global.h" // Includes ui_globalApp_t
#include <SDL2/SDL.h> // For SDL_GetWindowFromID, SDL_GetMouseState, etc.
#include <stdio.h> // For printf debugging
#include <stdlib.h> // For malloc, free
#include "ui_win.h"
#include "ui_forward.h"

int box_nb = 0; // debug 
int layer_nb = 0; // debug

static void global_destroy(ui_win_t* win, SDL_Event*e, void*data) 
{
	ui_box_destroy_forward(win, e, data);
	ui_whook_fire(&win->destroy, win, e, data);
}

// Helper to find ui_win_t from SDL_WindowID
ui_win_t* ui_find_window_by_id(ui_globalApp_t* app, uint32_t window_id) {
    ui_win_t* current_win = app->windows; 
    while (current_win) {
        if (current_win->id == window_id) {
            return current_win;
        }
        current_win = current_win->next;
    }
    return NULL;
}

// used to reset input(s) stored by the main struct (app)
void ui_reset_state_and_input(ui_globalApp_t* app, bool* running)
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
	if (ui_init() < 0) {
		printf("error init\n");
		return NULL;
	}
    app = (ui_globalApp_t*)calloc(1, sizeof(ui_globalApp_t));
    if (!app) {
        return NULL;
    }
	app->env = env;
	if (!app->env) {
		printf("Env not provided\n");
	}
	app->loading = false;
    app->state = 0;
	app->actions = NULL;
	app->focused_box = NULL;
	app->inputs = calloc(1, sizeof(char*));
	app->inputs[0] = NULL;
	app->input_nb = 0;
    app->windows = NULL;
	app->scale_x = 1.0f;
	app->scale_y = 1.0f;
	SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
    app->start = ui_start;
    return app;
}

// ui_global_free implementation
void ui_global_free(ui_globalApp_t* app) {
	printf("Quit application...\n");
	fflush(stdout);
    if (app) {
        // Free all windows managed by the app
        ui_win_t* current = app->windows;
        while (current) {
            ui_win_t* next = current->next;
            global_destroy(current, NULL, NULL);
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
	printf("DEBUG box: %d layer: %d\n", box_nb, layer_nb);
	fflush(stdout);
}

static void win_remove(ui_win_t** windows, ui_win_t* toremove)
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
			global_destroy(curr, NULL, NULL);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

static ui_win_t* global_keyboard_event(ui_globalApp_t *app, SDL_Event* e) 
{
	ui_win_t* win = ui_find_window_by_id(app, e->key.windowID);
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

static ui_win_t* global_mousewheel_event(ui_globalApp_t *app, SDL_Event* e)
{
	ui_win_t *win = ui_find_window_by_id(app, e->wheel.windowID);
	if (!win)
		return NULL;
	ui_whook_fire(&win->on_mouse_wheel, win, e, NULL);
	ui_box_event_forward(win, e, NULL);
	return win;
}

static ui_win_t *global_mousemotion_event(ui_globalApp_t* app, SDL_Event *e)
{
	ui_win_t *win = ui_find_window_by_id(app, e->motion.windowID);
	if (!win)
		return NULL;
	ui_whook_fire(&win->on_mouse_motion, win, e, NULL);
	ui_box_event_forward(win, e, NULL);
	return win;
}

static ui_win_t *global_mouseclick_event(ui_globalApp_t* app, SDL_Event *e) 
{
	ui_win_t *win;

	win = ui_find_window_by_id(app, e->button.windowID);
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

static ui_win_t* global_window_event(ui_globalApp_t* app, SDL_Event* e)
{
	ui_win_t *win;

	win = ui_find_window_by_id(app, e->window.windowID);
	if (!win)
		return NULL;
	ui_whook_fire(&win->on_window_event, win, e, NULL);
	ui_box_event_forward(win, e, NULL);
	return win;
}

static int	check_dead_window(ui_globalApp_t *app)
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
				win_remove(&app->windows, curr);
				windows_closed++;
			}
		}
		curr = next;
	}
	return windows_closed;
}

void ui_user_event() {

}

static ui_win_t* dispatch_event(ui_globalApp_t* app, SDL_Event *e)
{
	ui_win_t* win;

	win = NULL;
	while (SDL_PollEvent(e)) {
		ui_log_eventstr(e);
		switch (e->type) {
			case SDL_QUIT:
				app->state |= APP_QUIT; return NULL;
			case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
				win = global_mouseclick_event(app, e); break;
			case SDL_MOUSEMOTION:
				win = global_mousemotion_event(app, e); break;
			case SDL_MOUSEWHEEL:
				win = global_mousewheel_event(app, e); break;
			case SDL_KEYUP: case SDL_KEYDOWN: case SDL_TEXTINPUT:
				win = global_keyboard_event(app, e); break;
			case SDL_WINDOWEVENT:
				win = global_window_event(app, e); break;
		}
		if (win && !(win->state & WIN_QUIT))
			win->state = WIN_DIRTY;
	}
	return win;
}




static void global_render(ui_win_t* win, SDL_Event*e, void*data) 
{
	ui_whook_fire(&win->render, win, e, data);
	ui_box_render_forward(win, e, data);
	SDL_RenderPresent(win->renderer);
	win->state &= ~WIN_DIRTY;
}

static void global_update(ui_win_t* win, SDL_Event*e, void*data) 
{
	//ui_whook_update_default(curr, &e, NULL);
	ui_box_update_forward(win, e, data);
	// win->state &= WIN_STALE;
}

// Main loop
// 1. dispatch event to windows and forward event to boxes;
// 2. fire action hook if any (action are menu button, load, new image, ...)
//    when an action is set, an input might be provided, then stored in app->input[]
//    when the action finish, the app->input[] is used then freed
// 3. update all windows and boxes
// 4  render dirty windows + boxes only;
void ui_start(ui_globalApp_t *app)
{
	ui_win_t *curr;
	SDL_Event e;

	ui_log("1. start");
	printf("box: %d layer: %d\n", box_nb, layer_nb);
	fflush(stdout);
    while (!(app->state & APP_QUIT)) {
		if (check_dead_window(app) < 0)
			break;
		dispatch_event(app, &e);
		ui_whook_fire(&app->actions, app->windows, &e, app->inputs);
		curr = app->windows;
		while(curr) {
			if (curr->state & WIN_DIRTY) {
				ui_layout_pass(curr);
				global_update(curr, &e, NULL);
				global_render(curr, &e, NULL);
			}
			curr = curr->next;
		}
		SDL_Delay(1);
	}
}
