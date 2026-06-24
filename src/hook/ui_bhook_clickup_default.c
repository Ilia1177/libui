#include "ui_win.h"
#include "ui_box.h"

void ui_bhook_clickup_default(ui_box_t *b, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;

	if (b->flags & BOX_PRESSED) {
		// b->parent_window->state |= WIN_DIRTY;
		b->flags |= BOX_DIRTY;
	}
    b->flags &= ~BOX_PRESSED;
}
