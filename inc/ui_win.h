#ifndef UI_WIN_H
#define UI_WIN_H

#include "libui.h"   // gets SDL, SDL_Color, ui_pos_t

#define WIN_QUIT (1 << 0) 	//0001
#define WIN_SELECTED (1 << 1) //0010
#define WIN_MAIN (1 << 2) //0100
#define WIN_DIRTY (1 << 3) //1000
// typedef int (*ui_whook_fn_t)(ui_win_t*, SDL_Event*, void*);
typedef struct ui_winhandler_s {
	ui_whook_fn_t fn;
    struct ui_winhandler_s *next;
} ui_winhandler_t;
// Forward declaration
typedef struct ui_globalApp_s ui_globalApp_t;
typedef struct ui_box_s ui_box_t;
typedef struct ui_win_s
{
	SDL_Texture*	cache;
	SDL_Renderer*	renderer;
	SDL_Window*		ptr;
	SDL_Rect		area;
	ui_scale_t		scale;
	SDL_Color		*colors;
    uint32_t		id;
	short			state;
 
	// SDL_Rect		button_area;
	ui_globalApp_t	*global;
	ui_box_t		*boxes;
	ui_box_t		*canvas;
	float			zoom;

	const char*		label;
	SDL_Color		background_color;
	TTF_Font		*font;
	ui_winhandler_t *on_mouse_wheel;
	ui_winhandler_t *on_key_down;
	ui_winhandler_t *on_key_up;
	ui_winhandler_t *on_click_down;
	ui_winhandler_t *on_click_up;
	ui_winhandler_t *on_mouse_motion;
	ui_winhandler_t *on_window_event;
	ui_winhandler_t *destroy;
	ui_winhandler_t *update;
	ui_winhandler_t *render;
	struct ui_win_s *next;
} ui_win_t;

void ui_tool_drawcenter(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect box);
void ui_win_add_box(ui_win_t *win, ui_box_t *box);
void ui_win_add(ui_win_t **list, ui_win_t *win);

ui_scale_t ui_win_get_scale(ui_win_t *win);
ui_win_t* ui_win_create(ui_globalApp_t * app, SDL_Rect, const char* title, uint32_t flags);
int ui_whook_remove(ui_winhandler_t **list, ui_whook_fn_t fn);
 
int ui_whook_keydown_default(ui_win_t* win, SDL_Event* e, void* data);
int ui_whook_reset_global_state(ui_win_t* win, SDL_Event *e, void* data);
// int ui_whook_windirty(ui_win_t* win, SDL_Event *e, void *data);
// int ui_whook_quitkey(ui_win_t* win, SDL_Event *e, void *data);
int ui_whook_loadimage(ui_win_t* win, SDL_Event* e, void *pathraw);
// principales Hooks de la fenetre. happen each frame
int ui_whook_render_default(ui_win_t*, SDL_Event*, void*);
int ui_whook_destroy_default(ui_win_t*, SDL_Event*, void*);
int ui_whook_update_default(ui_win_t*, SDL_Event*, void*);
int ui_whook_clickup_default(ui_win_t*, SDL_Event*, void *);
int ui_whook_clickdown_default(ui_win_t*, SDL_Event*, void *);
int ui_whook_mousemotion_default(ui_win_t*, SDL_Event*, void *);
int ui_whook_mousewheel_default(ui_win_t*, SDL_Event*, void *);
int ui_whook_windowevent_default(ui_win_t*, SDL_Event*, void *);
int ui_whook_fire(ui_winhandler_t **list, ui_win_t *win, SDL_Event *e, void* data);
int ui_whook_prepend(ui_winhandler_t **list, ui_whook_fn_t fn); 
int ui_whook_append(ui_winhandler_t **list, ui_whook_fn_t fn);
int ui_whook_clean(ui_winhandler_t **list);
ui_win_t* ui_get_win_by_name(ui_globalApp_t* app, const char* name);
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
