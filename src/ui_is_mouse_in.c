#include "ui_box.h"
SDL_Point ui_box_mousepos(ui_box_t *box) {
    int gx, gy;
    SDL_GetGlobalMouseState(&gx, &gy);
    ui_win_t *win = box->parent_window;

    // global → window-relative logical
    int lx = gx - win->area.x;
    int ly = gy - win->area.y;

    // logical → physical
    int px = (int)(lx * win->scale.x);
    int py = (int)(ly * win->scale.y);

    // physical window → box-relative
    return (SDL_Point){
        px - box->area.x,
        py - box->area.y
    };
}

SDL_Point ui_win_mousepos(ui_win_t* win) {
    int gx, gy;
    SDL_GetGlobalMouseState(&gx, &gy);

    // global mouse → window-relative logical coords
    int lx = gx - win->area.x;
    int ly = gy - win->area.y;

    // logical → physical pixels
    int px = (int)(lx * win->scale.x);
    int py = (int)(ly * win->scale.y);

    SDL_Point p = {px, py};
	return p;
}

// bool ui_is_mouse_in(ui_box_t *box) {
//     int gx, gy;
//     SDL_GetGlobalMouseState(&gx, &gy);
//     ui_win_t *win = box->parent_window;
//
//     // global mouse → window-relative logical coords
//     int lx = gx - win->area.x;
//     int ly = gy - win->area.y;
//
//     // logical → physical pixels
//     int px = (int)(lx * win->scale.x);
//     int py = (int)(ly * win->scale.y);
//
//     SDL_Point p = {px, py};
//     return SDL_PointInRect(&p, &box->area);
// }
