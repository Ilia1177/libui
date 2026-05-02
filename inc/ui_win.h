#ifndef UI_WIN_H
#define UI_WIN_H

#include "libui.h"   // gets SDL, ui_rgba_t, ui_pos_t

#define WIN_QUIT (1 << 0) 	//0001
#define WIN_SELECTED (1 << 1) //0010
#define WIN_MAIN (1 << 2) //0100
#define WIN_DIRTY (1 << 3) //1000

typedef struct ui_winhandler_s {
    void (*fn)(ui_win_t*, SDL_Event*, void*);
    struct ui_winhandler_s *next;
} ui_winhandler_t;
// Forward declaration
typedef struct ui_globalApp_s ui_globalApp_t;
typedef struct ui_box_s ui_box_t;
typedef struct ui_win_s {
	SDL_Renderer* renderer;
	SDL_Window* win;
	SDL_Texture* texture;
	SDL_Rect area;
	ui_scale_t scale;
	char* title;
    uint32_t id; // New: Unique ID for the SDL Window
	short flags;
 
	SDL_Rect button_area;
	ui_globalApp_t *global;
	ui_box_t *menu;
	ui_box_t *canvas;
	ui_rgba_t background_color;


    // Updated: Event handler function pointers now take SDL_Event*
	ui_winhandler_t *on_key_down;
	ui_winhandler_t *on_key_up;
	ui_winhandler_t *on_click_down;
	ui_winhandler_t *on_click_up;
	ui_winhandler_t *on_mouse_wheel;
	ui_winhandler_t *on_mouse_motion;
	ui_winhandler_t *on_windows_event;
	ui_winhandler_t *destroy;
	ui_winhandler_t *update;
	ui_winhandler_t *render;
	
	bool editable;
	struct ui_win_s *next;
} ui_win_t;

void ui_win_add_box(ui_win_t *win, ui_box_t *box);

ui_scale_t ui_win_get_scale(ui_win_t *win);
ui_win_t* ui_win_create(int w, int h, ui_globalApp_t * app, char* title);
 
// principales Hooks de la fenetre. happen each frame
void	ui_win_render_default(ui_win_t*, SDL_Event*, void*);
void	ui_win_destroy_default(ui_win_t*, SDL_Event*, void*);
void 	ui_win_update_default(ui_win_t*, SDL_Event*, void*);
 
void	ui_win_on_click_up_handler(ui_win_t*, SDL_Event*, void *);
void	ui_win_on_click_down_handler(ui_win_t*, SDL_Event*, void *);
void	ui_win_on_mouse_motion_handler(ui_win_t*, SDL_Event*, void *);

void ui_win_event_fire(ui_winhandler_t *list, ui_win_t *win, SDL_Event *e, void* data);
void ui_win_handler_add(ui_winhandler_t **list, void (*fn)(ui_win_t*, SDL_Event*, void*));
void ui_win_handler_clean(ui_winhandler_t **list);
// New: Event registration functions (using SDL_Event*)
// void ui_win_set_on_resize(ui_win_t* win, void(*handler)(ui_win_t*, SDL_Event*));
// void ui_win_set_on_mousewheel(ui_win_t* win, void(*handler)(ui_win_t*, SDL_Event*));
// void ui_win_set_on_mousemotion(ui_win_t* win, void(*handler)(ui_win_t*, SDL_Event*));
// void ui_win_set_on_mouseup(ui_win_t* win, void(*handler)(ui_win_t*, SDL_Event*));
// void ui_win_set_on_mousedown(ui_win_t* win, void(*handler)(ui_win_t*, SDL_Event*));
// void ui_win_set_on_keyup(ui_win_t* win, void(*handler)(ui_win_t*, SDL_Event*));
// void ui_win_set_on_keydown(ui_win_t* win, void(*handler)(ui_win_t*, SDL_Event*));
// void ui_win_set_on_click_up(ui_win_t* win, void(*handler)(ui_win_t*, SDL_Event*));
// void ui_win_set_on_windows_event(ui_win_t* win, void(*handler)(ui_win_t*, SDL_Event*));


#endif
