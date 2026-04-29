#ifndef LIBUI_H
# define LIBUI_H
# include <SDL2/SDL_image.h>
# include <SDL2/SDL.h>

typedef struct ui_win_s {
	SDL_Renderer* renderer;
	SDL_Window* win;
	SDL_Texture* texture;
	ssize_t h;
	ssize_t w;
	char* title;
    int quit_flag; // New: Flag to signal UI loop to quit

	void(*destroy)(struct ui_win_s*);
	void(*resize)(struct ui_win_s*, int w, int h);
	void(*render)(struct ui_win_s*);

    // New: Event handler function pointers
    void(*on_key_down)(struct ui_win_s*, SDL_KeyboardEvent*);
    void(*on_key_up)(struct ui_win_s*, SDL_KeyboardEvent*);
    void(*on_mouse_button_down)(struct ui_win_s*, SDL_MouseButtonEvent*);
    void(*on_mouse_button_up)(struct ui_win_s*, SDL_MouseButtonEvent*);
    void(*on_mouse_motion)(struct ui_win_s*, SDL_MouseMotionEvent*);
    void(*on_mouse_wheel)(struct ui_win_s*, SDL_MouseWheelEvent*);
	
	struct ui_win_s *next;
} ui_win_t;

typedef struct ui_motor_s {
	ui_win_t *win;
} ui_motor_t;

int ui_init();
int ui_quit();
void ui_run(ui_win_t*);

void	ui_render(ui_win_t* win);
void	ui_winpos(ui_win_t* win, int x, int y);
ui_win_t* ui_winCreate(int w, int h, char* title);
void ui_winsize(ui_win_t *win, int w, int h);
void ui_destroy(ui_win_t* win);

// New: Event registration functions
void ui_on_key_down(ui_win_t* win, void(*handler)(ui_win_t*, SDL_KeyboardEvent*));
void ui_on_key_up(ui_win_t* win, void(*handler)(ui_win_t*, SDL_KeyboardEvent*));
void ui_on_mouse_button_down(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseButtonEvent*));
void ui_on_mouse_button_up(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseButtonEvent*));
void ui_on_mouse_motion(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseMotionEvent*));
void ui_on_mouse_wheel(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseWheelEvent*));

#endif
