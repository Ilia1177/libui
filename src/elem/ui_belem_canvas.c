#include "libui.h"

static void ui_bhook_drawbox_canvas(ui_box_t* box, SDL_Event* e, void* data)
{
    (void)e;
    (void)data;
    if (!box || (box->flags & BOX_HIDDEN))
        return;

    SDL_Renderer* render = box->parent_window->renderer;

    // 1. Draw the Main Background
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(render, box->color.r, box->color.g, box->color.b, box->color.a);
    SDL_RenderFillRect(render, &box->area);

    // 2. Define Grid Style
    const int cell_size = 20; // Size of each grid square in pixels
    
    // Grid color (Dark Gray). 
    // You can change the last value (Alpha) to make it transparent (e.g., 128).
    SDL_SetRenderDrawColor(render, 100, 100, 100, 255); 

    // 3. Draw Vertical Lines
    // We start at box->area.x and step by cell_size
    for (int x = box->area.x; x <= box->area.x + box->area.w; x += cell_size) {
        SDL_RenderDrawLine(render, x, box->area.y, x, box->area.y + box->area.h);
    }

    // 4. Draw Horizontal Lines
    // We start at box->area.y and step by cell_size
    for (int y = box->area.y; y <= box->area.y + box->area.h; y += cell_size) {
        SDL_RenderDrawLine(render, box->area.x, y, box->area.x + box->area.w, y);
    }
}

ui_box_t *ui_belem_canvas(ui_win_t* win) 
{
	ui_box_t* cnv = ui_box_create(win, 0, win->colors[3]);
	ui_bhook_append(&cnv->on_window_event, ui_bhook_fullheight);
	ui_bhook_append(&cnv->on_window_event, ui_bhook_fullwidth);
	ui_bhook_replace(cnv->render, ui_bhook_drawbox, ui_bhook_drawbox_canvas);
	return cnv;
}
