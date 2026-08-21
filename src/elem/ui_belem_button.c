#include "libui.h"

void ui_bhook_drawbox_button2(ui_box_t* box, SDL_Event* e, void* data)
{
    (void)e;
    (void)data;
    if (!box || (box->state & BOX_HIDDEN))
        return;

    SDL_Renderer* render = box->win->renderer;

    // Drop shadow
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_Rect shadow = {
        box->area.x,
        box->area.y,
        box->area.w,
        box->area.h
    };
    SDL_RenderFillRect(render, &shadow);

	SDL_Rect inside = {
		box->area.x + 1,
		box->area.y + 1,
		box->area.w - 2,
		box->area.h - 2
	};
    // Main box fill (existing)
    SDL_SetRenderDrawColor(render, box->color.r, box->color.g, box->color.b, box->color.a);
    SDL_RenderFillRect(render, &inside);
}

void ui_bhook_drawbox_button(ui_box_t* box, SDL_Event* e, void* data)
{
    (void)e;
    (void)data;
    if (!box || (box->state & BOX_HIDDEN))
        return;

    SDL_Renderer* render = box->win->renderer;
    SDL_Rect r = box->area;

    // 1. Draw the Main Background
    SDL_SetRenderDrawColor(render, box->color.r, box->color.g, box->color.b, box->color.a);
    SDL_RenderFillRect(render, &r);

    // 2. Check if button is pressed
    bool is_pressed = (box->state & BOX_CLICKED);

    // 3. Draw the Beveled Borders
    if (!is_pressed) {
        // --- NORMAL STATE (Raised) ---
        
        // Top and Left: White (Highlight)
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
        SDL_RenderDrawLine(render, r.x, r.y, r.x + r.w - 1, r.y);             // Top
        SDL_RenderDrawLine(render, r.x, r.y, r.x, r.y + r.h - 1);             // Left

        // Bottom and Right: Dark Gray (Shadow)
        SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
        SDL_RenderDrawLine(render, r.x, r.y + r.h - 1, r.x + r.w - 1, r.y + r.h - 1); // Bottom
        SDL_RenderDrawLine(render, r.x + r.w - 1, r.y, r.x + r.w - 1, r.y + r.h - 1); // Right

        // Inner Bottom and Right: Black (Deep Shadow) - Optional but gives that sharp Win98 look
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderDrawLine(render, r.x + 1, r.y + r.h - 2, r.x + r.w - 2, r.y + r.h - 2); // Bottom Inner
        SDL_RenderDrawLine(render, r.x + r.w - 2, r.y + 1, r.x + r.w - 2, r.y + r.h - 2); // Right Inner

    } else {
        // --- PRESSED STATE (Sunken) ---

        // Top and Left: Dark Gray (Shadow)
        SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
        SDL_RenderDrawLine(render, r.x, r.y, r.x + r.w - 1, r.y);             // Top
        SDL_RenderDrawLine(render, r.x, r.y, r.x, r.y + r.h - 1);             // Left

        // Inner Top and Left: Black (Deep Shadow)
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderDrawLine(render, r.x + 1, r.y + 1, r.x + r.w - 2, r.y + 1); // Top Inner
        SDL_RenderDrawLine(render, r.x + 1, r.y + 1, r.x + 1, r.y + r.h - 2); // Left Inner

        // Bottom and Right: White (Highlight)
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
        SDL_RenderDrawLine(render, r.x, r.y + r.h - 1, r.x + r.w - 1, r.y + r.h - 1); // Bottom
        SDL_RenderDrawLine(render, r.x + r.w - 1, r.y, r.x + r.w - 1, r.y + r.h - 1); // Right
    }
}

ui_box_t*	ui_belem_button(ui_win_t* win, SDL_Texture* texture)
{
		ui_box_t*	btn;
		int h, w;

		SDL_QueryTexture(texture, NULL, NULL, &w, &h);

		btn = ui_box_create(win, 0);
		btn->color = DEFAULT_BUTTON_COLOR;
		btn->area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
		btn->state |= BOX_CLICKABLE | BOX_HOVERABLE;
		ui_layout_set(btn, UI_LAYOUT_CONTENT_ALIGN_CENTER_X | UI_LAYOUT_CONTENT_ALIGN_CENTER_Y, true);
		ui_layer_make(btn, texture);
		ui_bhook_replace(btn->render, ui_bhook_drawbox, ui_bhook_drawbox_button);
		return btn;
}
