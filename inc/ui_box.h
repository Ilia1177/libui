#ifndef UI_BOX_H
#define UI_BOX_H

#include "libui.h" // Includes SDL
#include "ui_global.h" // For ui_globalApp_t
#include "ui_win.h" // For ui_win_t and its ID
#include "ui_elem.h"

// typedef enum menutype {
// 	UI_HORIZONTAL_MENU,
// 	UI_VERTICAL_MENU,
// 	UI_FULLWINDOW_MENU,
// 	UI_NONE
// } menutype_e;

#define BOX_MENU_H		50
#define BOX_MENU_W		200
#define MENU_GAP_X		10
#define MENU_GAP_Y		10
#define MENU_OFFSET_X	50
#define MENU_OFFSET_Y	50

// define current state of the box
#define BOX_HOVERED     (1 << 0)
#define BOX_PRESSED     (1 << 1)
#define BOX_HIDDEN      (1 << 2)
#define BOX_FOCUSED		(1 << 3)
#define BOX_CLICKED		(1 << 4)
#define BOX_DRAGGED 	(1 << 5)
#define BOX_DISABLE 	(1 << 6)
#define BOX_DIRTY	 	(1 << 7)  // need render
#define BOX_STALE	 	(1 << 8) // need update

// define what is the box
#define BOX_INPUTABLE 	(1 << 9)
#define BOX_CLICKABLE 	(1 << 10)
#define BOX_HOVERABLE 	(1 << 11)

#define INPUT_SIZE_MAX	64

typedef struct s_ui_input_data {
    char* buffer;
    int size;
    int sizemax;
} ui_input_data_t;

typedef void (*ui_bhook_fn_t)(ui_box_t*, SDL_Event*, void*);

typedef struct ui_boxhandler_s
{
	ui_bhook_fn_t fn;
	struct ui_boxhandler_s *next;
} ui_boxhandler_t;


typedef struct padding_s {
	int top;
	int bottom;
	int left;
	int right;
} padding_t;

typedef struct ui_box_s 
{
	SDL_Rect		area;
    SDL_Color       color;
	int 				border;
	SDL_Point		zoom_origin;
	float			zoom_amt;
	padding_t		padding;
    uint32_t        state;
    uint32_t        layout;
	const char*		label;
	void*			data;
	ui_layer_t*		layers;
    ui_win_t*		parent_window;

	ui_boxhandler_t *on_window_event;
	ui_boxhandler_t *on_key_down;
	ui_boxhandler_t *on_mouse_wheel;
	ui_boxhandler_t *on_mouse_motion;
    ui_boxhandler_t *on_click_down;
    ui_boxhandler_t *on_click_up;

	ui_boxhandler_t *update;
	ui_boxhandler_t *destroy;
    ui_boxhandler_t *render;

	struct ui_box_s *parent;
	struct ui_box_s *childs;
    struct ui_box_s *next;
    struct ui_box_s *prev;
} ui_box_t;

// ui_box_t*	ui_elem_menu_list(ui_box_t* menu, char*);
// ui_box_t*	ui_elem_menu_item(ui_box_t* list, char* label, ui_bhook_fn_t);
// ui_box_t*	ui_elem_menu_navbar(ui_win_t *win, menutype_e type);
// ui_box_t*	ui_box_menu_list_option_add(ui_win_t *win, ui_box_t **list, void(*fn)(ui_box_t*, SDL_Event*, void*));

void ui_box_move(ui_box_t* b, SDL_Rect move);
void 	ui_box_swap(ui_box_t *a, ui_box_t *b); // not used
void 	ui_box_center_layers(ui_box_t* box, SDL_Rect* r);
void 	ui_box_apply_all(ui_box_t *list, ui_bhook_fn_t fn);
ui_box_t* ui_box_hovered(ui_box_t* boxes, SDL_Point *p);
ui_box_t*	ui_box_last(ui_box_t* boxes);
ui_box_t*	ui_box_create(ui_win_t* parent_window, const uint32_t layout, SDL_Color color);
ui_box_t*	ui_box_iter(ui_box_t *b, int n);
void		ui_box_add_root(ui_box_t **parent, ui_box_t *new);
void		ui_box_add_child(ui_box_t *parent, ui_box_t *new);
int			ui_box_count_prev(ui_box_t *);
int			ui_box_count_next(ui_box_t *);
int			ui_box_count_all(ui_box_t *);
void		ui_box_flags(ui_box_t *b, short flag, bool add, bool all);

void ui_box_layout(ui_box_t* b, uint32_t flag, bool add, bool all);
void		ui_box_bring_to_front(ui_box_t *b);
int			ui_box_remove(ui_box_t **list, ui_box_t* box);

ui_box_t* ui_get_box_by_name(ui_box_t*, const char* label);
// Hook tools
void	ui_bhook_clean(ui_boxhandler_t **list);
void	ui_bhook_prepend(ui_boxhandler_t **list, ui_bhook_fn_t fn);
void	ui_bhook_append(ui_boxhandler_t **list, ui_bhook_fn_t fn);
void	ui_bhook_remove(ui_boxhandler_t **list, ui_bhook_fn_t fn);
void	ui_bhook_fire(ui_boxhandler_t*, ui_box_t*, SDL_Event*, void*);
void	ui_bhook_replace(ui_boxhandler_t *handler, ui_bhook_fn_t old, ui_bhook_fn_t new);



// Loop hooks added by default to the box obj;
void      	ui_bhook_destroy_default(ui_box_t* box, SDL_Event* e, void* data);
void 		ui_bhook_update_default(ui_box_t* box, SDL_Event *e, void *data);
void      	ui_bhook_render_default(ui_box_t* box, SDL_Event *e, void *data);
void      	ui_bhook_mousewheel_default(ui_box_t* box, SDL_Event *e, void *data);


// render hook
void ui_bhook_drawbox(ui_box_t* box, SDL_Event* e, void* data);
void ui_bhook_drawpressed(ui_box_t* box, SDL_Event* e, void* data);
void ui_bhook_drawfocused(ui_box_t* box, SDL_Event* e, void* data);
void ui_bhook_drawhovered(ui_box_t* box, SDL_Event* e, void* data);
void ui_bhook_drawcliplayers(ui_box_t* box, SDL_Event* e, void* data);
void ui_bhook_drawlayers(ui_box_t* box, SDL_Event* e, void* data);
void ui_bhook_drawtextfocused(ui_box_t* box, SDL_Event* e, void* data);
// Event hooks added by default to the box obj;
void		ui_bhook_windowevent_default(ui_box_t *b, SDL_Event* e, void* data);
void		ui_bhook_clickup_default(ui_box_t *, SDL_Event* e, void* data);
void		ui_bhook_clickdown_default(ui_box_t *,  SDL_Event* e, void* data);
void 		ui_bhook_mousemotion_default(ui_box_t *box, SDL_Event* e, void* data);

// Custom hooks;
//
void ui_bhook_drawborder(ui_box_t *cnv, SDL_Event *e, void *data);
void ui_bhook_zoomin(ui_box_t *cnv, SDL_Event *e, void *data);
// void ui_bhook_fullwindow_button(ui_box_t* btn, SDL_Event*e, void* data);
void	ui_bhook_valid_input(ui_box_t* b, SDL_Event* e, void* data);
// void	ui_bhook_canvassize(ui_box_t* b, SDL_Event *e, void* data);
void	ui_bhook_inputcancel(ui_box_t *box, SDL_Event *e, void *data);
void	ui_bhook_inputvalid(ui_box_t *box, SDL_Event *e, void *data);
void	ui_bhook_drawcliplayers(ui_box_t* box, SDL_Event* e, void* data);
// void	ui_bhook_catch_input(ui_box_t *box, SDL_Event *e, void *data);
void	ui_bhook_wincenter(ui_box_t* box, SDL_Event* e, void* data);
void	ui_bhook_fullheight(ui_box_t*, SDL_Event*, void*);
void	ui_bhook_fullwidth(ui_box_t*, SDL_Event*, void*);
// void	ui_bhook_inputfocus(ui_box_t*, SDL_Event*, void*);
void	ui_bhook_revealchild(ui_box_t *box, SDL_Event* e, void* data);
// void	ui_bhook_nofocused(ui_box_t*, SDL_Event*, void*);
// void	ui_bhook_nohovered(ui_box_t*, SDL_Event*, void*);
// void	ui_bhook_nopressed(ui_box_t*, SDL_Event*, void*);
void	ui_bhook_winclose(ui_box_t*, SDL_Event*, void*);
// void	ui_bhook_maxsize(ui_box_t *cnv, SDL_Event *e, void *data);
void	ui_bhook_movelayer(ui_box_t*, SDL_Event*, void*);
void	ui_bhook_label2texture(ui_box_t *box, SDL_Event *e, void* data);
#endif
