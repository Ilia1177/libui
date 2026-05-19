#include "ui_box.h"

void ui_bhook_drawbox(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	SDL_Renderer	*render;

    if (!box || (box->flags & BOX_HIDDEN))
        return;
	render = box->parent_window->renderer;
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(render, 
			box->color.r, 
			box->color.g, 
			box->color.b, 
			box->color.a);
    SDL_RenderFillRect(render, &box->area);
}

void ui_bhook_drawpressed(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

    if (!box || (box->flags & BOX_HIDDEN) || !(box->flags & BOX_PRESSED))
        return;
	win = box->parent_window;
	SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 80);
	SDL_RenderFillRect(win->renderer, &box->area);
}

void ui_bhook_drawhovered(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

	if (!box || (box->flags & BOX_HIDDEN) || !(box->flags & BOX_HOVERED))
        return;
	win = box->parent_window;
    SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 50);
	SDL_RenderFillRect(win->renderer, &box->area);
}

void ui_bhook_drawcliplayers(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

    if (!box || (box->flags & BOX_HIDDEN))
        return;
	win = box->parent_window;
   	SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetClipRect(win->renderer, &box->area);
	if (box->layers) {
		ui_layer_t *curr = box->layers;
		while (curr) {
			if (curr->texture)
				SDL_RenderCopy(win->renderer, curr->texture, NULL, &curr->area);
			curr = curr->next;
		}
	}
	SDL_RenderSetClipRect(win->renderer, NULL);
}

void ui_bhook_drawlayers_save(ui_box_t *box, SDL_Event *e, void *data) {
    (void)e;
    (void)data;
    if (!box || (box->flags & BOX_HIDDEN)) return;

    ui_win_t *win  = box->parent_window;
    float     zoom = box->zoom_amt;
    int       cx   = box->zoom_origin.x + box->area.x;
    int       cy   = box->zoom_origin.y + box->area.y;

    SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);

    ui_layer_t *curr = box->layers;
    while (curr) {
        if (!curr->texture) { curr = curr->next; continue; }
        int offset_x = curr->area.x - cx;
        int offset_y = curr->area.y - cy;
        SDL_Rect dest = {
            cx + (int)(offset_x * zoom),
            cy + (int)(offset_y * zoom),
            (int)(curr->area.w * zoom),
            (int)(curr->area.h * zoom)
        };
        SDL_RenderCopy(win->renderer, curr->texture, NULL, &dest);
        curr = curr->next;
    }
}
void ui_bhook_drawlayers(ui_box_t *box, SDL_Event *e, void *data) {
    (void)e;
    (void)data;
    if (!box || (box->flags & BOX_HIDDEN)) return;

    ui_win_t *win  = box->parent_window;

    SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);

    ui_layer_t *curr = box->layers;
    while (curr) {
        if (!curr->texture) { curr = curr->next; continue; }
		SDL_Rect dest = ui_layer_zoomed_area(curr);
        SDL_RenderCopy(win->renderer, curr->texture, NULL, &dest);
        curr = curr->next;
    }
}
void ui_bhook_drawfocused(ui_box_t *box, SDL_Event *e, void *data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

	if (!box || (box->flags & BOX_HIDDEN) || !(box->flags & BOX_FOCUSED))
		return;
	win = box->parent_window;
	SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(win->renderer, 12, 23, 23, 50);
	SDL_RenderFillRect(win->renderer, &box->area);
}

void ui_bhook_drawtextfocused(ui_box_t *box, SDL_Event *e, void *data) {
    (void)e;
    (void)data;
    if (!box || (box->flags & BOX_HIDDEN) || !(box->flags & BOX_FOCUSED))
        return;
    ui_win_t *win = box->parent_window;
    static Uint32 last_blink = 0;
    static bool   visible    = true;
    Uint32 now = SDL_GetTicks();
    if (now - last_blink > 500) {
        visible    = !visible;
        last_blink = now;
    }
    if (!visible) 
		return;

    int tw = 0;
    if (box->layers && box->layers->texture)
        SDL_QueryTexture(box->layers->texture, NULL, NULL, &tw, NULL);

    int padding = 4;
    int cx      = box->area.x + padding + tw;          // right after text
    int max_cx  = box->area.x + box->area.w - padding; // right edge limit
    cx = (cx < max_cx) ? cx : max_cx;                  // clamp to box

    int cy = box->area.y + padding;
    int ch = box->area.h - padding * 2;

    SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(win->renderer, cx, cy, cx, cy + ch);
}

void ui_bhook_render_default(ui_box_t *box, SDL_Event*e, void* data)
{
    SDL_Renderer *renderer;

    if (!box || (box->flags & BOX_HIDDEN))
        return;
	renderer = box->parent_window->renderer;
    if (!renderer) 
		return;
    ui_box_t *current = box->childs;
    while (current) {
        if (current->render)
            ui_bhook_fire(current->render, current, e, data);
        current = current->next;
    }
}
