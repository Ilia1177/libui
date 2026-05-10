#include "ui_win.h"

void	ui_bhook_clickup_default(ui_box_t *b, SDL_Event* e, void* data)
{
    b->flags &= ~BOX_PRESSED;
	ui_box_t *curr = b->list;
	while(curr) {
		if(curr->on_click_up)
			ui_bhook_fire(curr->on_click_up, curr, e, data);
		curr = curr->next;
	}
}

void	ui_bhook_windowevent_default(ui_box_t *b, SDL_Event* e, void* data)
{

	ui_box_t *curr = b->list;
	while(curr) {
		if(curr->on_window_event)
            ui_bhook_fire(curr->on_window_event, curr, e, data);
		curr = curr->next;
	}

}
void	ui_bhook_clickdown_default(ui_box_t *b, SDL_Event* e, void* data) {
	(void)data;
	SDL_MouseButtonEvent *btn = &e->button;
	ui_win_t *win = b->parent_window;
    int px = (int)(btn->x * win->scale.x);
    int py = (int)(btn->y * win->scale.y);
    SDL_Point p = {px, py};
    if (SDL_PointInRect(&p, &b->area) && (!b->list) && b->flags & BOX_HOVERED) {
        b->flags |= BOX_CLICKED;
        b->flags |= BOX_PRESSED;
		printf("box clicked\n");
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

ui_box_t* hovered_box(ui_box_t* boxes, SDL_Point *p)
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

	ui_box_t *top_hovered_menubox = hovered_box(win->menu, &p);
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

void ui_bhook_update_default(ui_box_t* box, SDL_Event *e, void *data)
{
	ui_box_t *current = box->list;
	box->flags &= ~BOX_CLICKED;
	while(current) {
		if (current->update) {
			ui_bhook_fire(current->update, current, e, data);
		}
		current = current->next;
	}
}

