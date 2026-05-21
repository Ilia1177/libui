#include "libui.h"

SDL_Texture *create_white_texture(SDL_Renderer *renderer, int w, int h)
{
    SDL_Texture *tex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w,
        h
    );

    if (!tex)
    {
        SDL_Log("SDL_CreateTexture failed: %s", SDL_GetError());
        return NULL;
    }

    /* Save current render target */
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);

    /* Draw into the texture */
    SDL_SetRenderTarget(renderer, tex);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    /* Restore previous target */
    SDL_SetRenderTarget(renderer, old_target);

    return tex;
}

SDL_Rect ui_layer_zoomed_area(ui_layer_t *layer) {
    float zoom = layer->parent_box->zoom_amt;
    return (SDL_Rect){
        layer->area.x,
        layer->area.y,
        (int)(layer->area.w * zoom),
        (int)(layer->area.h * zoom)
    };
}
// ui_layer_selected
ui_layer_t *ui_layer_selected(ui_layer_t *layers, SDL_Point *p) {
    ui_layer_t *selected = NULL;
    ui_layer_t *layer    = layers;
    while (layer) {
        SDL_Rect zoomed = ui_layer_zoomed_area(layer);
        if (SDL_PointInRect(p, &zoomed))
            selected = layer;
        layer = layer->next;
    }
    return selected;
}

int ui_layer_count(ui_layer_t* layers) {
	int n = 0;

	while(layers) {
		n++;
		layers = layers->next;
	}
	return n;
}

ui_layer_t*	ui_layer_make(ui_box_t *box, SDL_Texture *texture)
{
	if (!texture)
		return NULL;
	ui_layer_t *new = calloc(sizeof(ui_layer_t), 1);
	new->parent_box = box;
	new->texture = texture;
	new->area.x = 0;
	new->area.y = 0;
	SDL_QueryTexture(texture, NULL, NULL, &new->area.w, &new->area.h);
	new->area = ui_area_center(box->area, new->area);
	ui_layer_add(&box->layers, new);
	return new;
}

void	ui_layer_clean(ui_layer_t** list) {
	if(!list || !*list)
		return;
	ui_layer_t* curr = *list;
	while(curr) {
		ui_layer_t* next = curr->next;
		SDL_DestroyTexture(curr->texture);
		free(curr);
		curr = next;
	}
	*list = NULL;
}

void 	ui_box_apply_all(ui_box_t *list, ui_bhook_fn_t fn)
{
	ui_box_t *curr;
	ui_box_t *next;
	curr = list;
	while(curr) {
		next = curr->next;
		fn(curr, NULL, NULL);
		curr = next;
	}
}

void 	ui_box_center_layers(ui_box_t* box, SDL_Rect* offset)
{
	if(!box)
		return;
	ui_layer_t* curr = box->layers;
	while(curr) {
		curr->area = ui_area_center(box->area, curr->area);
		if(offset && offset->x)
			curr->area.x = box->area.x + offset->x;
		curr = curr->next;
	}
}

void	ui_layer_add(ui_layer_t**list, ui_layer_t* layer) {
		
	ui_layer_t *curr;

	if(!layer || !*list) {
		*list = layer;
		return;
	}
	curr = *list;
	while (curr->next) {
		curr = curr->next;
	}
	curr->next = layer;
}
SDL_Texture *ui_tex_path(SDL_Renderer *renderer, const char *path) {
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Texture *target = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        surface->w, surface->h);
    if (!target) {
        SDL_FreeSurface(surface);
        return NULL;
    }
    SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);

    // blit surface directly into target via a temp texture
    SDL_Texture *src = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Texture *old = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, target);
    SDL_RenderCopy(renderer, src, NULL, NULL);
    SDL_SetRenderTarget(renderer, old);
    SDL_DestroyTexture(src);

    return target;
}
// SDL_Texture *ui_texture_path(SDL_Renderer *renderer, const char *path) {
//     SDL_Surface *surface = IMG_Load(path);
//     if (!surface) {
//         fprintf(stderr, "IMG_Load error: %s\n", IMG_GetError());
//         return NULL;
//     }
//
//     // create static texture from surface
//     SDL_Texture *src = SDL_CreateTextureFromSurface(renderer, surface);
//     SDL_FreeSurface(surface);
//     if (!src) {
//         fprintf(stderr, "CreateTextureFromSurface error: %s\n", SDL_GetError());
//         return NULL;
//     }
//
//     // convert to target texture so we can draw into it
//     int w, h;
//     SDL_QueryTexture(src, NULL, NULL, &w, &h);
//     SDL_Texture *target = SDL_CreateTexture(renderer,
//         SDL_PIXELFORMAT_RGBA8888,
//         SDL_TEXTUREACCESS_TARGET,
//         w, h);
//     if (!target) {
//         SDL_DestroyTexture(src);
//         return NULL;
//     }
//     SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
//
//     // copy src into target
//     SDL_Texture *old = SDL_GetRenderTarget(renderer);
//     SDL_SetRenderTarget(renderer, target);
//     SDL_RenderCopy(renderer, src, NULL, NULL);
//     SDL_SetRenderTarget(renderer, old);
//
//     SDL_DestroyTexture(src);
//
//     return target;
// }

void ui_texture_draw2(ui_layer_t *layer, SDL_Point p) {
    if (!layer || !layer->texture) return;

    SDL_Renderer *renderer = layer->parent_box->parent_window->renderer;
    SDL_Texture  *old_target = SDL_GetRenderTarget(renderer);  // save
    // redirect rendering into the layer texture
    SDL_SetRenderTarget(renderer, layer->texture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // draw at position relative to layer
    int rx = p.x - layer->area.x;
    int ry = p.y - layer->area.y;
	printf("layer draw at: x: %d, y: %d\n", rx, ry);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect dot = {rx - 5, ry - 5, 10, 10};
    SDL_RenderFillRect(renderer, &dot);
    // restore screen as render target
    SDL_SetRenderTarget(renderer, old_target);
	layer->parent_box->parent_window->state |= WIN_DIRTY;
}

void ui_layer_draw_at(ui_layer_t *layer, SDL_Point p) {
    if (!layer || !layer->texture) return;

    SDL_Renderer *renderer = layer->parent_box->parent_window->renderer;

    // check texture access
    // Uint32 format;
    // int access, tw, th;
    // SDL_QueryTexture(layer->texture, &format, &access, &tw, &th);

    // p is box-relative, layer->area is window-relative
    // so rx/ry should be in texture space
    // int rx = p.x - (layer->area.x - layer->parent_box->area.x);
    // int ry = p.y - (layer->area.y - layer->parent_box->area.y);
    // printf("drawing at texture coords: %d, %d\n", rx, ry);

	int size = 5;
    SDL_Texture *old = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, layer->texture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // red — easy to see
    SDL_Rect dot = {p.x - (size / 2), p.y - (size / 2), size, size};         // big dot — easy to see
    SDL_RenderFillRect(renderer, &dot);
    SDL_SetRenderTarget(renderer, old);

    layer->parent_box->parent_window->state |= WIN_DIRTY;
}
