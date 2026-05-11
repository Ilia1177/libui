#ifndef UI_GLOBAL_H
#define UI_GLOBAL_H
#include "libui.h"   // gets ui_pos_t fully defined, forward decl handles ui_win_t
#include "ui_box.h"  // New: Include ui_box.h

typedef struct ui_winhandler_s ui_winhandler_t;
typedef struct ui_win_s ui_win_t;

# define GLOBAL_LOADING (1 << 0)
# define GLOBAL_QUIT (1 << 1)

typedef struct ui_globalApp_s 
{
	SDL_Point 	mouse;
	ui_win_t*	windows;
	ui_box_t*	focused_box;
	uint32_t 	state;
	char*		input;

	bool loading;
	// SDL_Rect  button_area;
	SDL_Color menu_color_1;
	SDL_Color menu_color_2;
    // Global Event Handlers
    void(*on_key_down)(struct ui_globalApp_s*, SDL_Event*);
    void(*on_key_up)(struct ui_globalApp_s*, SDL_Event*);
    void(*on_mouse_button_down)(struct ui_globalApp_s*, SDL_Event*);
    void(*on_mouse_button_up)(struct ui_globalApp_s*, SDL_Event*);
    void(*on_mouse_motion)(struct ui_globalApp_s*, SDL_Event*);
    void(*on_mouse_wheel)(struct ui_globalApp_s*, SDL_Event*);
    void(*on_window_resized)(struct ui_globalApp_s*, ui_win_t*, int w, int h);

	ui_winhandler_t *tool;
	ui_winhandler_t *actions;
	void(*start)(struct ui_globalApp_s*);
} ui_globalApp_t;
 
ui_globalApp_t* ui_global_init(char* name);
void			ui_global_free(ui_globalApp_t*);
void 			ui_start(ui_globalApp_t *app); // This is ui_run
void            ui_global_add_window(ui_globalApp_t* app, ui_win_t* win); // New: Function to add window to list
void            ui_global_remove_window(ui_globalApp_t* app, ui_win_t* win); // New: Function to remove window from list

// Event registration functions for ui_globalApp_t (now taking SDL_Event*)
void ui_global_set_on_key_down(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_Event*));
void ui_global_set_on_key_up(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_Event*));
void ui_global_set_on_mouse_button_down(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_Event*));
void ui_global_set_on_mouse_button_up(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_Event*));
void ui_global_set_on_mouse_motion(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_Event*));
void ui_global_set_on_mouse_wheel(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_Event*));
void ui_global_set_on_window_resized(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, ui_win_t*, int w, int h)); // Keep this signature

#endif
