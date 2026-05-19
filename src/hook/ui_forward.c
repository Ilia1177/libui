#include "ui_forward.h"

static void forward_one_box(ui_box_t *box, SDL_Event *e, void *data, ui_forward_type_t type)
{
	if (!box)
		return;

	switch (type) {
	case UI_FORWARD_CLICK_DOWN:
		if (box->on_click_down)
			ui_bhook_fire(box->on_click_down, box, e, data);
		break;
	case UI_FORWARD_CLICK_UP:
		if (box->on_click_up)
			ui_bhook_fire(box->on_click_up, box, e, data);
		break;
	case UI_FORWARD_MOUSE_MOTION:
		if (box->on_mouse_motion)
			ui_bhook_fire(box->on_mouse_motion, box, e, data);
		break;
	case UI_FORWARD_MOUSE_WHEEL:
		if (box->on_mouse_wheel)
			ui_bhook_fire(box->on_mouse_wheel, box, e, data);
		break;
	case UI_FORWARD_KEY_DOWN:
		if (box->on_key_down)
			ui_bhook_fire(box->on_key_down, box, e, data);
		break;
	case UI_FORWARD_WINDOW_EVENT:
		if (box->on_window_event)
			ui_bhook_fire(box->on_window_event, box, e, data);
		break;
	case UI_FORWARD_UPDATE:
		if (box->update)
			ui_bhook_fire(box->update, box, e, data);
		break;
	case UI_FORWARD_RENDER:
		if (box->render)
			ui_bhook_fire(box->render, box, e, data);
		break;
	}

	ui_box_t *child = box->childs;
	while (child) {
		forward_one_box(child, e, data, type);
		child = child->next;
	}
}

void ui_forward_to_boxes(ui_win_t *win, SDL_Event *e, void *data, ui_forward_type_t type)
{
	if (!win || (win->state & WIN_QUIT))
		return;

	if (type == UI_FORWARD_RENDER) {
		if (win->canvas)
			forward_one_box(win->canvas, e, data, type);
		ui_box_t *root = win->boxes;
		while (root) {
			forward_one_box(root, e, data, type);
			root = root->next;
		}
		SDL_RenderPresent(win->renderer);
		return;
	}

	ui_box_t *root = win->boxes;
	while (root) {
		forward_one_box(root, e, data, type);
		root = root->next;
	}
	if (win->canvas)
		forward_one_box(win->canvas, e, data, type);
}
