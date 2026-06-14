#include "ui_win.h"
#include "ui_box.h"

void ui_bhook_mousemotion_default(ui_box_t *box, SDL_Event* e, void* data)
{
	(void)data;
	(void)e;
	ui_win_t* win;
	SDL_Point p;

	uint32_t previous = box->flags;
	win = box->parent_window;
	p = ui_win_mousepos(win);

	ui_box_t *hovered_box = ui_box_hovered(win->boxes, &p);
	if(hovered_box == box) {
		box->flags |= BOX_HOVERED;
	} else if (ui_box_hovered(win->canvas, &p) == box && !hovered_box) {
		box->flags |= BOX_HOVERED;
	} else {
		box->flags &= ~BOX_HOVERED;
        box->flags &= ~BOX_PRESSED;
	}

	if (previous != box->flags) {
		box->parent_window->state |= WIN_DIRTY;
		box->flags |= BOX_DIRTY;
		ui_box_flags(box->childs, BOX_DIRTY, true, true);
	}
}
