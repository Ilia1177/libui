#include "ui_win.h"



void ui_whook_clickdown_default(ui_win_t *win, SDL_Event *e, void* data)
{
	if (!win || win->flags & WIN_QUIT)
		return;
	ui_box_t *curr = win->menu;
	while(curr) {
		if(curr->on_click_down)
			ui_bhook_fire(curr->on_click_down, curr, e, data);
		curr=curr->next;
	}
	ui_box_t *canvas = win->canvas;
	if(canvas && canvas->on_click_down){
		printf("update canvas\n");
		fflush(stdout);
		ui_bhook_fire(canvas->on_click_down, canvas, e, data);
	}
}

void ui_whook_clickup_default(ui_win_t *win, SDL_Event* e, void *data)
{
	if (!win || win->flags & WIN_QUIT)
		return;

	ui_box_t *curr = win->menu;
	while(curr) {
		if (curr->on_click_up) {
			ui_bhook_fire(curr->on_click_up, curr, e, data);
		}
		curr = curr->next;
	}
	ui_box_t *canvas = win->canvas;
	if(canvas && canvas->on_click_up){
		printf("update canvas\n");
		fflush(stdout);
		ui_bhook_fire(canvas->on_click_up, canvas, e, data);
	}

}

void ui_whook_mousemotion_default(ui_win_t *win, SDL_Event* e, void* data)
{
	if (win->flags & WIN_QUIT)
		return;

	(void)data;
	ui_box_t *curr = win->menu;
	while(curr) {
		if (curr->on_mouse_motion) {
			ui_bhook_fire(curr->on_mouse_motion, curr, e, data);
		}
		curr = curr->next;
	}
	ui_box_t *canvas = win->canvas;
	if(canvas && canvas->on_mouse_motion){
		ui_bhook_fire(canvas->on_mouse_motion, canvas, e, data);
	}
}

void ui_whook_update_default(ui_win_t* win, SDL_Event*e, void* data)
{
	if (win->flags & WIN_QUIT)
		return;

	ui_box_t *current = win->menu;
	while (current) {
		if (current->update) {
			ui_bhook_fire(current->update, current, e, data);
		}
		current = current->next;
	}
	ui_box_t *canvas = win->canvas;
	if(canvas && canvas->update){
		ui_bhook_fire(canvas->update, canvas, e, data);
	}
}

void	ui_whook_render_default(ui_win_t* win, SDL_Event* e, void* data) {
	        // Render
	if (!win || !win->renderer || win->flags & WIN_QUIT) //|| !(win->flags & WIN_DIRTY))
		return;

	// char *time = ui_get_time();
	SDL_Color color = win->background_color;
	SDL_SetRenderDrawColor(win->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(win->renderer);
	if (win->texture) {
		SDL_Rect dest = { 0, 0, win->area.w, win->area.h };
		SDL_RenderCopy(win->renderer, win->texture, NULL, &dest);
		printf("render texture\n");
	}
	if (win->canvas && win->canvas->render) {
		ui_bhook_fire(win->canvas->render, win->canvas, e, data);
	}
	ui_box_t *current = win->menu;
	while(current) {
		if (current->render) {
			ui_bhook_fire(current->render, current, e, data);
		}
		current = current->next;
	}
	win->flags &= ~WIN_DIRTY;

	SDL_RenderPresent(win->renderer);
}

