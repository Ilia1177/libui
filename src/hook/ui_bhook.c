#include "libui.h"

void	ui_bhook_canvassize(ui_box_t* b, SDL_Event *e, void* data)
{
	ui_win_t* win;
	ui_box_t* menu;
	(void)e;
	(void)data;

	win = b->parent_window;
	menu = win->boxes;
	switch(win->boxes->type) {
		case UI_VERTICAL_MENU:
			b->area = ui_area(menu->area.w, 0, 
					win->area.w - menu->area.w, win->area.h);
			break;
		case UI_HORIZONTAL_MENU:
			b->area = ui_area(0, menu->area.h, 
					win->area.w, win->area.h - menu->area.h);
		default :
			break;
	}
}

void ui_bhook_nopressed(ui_box_t *b, SDL_Event *e, void* data) {
	(void)e;
	(void)data;
	b->flags &= ~BOX_PRESSED;
}

void ui_bhook_nofocused(ui_box_t *b, SDL_Event *e, void* data) {
	(void)e;
	(void)data;
	b->flags &= ~BOX_FOCUSED;
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
		win->state |= WIN_QUIT;
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
		app->state &= ~APP_LOADING;
		box->flags &= ~BOX_FOCUSED;
		box->parent_window->state |= WIN_QUIT;
		box->parent_window->global->windows->state |= WIN_DIRTY;
		if (box->data)
			free(box->data);
		box->data = NULL;
	}
}

void transfert_all_input(ui_globalApp_t* app, ui_box_t* box)
{
	if (!box) {
		return;
	} else if (box->data) {
		printf("valid input\n");
		fflush(stdout);
		// append input to app->inputs
		int n = app->input_nb;
		app->inputs = ui_realloc(app->inputs, 
			(app->input_nb + 1) * sizeof(char*),  // old
			(app->input_nb + 2) * sizeof(char*)
				);
		app->inputs[n]     = box->data;  // store current input
		app->inputs[n + 1] = NULL;        // NULL terminate like argv
		app->input_nb++;

		box->data = NULL;
		box->flags &= ~BOX_FOCUSED;
		box->parent_window->state |= WIN_QUIT;
		box->parent_window->global->windows->state |= WIN_DIRTY;
	}
    transfert_all_input(app, box->list);
    transfert_all_input(app, box->next);
}

void ui_bhook_valid_input(ui_box_t* b, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_globalApp_t *app;

	if (!(b->flags & BOX_CLICKED))
		return;
	app = b->parent_window->global;
	transfert_all_input(app, b->parent_window->boxes);
}

// void ui_bhook_inputvalid(ui_box_t *box, SDL_Event *e, void *data) {
//     (void)data;
//     // if (!box || !e) return;
//     ui_globalApp_t *app = box->parent_window->global;
//
//     if ((e && e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_RETURN)
//             || box->flags & BOX_CLICKED) {
//
//         // append input to app->inputs
//         int n = app->input_nb;
//         app->inputs = ui_realloc(app->inputs, 
// 			(app->input_nb + 1) * sizeof(char*),  // old
// 			(app->input_nb + 2) * sizeof(char*)
// 				);
//         app->inputs[n]     = box->input;  // store current input
//         app->inputs[n + 1] = NULL;        // NULL terminate like argv
//         app->input_nb++;
//
//         box->input = NULL;
//         box->flags &= ~BOX_FOCUSED;
//         box->parent_window->state |= WIN_QUIT;
//         box->parent_window->global->windows->state |= WIN_DIRTY;
//     }
// }
// void ui_bhook_inputvalid(ui_box_t *box, SDL_Event *e, void *data)
// {
// 	(void)data;
// 	(void)e;
// 	ui_globalApp_t *app = box->parent_window->global;  
//
// 	if ((e && e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_RETURN)
// 			|| box->flags & BOX_CLICKED) {
// 		app->inputs = box->input;
// 		box->input = NULL;
// 		box->flags &= ~BOX_FOCUSED;
// 		box->parent_window->state |= WIN_QUIT;
// 		box->parent_window->global->windows->state |= WIN_DIRTY;
// 		SDL_StopTextInput();
// 		printf("valid input: loading: %d\n", app->loading);
// 	}
//
// }

void ui_bhook_inputcatch(ui_box_t *box, SDL_Event *e, void *data) 
{
    (void)data;
    if (!box || !(box->flags & BOX_FOCUSED) || !e)
        return;
	char *input = (char *)box->data;
    // ui_globalApp_t *app = box->parent_window->global;
    bool updated = false;

	int input_size = box->data ? strlen((char*)box->data) : 0;
    if (e->type == SDL_TEXTINPUT) {
        int add = strlen(e->text.text);
        if (input_size + add < INPUT_SIZE_MAX) {
            strcat(box->data, e->text.text);
            // box->input_size += add;
            updated = true;
        }
    } else if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (input_size > 0) {
                    input[--input_size] = '\0';
                    updated = true;
                }
                break;
            case SDLK_ESCAPE:
        		SDL_StopTextInput();
				ui_bhook_inputcancel(box, e, NULL);
                break;
            case SDLK_RETURN:
        		SDL_StopTextInput();
				transfert_all_input(box->parent_window->global, box);
				break;
        }
    }

    if (updated || !box->layers) {
        ui_layer_clean(&box->layers);
        SDL_Color c = {0, 0, 0, 255};
        SDL_Texture *texture = ui_tool_text2texture(box->parent_window,
            input_size > 0 ? box->data : "", c);

        // int tw, th;
        // SDL_QueryTexture(texture, NULL, NULL, &tw, &th);

        ui_layer_t *layer = ui_layer_make(box, texture);

        if (layer->area.w <= box->area.w) {
            // text fits — center vertically, align left with padding
            layer->area = (SDL_Rect){
                box->area.x + 4,
                box->area.y + (box->area.h - layer->area.h) / 2,
                layer->area.w, layer->area.h
            };
        } else {
            // text overflows — shift left, show end of text
            layer->area = (SDL_Rect){
                box->area.x + box->area.w - layer->area.w - 4,  // shift left
                box->area.y + (box->area.h - layer->area.h) / 2,
                layer->area.w, layer->area.h
            };
        }
        // ui_layer_add(&box->layers, layer);
		box->parent_window->state |= WIN_DIRTY;
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
	box->area = ui_area_center(
		(SDL_Rect){x, y, win->area.w, win->area.h},
		(SDL_Rect){0, 0, area.w, area.h});
	if (box->layers) {
		box->layers->area = ui_area_center(box->area, box->layers->area);
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
			selected->area.x += dx;
			selected->area.y += dy;
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
	if(win->boxes && win->boxes != box) {
		box->area.y = BOX_MENU_H + (2 * MENU_GAP_Y) + win->boxes->border;
		box->area.h = win->area.h - (BOX_MENU_H + (2 * MENU_GAP_Y) - win->boxes->border);
	}
	box->area.h = win->area.h;
	box->area.w = win->area.w;
}
