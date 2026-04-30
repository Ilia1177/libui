#ifndef UI_BOX_H
#define UI_BOX_H

#include "libui.h"

#define BOX_HOVERED     (1 << 0)
#define BOX_PRESSED     (1 << 1)
#define BOX_HIDDEN      (1 << 2)
#define BOX_DIRTY 		(1 << 3)

typedef struct ui_globalApp_s  ui_globalApp_t;
typedef struct ui_win_s        ui_win_t;

typedef struct ui_box_s {
	SDL_Rect		area;
    ui_rgba_t       color;
	int				padding;
	int 			border;
    short           flags; // For BOX_HOVERED, BOX_PRESSED, etc.
	char*			label;
    // Event handlers for the box (take box itself, global app context, and event data)
	void			(*on_mouse_motion)(struct ui_box_s*, SDL_MouseMotionEvent*);
    void            (*on_click_down)(struct ui_box_s*, SDL_MouseButtonEvent*);
    void            (*on_click_up)(struct ui_box_s*, SDL_MouseButtonEvent*);

	void			(*update) (struct ui_box_s*);
    void            (*render)(struct ui_box_s*); // Render function

	struct ui_box_s *child_boxes;
    struct ui_box_s *next;
    ui_win_t*      parent_window; // To know which window it belongs to
} ui_box_t;

ui_box_t 	*ui_box_menu_create(ui_win_t*win, char** av);
ui_box_t	*ui_box_create(SDL_Rect rect, ui_rgba_t color, ui_win_t* parent_window);
void		ui_box_add(ui_box_t **list, ui_box_t *new);
void      	ui_box_destroy(ui_box_t* box);

void 		ui_box_update_default(ui_box_t* box);
void      	ui_box_render_default(ui_box_t* box);

void		ui_box_on_click_up_handler(ui_box_t *, SDL_MouseButtonEvent* e);
void		ui_box_on_click_down_handler(ui_box_t *, SDL_MouseButtonEvent* e);
void 		ui_box_on_mouse_motion_handler(ui_box_t *box, SDL_MouseMotionEvent* e);
#endif
