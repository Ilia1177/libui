#include "ui_win.h"
#include "ui_box.h"

void ui_bhook_clickdown_default(ui_box_t *b, SDL_Event* e, void* data) {
	(void)data;
	(void)e;
	SDL_Point p;

	ui_log("check box clicked down");
	uint32_t previous = b->flags;
	p = ui_win_mousepos(b->parent_window);
    if (SDL_PointInRect(&p, &b->area) && b->flags & BOX_HOVERED) {
        b->flags |= BOX_CLICKED;
        b->flags |= BOX_PRESSED;
		// b->flags |= BOX_FOCUSED;
    } else {
		b->flags &= ~BOX_FOCUSED;
        b->flags &= ~BOX_PRESSED;
    }
	if (previous != b->flags) {
		b->parent_window->state |= WIN_DIRTY;
		b->flags |= BOX_DIRTY;
	}
}
