#include "libui.h"

void	ui_win_render_default(ui_win_t* win, SDL_Event* e, void* data) {
	        // Render
	(void)data;
	(void)e;
	if (!win || !win->renderer || !(win->flags & WIN_DIRTY))
		return;

		printf("render window %d\n", win->id);
		fflush(stdout);
		ui_rgba_t color = win->background_color;
        SDL_SetRenderDrawColor(win->renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(win->renderer);
		if (win->texture) {
			SDL_Rect dest = { 0, 0, win->area.w, win->area.h };
			SDL_RenderCopy(win->renderer, win->texture, NULL, &dest);
			printf("render texture\n");
		}
		fflush(stdout);
		ui_box_t *current = win->menu;
		while(current) {
			if (current->render) {
				printf("rendering box (menu)\n");
				ui_box_event_fire(current->render, current, e, data);
			}
			current = current->next;
		}
        SDL_RenderPresent(win->renderer);
		win->flags &= ~WIN_DIRTY;
		printf("window: finish render\n");
		fflush(stdout);
}

