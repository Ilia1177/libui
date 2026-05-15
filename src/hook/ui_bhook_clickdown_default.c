#include "ui_win.h" // For ui_box_t, ui_win_t, SDL_Event
#include "ui_box.h" // For ui_bhook_fire, BOX_FOCUSED, BOX_CLICKED, BOX_PRESSED, ui_box_flags

void	ui_bhook_clickdown_default(ui_box_t *b, SDL_Event* e, void* data) {
	(void)data;
	SDL_MouseButtonEvent *btn = &e->button;
	ui_win_t *win = b->parent_window;
    int px = (int)(btn->x * win->scale.x);
    int py = (int)(btn->y * win->scale.y);
    SDL_Point p = {px, py};
	bool focused = b->flags & BOX_FOCUSED;

    if (SDL_PointInRect(&p, &b->area) && (!b->list) && b->flags & BOX_HOVERED) {
        b->flags |= BOX_CLICKED;
        b->flags |= BOX_PRESSED;
		ui_box_flags(win->boxes, BOX_FOCUSED, false);
		ui_box_flags(win->canvas, BOX_FOCUSED, false);
		if (!focused) {
			b->flags |= BOX_FOCUSED;
			printf("box focused\n");
		}
    } else {
        b->flags &= ~BOX_PRESSED;  // release even if mouse moved off
    }
	ui_box_t *curr = b->list;
	while(curr) {
		if(curr->on_click_down)
            ui_bhook_fire(curr->on_click_down, curr, e, data);
		curr = curr->next;
	}
}