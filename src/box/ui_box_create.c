#include "ui_box.h"

ui_box_t* ui_box_create(SDL_Rect rect, ui_rgba_t color, ui_win_t* parent_window) {
    ui_box_t* box = (ui_box_t*)malloc(sizeof(ui_box_t));
    if (!box) {
        fprintf(stderr, "Failed to allocate ui_box_t\n");
        return NULL;
    }

	box->area = rect;
    box->color = color;
    box->parent_window = parent_window;
	box->parent_window->flags |= WIN_DIRTY;

    ui_box_handler_add(&box->render, ui_box_render_default); // Set default rendere)r
	ui_box_handler_add(&box->update, ui_box_update_default);
    ui_box_handler_add(&box->destroy, ui_box_destroy_default);

	ui_box_handler_add(&box->on_mouse_motion, ui_box_on_mouse_motion_handler);
    ui_box_handler_add(&box->on_click_up, ui_box_on_click_up_handler);
    ui_box_handler_add(&box->on_click_down, ui_box_on_click_down_handler);

    box->next = NULL;

    return box;
}

