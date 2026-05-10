#include "ui_box.h"

void ui_bhook_drawbox(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	int			size;
	ui_win_t	*win;
	SDL_Rect	border;

	size = box->border;
	win = box->parent_window;
    if (size > 0) {
        border.x = box->area.x - size;
		border.y = box->area.y - size;
		border.w = box->area.w + 2 * size;
		border.h = box->area.h + 2 * size;
        SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(win->renderer, &border);
    }
    SDL_SetRenderDrawColor(win->renderer, 
			box->color.r, 
			box->color.g, 
			box->color.b, 
			box->color.a);
    SDL_RenderFillRect(win->renderer, &box->area);
}

void ui_bhook_drawpressed(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

	win = box->parent_window;
    if (box->flags & BOX_PRESSED) {
        SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 80);
        SDL_RenderFillRect(win->renderer, &box->area);
    }
}

void ui_bhook_drawhovered(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

	win = box->parent_window;
    SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);
    if (box->flags & BOX_HOVERED) {
        SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 50);
        SDL_RenderFillRect(win->renderer, &box->area);
    }
}

void ui_bhook_drawlayers(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;
	// int w, h;
	// SDL_Rect dest;

	win = box->parent_window;
   	SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);
	// if (box->layers && box->layers->texture && box->label) {
	// 	SDL_QueryTexture(box->layers->texture, NULL, NULL, &w, &h);
	// 	dest.x = box->area.x + (BOX_MENU_W - box->layers->dimension.w) / 2;
	// 	dest.y = box->area.y + (BOX_MENU_H - box->layers->dimension.h) / 2;
	// 	dest.w = box->layers->dimension.w;
	// 	dest.h = box->layers->dimension.h;
	// 	SDL_RenderCopy(win->renderer, box->layers->texture, NULL, &dest);
	// } else 
	if (box->layers) {
		ui_layer_t *curr = box->layers;
		while (curr) {
			if (curr->texture)
				SDL_RenderCopy(win->renderer, curr->texture, NULL, &curr->dimension);
			curr = curr->next;
		}
	}
}

void ui_bhook_drawfocused(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

	win = box->parent_window;
	if (box->flags & BOX_FOCUSED) {
		int tw = 0;
		ui_layer_t *curr = box->layers;
		while (curr) {
			SDL_QueryTexture(curr->texture, NULL, NULL, &tw, NULL);
			int cx = box->area.x + (box->area.w / 2) + (tw / 2) + 2;
			int cy = box->area.y + 4;
			SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 255);
			SDL_RenderDrawLine(win->renderer, cx, cy, cx, cy + box->area.h - 8);
			curr = curr->next;
		}
	}
}

void ui_bhook_render_default(ui_box_t *box, SDL_Event*e, void* data)
{
    if (!box || (box->flags & BOX_HIDDEN))
        return;

    SDL_Renderer *renderer;

	renderer = box->parent_window->renderer;
    if (!renderer) 
		return;
    ui_bhook_drawbox(box, NULL, NULL);
	ui_bhook_drawhovered(box, NULL, NULL);
	ui_bhook_drawpressed(box, NULL, NULL);
	ui_bhook_drawfocused(box, NULL, NULL);
	ui_bhook_drawlayers(box, NULL, NULL);
    ui_box_t *current = box->list;
    while (current) {
        if (current->render)
            ui_bhook_fire(current->render, current, e, data);
        current = current->next;
    }
}
