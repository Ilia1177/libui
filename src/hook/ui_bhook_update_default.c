#include "ui_win.h"
#include "ui_box.h"

void ui_bhook_update_default(ui_box_t* box, SDL_Event *e, void *data)
{
	(void)e;
	(void)data;
	(void)box;
	// box->flags &= ~BOX_CLICKED;
}
