#include "libui.h"

SDL_Rect ui_area_center(SDL_Rect boxarea, SDL_Rect tocenter) {
    return (SDL_Rect){
        boxarea.x + (boxarea.w - tocenter.w) / 2,
        boxarea.y + (boxarea.h - tocenter.h) / 2,
        tocenter.w,
        tocenter.h
    };
}

SDL_Texture *ui_tex_str(ui_win_t* win, const char *text, SDL_Color color)
{
	// SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(win->font, text, color, win->area.w);
    SDL_Surface *surface = TTF_RenderText_Blended(win->font, text, color);
    if (!surface) {
		printf("Error: %s\n", SDL_GetError());
		return NULL;
	} 
    SDL_Texture *texture = SDL_CreateTextureFromSurface(win->renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void ui_tool_drawcenter(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect box) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dest = {
        box.x + (BOX_MENU_W - w) / 2,
        box.y + (BOX_MENU_H - h) / 2,
        w, h
    };
	// SDL_Rect dest = ui_tool_rectcenter(box, (SDL_Rect){0, 0, w, h});
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}
