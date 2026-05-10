#include "ui_box.h"

ui_box_t* ui_box_create(SDL_Rect rect, SDL_Color color, ui_win_t* parent_window) {
    ui_box_t* box = (ui_box_t*)calloc(1, sizeof(ui_box_t));
    if (!box) {
        fprintf(stderr, "Failed to allocate ui_box_t\n");
        return NULL;
    }

	box->area = rect;
    box->color = color;
    box->parent_window = parent_window;
	box->parent_window->flags |= WIN_DIRTY;

    ui_bhook_add(&box->render, ui_bhook_render_default); // Set default rendere)r
	ui_bhook_add(&box->update, ui_bhook_update_default);
    ui_bhook_add(&box->destroy, ui_bhook_destroy_default);

	ui_bhook_add(&box->on_window_event, ui_bhook_windowevent_default);
	ui_bhook_add(&box->on_mouse_motion, ui_bhook_mousemotion_default);
    ui_bhook_add(&box->on_click_up, ui_bhook_clickup_default);
    ui_bhook_add(&box->on_click_down, ui_bhook_clickdown_default);

    box->prev = NULL;
    box->next = NULL;

    return box;
}

