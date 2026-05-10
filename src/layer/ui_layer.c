#include "libui.h"

int ui_layer_count(ui_layer_t* layers) {
	int n = 0;

	while(layers) {
		n++;
		layers = layers->next;
	}
	return n;
}

ui_layer_t*	ui_layer_create(ui_box_t *box, SDL_Texture *texture)
{
	ui_layer_t *new = calloc(sizeof(ui_layer_t), 1);
	new->parent_box = box;// 42 FORBIDDEN
	new->texture = texture;
	new->dimension.x = 0;
	new->dimension.y = 0;
	SDL_QueryTexture(texture, NULL, NULL, &new->dimension.w, &new->dimension.h);
	return new;
}

void	ui_layer_destroy(ui_layer_t** list) {
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

