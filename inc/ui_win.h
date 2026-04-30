#ifndef UI_WIN_H
#define UI_WIN_H

#include "libui.h"   // gets SDL, ui_rgba_t, ui_pos_t
#include "ui_box.h"  // New: Include ui_box.h

#define WIN_QUIT (1 << 0) 	//0001
#define WIN_SELECTED (1 << 1) //0010
#define WIN_MAIN (1 << 2) //0100
#define WIN_DIRTY (1 << 3) //1000
// Forward declaration
typedef struct ui_globalApp_s ui_globalApp_t;

// typedef struct ui_box_s ui_box_t;

typedef struct ui_win_s {
	SDL_Renderer* renderer;
	SDL_Window* win;
	SDL_Texture* texture;
	SDL_Rect area;
	ui_scale_t scale;
	char* title;
    uint32_t id; // New: Unique ID for the SDL Window
	short flags;
 
	ui_box_t *boxes;
	ui_rgba_t background_color;
	void(*on_key_down)(struct ui_win_s*, SDL_KeyboardEvent*);
	void(*on_key_up)(struct ui_win_s*, SDL_KeyboardEvent*);
	void(*on_mouse_down)(struct ui_win_s*, SDL_MouseButtonEvent*);
	void(*on_click_up)(struct ui_win_s*, SDL_MouseButtonEvent*);
	void(*on_mouse_wheel)(struct ui_win_s*, SDL_MouseWheelEvent*);
	void(*on_mouse_motion)(struct ui_win_s*, SDL_MouseMotionEvent*);
	void(*on_windows_event)(struct ui_win_s*, SDL_WindowEvent*);
	void(*destroy)(struct ui_win_s*);
	void(*update)(struct ui_win_s*);
	void(*render)(struct ui_win_s*);
	
	bool editable;
	struct ui_win_s *next;
} ui_win_t;

void ui_win_add_box(ui_win_t *win, ui_box_t *box);

void ui_win_on_click_up_handler(ui_win_t *win, SDL_MouseButtonEvent* e);
// void ui_win_on_resize_default(ui_win_t *win, SDL_WindowEvent*);
ui_scale_t ui_win_get_scale(ui_win_t *win);
ui_win_t* ui_win_create(int w, int h, char* title);

// principales Hooks de la fenetre. happen each frame
void	ui_win_render(ui_win_t* win);
void	ui_win_destroy(ui_win_t* win);
void 	ui_win_update(ui_win_t*);

// SDL_Rect ui_win_scale_rect(ui_win_t *win, SDL_Rect r);

// void	ui_win_set_resize(ui_win_t* win, void(*handler)(ui_win_t*, SDL_WindowEvent*));
// void	ui_win_set_mousewheel(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseWheelEvent*));
// void	ui_win_set_mousemotion(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseMotionEvent*));
// void	ui_win_set_mouseup(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseButtonEvent*));
// void	ui_win_set_mousedown(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseButtonEvent*));
// void	ui_win_set_keyup(ui_win_t* win, void(*handler)(ui_win_t*, SDL_KeyboardEvent*));
// void	ui_win_set_keydown(ui_win_t* win, void(*handler)(ui_win_t*, SDL_KeyboardEvent*));

// Event Hook
void ui_win_on_mouse_motion_handler(ui_win_t *win, SDL_MouseMotionEvent* e);

#endif
