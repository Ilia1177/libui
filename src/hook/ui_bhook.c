#include "libui.h"

void ui_bhook_nopressed(ui_box_t *b, SDL_Event *e, void* data) {
	(void)e;
	(void)data;
	b->flags &= ~BOX_PRESSED;
}

void ui_bhook_nohovered(ui_box_t *b, SDL_Event *e, void* data) {
	(void)e;
	(void)data;
	b->flags &= ~BOX_HOVERED;
}

void ui_bhook_winclose(ui_box_t* b, SDL_Event* e, void* data) {
	(void)e;
	(void)data;
	ui_win_t *win = b->parent_window;
	if (b->flags & BOX_PRESSED) {
		printf("WIN_QUIT window %d\n", win->id);
		fflush(stdout);
		win->flags |= WIN_QUIT;
	}
}

void ui_bhook_label2texture(ui_box_t *cnv, SDL_Event *e, void* data)
{
	(void)e;
	(void)data;
	//update current ....
	SDL_Texture* texture = NULL;
	if (cnv->label && !cnv->layers) {
		 texture = ui_tool_text2texture(cnv->parent_window, cnv->label, (SDL_Color){255,255,255,255});
		 if (!texture)
			printf("FAIL creating texture\n");
		ui_layer_add(&cnv->layers, ui_layer_create(cnv, texture));
		// int w, h;
		// SDL_QueryTexture(cnv->layers->texture, NULL, NULL, &w, &h);
		// SDL_Rect dest = {
		// 	cnv->area.x + (BOX_MENU_W - w) / 2,
		// 	cnv->area.y + (BOX_MENU_H - h) / 2,
		// 	w, h
		// };
		// SDL_RenderCopy(cnv->parent_window->renderer, cnv->layers->texture, NULL ,&dest);
		// cnv->layers->dimension = dest;
	}
}

void ui_bhook_unhidechild(ui_box_t *box, SDL_Event* e, void* data) {

	(void)data;
	(void)e;
	fflush(stdout);
	if (box->flags & BOX_HOVERED) {
		int boxnb = ui_box_count(box->list);
		box->area.h = (1 + boxnb) * BOX_MENU_H;
		printf("unhidden child !!\n");
		fflush(stdout);
		ui_box_flags(box->list, BOX_HIDDEN, false);
	} else {
		ui_box_flags(box->list, BOX_HIDDEN, true);
		box->area.h = BOX_MENU_H;
	}
}

void ui_bhook_movelayer(ui_box_t *cnv, SDL_Event *e, void *data) {
    (void)e;
    (void)data;
    static SDL_Point last = {0, 0};

    ui_layer_t *selected = cnv->layers;
    if (!selected) return;

    SDL_Point p = ui_box_mousepos(cnv);

    if (cnv->flags & BOX_PRESSED) {
        if (last.x == 0 && last.y == 0) {
            // first press — initialize last position
            last = p;
            return;
        }
        // compute delta
        int dx = p.x - last.x;
        int dy = p.y - last.y;

        // move the layer
        selected->dimension.x += dx;
        selected->dimension.y += dy;

        printf("MOVE LAYER dx=%d dy=%d -> x=%d y=%d\n",
            dx, dy, selected->dimension.x, selected->dimension.y);
        fflush(stdout);

        last = p;  // update for next frame
    } else {
        // reset when not pressed
        last = (SDL_Point){0, 0};
    }
}

void	ui_bhook_maxsize(ui_box_t *cnv, SDL_Event *e, void *data) {
	(void)e;
	(void)data;
	ui_win_t *win = cnv->parent_window;
	if(win->menu) {
		cnv->area.y = BOX_MENU_H + (2 * MENU_GAP_Y) + win->menu->border;
		cnv->area.h = win->area.h - (BOX_MENU_H + (2 * MENU_GAP_Y) - win->menu->border);
	}
	cnv->area.h = win->area.h;
	cnv->area.w = win->area.w;
}
