#ifndef UI_BOX_H
#define UI_BOX_H

#include "libui.h" // Includes SDL
#include "ui_global.h" // For ui_globalApp_t
#include "ui_win.h" // For ui_win_t and its ID

#define BOX_MENU_H		50
#define BOX_MENU_W		200
#define MENU_GAP_X		10
#define MENU_GAP_Y		4
#define MENU_OFFSET_X	50
#define MENU_OFFSET_Y	2

#define BOX_HOVERED     (1 << 0)
#define BOX_PRESSED     (1 << 1)
#define BOX_HIDDEN      (1 << 2)
#define BOX_DIRTY 		(1 << 3)

typedef struct ui_boxhandler_s {
	void(*fn)(ui_box_t*, SDL_Event*, void*);
	struct ui_boxhandler_s *next;
} ui_boxhandler_t;

typedef struct ui_box_s {
	SDL_Rect		area;
    SDL_Color       color;
	int				padding;
	int 			border;
    short           flags; // For BOX_HOVERED, BOX_PRESSED, etc.
	char*			label;
	ui_layer_t*		layers;
    // Event handlers for the box (now taking ui_globalApp_t* and SDL_Event*)
	ui_boxhandler_t *on_mouse_motion;
    ui_boxhandler_t *on_click_down;
    ui_boxhandler_t *on_click_up;

	ui_boxhandler_t *update;
	ui_boxhandler_t *destroy;
    ui_boxhandler_t *render;

	struct ui_box_s *list;
    struct ui_box_s *next;
    ui_win_t*      parent_window; // To know which window it belongs to
} ui_box_t;

ui_box_t*	ui_elem_menulist(ui_box_t* menu, char*);
ui_box_t*	ui_elem_menubutton(ui_box_t* list, void(*fn)(ui_box_t*, SDL_Event*, void*));
ui_box_t*	ui_elem_menubar(ui_win_t *win, char** labels);
// ui_box_t*	ui_box_menu_list_option_add(ui_win_t *win, ui_box_t **list, void(*fn)(ui_box_t*, SDL_Event*, void*));

ui_box_t* ui_box_last(ui_box_t* boxes);
ui_box_t*	ui_box_create(SDL_Rect rect, SDL_Color color, ui_win_t* parent_window);
ui_box_t*	ui_box_iter(ui_box_t *b, int n);
void		ui_box_add(ui_box_t **list, ui_box_t *new);
int			ui_box_count(ui_box_t *);
int			ui_box_count_all(ui_box_t *);
void		ui_box_flags(ui_box_t *b, short flag, bool add);

// Hook tools
void	ui_bhook_clean(ui_boxhandler_t **list);
void	ui_bhook_add(ui_boxhandler_t**, void(*fn)(ui_box_t*, SDL_Event*, void*));
void	ui_bhook_fire(ui_boxhandler_t*, ui_box_t*, SDL_Event*, void*);
void	ui_bhook_add(ui_boxhandler_t **list, void (*fn)(ui_box_t*, SDL_Event*, void*));

// Loop hooks added by default to the box obj;
void      	ui_bhook_destroy_default(ui_box_t* box, SDL_Event* e, void* data);
void 		ui_bhook_update_default(ui_box_t* box, SDL_Event *e, void *data);
void      	ui_bhook_render_default(ui_box_t* box, SDL_Event *e, void *data);

// Event hooks added by default to the box obj;
void		ui_bhook_clickup_default(ui_box_t *, SDL_Event* e, void* data);
void		ui_bhook_clickdown_default(ui_box_t *,  SDL_Event* e, void* data);
void 		ui_bhook_mousemotion_default(ui_box_t *box, SDL_Event* e, void* data);

// Custom hooks;
void	ui_bhook_unhidechild(ui_box_t *box, SDL_Event* e, void* data);
void	ui_bhook_nohovered(ui_box_t*, SDL_Event*, void*);
void	ui_bhook_nopressed(ui_box_t*, SDL_Event*, void*);
void	ui_bhook_winclose(ui_box_t*, SDL_Event*, void*);
void	ui_bhook_maxsize(ui_box_t *cnv, SDL_Event *e, void *data);
void	ui_bhook_movelayer(ui_box_t*, SDL_Event*, void*);
void	ui_bhook_label2texture(ui_box_t *box, SDL_Event *e, void* data);
#endif
