#include "ui_win.h"

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

    ui_box_t *found = ui_next_focusable(box->childs);
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

    ui_box_t *found = ui_find_focused(box->childs);
    if (found)
        return found;

    return ui_find_focused(box->next);
}

static ui_box_t *ui_next_node(ui_box_t *box)
{
    if (!box)
        return NULL;

    if (box->childs)
        return box->childs;

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

    if (first) {
        first->flags |= BOX_FOCUSED;
        // first->flags |= BOX_HOVERED;
	}
}

int ui_whook_keydown_default(ui_win_t* win, SDL_Event* e, void* data) {
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



