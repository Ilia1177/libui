#include "libui.h"
#include "math.h"
// #include "libft.h"

// in ui_bhook_zoomin — capture mouse at scroll time
// void ui_bhook_zoomin_save(ui_box_t *cnv, SDL_Event *e, void *data) {
//     (void)data;
//     if (!cnv || !e || e->type != SDL_MOUSEWHEEL) return;
//     if (!cnv->layers) return;
//
//     float delta = (e->wheel.y > 0) ? 0.1f : -0.1f;
//     cnv->zoom_amt = clampf(
//         cnv->zoom_amt + delta, 0.1f, 10.0f);
//
//     // freeze zoom origin at current mouse pos
//     cnv->zoom_origin = ui_box_mousepos(cnv);
//
//     cnv->parent_window->state |= WIN_DIRTY;
// }


// void ui_bhook_nopressed(ui_box_t *b, SDL_Event *e, void* data) {
// 	(void)e;
// 	(void)data;
// 	b->flags &= ~BOX_PRESSED;
// }

// void ui_bhook_nofocused(ui_box_t *b, SDL_Event *e, void* data) {
// 	(void)e;
// 	(void)data;
// 	b->flags &= ~BOX_FOCUSED;
// }

// void ui_bhook_nohovered(ui_box_t *b, SDL_Event *e, void* data) {
// 	(void)e;
// 	(void)data;
// 	b->flags &= ~BOX_HOVERED;
// }



//
// void ui_bhook_inputcancel(ui_box_t *box, SDL_Event *e, void *data) 
// {
// 	(void)data;
// 	(void)e;
// 	ui_globalApp_t *app = box->parent_window->global;
//
// 	if ((e && e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE)
// 			|| box->flags & BOX_CLICKED) {
// 		app->loading = false;
// 		app->state &= ~APP_LOADING;
// 		box->flags &= ~BOX_FOCUSED;
// 		box->parent_window->state |= WIN_QUIT;
// 		box->parent_window->global->windows->state |= WIN_DIRTY;
// 		if (box->data)
// 			free(box->data);
// 		box->data = NULL;
// 	}
// }

void ui_bhook_winclose(ui_box_t* b, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win = b->parent_window;
	if (b->flags & BOX_CLICKED) {
		win->state |= WIN_QUIT;
	}
}

void	ui_bhook_wincenter(ui_box_t* box, SDL_Event* e, void* data) {
	(void)e;
	SDL_Rect* offset = (SDL_Rect*)data;

	ui_win_t *win = box->parent_window;
	// ui_globalApp_t *ref = win->global;
	SDL_Rect area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
	int x = 0;
	int y = 0;
	if (offset) {
		x = offset->x;
		y = offset->y;
	}
	box->area = ui_area_center(
		(SDL_Rect){x, y, win->area.w, win->area.h},
		(SDL_Rect){0, 0, area.w, area.h});
	if (box->layers) {
		box->layers->area = ui_area_center(box->area, box->layers->area);
	}
}

