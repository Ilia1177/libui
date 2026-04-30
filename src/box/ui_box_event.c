#include "libui.h"

void	ui_box_on_click_up_handler(ui_box_t *b, SDL_MouseButtonEvent* e) {

	(void)e;
	// update 
	if (ui_is_mouse_in(b)) {
		b->flags &= ~BOX_PRESSED;
	}
	// ... (nothing) ...
	// update children
	ui_box_t *curr = b->child_boxes;
	while(curr) {
		if(curr->on_click_up)
			curr->on_click_up(curr, e);
		curr = curr->next;
	}
}

void ui_box_update_default(ui_box_t* box) {
	//update current ....
	// if(box->on_hover_enter) {
	// 	box->on_mouse_motion(box);
	// }
	//update childs
	printf("box: update\n");
	fflush(stdout);
	ui_box_t *current = box->child_boxes;
	while(current) {
		if (current->update)
			current->update(current);
		current = current->next;
	}
}
void ui_box_on_mouse_motion_handler(ui_box_t *box, SDL_MouseMotionEvent* e) {
	(void)e;

	printf("box: on mouse motion\n");
	fflush(stdout);
	if (ui_is_mouse_in(box)) {
		box->flags |= BOX_HOVERED;
		printf("box: hover!\n");
		fflush(stdout);
	} else {
		box->flags &= ~BOX_HOVERED;
	}
		// calculate if is hover
    // convert global mouse to window-relative
	ui_box_t *curr = box->child_boxes;
	while(curr) {
		if (curr->on_mouse_motion) 
			curr->on_mouse_motion(curr, e);
		curr = curr->next;
	}
}

void	ui_box_on_click_down_handler(ui_box_t *b, SDL_MouseButtonEvent* e) {
	(void)e;
	// update 
	if (ui_is_mouse_in(b)) {
		b->flags &= ~BOX_PRESSED;
	}
	// update children
	ui_box_t *curr = b->child_boxes;
	while(curr) {
		if(curr->on_click_down)
			curr->on_click_down(curr, e);
		curr = curr->next;
	}
}

void ui_box_render_default(ui_box_t* box) {

    if (!box || (box->flags & BOX_HIDDEN) || !box->parent_window) {
        return;
    }
	printf("render box: w:%d\n", box->area.w);
	fflush(stdout);
	SDL_Renderer* renderer = box->parent_window->renderer;
	if (!renderer) 
		return;

	// draw border
	int m = box->border;
	SDL_Rect area = box->area;
	SDL_Rect border = {area.x - m, area.y - m, area.w + 2 * m, area.h + 2 * m};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black border
    SDL_RenderFillRect(renderer, &border);

	// draw inside of box
    SDL_Rect new_box = {area.x, area.y, area.w, area.h};
    SDL_SetRenderDrawColor(renderer, box->color.r, box->color.g, box->color.b, box->color.a);
    SDL_RenderFillRect(renderer, &new_box);

    // Render on hover/press for visual feedback
    if (box->flags & BOX_HOVERED) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &new_box);
    }
    if (box->flags & BOX_PRESSED) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &new_box);
    }

	ui_box_t *current = box->child_boxes;
	while(current) {
		if (current->render)
			current->render(current);
		current = current->next;
	}
}

