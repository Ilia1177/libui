#ifndef UI_BOX_H
#define UI_BOX_H

#include "libui.h" // Includes SDL
#include "ui_global.h" // For ui_globalApp_t
#include "ui_win.h" // For ui_win_t and its ID

#define BOX_MENU_H		40
#define BOX_MENU_W		100
#define BOX_HOVERED     (1 << 0)
#define BOX_PRESSED     (1 << 1)
#define BOX_HIDDEN      (1 << 2)
#define BOX_DIRTY 		(1 << 3)

// Forward declarations if not already included through libui.h/ui_global.h/ui_win.h
// typedef struct ui_globalApp_s  ui_globalApp_t;
// typedef struct ui_win_s        ui_win_t;

typedef struct ui_box_s {
	SDL_Rect		area;
    ui_rgba_t       color;
	int				padding;
	int 			border;
    short           flags; // For BOX_HOVERED, BOX_PRESSED, etc.
	char*			label;
    // Event handlers for the box (now taking ui_globalApp_t* and SDL_Event*)
	void			(*on_mouse_motion)(struct ui_box_s*,  SDL_Event*);
    void            (*on_click_down)(struct ui_box_s*,  SDL_Event*);
    void            (*on_click_up)(struct ui_box_s*,  SDL_Event*);

	void			(*update) (struct ui_box_s*);
    void            (*render)(struct ui_box_s*); // Render function

	struct ui_box_s *child_boxes;
    struct ui_box_s *next;
    ui_win_t*      parent_window; // To know which window it belongs to
} ui_box_t;

// Function declarations for ui_box

	int ui_box_count(ui_box_t *);
ui_box_t *ui_box_menu_list_create(char* label, ui_win_t *win, int subnb);
ui_box_t 	*ui_box_menu_create(ui_win_t*win, char** av);
ui_box_t	*ui_box_create(SDL_Rect rect, ui_rgba_t color, ui_win_t* parent_window);
void		ui_box_add(ui_box_t **list, ui_box_t *new);
void      	ui_box_destroy(ui_box_t* box);
ui_box_t* ui_box_iter(ui_box_t *b, int n);

void 		ui_box_update_default(ui_box_t* box);
void      	ui_box_render_default(ui_box_t* box);

// Event registration functions for ui_box_t
// void ui_box_set_on_mouse_motion(ui_box_t* box, void(*handler)(ui_box_t*, SDL_Event*));
// void ui_box_set_on_click_down(ui_box_t* box, void(*handler)(ui_box_t*, SDL_Event*));
// void ui_box_set_on_click_up(ui_box_t* box, void(*handler)(ui_box_t*, SDL_Event*));

// Specific handler implementations declared (now taking ui_globalApp_t* and SDL_Event*)
void		ui_box_on_click_up_handler(ui_box_t *, SDL_Event* e);
void		ui_box_on_click_down_handler(ui_box_t *,  SDL_Event* e);
void 		ui_box_on_mouse_motion_handler(ui_box_t *box, SDL_Event* e);

#endif
