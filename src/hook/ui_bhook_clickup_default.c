#include "ui_win.h"
#include "ui_box.h"

void ui_bhook_clickup_default(ui_box_t *b, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
    b->flags &= ~BOX_PRESSED;
}
