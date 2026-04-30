#ifndef UI_GLOBAL_H
#define UI_GLOBAL_H
#include "libui.h"   // gets ui_pos_t fully defined, forward decl handles ui_win_t
#include "ui_box.h"  // New: Include ui_box.h

typedef struct ui_win_s ui_win_t;

typedef struct ui_globalApp_s {
	ui_pos_t 	mouse;
	ui_win_t 	*windows_list; // Renamed for clarity
	ui_win_t 	*selected_window;
	short 		flags;

    // Global Event Handlers
    void(*on_key_down)(struct ui_globalApp_s*, SDL_KeyboardEvent*);
    void(*on_key_up)(struct ui_globalApp_s*, SDL_KeyboardEvent*);
    void(*on_mouse_button_down)(struct ui_globalApp_s*, SDL_MouseButtonEvent*);
    void(*on_mouse_button_up)(struct ui_globalApp_s*, SDL_MouseButtonEvent*);
    void(*on_mouse_motion)(struct ui_globalApp_s*, SDL_MouseMotionEvent*);
    void(*on_mouse_wheel)(struct ui_globalApp_s*, SDL_MouseWheelEvent*);
    void(*on_window_resized)(struct ui_globalApp_s*, ui_win_t*, int w, int h); // Pass ui_win_t* to know which window resized

	void(*start)(struct ui_globalApp_s*);
} ui_globalApp_t;
 
ui_globalApp_t* ui_global_init();
void			ui_global_free(ui_globalApp_t*);
void 			ui_start(ui_globalApp_t *app); // This is ui_run
void            ui_global_add_window(ui_globalApp_t* app, ui_win_t* win); // New: Function to add window to list
void            ui_global_remove_window(ui_globalApp_t* app, ui_win_t* win); // New: Function to remove window from list

// Event registration functions for ui_globalApp_t
void ui_global_set_on_key_down(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_KeyboardEvent*));
void ui_global_set_on_key_up(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_KeyboardEvent*));
void ui_global_set_on_mouse_button_down(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_MouseButtonEvent*));
void ui_global_set_on_mouse_button_up(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_MouseButtonEvent*));
void ui_global_set_on_mouse_motion(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_MouseMotionEvent*));
void ui_global_set_on_mouse_wheel(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_MouseWheelEvent*));
void ui_global_set_on_window_resized(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, ui_win_t*, int w, int h));

#endif
