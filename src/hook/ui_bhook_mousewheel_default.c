#include "ui_box.h"

void ui_bhook_mousewheel_default(ui_box_t *box, SDL_Event *e, void *data) {

    SDL_Renderer *renderer;

	printf("box hook: mouse wheel\n");
    if (!box || (box->flags & BOX_HIDDEN))
        return;
	renderer = box->parent_window->renderer;
    if (!renderer) 
		return;
    ui_box_t *current = box->childs;
    while (current) {
        if (current->render)
            ui_bhook_fire(current->on_mouse_wheel, current, e, data);
        current = current->next;
    }
}
