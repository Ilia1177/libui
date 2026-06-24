#include "ui_box.h"
#include "math.h"

// #include "libft.h"
ui_slider_data_t ui_slider_data(float max, float min, float def) {
	return (ui_slider_data_t) {
		max, min, def, def, false, NULL
	};
}


void	ui_bhook_slider_mousemotion(ui_box_t*slider, SDL_Event* e, void *d) {
	(void)d;
    ui_slider_data_t *s = (ui_slider_data_t *)slider->data;
	if (!s || !s->isdragging)
		return;
	int mx = e->motion.x;

	float t = (float)(mx - slider->area.x)
			/ (float)slider->area.w;

	t = clampf(t, 0.0f, 1.0f);
	s->current_value =
		s->min_value
		+ t * (s->max_value - s->min_value);
}

void	ui_bhook_slider_clickdown(ui_box_t*slider, SDL_Event* e, void *d)
{
	(void)d;

    ui_slider_data_t *s = (ui_slider_data_t *)slider->data;
    if (!s)
        return;
	int mx = e->button.x;
	int my = e->button.y;
	if (mx >= slider->area.x &&
		mx <= slider->area.x + slider->area.w &&
		my >= slider->area.y &&
		my <= slider->area.y + slider->area.h)
	{
		s->isdragging = true;
	}
}
void	ui_bhook_slider_destroy(ui_box_t*slider, SDL_Event* e, void *d)
{
	(void)e;
	(void)d;
    ui_slider_data_t *s = (ui_slider_data_t *)slider->data;
	if(s) {
		if(s->target)
			free(s->target);
		free(s);
	}
	printf("Destroy slider\n");
	fflush(stdout);
}

void	ui_bhook_slider_clickup(ui_box_t*slider, SDL_Event* e, void *d)
{
	(void)e;
	(void)d;
    ui_slider_data_t *s = (ui_slider_data_t *)slider->data;
	s->isdragging = false;
	float* target = s->target;
	if(target)
		*target = s->current_value;
}

void ui_bhook_drawslider(ui_box_t* slider, SDL_Event* e, void* data)
{
    (void)data;
	(void)e;
    if (!slider)
        return;

    SDL_Renderer *r = slider->parent_window->renderer;
    ui_slider_data_t *s = (ui_slider_data_t *)slider->data;

    if (!s)
        return;

    /*
    ** ------------------------
    ** DRAW TRACK
    ** ------------------------
    */

    SDL_Rect track = {
        slider->area.x,
        slider->area.y + slider->area.h / 2 - 3,
        slider->area.w,
        6
    };

    SDL_SetRenderDrawColor(r, 70, 70, 70, 255);
    SDL_RenderFillRect(r, &track);

    /*
    ** ------------------------
    ** DRAW KNOB
    ** ------------------------
    */

    float t =
        (s->current_value - s->min_value)
        / (s->max_value - s->min_value);

    int knob_x = slider->area.x + (int)(t * slider->area.w);

    SDL_Rect knob = {
        knob_x - 6,
        slider->area.y,
        12,
        slider->area.h
    };

    SDL_SetRenderDrawColor(r, 220, 220, 220, 255);
    SDL_RenderFillRect(r, &knob);
}

ui_box_t* ui_belem_slider(ui_win_t* win, ui_slider_data_t parameters)
{
    ui_box_t *slider;


    slider = ui_box_create(win, 0, win->colors[2]);
    slider->area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);

    slider->data = calloc(1, sizeof(ui_slider_data_t));

    ui_slider_data_t *slider_data =
        (ui_slider_data_t *)slider->data;

    *slider_data = parameters;

    slider_data->current_value =
        slider_data->default_value;

	ui_bhook_replace(slider->render, ui_bhook_drawbox, ui_bhook_drawslider);
	// ui_bhook_remove(&slider->render, ui_bhook_drawbox);
    // ui_bhook_prepend(&slider->render, ui_bhook_drawslider);
    // ui_bhook_prepend(&slider->update, ui_bhook_keydownslider);
    ui_bhook_append(&slider->on_mouse_motion, ui_bhook_slider_mousemotion);
    ui_bhook_append(&slider->on_click_down, ui_bhook_slider_clickdown);
    ui_bhook_append(&slider->on_click_up, ui_bhook_slider_clickup);
    ui_bhook_prepend(&slider->destroy, ui_bhook_slider_destroy);
    return slider;
}
