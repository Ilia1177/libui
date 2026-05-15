#include "ui_box.h"
#include "libft.h"

void ui_bhook_keydownslider(ui_box_t* slider, SDL_Event* e, void* data) 
{
	(void)data;
    ui_slider_data_t *s = (ui_slider_data_t *)slider->data;
    if (!s)
        return;
    if (e)
    {
        if (e->type == SDL_MOUSEBUTTONDOWN)
        {
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
        else if (e->type == SDL_MOUSEBUTTONUP)
        {
            s->isdragging = false;
        }
        else if (e->type == SDL_MOUSEMOTION && s->isdragging)
        {
            int mx = e->motion.x;

            float t = (float)(mx - slider->area.x)
                    / (float)slider->area.w;

            t = ft_clampf(t, 0.0f, 1.0f);

            s->current_value =
                s->min_value
                + t * (s->max_value - s->min_value);
        }
    }
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
    // if (e)
    // {
    //     if (e->type == SDL_MOUSEBUTTONDOWN)
    //     {
    //         int mx = e->button.x;
    //         int my = e->button.y;
    //
    //         if (mx >= slider->area.x &&
    //             mx <= slider->area.x + slider->area.w &&
    //             my >= slider->area.y &&
    //             my <= slider->area.y + slider->area.h)
    //         {
    //             s->isdragging = true;
    //         }
    //     }
    //     else if (e->type == SDL_MOUSEBUTTONUP)
    //     {
    //         s->isdragging = false;
    //     }
    //     else if (e->type == SDL_MOUSEMOTION && s->isdragging)
    //     {
    //         int mx = e->motion.x;
    //
    //         float t = (float)(mx - slider->area.x)
    //                 / (float)slider->area.w;
    //
    //         t = ft_clampf(t, 0.0f, 1.0f);
    //
    //         s->current_value =
    //             s->min_value
    //             + t * (s->max_value - s->min_value);
    //     }
    // }

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
    SDL_Rect area;

    area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);

    slider = ui_box_create(win, area, win->colors[2]);

    slider->data = ft_calloc(1, sizeof(ui_slider_data_t));

    ui_slider_data_t *slider_data =
        (ui_slider_data_t *)slider->data;

    *slider_data = parameters;

    slider_data->current_value =
        slider_data->default_value;

	ui_bhook_remove(&slider->render, ui_bhook_drawbox);
    ui_bhook_add(&slider->render, ui_bhook_drawslider);
    ui_bhook_add(&slider->update, ui_bhook_keydownslider);

    return slider;
}
