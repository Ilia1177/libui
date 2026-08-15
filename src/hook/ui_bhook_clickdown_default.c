#include "ui_win.h"
#include "ui_box.h"

void ui_bhook_clickdown_default(ui_box_t *b, SDL_Event* e, void* data) {
	(void)data;
	(void)e;
	SDL_Point p;

	uint32_t previous = b->state;
	p = ui_win_mousepos(b->parent_window);
    if (SDL_PointInRect(&p, &b->area) && b->state & BOX_HOVERED) {
        b->state |= BOX_CLICKED;
        b->state |= BOX_PRESSED;
		// b->state |= BOX_FOCUSED;
    } else {
		b->state &= ~BOX_FOCUSED;
        b->state &= ~BOX_PRESSED;
    }
	if (previous != b->state) {
		b->state |= BOX_DIRTY;
	}
}
