#include "ui_win.h" // For ui_box_t, ui_win_t, SDL_Event
#include "ui_box.h" // For ui_bhook_fire, BOX_HIDDEN, BOX_HOVERED

static ui_box_t* hovered_box(ui_box_t* boxes, SDL_Point *p)
{
	if ( !boxes || (boxes->flags & BOX_HIDDEN))
		return NULL;
	ui_box_t* selected = NULL;
	ui_box_t* curr = boxes;
	while(curr) {
		if(SDL_PointInRect(p, &curr->area)) {
			selected = curr;
		}
		ui_box_t *child = hovered_box(curr->list, p);
		if (child) {
			selected = child;
		}
		curr = curr->next;
	}
	return selected;
}

void ui_bhook_mousemotion_default(ui_box_t *box, SDL_Event* e, void* data)
{
	SDL_MouseMotionEvent *btn = &e->motion;
	ui_win_t *win = box->parent_window;
    int px = (int)(btn->x * win->scale.x);
    int py = (int)(btn->y * win->scale.y);
    SDL_Point p = {px, py};

	ui_box_t *top_hovered_menubox = hovered_box(win->boxes, &p);
	ui_box_t *cnv_hovered = hovered_box(win->canvas, &p);
	if(top_hovered_menubox == box) {
		box->flags |= BOX_HOVERED;
	} else if (cnv_hovered == box && !top_hovered_menubox) {
		box->flags |= BOX_HOVERED;
	} else {
		box->flags &= ~BOX_HOVERED;
        box->flags &= ~BOX_PRESSED;
	}
	ui_box_t *curr = box->list;
	while(curr) {
		if (curr->on_mouse_motion) 
			ui_bhook_fire(curr->on_mouse_motion, curr, e, data);
		curr = curr->next;
	}
}