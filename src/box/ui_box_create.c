#include "ui_box.h"

ui_box_t* ui_box_create(ui_win_t* win, uint32_t layout, SDL_Color color) {
    ui_box_t* box = (ui_box_t*)calloc(1, sizeof(ui_box_t));
    if (!box) {
        return NULL;
    }

	box_nb++; // DEBUG
	box->layout = layout;
    box->color = color;
    box->parent_window = win;
	box->parent_window->state |= WIN_DIRTY;
	box->zoom_origin = (SDL_Point){box->area.w / 2, box->area.h / 2};
	box->zoom_amt = 1.0f;

    ui_bhook_prepend(&box->render, ui_bhook_render_default);
    ui_bhook_prepend(&box->render, ui_bhook_drawlayers);
    ui_bhook_prepend(&box->render, ui_bhook_drawfocused);
    ui_bhook_prepend(&box->render, ui_bhook_drawhovered);
    ui_bhook_prepend(&box->render, ui_bhook_drawpressed);
    ui_bhook_prepend(&box->render, ui_bhook_drawbox);
    ui_bhook_prepend(&box->render, ui_bhook_drawborder);

	ui_bhook_prepend(&box->update, ui_bhook_update_default);
    ui_bhook_prepend(&box->destroy, ui_bhook_destroy_default);

	ui_bhook_prepend(&box->on_window_event, ui_bhook_windowevent_default);
	ui_bhook_prepend(&box->on_mouse_motion, ui_bhook_mousemotion_default);
    ui_bhook_prepend(&box->on_click_up, ui_bhook_clickup_default);
    ui_bhook_prepend(&box->on_click_down, ui_bhook_clickdown_default);
	box->on_mouse_wheel = NULL;

    box->prev = NULL;
    box->next = NULL;

    return box;
}

