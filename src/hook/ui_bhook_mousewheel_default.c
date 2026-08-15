#include "ui_box.h"

void ui_bhook_mousewheel_default(ui_box_t *box, SDL_Event *e, void *data) {
	(void)e;
	(void)data;
    if (!box || (box->state & BOX_HIDDEN))
        return;
	if(box->state & BOX_HOVERED) {
		box->state |= BOX_DIRTY;
	}
}
