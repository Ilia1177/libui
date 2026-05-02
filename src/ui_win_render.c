#include "ui_win.h"

void	ui_win_render_default(ui_win_t* win) {
	        // Render
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
		ui_box_t *current = win->menu;
		while(current) {
			int i = 0;
			fflush(stdout);
			if (current->render) {
				printf("render box %d\n", i);
				current->render(current);
			}
			current = current->next;
		}
        SDL_RenderPresent(win->renderer);
		win->flags &= ~WIN_DIRTY;
}

