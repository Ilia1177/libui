#include "ui_win.h"

ui_scale_t ui_win_get_scale(ui_win_t *win) {
    int lw, lh, pw, ph;
    SDL_GetWindowSize(win->ptr, &lw, &lh);
    SDL_GL_GetDrawableSize(win->ptr, &pw, &ph);
	win->scale.x = (float)pw / lw;
	win->scale.y = (float)ph / lh;
    return (win->scale);
}

SDL_Rect ui_win_scale_rect(ui_win_t *win, SDL_Rect r) {
    return (SDL_Rect){
        .x = (int)(r.x * win->scale.x),
        .y = (int)(r.y * win->scale.y),
        .w = (int)(r.w * win->scale.x),
        .h = (int)(r.h * win->scale.y)
    };
}
