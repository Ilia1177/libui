#include "libui.h"
#include "ui_box.h"
#include "ui_global.h" // For ui_globalApp_t
#include <stdlib.h> // For malloc, free
#include <stdio.h> // For fprintf, printf (debugging)

// Helper to check if a point is inside a box
// static int is_point_in_box(int px, int py, ui_box_t* box) {
//     return (px >= box->pos.x && px < (box->pos.x + box->w) &&
//             py >= box->pos.y && py < (box->pos.y + box->h));
// }

ui_box_t* ui_box_create(SDL_Rect rect, ui_rgba_t color, ui_win_t* parent_window) {
    ui_box_t* box = (ui_box_t*)malloc(sizeof(ui_box_t));
    if (!box) {
        fprintf(stderr, "Failed to allocate ui_box_t\n");
        return NULL;
    }

	box->area = rect;
    box->color = color;
    box->flags = BOX_DIRTY; // lags set initially
    box->parent_window = parent_window;
	box->parent_window->flags &= WIN_DIRTY;

    box->render = ui_box_render_default; // Set default renderer
	box->update = ui_box_update_default;

    box->on_click_up = ui_box_on_click_up_handler;
    box->on_click_down = ui_box_on_click_down_handler;

    box->next = NULL;

    return box;
}

void ui_box_destroy(ui_box_t* box) {
    if (box) {
        free(box);
    }
	ui_box_t *current = box->child_boxes;
	while(current) {
		ui_box_t *next = current->next; 
		ui_box_destroy(current);
		current = next;
	}
}



// void ui_box_set_onclick(ui_box_t* box, void (*handler)(ui_box_t*, ui_globalApp_t*, SDL_MouseButtonEvent*)) {
//     if (box) {
//         box->on_click_up = handler;
//     }
// }

// void ui_box_set_onhover_enter(ui_box_t* box, void (*handler)(ui_box_t*, ui_globalApp_t*, SDL_MouseMotionEvent*)) {
//     if (box) {
//         box->on_hover_enter = handler;
//     }
// }

// void ui_box_set_onhover_leave(ui_box_t* box, void (*handler)(ui_box_t*, ui_globalApp_t*, SDL_MouseMotionEvent*)) {
//     if (box) {
//         box->on_hover_leave = handler;
//     }
// }
