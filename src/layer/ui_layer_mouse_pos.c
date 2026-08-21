#include "libui.h"

SDL_Point ui_layer_mousepos(ui_layer_t *layer) {
    ui_box_t *box = layer->parent_box;
    SDL_Point box_mouse = ui_box_mousepos(box);
    float zoom = box->zoom_amt;

    if (layer->angle == 0.0) {
        return (SDL_Point){
            (int)((box_mouse.x - layer->area.x) / zoom),
            (int)((box_mouse.y - layer->area.y) / zoom)
        };
    }

    double cx = layer->area.x + layer->area.w * zoom / 2.0;
    double cy = layer->area.y + layer->area.h * zoom / 2.0;
    double mx = box_mouse.x;
    double my = box_mouse.y;
    double dx = mx - cx;
    double dy = my - cy;
    double angle_rad = -layer->angle * M_PI / 180.0;
    double cos_a = cos(angle_rad);
    double sin_a = sin(angle_rad);
    double rx = dx * cos_a - dy * sin_a;
    double ry = dx * sin_a + dy * cos_a;
    return (SDL_Point){
        (int)(layer->area.w / 2.0 + rx / zoom),
        (int)(layer->area.h / 2.0 + ry / zoom)
    };
}

