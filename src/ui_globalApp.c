#include "ui_global.h" // Includes ui_globalApp_t
#include <SDL2/SDL.h> // For SDL_GetWindowFromID, SDL_GetMouseState, etc.
#include <stdio.h> // For printf debugging
#include <stdlib.h> // For malloc, free
#include "ui_win.h"

// Helper to find ui_win_t from SDL_WindowID
ui_win_t* find_window_by_id(ui_globalApp_t* app, uint32_t window_id) {
    ui_win_t* current_win = app->windows_list; 
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

    app->windows_list = NULL; // Initialize list of windows_list
	app->windows_list = ui_win_create(400, 400, "main");
	SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
	app->selected_window = app->windows_list;
    app->flags = 0;
    app->start = ui_start; // Set the start function pointer

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
    if (app) {
        // Free all windows managed by the app
        ui_win_t* current = app->windows_list;
        while (current) {
            ui_win_t* next = current->next;
            ui_win_destroy(current); // Call window destroy function
            current = next;
        }
        ui_quit(); // Quit SDL
        free(app);
    }
}

// ui_run implementation (main event loop)
void ui_start(ui_globalApp_t *app) {
    SDL_Event e;
    while (!(app->flags & WIN_QUIT)) {
        while (SDL_PollEvent(&e)) {
			    printf("event type=%u\n", e.type);
				fflush(stdout);
			uint32_t win_id = 0;
			ui_win_t *win = NULL;
			switch (e.type) {
				case SDL_QUIT:
					app->flags |= WIN_QUIT;
					return;
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetGlobalMouseState(&app->mouse.x, &app->mouse.y);
					win_id = e.button.windowID;
					if ((win = find_window_by_id(app, win_id)) && win->on_mouse_down)
						win->on_mouse_down(win, &e.button);
					break;
				case SDL_MOUSEBUTTONUP:
					win_id = e.button.windowID;
					if ((win = find_window_by_id(app, win_id)) && win->on_click_up)
						win->on_click_up(win, &e.button);
					break;
				case SDL_MOUSEMOTION:
					win_id = e.motion.windowID;
					printf("App: on mouse motion\n");
					fflush(stdout);
					if ((win = find_window_by_id(app, win_id)) && win->on_mouse_motion)
						win->on_mouse_motion(win, &e.motion);
					break;
				case SDL_MOUSEWHEEL:
					win_id = e.wheel.windowID;
					if ((win = find_window_by_id(app, win_id)) && win->on_mouse_wheel)
						win->on_mouse_wheel(win, &e.wheel);
					break;
				case SDL_KEYDOWN:
					win_id = e.key.windowID;
					if((win = find_window_by_id(app, win_id)) && win->on_key_down)
						win->on_key_down(win, &e.key);
					break;
				case SDL_KEYUP:
					win_id = e.key.windowID;
					if((win = find_window_by_id(app, win_id)) && win->on_key_up)
						win->on_key_up(win, &e.key);
					break;

				case SDL_WINDOWEVENT:
					if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
						app->flags |= WIN_QUIT;
						// remove_window(app, win_id);
					}
					win_id = e.window.windowID;
					if((win = find_window_by_id(app, win_id))) {
						if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
							SDL_GL_GetDrawableSize(win->win, &win->area.w, &win->area.h);
							ui_win_get_scale(win);
						} else if(e.window.event == SDL_WINDOWEVENT_MOVED) {
							SDL_GetWindowPosition(win->win, &win->area.x, &win->area.y);
						}
						if (win->on_windows_event)          // optional user reaction
							win->on_windows_event(win, &e.window);
					}
					break;
				}
				if (!win) {
					break;
				}
				win->flags |= WIN_DIRTY;
				if (win->update)
					win->update(win);
				if (win->render)
					win->render(win);
			}
			// ui_win_t* current_win = app->windows_list;
			// while (current_win) {
			// 	if (current_win->update)
			// 		current_win->update(current_win);
			// 	if (current_win->render)
			// 		current_win->render(current_win);
			// 	current_win = current_win->next;
			// }
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
