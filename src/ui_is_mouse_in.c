#include "ui_box.h"
SDL_Point ui_layer_mousepos22(ui_layer_t *layer) {
    ui_box_t  *box       = layer->parent_box;
    SDL_Point  box_mouse = ui_box_mousepos(box);
    float      zoom      = box->zoom_amt;
    SDL_Point  origin    = box->zoom_origin;

    // reverse zoom transform around origin
    int unzoomed_x = origin.x + (int)((box_mouse.x - origin.x) / zoom);
    int unzoomed_y = origin.y + (int)((box_mouse.y - origin.y) / zoom);

    // layer->area is window-relative, convert to box-relative
    return (SDL_Point){
        unzoomed_x - (layer->area.x - box->area.x),
        unzoomed_y - (layer->area.y - box->area.y)
    };
}
SDL_Point ui_layer_mousepos(ui_layer_t *layer) {
    ui_box_t  *box       = layer->parent_box;
    SDL_Point  box_mouse = ui_box_mousepos(box);
    float      zoom      = box->zoom_amt;

    // convert mouse to texture coords accounting for zoom
    return (SDL_Point){
        (int)((box_mouse.x - (layer->area.x - box->area.x)) / zoom),
        (int)((box_mouse.y - (layer->area.y - box->area.y)) / zoom)
    };
}
SDL_Point ui_layer_mousepos2(ui_layer_t *layer) {

	SDL_Point box_mouse;
	ui_box_t* box;

	box = layer->parent_box;
	box_mouse = ui_box_mousepos(box);
	return (SDL_Point){
        box_mouse.x - (layer->area.x - box->area.x),
        box_mouse.y - (layer->area.y - box->area.y)  // removes the 70
    };
}

SDL_Point ui_box_mousepos(ui_box_t *box) {
	SDL_Point win_mouse;

	win_mouse = ui_win_mousepos(box->parent_window);
    return (SDL_Point){
        win_mouse.x - box->area.x,
        win_mouse.y - box->area.y
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
