#include "ui_box.h"

void ui_bhook_drawborder(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	SDL_Renderer	*render;
	SDL_Rect		strip;
	const frame_t	b = box->border;

    if (!box || (box->state & BOX_HIDDEN))
        return;
	
	render = box->win->renderer;
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);

	if (b.top) {
		strip = (SDL_Rect){box->area.x, box->area.y, box->area.w, b.top};
		SDL_RenderFillRect(render, &strip);
	}
	if (b.bottom) {
		strip = (SDL_Rect){box->area.x, box->area.y + box->area.h - b.bottom, box->area.w, b.bottom};
		SDL_RenderFillRect(render, &strip);
	}
	if (b.left) {
		strip = (SDL_Rect){box->area.x, box->area.y, b.left, box->area.h};
		SDL_RenderFillRect(render, &strip);
	}
	if (b.right) {
		strip = (SDL_Rect){box->area.x + box->area.w - b.right, box->area.y, b.right, box->area.h};
		SDL_RenderFillRect(render, &strip);
	}
}

void ui_bhook_drawbox(ui_box_t* box, SDL_Event* e, void* data)
{
    (void)e;
    (void)data;
    if (!box || (box->state & BOX_HIDDEN))
        return;
	// if (!(box->state & BOX_DIRTY))
	// 	return;

    SDL_Renderer* render = box->win->renderer;

    // Drop shadow
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

    // Main box fill (existing)
    SDL_SetRenderDrawColor(render, box->color.r, box->color.g, box->color.b, box->color.a);
    SDL_RenderFillRect(render, &box->area);
}

void ui_bhook_drawpressed(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

	// if (!(box->state & BOX_DIRTY))
	// 	return;
    if (!box || (box->state & BOX_HIDDEN) || !(box->state & BOX_PRESSED))
        return;
	if (box->state & BOX_DISABLE)
		return;
		//   if (!(box->layout & UI_LAYOUT_DIRTY))
		// return;
	// if (!(box->state & BOX_DIRTY))
	// 	return;
	win = box->win;
	SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 80);
	SDL_RenderFillRect(win->renderer, &box->area);
}

void ui_bhook_drawhovered(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

	// if (!(box->state & BOX_DIRTY))
	// 	return;
	if (!box || (box->state & BOX_HIDDEN) || !(box->state & BOX_HOVERED))
        return;
	if (box->state & BOX_DISABLE)
		return;
	// if (!(box->layout & UI_LAYOUT_DIRTY))
	// 	return;
	// if (!(box->state & BOX_DIRTY))
	// 	return;
	win = box->win;
    SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 50);
	SDL_RenderFillRect(win->renderer, &box->area);
}

void ui_bhook_drawcliplayers(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

	// if (!(box->state & BOX_DIRTY))
	// 	return;
    if (!box || (box->state & BOX_HIDDEN))
        return;
		//   if (!(box->layout & UI_LAYOUT_DIRTY))
		// return;
	// if (!(box->state & BOX_DIRTY))
	// 	return;
	win = box->win;

   	SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetClipRect(win->renderer, &box->area);
	if (box->layers) {
		ui_layer_t *curr = box->layers;
		while (curr) {
			if (curr->texture) {
				SDL_Rect dest = curr->area;
				dest.x += box->area.x;
				dest.y += box->area.y;
				SDL_SetTextureBlendMode(curr->texture, curr->blend_mode);
				SDL_RenderCopyEx(win->renderer, curr->texture, NULL, &dest, curr->angle, NULL, SDL_FLIP_NONE);
			}
			curr = curr->next;
		}
	}
	SDL_RenderSetClipRect(win->renderer, NULL);
}


void ui_bhook_drawlayers(ui_box_t *box, SDL_Event *e, void *data) {
    (void)e;
    (void)data;

    if (!box || (box->state & BOX_HIDDEN))
		return;

	// if (!(box->state & BOX_DIRTY))
	// 	return;
    ui_win_t *win  = box->win;

	// SDL_SetRenderDrawBlendMode(win->renderer, curr->blend_mode);
	if(box->layout & UI_LAYOUT_CLIP)
		SDL_RenderSetClipRect(win->renderer, &box->area);
    ui_layer_t *curr = box->layers;
    while (curr) {
        if (!curr->texture) { curr = curr->next; continue; }
		SDL_Rect dest = ui_layer_zoomed_area(curr);
		SDL_SetTextureBlendMode(curr->texture, curr->blend_mode);
		SDL_RenderCopyEx(win->renderer, curr->texture, NULL, &dest, curr->angle, NULL, SDL_FLIP_NONE);
        curr = curr->next;
    }
	if(box->layout & UI_LAYOUT_CLIP)
		SDL_RenderSetClipRect(win->renderer, NULL);
}

void ui_bhook_drawfocused(ui_box_t *box, SDL_Event *e, void *data)
{
	(void)e;
	(void)data;
	ui_win_t *win;

	if (!box || (box->state & BOX_HIDDEN) || !(box->state & BOX_FOCUSED))
		return;
	if (box->state & BOX_DISABLE)
		return;
	// if (!(box->state & BOX_DIRTY))
	// 	return;
		//   if (!(box->layout & UI_LAYOUT_DIRTY))
		// return;
	win = box->win;
	SDL_SetRenderDrawBlendMode(win->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(win->renderer, 12, 23, 23, 50);
	SDL_RenderFillRect(win->renderer, &box->area);
}

void ui_bhook_drawtextfocused(ui_box_t *box, SDL_Event *e, void *data) {
    (void)e;
    (void)data;
    if (!box || (box->state & BOX_HIDDEN) || !(box->state & BOX_FOCUSED))
        return;
	// if (!(box->state & BOX_DIRTY))
	// 	return;
		//   if (!(box->layout & UI_LAYOUT_DIRTY))
		// return;
	// if (!(box->state & BOX_DIRTY))
	// 	return;
    ui_win_t *win = box->win;
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
    int cx      = box->area.x + padding + tw;
    int max_cx  = box->area.x + box->area.w - padding;
    cx = (cx < max_cx) ? cx : max_cx;

    int cy = box->area.y + padding;
    int ch = box->area.h - padding * 2;

    SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(win->renderer, cx, cy, cx, cy + ch);
}

void ui_bhook_render_default(ui_box_t *box, SDL_Event*e, void* data)
{
	(void)e;
	(void)data;
	
	// box->state &= ~BOX_CLICKED;
    if (!box || (box->state & BOX_HIDDEN))
        return;
		//   if (!(box->layout & UI_LAYOUT_DIRTY))
		// return;
}
