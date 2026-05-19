#ifndef UI_TOOL_H
#define UI_TOOL_H

#include <SDL2/SDL.h>

typedef struct ui_globalApp_s ui_globalApp_t;
typedef struct ui_box_s ui_box_t;

typedef struct ui_tool_s {
    const char *name;
    void *data;

    void (*on_activate)(ui_box_t *canvas, void *data);
    void (*on_deactivate)(ui_box_t *canvas, void *data);

    void (*on_click_down)(ui_box_t *canvas, SDL_Event *e, void *data);
    void (*on_click_up)(ui_box_t *canvas, SDL_Event *e, void *data);
    void (*on_mouse_motion)(ui_box_t *canvas, SDL_Event *e, void *data);
    void (*on_key_down)(ui_box_t *canvas, SDL_Event *e, void *data);

    void (*on_update)(ui_box_t *canvas, void *data);

    void (*on_render)(ui_box_t *canvas, SDL_Renderer *r, void *data);

    void (*on_cleanup)(void *data);
} ui_tool_t;

void ui_tool_init(ui_tool_t *tool, const char *name, void *data);
void ui_tool_activate(ui_globalApp_t *app, ui_tool_t *tool, ui_box_t *canvas);
void ui_tool_deactivate(ui_globalApp_t *app, ui_box_t *canvas);

#endif
