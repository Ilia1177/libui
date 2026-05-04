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

ui_globalApp_t* ui_global_init() {
	int status = ui_init();
	if (status)
		return NULL;
    ui_globalApp_t* app = (ui_globalApp_t*)malloc(sizeof(ui_globalApp_t));
    if (!app) {
        fprintf(stderr, "Failed to allocate ui_globalApp_t\n");
        return NULL;
    }

    app->windows = NULL; // Initialize list of windows_list
	app->windows = ui_win_create(400, 400, app, "main");
	SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
	app->selected_window = app->windows;
    app->flags = 0;
    app->start = ui_start; // Set the start function pointer
 	app->menu_color_1 = (SDL_Color){124, 56, 210, 255};
 	app->menu_color_2 = (SDL_Color){138, 46, 2, 255};
	app->button_area = (SDL_Rect){50, 4, BOX_MENU_W, BOX_MENU_H};
    // // Call SDL_Init if libui is responsible for it
    // if (ui_init() != 0) {
    //     fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    //     free(app);
    //     return NULL;
    // }

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
            ui_whook_destroy_default(current, NULL, NULL); // Call window destroy function
            current = next;
        }
        ui_quit(); // Quit SDL
        free(app);
    }
}

void remove_window(ui_globalApp_t *app, uint32_t win_id) {
    ui_win_t *curr = app->windows;
    ui_win_t *prev = NULL;
    while (curr) {
        if (curr->id == win_id) {
            // unlink first
            if (prev)
                prev->next = curr->next;
            else
                app->windows = curr->next;
            // then destroy
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
	if (!win || win->flags & WIN_QUIT)
		return NULL;
	switch(e->type) {
		case SDL_KEYDOWN:
			if (win->on_key_down)
				ui_whook_fire(win->on_key_down, win, e, NULL);
			break;
		case SDL_KEYUP:
			if (win->on_key_up)
				ui_whook_fire(win->on_key_up, win, e, NULL);
	}
	return win;
}

ui_win_t* ui_global_mousewheel(ui_globalApp_t *app, SDL_Event* e)
{
	ui_win_t *win = find_window_by_id(app, e->wheel.windowID);
	if (!win || win->flags & WIN_QUIT)
		return NULL;
	else if (win->on_mouse_wheel)
		ui_whook_fire(win->on_mouse_wheel, win, e, NULL);
	return win;
}

ui_win_t *ui_global_mousemotion(ui_globalApp_t* app, SDL_Event *e) {
	ui_win_t *win = find_window_by_id(app, e->motion.windowID);
	if (!win || win->flags & WIN_QUIT)
		return NULL;
	else if (win->on_mouse_motion)
		ui_whook_fire(win->on_mouse_motion, win, e, NULL);
	return win;
}

ui_win_t *ui_global_mouseclick(ui_globalApp_t* app, SDL_Event *e) {
	ui_win_t *win = find_window_by_id(app, e->button.windowID);
	if (!win || win->flags & WIN_QUIT)
		return NULL;
	switch (e->type) {
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
			if (win->on_click_down)
				ui_whook_fire(win->on_click_down, win, e, NULL);
			break;
		case SDL_MOUSEBUTTONUP:
			SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
			if (win->on_click_up)
				ui_whook_fire(win->on_click_up, win, e, NULL);
			break;
	}
	return win;
}

ui_win_t* ui_gloval_windowevent(ui_globalApp_t* app, SDL_Event* e)
{
	uint32_t win_id = e->window.windowID;
	ui_win_t *win = find_window_by_id(app, win_id);
	if(e->window.event == SDL_WINDOWEVENT_CLOSE && win_id == 1) {
		app->flags |= WIN_QUIT;
	} else if (e->window.event == SDL_WINDOWEVENT_RESIZED && win) {
		SDL_GL_GetDrawableSize(win->win, &win->area.w, &win->area.h);
		ui_win_get_scale(win);
	} else if(e->window.event == SDL_WINDOWEVENT_MOVED && win) {
		SDL_GetWindowPosition(win->win, &win->area.x, &win->area.y);
	}
	return win;
}

// ui_run implementation (main event loop)
void ui_start(ui_globalApp_t *app) {
    SDL_Event e;
    while (!(app->flags & WIN_QUIT)) {
		ui_win_t *curr = app->windows;
        while (SDL_PollEvent(&e)) {
			// ← cleanup BEFORE polling events, safe moment
			while (curr) {
				ui_win_t *next = curr->next;
				if (curr->flags & WIN_QUIT) {
					if (curr->id == 1)
						return;
					else
						remove_window(app, curr->id);
				}
				curr = next;
			}

			printf("event type=%u\n", e.type);
			fflush(stdout);
			ui_win_t *win = NULL;
			switch (e.type) {
				case SDL_QUIT:
					app->flags |= WIN_QUIT; return;
				case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
					win = ui_global_mouseclick(app, &e); break;
				case SDL_MOUSEMOTION:
					win = ui_global_mousemotion(app, &e); break;
				case SDL_MOUSEWHEEL:
					win = ui_global_mousewheel(app, &e); break;
				case SDL_KEYUP: case SDL_KEYDOWN:
					win = ui_global_keyboard(app, &e); break;
				case SDL_WINDOWEVENT:
					win = ui_gloval_windowevent(app, &e); break;
			}
			if (win)
				win->flags |= WIN_DIRTY;
		}
		curr = app->windows;
		while(curr) {
			ui_win_t* next = curr->next;
			if (!(curr->flags & WIN_QUIT)) {
				printf("win %d: update && render\n", curr->id);
				fflush(stdout);
				ui_whook_fire(curr->update, curr, NULL, NULL);
				ui_whook_fire(curr->render, curr, NULL, NULL);
			}
			curr = next;
		}
	}
	SDL_Delay(1);
}

// // Event registration functions (to set app's handlers)
// // These functions will be called by the application to set the global handlers
// void ui_on_key_down_set(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_KeyboardEvent*)) { app->on_key_down = handler; }
// void ui_on_key_up_set(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_KeyboardEvent*)) { app->on_key_up = handler; }
// void ui_on_mouse_button_down_set(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_MouseButtonEvent*)) { app->on_mouse_button_down = handler; }
// void ui_on_mouse_button_up_set(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_MouseButtonEvent*)) { app->on_mouse_button_up = handler; }
// void ui_on_mouse_motion_set(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_MouseMotionEvent*)) { app->on_mouse_motion = handler; }
// void ui_on_mouse_wheel_set(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_MouseWheelEvent*)) { app->on_mouse_wheel = handler; }
// void ui_on_window_resized_set(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, ui_win_t*, int w, int h)) { app->on_window_resized = handler; }
