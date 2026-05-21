#include "ui_forward.h"

static void event_one_box(ui_box_t *box, SDL_Event *e, void *data)
{
	if (!box || !e)
		return;
	switch (e->type) {
	case SDL_MOUSEBUTTONDOWN:
		ui_bhook_fire(box->on_click_down, box, e, data);
		break;
	case SDL_MOUSEBUTTONUP:
		ui_bhook_fire(box->on_click_up, box, e, data);
		break;
	case SDL_MOUSEMOTION:
		ui_bhook_fire(box->on_mouse_motion, box, e, data);
		break;
	case SDL_MOUSEWHEEL:
		ui_bhook_fire(box->on_mouse_wheel, box, e, data);
		break;
	case SDL_KEYDOWN: case SDL_TEXTINPUT:
		ui_bhook_fire(box->on_key_down, box, e, data);
		break;
	case SDL_WINDOWEVENT:
		ui_bhook_fire(box->on_window_event, box, e, data);
		break;
	}

	ui_box_t *child = box->childs;
	while (child) {
		event_one_box(child, e, data);
		child = child->next;
	}
}

static void update_one_box(ui_box_t* b, SDL_Event *e, void* data) 
{
	if (!b)
		return;
	ui_bhook_fire(b->update, b, e, data);

	ui_box_t* child = b->childs;
	while(child) {
		update_one_box(child, e, data);
		child = child->next;
	}
}

static void render_one_box(ui_box_t* b, SDL_Event *e, void* data) 
{
	if (!b)
		return;
	ui_bhook_fire(b->render, b, e, data);
	ui_box_t* child = b->childs;
	while(child) {
		render_one_box(child, e, data);
		child = child->next;
	}
}

void ui_box_render_forward(ui_win_t* win, SDL_Event *e, void *data) 
{
	if(!win)
		return;
	// ui_log("render canvas");
	// render_one_box(win->canvas, e, data);
	// ui_log("done render canvas");
	ui_box_t *root = win->boxes;
	while (root) {
		render_one_box(root, e, data);
		root = root->next;
	}
	return;
}

void ui_box_update_forward(ui_win_t* win, SDL_Event *e, void *data) 
{
	if(!win)
		return
	update_one_box(win->canvas, e, data);
	ui_box_t *root = win->boxes;
	while (root) {
		update_one_box(root, e, data);
		root = root->next;
	}
	return;
}

void ui_box_event_forward(ui_win_t *win, SDL_Event *e, void *data)
{
	if (!win || (win->state & WIN_QUIT))
		return;
	ui_box_t *root = win->boxes;
	while (root) {
		event_one_box(root, e, data);
		root = root->next;
	}
	if (win->canvas) {
		event_one_box(win->canvas, e, data);
	}
}
