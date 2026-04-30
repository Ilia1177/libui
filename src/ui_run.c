#include "libui.h"

// void ui_start(ui_globalApp* app) {
// 	SDL_Event e;
// 	while (!(app->flags & WIN_QUIT)) { // Check win->quit_flag
// 		while (SDL_PollEvent(&e)) {
// 			if (e.type == SDL_QUIT) {
// 				app->flags |= WIN_QUIT;
// 			}
// 			ui_win_t* win = app->selected_window;
// 			// --- New event dispatching logic ---
// 			else if (e.type == SDL_KEYDOWN) {
// 				if (win->on_key_down) {
// 					win->on_key_down(app, &e.key);
// 				}
// 			}
// 			else if (e.type == SDL_KEYUP) {
// 				if (win->on_key_up) {
// 					win->on_key_up(app, &e.key);
// 				}
// 			}
// 			else if (e.type == SDL_MOUSEBUTTONDOWN) {
// 				if (win->on_mouse_button_down) {
// 					win->on_mouse_button_down(app, &e.button);
// 				}
// 			}
// 			else if (e.type == SDL_MOUSEBUTTONUP) {
// 				if (win->on_mouse_button_up) {
// 					win->on_mouse_button_up(app, &e.button);
// 				}
// 			}
// 			else if (e.type == SDL_MOUSEMOTION) {
// 				if (win->on_mouse_motion) {
// 					win->on_mouse_motion(app, &e.motion);
// 				}
// 			}
//             else if (e.type == SDL_MOUSEWHEEL) {
//                 if (win->on_mouse_wheel) {
//                     win->on_mouse_wheel(app, &e.wheel);
//                 }
//             }
//             else if (e.type == SDL_WINDOWEVENT) {
//                 if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
//                     win->w = e.window.data1;
//                     win->h = e.window.data2;
//                     // if (win->on_window_resized) {
//                         // win->on_window_resized(win, win->w, win->h);
//                     // }
//                 }
//             }
// 			// --- End new event dispatching logic ---
// 		}
// 		app->render(app);
// 	}
// }

