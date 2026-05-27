#include "libui.h"
#include "math.h"
// #include "libft.h"

void ui_bhook_fullwindow_button(ui_box_t* btn, SDL_Event*e, void* data) 
{
	ui_win_t* win = btn->parent_window;

	(void)e;
	(void)data;

	ui_layer_t* layer = btn->layers;
	int count = ui_box_count_prev(btn);
	btn->area.w = (win->area.w - 3 * MENU_GAP_X) / 2;
	if (count % 2 == 1 || count % 2 == 2) {
		btn->area.x = MENU_GAP_X;
	} else {
		btn->area.x = MENU_GAP_X * 2 + btn->area.w;
	}
	if(layer)
		layer->area.x = btn->area.x + (BOX_MENU_W - layer->area.w) / 2;
	btn->parent_window->state |= WIN_DIRTY;
}

void	ui_bhook_canvassize(ui_box_t* b, SDL_Event *e, void* data)
{
	ui_log("hook update canvas size");
	ui_win_t* win;
	ui_box_t* menu;
	(void)e;
	(void)data;

	win = b->parent_window;
	menu = win->boxes;
	int menuH = menu->area.h;
	int menuW = menu->area.w;
	int winH = win->area.h;
	int winW = win->area.w;
	if (menuW == winW && menuH == winH) {
		return;
	} else if (menuW == winW) {
		b->area = ui_area(0, menuH, winW, winH - menuH);
	} else if (menuH == winH) {
		b->area = ui_area(menuW, 0, winW - menuW, winH);
	}
}

// in ui_bhook_zoomin — capture mouse at scroll time
void ui_bhook_zoomin_save(ui_box_t *cnv, SDL_Event *e, void *data) {
    (void)data;
    if (!cnv || !e || e->type != SDL_MOUSEWHEEL) return;
    if (!cnv->layers) return;

    float delta = (e->wheel.y > 0) ? 0.1f : -0.1f;
    cnv->zoom_amt = clampf(
        cnv->zoom_amt + delta, 0.1f, 10.0f);

    // freeze zoom origin at current mouse pos
    cnv->zoom_origin = ui_box_mousepos(cnv);

    cnv->parent_window->state |= WIN_DIRTY;
}
void ui_bhook_zoomin(ui_box_t *cnv, SDL_Event *e, void *data) {
    (void)data;
    if (!cnv || !e || e->type != SDL_MOUSEWHEEL) return;
    if (!cnv->layers) return;

    float delta = (e->wheel.y > 0) ? 0.1f : -0.1f;
    float old_zoom = cnv->zoom_amt;
    float new_zoom = clampf(old_zoom + delta, 0.1f, 10.0f);

    // zoom origin = mouse position in box space
    SDL_Point mouse = ui_box_mousepos(cnv);

    // adjust layer positions to zoom around mouse
    ui_layer_t *curr = cnv->layers;
    while (curr) {
        curr->area.x = mouse.x + cnv->area.x + 
            (int)((curr->area.x - mouse.x - cnv->area.x) * new_zoom / old_zoom);
        curr->area.y = mouse.y + cnv->area.y + 
            (int)((curr->area.y - mouse.y - cnv->area.y) * new_zoom / old_zoom);
        curr = curr->next;
    }

    cnv->zoom_amt = new_zoom;
    cnv->parent_window->state |= WIN_DIRTY;
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
	printf("win close\n");
	fflush(stdout);
	ui_win_t *win = b->parent_window;
	if (b->flags & BOX_CLICKED) {
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

	uint32_t state = box->flags;
	SDL_Point p = ui_win_mousepos(box->parent_window);
	if (state & BOX_HOVERED || state & BOX_CLICKED || ui_box_hovered(box->childs, &p)) {
	// if (box->flags & BOX_HOVERED || ui_box_hovered(box->childs, &p)) {
		ui_box_flags(box->childs, BOX_HIDDEN, false, false);
	} else {
		ui_box_t *curr = box->childs;
		while (curr) {
			if (curr->flags & BOX_HOVERED || curr->flags & BOX_FOCUSED) 
				return;
			curr = curr->next;
		}
		ui_box_flags(box->childs, BOX_HIDDEN, true, true);
	}
}

void ui_bhook_inputfocus(ui_box_t *box, SDL_Event *e, void *data) {
    (void)e;
    (void)data;
    if (box->flags & BOX_CLICKED || box->flags & BOX_FOCUSED) {
		if (box->flags & BOX_INPUTABLE)
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
	static int box_nb = 0;
	if (!box) {
		return;
	} else if (box->data) {
		printf("Input is: %s\n", (char*)box->data);
		fflush(stdout);
		int n = app->input_nb;
		app->inputs = ui_realloc(app->inputs,
			(app->input_nb + 1) * sizeof(char*),  //0ld
			(app->input_nb + 2) * sizeof(char*));
		app->inputs[n]     = box->data;		// store current input
		app->inputs[n + 1] = NULL;			// NULL terminate like argv
		app->input_nb++;
		printf("input nb: %d\n", app->input_nb);
		fflush(stdout);
		box->data = NULL;
		box->flags &= ~BOX_FOCUSED;
		// box->parent_window->state |= WIN_QUIT;
		// box->parent_window->global->windows->state |= WIN_DIRTY;
	}
    transfert_all_input(app, box->childs);
    transfert_all_input(app, box->next);
	box_nb++;
	printf("Box number: %d\n", box_nb);
	fflush(stdout);
}

void ui_bhook_valid_input(ui_box_t* b, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	ui_globalApp_t *app;

	if (!(b->flags & BOX_CLICKED))
		return;
	app = b->parent_window->global;
	printf("hook: valid input\n");
	fflush(stdout);
	// transfert_all_input(b->parent_window);
	transfert_all_input(app, b->parent_window->boxes);
	app->windows->state |= WIN_DIRTY;
	b->parent_window->state |= WIN_QUIT;
	printf("total boxe: %d\n", ui_box_count_all(b->parent_window->boxes));
	fflush(stdout);
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

void ui_bhook_catch_input(ui_box_t *box, SDL_Event *e, void *data) 
{
    (void)data;
	ui_win_t* win;
	char* input;
	int len;
	bool updated;
    if (!box || !(box->flags & BOX_FOCUSED) || !e)
        return;
	win = box->parent_window;
	input = (char *)box->data;
    updated = false;
	len = box->data ? strlen((char*)box->data) : 0;
    if (e->type == SDL_TEXTINPUT) {
        int add = strlen(e->text.text);
        if (len + add < INPUT_SIZE_MAX) {
            strcat(box->data, e->text.text);
            updated = true;
        }
    } else if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (len > 0) {
                    input[--len] = '\0';
                    updated = true;
                } break;
            case SDLK_ESCAPE:
				//     		SDL_StopTextInput();
				// ui_bhook_inputcancel(box, e, NULL);
                break;
            case SDLK_RETURN:
        		SDL_StopTextInput();
				// transfert_all_input(win->global, win->boxes);
				break;
        }
    }

    if (updated || (!box->layers && len > 0)) {
        ui_layer_clean(&box->layers);
        SDL_Color c = {0, 0, 0, 255};
        ui_layer_t *layer = ui_layer_make(box, ui_tex_str(win, box->data, c));
		if (!layer) {
			return;
		} if (layer->area.w <= box->area.w) {
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
    }
	win->state |= WIN_DIRTY;
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



// void ui_bhook_movelayer(ui_box_t *cnv, SDL_Event *e, void *data)
// {
//     (void)e;
//     (void)data;
//     static SDL_Point last = {0, 0};
//     static ui_layer_t *selected = NULL;
//
//     SDL_Point p = ui_box_mousepos(cnv);
//     if (cnv->flags & BOX_PRESSED) {
// 		if(!selected)
// 			selected = ui_layer_selected(cnv->layers, &p);
//     	if (selected) {
// 			if (last.x == 0 && last.y == 0) {
// 				last = p;
// 				return;
// 			}
// 			// compute delta
// 			int dx = p.x - last.x;
// 			int dy = p.y - last.y;
// 			// move the layer
// 			selected->area.x += dx;
// 			selected->area.y += dy;
// 			last = p;  // update for next frame
// 		}
// 		cnv->selected_layer = selected;
//     } else {
// 		selected = NULL;
//         last = (SDL_Point){0, 0};
//     }
// }
//
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
