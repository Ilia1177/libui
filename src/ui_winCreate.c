#include "libui.h"
#include <unistd.h>

int ui_init() {
    return SDL_Init(SDL_INIT_VIDEO);
}

int ui_quit() {
    SDL_Quit();
	return 0;
}

void ui_winsize(ui_win_t *win, int w, int h)
{
    SDL_SetWindowSize(win->win, w, h);
}

void ui_winUpdate(ui_win_t* win, void *data)
{
	SDL_UpdateTexture(win->texture, NULL, data, win->w * 4);
}

void ui_run(ui_win_t* win) {
	SDL_Event e;
	while (!win->quit_flag) { // Check win->quit_flag
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				win->quit_flag = 1; // Set quit_flag
			}
			// --- New event dispatching logic ---
			else if (e.type == SDL_KEYDOWN) {
				if (win->on_key_down) {
					win->on_key_down(win, &e.key);
				}
			}
			else if (e.type == SDL_KEYUP) {
				if (win->on_key_up) {
					win->on_key_up(win, &e.key);
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (win->on_mouse_button_down) {
					win->on_mouse_button_down(win, &e.button);
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP) {
				if (win->on_mouse_button_up) {
					win->on_mouse_button_up(win, &e.button);
				}
			}
			else if (e.type == SDL_MOUSEMOTION) {
				if (win->on_mouse_motion) {
					win->on_mouse_motion(win, &e.motion);
				}
			}
			else if (e.type == SDL_MOUSEWHEEL) {
				if (win->on_mouse_wheel) {
					win->on_mouse_wheel(win, &e.wheel);
				}
			}
			// --- End new event dispatching logic ---
		}
		win->render(win);
	}
}

void	ui_render(ui_win_t* win) {
	        // Render
        SDL_SetRenderDrawColor(win->renderer, 30, 255, 30, 255);
        SDL_RenderClear(win->renderer);
		if (win->texture) {
			SDL_Rect dest = { 0, 0, win->w, win->h };
			SDL_RenderCopy(win->renderer, win->texture, NULL, &dest);
		}
        SDL_RenderPresent(win->renderer);
}

void	ui_winpos(ui_win_t* win, int x, int y)
{
    SDL_SetWindowPosition(win->win, x, y);
}

void	ui_destroy(ui_win_t* win)
{
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->win);
	free(win);
}

ui_win_t* ui_winCreate(int w, int h, char* title)
{
	ui_win_t *window = (ui_win_t*)malloc(sizeof(ui_win_t));

	if (!window) {
		return NULL;
	}
	window->quit_flag = 0; // Initialize quit flag
	window->win = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!window->win) {
        return NULL;
    }
	window->renderer = SDL_CreateRenderer(
        window->win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!window->renderer) {
        return NULL;
    }
	window->destroy = ui_destroy;
	window->render = ui_render;
	window->resize = ui_winsize;

    // Initialize event handler pointers to NULL
    window->on_key_down = NULL;
    window->on_key_up = NULL;
    window->on_mouse_button_down = NULL;
    window->on_mouse_button_up = NULL;
    window->on_mouse_motion = NULL;
    window->on_mouse_wheel = NULL;

	return window;
}

// Event registration functions
void ui_on_key_down(ui_win_t* win, void(*handler)(ui_win_t*, SDL_KeyboardEvent*)) {
    win->on_key_down = handler;
}

void ui_on_key_up(ui_win_t* win, void(*handler)(ui_win_t*, SDL_KeyboardEvent*)) {
    win->on_key_up = handler;
}

void ui_on_mouse_button_down(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseButtonEvent*)) {
    win->on_mouse_button_down = handler;
}

void ui_on_mouse_button_up(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseButtonEvent*)) {
    win->on_mouse_button_up = handler;
}

void ui_on_mouse_motion(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseMotionEvent*)) {
    win->on_mouse_motion = handler;
}

void ui_on_mouse_wheel(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseWheelEvent*)) {
    win->on_mouse_wheel = handler;
}
