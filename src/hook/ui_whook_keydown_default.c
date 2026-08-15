#include "ui_win.h"

static int ui_box_focusable(ui_box_t* box)
{
    if (!box)
        return 0;

    if (box->state & BOX_HIDDEN)
        return 0;

    if (box->state & BOX_DISABLE)
        return 0;

    return 1;
}

static ui_box_t* ui_next_focusable(ui_box_t* box)
{
    if (!box)
        return NULL;

    if (ui_box_focusable(box))
        return box;

    ui_box_t* found = ui_next_focusable(box->childs);
    if (found)
        return found;

    return ui_next_focusable(box->next);
}

static ui_box_t* ui_find_focused(ui_box_t* box)
{
    if (!box)
        return NULL;

    if (box->state & BOX_FOCUSED)
        return box;

    ui_box_t* found = ui_find_focused(box->childs);
    if (found)
        return found;

    return ui_find_focused(box->next);
}

static ui_box_t* ui_next_node(ui_box_t* box)
{
    if (!box)
        return NULL;

    if (box->childs)
        return box->childs;

    while (box) {
        if (box->next)
            return box->next;

        box = box->parent;
    }

    return NULL;
}

void ui_focus_next(ui_box_t* root)
{
    if (!root)
        return;
	printf("focus next !\n");
	fflush(stdout);
    ui_box_t* current = ui_find_focused(root);

    if (!current) {
        ui_box_t* first = ui_next_focusable(root);

        if (first) {
            first->state |= BOX_FOCUSED;
		}

		ui_log("first box is focused & return");
        return;
    }

	ui_log("focus removed from current");
    current->state &= ~BOX_FOCUSED;
	if (current->state & BOX_INPUTABLE)
		SDL_StopTextInput();

    ui_box_t* iter = ui_next_node(current);

    while (iter) {
        if (ui_box_focusable(iter)) {
            iter->state |= BOX_FOCUSED;
			ui_log("next is focused");
            return;
        }
        iter = ui_next_node(iter);
    }
    ui_box_t* first = ui_next_focusable(root);
    if (first) {
        first->state |= BOX_FOCUSED;
		ui_log("first is focused");
    }
}

int ui_whook_keydown_default(ui_win_t* win, SDL_Event* e, void* data)
{
    (void)data;
    ui_box_t* focused;
    printf("win keydown default !\n");
    if (!e || !win) {
        return 1;
    } else if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
        case SDLK_TAB:
            ui_focus_next(win->boxes);
            break;
        case SDLK_RETURN:
            focused = ui_find_focused(win->boxes);
            if (!focused) {
				ui_focus_next(win->boxes);
			} else if (focused->state & BOX_CLICKABLE) {
				if (focused->state & BOX_CLICKED)
					ui_focus_next(focused);
				else
					focused->state |= BOX_CLICKED;
			} else if (focused->state & BOX_INPUTABLE) {
				ui_focus_next(focused);
			}
            break;
        case SDLK_ESCAPE:
            win->state |= WIN_QUIT;
			ui_reset_state_and_input(win->global, NULL);
            break;
        default:
            break;
        }
        win->state |= WIN_DIRTY;
    }
    return 1;
}
