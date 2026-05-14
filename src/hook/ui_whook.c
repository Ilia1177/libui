#include "ui_win.h"

int ui_whook_reset_global_state(ui_win_t* win, SDL_Event *e, void* data) 
{
	printf("reset state'\n");
	fflush(stdout);
	(void)data;
	if(e && e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_CLOSE)
	{
		win->global->loading = false;
		win->global->state &= ~APP_LOADING;
		printf("reset: loading: %d\n", win->global->state & APP_LOADING);
		return 0;
	}
	return 1;
}

int ui_whook_loadimage(ui_win_t* win, SDL_Event* e, void *data) 
{
	(void)e;
	(void)data;
	ui_globalApp_t* app;
	static bool running = false;

	app = win->global;
	if (!(app->state & APP_LOADING) && !running) {
		ui_welem_input(win);
		app->state |= APP_LOADING;
		running = true;
		return 1;
	} else if (!(app->state & APP_LOADING) && running) {
		running = false;
		return 0;
	} else if(app->input_nb <= 0)
		return 1;
	SDL_Surface *surface = IMG_Load((char*)app->inputs[0]);
	reset_state_and_input(app, &running);
	if (!surface)
		return -1;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(win->renderer, surface);
	SDL_FreeSurface(surface);
	if (!tex)
		return -1;
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	ui_layer_make(win->canvas, tex);
	win->state |= WIN_DIRTY;
	return 0;
}

static int ui_box_focusable(ui_box_t *box)
{
    if (!box)
        return 0;

    if (box->flags & BOX_HIDDEN)
        return 0;

    if (box->flags & BOX_DISABLE)
        return 0;

    return 1;
}

static ui_box_t *ui_next_focusable(ui_box_t *box)
{
    if (!box)
        return NULL;

    if (ui_box_focusable(box))
        return box;

    ui_box_t *found = ui_next_focusable(box->list);
    if (found)
        return found;

    return ui_next_focusable(box->next);
}

static ui_box_t *ui_find_focused(ui_box_t *box)
{
    if (!box)
        return NULL;

    if (box->flags & BOX_FOCUSED)
        return box;

    ui_box_t *found = ui_find_focused(box->list);
    if (found)
        return found;

    return ui_find_focused(box->next);
}

static ui_box_t *ui_next_node(ui_box_t *box)
{
    if (!box)
        return NULL;

    if (box->list)
        return box->list;

    while (box)
    {
        if (box->next)
            return box->next;

        box = box->parent;
    }

    return NULL;
}
void ui_focus_next(ui_box_t *root)
{
    if (!root)
        return;

    ui_box_t *current = ui_find_focused(root);

    if (!current)
    {
        ui_box_t *first = ui_next_focusable(root);

        if (first)
            first->flags |= BOX_FOCUSED;

        return;
    }

    current->flags &= ~BOX_FOCUSED;

    ui_box_t *iter = ui_next_node(current);

    while (iter)
    {
        if (ui_box_focusable(iter))
        {
            iter->flags |= BOX_FOCUSED;
            return;
        }

        iter = ui_next_node(iter);
    }

    ui_box_t *first = ui_next_focusable(root);

    if (first)
        first->flags |= BOX_FOCUSED;
}

int ui_whook_next_focus(ui_win_t* win, SDL_Event* e, void* data) {
	(void)data;
	ui_box_t* focused;
	if (!e || !win) {
		return 1;
	} else if (e->type == SDL_KEYDOWN) {
		switch (e->key.keysym.sym) {
			case SDLK_TAB:
				ui_focus_next(win->boxes);
				return 1;
			case SDLK_RETURN:
				focused = ui_find_focused(win->boxes);
				focused->flags |= BOX_CLICKED;
			default:
				return 1;
		}
	}
	return 1;
}

int ui_whook_quitkey(ui_win_t* win, SDL_Event *e, void *data)
{
	(void)data;

	if (!e || !win) {
		return 1;
	} else if (e->type == SDL_KEYDOWN) {
		switch (e->key.keysym.sym) {
			case SDLK_RETURN: case SDLK_ESCAPE:
				win->state |= WIN_QUIT;
				return 0;

			default:
				break;
		}
	}
	return 1;
} 

