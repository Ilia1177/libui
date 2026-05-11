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

void ui_bhook_winclose(ui_box_t* b, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_win_t *win = b->parent_window;
	if (b->flags & BOX_PRESSED) {
		win->flags |= WIN_QUIT;
	}
}

void	ui_bhook_fullheight(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	box->area.h = box->parent_window->area.h;
}

void	ui_bhook_fullwidth(ui_box_t* box, SDL_Event* e, void* data) {
	(void)e;
	(void)data;
	box->area.w = box->parent_window->area.w;
}

void ui_bhook_revealchild(ui_box_t *box, SDL_Event* e, void* data) {

	(void)data;
	(void)e;
	if (box->flags & BOX_HOVERED) {
		ui_box_flags(box->list, BOX_HIDDEN, false);
	} else {
		ui_box_t *curr = box->list;
		while (curr) {
			if (curr->flags & BOX_HOVERED) 
				return;
			curr = curr->next;
		}
		ui_box_flags(box->list, BOX_HIDDEN, true);
	}
}

void ui_bhook_inputfocus(ui_box_t *box, SDL_Event *e, void *data) {
    (void)e;
    (void)data;
    if (box->flags & BOX_PRESSED) {
        SDL_StartTextInput();  
		box->flags |= BOX_FOCUSED;
		box->flags &= ~BOX_PRESSED;
    }
}

void ui_bhook_inputcancel(ui_box_t *box, SDL_Event *e, void *data) 
{
	(void)data;
	(void)e;
	ui_globalApp_t *app = box->parent_window->global;

	if ((e && e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE)
			|| box->flags & BOX_CLICKED) {
		app->loading = false;
		box->flags &= ~BOX_FOCUSED;
		box->parent_window->flags |= WIN_QUIT;
		box->parent_window->global->windows->flags |= WIN_DIRTY;
		if (box->input)
			free(box->input);
		box->input = NULL;
		SDL_StopTextInput();
	}
}

void ui_bhook_inputvalid(ui_box_t *box, SDL_Event *e, void *data)
{
	(void)data;
	(void)e;
	ui_globalApp_t *app = box->parent_window->global;  

	if ((e && e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_RETURN)
			|| box->flags & BOX_CLICKED) {
		app->input = box->input;
		box->input = NULL;
		box->flags &= ~BOX_FOCUSED;
		box->parent_window->flags |= WIN_QUIT;
		box->parent_window->global->windows->flags |= WIN_DIRTY;
		SDL_StopTextInput();
	}

}

void ui_bhook_inputcatch(ui_box_t *box, SDL_Event *e, void *data) {
    (void)data;
    if (!box || !(box->flags & BOX_FOCUSED) || !e)
        return;

    // ui_globalApp_t *app = box->parent_window->global;
    bool updated = false;

    if (e->type == SDL_TEXTINPUT) {
        int add = strlen(e->text.text);
        if (box->input_size + add < box->input_sizemax) {
            strcat(box->input, e->text.text);
            box->input_size += add;
            updated = true;
        }
    } else if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (box->input_size > 0) {
                    box->input[--box->input_size] = '\0';
                    updated = true;
                }
                break;
            case SDLK_ESCAPE:
				ui_bhook_inputcancel(box, e, NULL);
                return;
            case SDLK_RETURN:
				ui_bhook_inputvalid(box, e, NULL);
				return;
        }
    }

    if (updated || !box->layers) {
        ui_layer_destroy(&box->layers);
        SDL_Color c = {233, 210, 200, 255};
        SDL_Texture *texture = ui_tool_text2texture(box->parent_window,
            box->input_size > 0 ? box->input : "", c);

        int tw, th;
        SDL_QueryTexture(texture, NULL, NULL, &tw, &th);

        ui_layer_t *layer = ui_layer_create(box, texture);

        if (tw <= box->area.w) {
            // text fits — center vertically, align left with padding
            layer->dimension = (SDL_Rect){
                box->area.x + 4,
                box->area.y + (box->area.h - th) / 2,
                tw, th
            };
        } else {
            // text overflows — shift left, show end of text
            layer->dimension = (SDL_Rect){
                box->area.x + box->area.w - tw - 4,  // shift left
                box->area.y + (box->area.h - th) / 2,
                tw, th
            };
        }
        ui_layer_add(&box->layers, layer);
    }
}

void	ui_bhook_wincenter(ui_box_t* box, SDL_Event* e, void* data) {
	(void)e;
	SDL_Rect* offset = (SDL_Rect*)data;

	ui_win_t *win = box->parent_window;
	// ui_globalApp_t *ref = win->global;
	SDL_Rect area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
	int x = 0;
	int y = 0;
	if (offset) {
		x = offset->x;
		y = offset->y;
	}
	box->area = ui_tool_rectcenter(
		(SDL_Rect){x, y, win->area.w, win->area.h},
		(SDL_Rect){0, 0, area.w, area.h});
	if (box->layers) {
		box->layers->dimension = ui_tool_rectcenter(box->area, box->layers->dimension);
	}
}



void ui_bhook_movelayer(ui_box_t *cnv, SDL_Event *e, void *data)
{
    (void)e;
    (void)data;
    static SDL_Point last = {0, 0};
    static ui_layer_t *selected = NULL;

    SDL_Point p = ui_box_mousepos(cnv);
    if (cnv->flags & BOX_PRESSED) {
		if(!selected)
			selected = ui_layer_selected(cnv->layers, &p);
    	if (selected) {
			if (last.x == 0 && last.y == 0) {
				last = p;
				return;
			}
			// compute delta
			int dx = p.x - last.x;
			int dy = p.y - last.y;
			// move the layer
			selected->dimension.x += dx;
			selected->dimension.y += dy;
			last = p;  // update for next frame
		}
		cnv->selection = selected;
    } else {
		selected = NULL;
        last = (SDL_Point){0, 0};
    }
}

void	ui_bhook_maxsize(ui_box_t *box, SDL_Event *e, void *data) 
{
	(void)e;
	(void)data;
	ui_win_t *win = box->parent_window;
	if(win->menu && win->menu != box) {
		box->area.y = BOX_MENU_H + (2 * MENU_GAP_Y) + win->menu->border;
		box->area.h = win->area.h - (BOX_MENU_H + (2 * MENU_GAP_Y) - win->menu->border);
	}
	box->area.h = win->area.h;
	box->area.w = win->area.w;
}
