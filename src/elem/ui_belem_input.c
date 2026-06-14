#include "libui.h"

static void ui_bhook_catch_input(ui_box_t *box, SDL_Event *e, void *data) 
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

static int ui_whook_windirty(ui_win_t* win, SDL_Event *e, void *data) {
	(void)data;
	(void)e;
	if (!win)
		return 0;
	win->state |= WIN_DIRTY;
	return 1;
}

static void ui_bhook_inputfocus(ui_box_t *box, SDL_Event *e, void *data) {
    (void)e;
    (void)data;
    if (box->flags & BOX_CLICKED || box->flags & BOX_FOCUSED) {
		if (box->flags & BOX_INPUTABLE)
			SDL_StartTextInput();
		box->flags |= BOX_FOCUSED;
		box->flags &= ~BOX_PRESSED;
    }
}

ui_box_t *ui_belem_input(ui_win_t *win, int max_len)
{
	(void)max_len;
	ui_box_t		*input = NULL;

	SDL_Rect area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
	input = ui_box_create(win, area, COLOR_WHITE);
    input->data = calloc(INPUT_SIZE_MAX + 1, sizeof(char));
	input->flags |= BOX_INPUTABLE;
	ui_whook_append(&win->render, ui_whook_windirty);
    ui_bhook_prepend(&input->update, ui_bhook_inputfocus);
    ui_bhook_append(&input->on_key_down, ui_bhook_catch_input);
	ui_bhook_replace(input->render, ui_bhook_drawlayers, ui_bhook_drawcliplayers);
	ui_bhook_replace(input->render, ui_bhook_drawfocused, ui_bhook_drawtextfocused);
    return input;
}
