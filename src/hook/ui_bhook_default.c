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

void	ui_bhook_clickdown_default(ui_box_t *b, SDL_Event* e, void* data) {
	(void)data;
	SDL_MouseButtonEvent *btn = &e->button;
	ui_win_t *win = b->parent_window;
    int px = (int)(btn->x * win->scale.x);
    int py = (int)(btn->y * win->scale.y);
    SDL_Point p = {px, py};
    if (SDL_PointInRect(&p, &b->area) && (!b->list)) {
        b->flags |= BOX_PRESSED;
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

void ui_bhook_mousemotion_default(ui_box_t *box, SDL_Event* e, void* data)
{
	printf("mouse motion on box\n");
		fflush(stdout);
	SDL_MouseMotionEvent *btn = &e->motion;
	ui_win_t *win = box->parent_window;
    int px = (int)(btn->x * win->scale.x);
    int py = (int)(btn->y * win->scale.y);
    SDL_Point p = {px, py};
    if (SDL_PointInRect(&p, &box->area)) {
		box->flags |= BOX_HOVERED;
		printf("Hovered box\n");
		fflush(stdout);
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
	while(current) {
		if (current->update) {
			ui_bhook_fire(current->update, current, e, data);
		}
		current = current->next;
	}
}

void ui_bhook_render_default(ui_box_t *box, SDL_Event*e, void* data) {
    if (!box || (box->flags & BOX_HIDDEN) || !box->parent_window)
        return;
    SDL_Renderer *renderer = box->parent_window->renderer;
    if (!renderer) return;

	fflush(stdout);
    int m = box->border;
    SDL_Rect area = box->area;
    if (m > 0) {
		// draw border
        SDL_Rect border = {area.x - m, area.y - m, area.w + 2 * m, area.h + 2 * m};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &border);
    }

    // fill
    SDL_SetRenderDrawColor(renderer, box->color.r, box->color.g, box->color.b, box->color.a);
    SDL_RenderFillRect(renderer, &area);

    // hover / press overlay
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (box->flags & BOX_HOVERED) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
        SDL_RenderFillRect(renderer, &area);
    }
    if (box->flags & BOX_PRESSED) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
        SDL_RenderFillRect(renderer, &area);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	if (box->layers && box->layers->texture && box->label) {

		int w, h;
		SDL_QueryTexture(box->layers->texture, NULL, NULL, &w, &h);

		SDL_Rect dest = {
			box->area.x + (BOX_MENU_W - box->layers->dimension.w) / 2,
			box->area.y + (BOX_MENU_H - box->layers->dimension.h) / 2,
			box->layers->dimension.w,
			box->layers->dimension.h
		};
		SDL_RenderCopy(renderer, box->layers->texture, NULL, &dest);
	} else if (box->layers) {
		ui_layer_t *curr = box->layers;
		while (curr) {
			if (curr->texture)
				SDL_RenderCopy(renderer, curr->texture, NULL, &curr->dimension);
			curr = curr->next;
		}
	}
    // children
    ui_box_t *current = box->list;
    while (current) {
        if (current->render)
            ui_bhook_fire(current->render, current, e, data);
        current = current->next;
    }
}
