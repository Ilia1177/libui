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

ui_layer_t* ui_layer_selected(ui_layer_t* layers, SDL_Point* p) {
	ui_layer_t *selected = NULL;
	ui_layer_t *layer = layers;
	while(layer) {
		if(SDL_PointInRect(p, &layer->area)) {
			selected = layer;
		}
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
	ui_layer_t *new = calloc(sizeof(ui_layer_t), 1);
	new->parent_box = box;// 42 FORBIDDEN
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

void	ui_layer_add(ui_layer_t**list, ui_layer_t* layer) {
		
	if(!layer || !*list) {
		*list = layer;
		return;
	}
	ui_layer_t *curr = *list;

	while (curr->next) {
		curr = curr->next;
	}
	curr->next = layer;
}

