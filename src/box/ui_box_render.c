#include "ui_box.h"

// void ui_box_render_default(ui_box_t* box) {
//
//     if (!box || (box->flags & BOX_HIDDEN) || !box->parent_window) {
//         return;
//     }
// 	printf("render box: w:%d\n", box->area.w);
// 	fflush(stdout);
// 	SDL_Renderer* renderer = box->parent_window->renderer;
// 	if (!renderer) 
// 		return;
//
// 	// draw border
// 	int m = box->border;
// 	SDL_Rect area = box->area;
// 	SDL_Rect border = {area.x - m, area.y - m, area.w + 2 * m, area.h + 2 * m};
//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black border
//     SDL_RenderFillRect(renderer, &border);
//
// 	// draw inside of box
//     SDL_Rect new_box = {area.x, area.y, area.w, area.h};
//     SDL_SetRenderDrawColor(renderer, box->color.r, box->color.g, box->color.b, box->color.a);
//     SDL_RenderFillRect(renderer, &new_box);
//
//     // Render on hover/press for visual feedback
//     if (box->flags & BOX_HOVERED) {
//         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//         SDL_RenderDrawRect(renderer, &new_box);
//     }
//     if (box->flags & BOX_PRESSED) {
//         SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//         SDL_RenderDrawRect(renderer, &new_box);
//     }
//
// 	ui_box_t *current = box->child_boxes;
// 	while(current) {
// 		if (current->render)
// 			current->render(current);
// 		current = current->next;
// 	}
// }
void ui_box_render_default(ui_box_t *box) {
    if (!box || (box->flags & BOX_HIDDEN) || !box->parent_window)
        return;
    SDL_Renderer *renderer = box->parent_window->renderer;
    if (!renderer) return;

    // border
    int m = box->border;
    SDL_Rect area = box->area;
    if (m > 0) {
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

    // children
    ui_box_t *current = box->child_boxes;
    while (current) {
        if (current->render)
            current->render(current);
        current = current->next;
    }
}
