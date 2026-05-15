#include "libui.h" // Should pull in SDL stuff, ui_win.h, ui_box.h, ui_elem.h
#include "libft.h" // For ft_calloc
#include <math.h> // For roundf

// Structure to hold slider-specific data (moved to top)
typedef struct s_ui_slider_data {
    float           min_value;
    float           max_value;
    float           step; // Optional: for discrete steps
    ui_bhook_fn_t   on_value_change; // Callback for when value changes
    bool            is_dragging;
} ui_slider_data_t;

// Simple utility function to check if a point is within a rectangle
static bool ui_point_in_rect(int x, int y, SDL_Rect rect) {
    return x >= rect.x && x < rect.x + rect.w &&
           y >= rect.y && y < rect.y + rect.h;
}

// Hook to handle mouse button down on the slider
static void ui_bhook_slider_mouse_down(ui_box_t* box, SDL_Event* e, void* data) {
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        if (ui_point_in_rect(e->button.x, e->button.y, box->area)) {
            ui_slider_data_t* slider_data = (ui_slider_data_t*)data;
            if (slider_data) {
                slider_data->is_dragging = true;
                // Update value immediately on click
                float ratio = (float)(e->button.x - box->area.x) / box->area.w;
                box->value = slider_data->min_value + ratio * (slider_data->max_value - slider_data->min_value);
                if (slider_data->step > 0) {
                    box->value = roundf(box->value / slider_data->step) * slider_data->step;
                }
                if (slider_data->on_value_change) {
                    slider_data->on_value_change(box, e, data);
                }
            }
        }
    }
}

// Hook to handle mouse motion (dragging)
static void ui_bhook_slider_mouse_motion(ui_box_t* box, SDL_Event* e, void* data) {
    ui_slider_data_t* slider_data = (ui_slider_data_t*)data;
    if (slider_data && slider_data->is_dragging && e->type == SDL_MOUSEMOTION) {
        float new_x = (float)e->motion.x;
        if (new_x < box->area.x) new_x = box->area.x;
        if (new_x > box->area.x + box->area.w) new_x = box->area.x + box->area.w;

        float ratio = (new_x - box->area.x) / box->area.w;
        box->value = slider_data->min_value + ratio * (slider_data->max_value - slider_data->min_value);
        if (slider_data->step > 0) {
            box->value = roundf(box->value / slider_data->step) * slider_data->step;
                }

        // Clamp value to min/max
        if (box->value < slider_data->min_value) box->value = slider_data->min_value;
        if (box->value > slider_data->max_value) box->value = slider_data->max_value;

        if (slider_data->on_value_change) {
            slider_data->on_value_change(box, e, data);
        }
    }
}

// Hook to handle mouse button up (stop dragging)
static void ui_bhook_slider_mouse_up(ui_box_t* box, SDL_Event* e, void* data) {
    (void)box; // Mark as unused
    ui_slider_data_t* slider_data = (ui_slider_data_t*)data;
    if (slider_data && slider_data->is_dragging && e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT) {
        slider_data->is_dragging = false;
    }
}

// Hook to render the slider
static void ui_bhook_slider_render(ui_box_t* box, SDL_Event* e, void* data) {
    (void)e; // Mark as unused

    ui_slider_data_t* slider_data = (ui_slider_data_t*)data;
    if (!slider_data) return;

    // Draw the slider track
    SDL_SetRenderDrawColor(box->parent_window->renderer, 50, 50, 50, 255); // Dark gray track
    SDL_RenderFillRect(box->parent_window->renderer, &box->area);

    // Calculate thumb position
    float value_ratio = (box->value - slider_data->min_value) / (slider_data->max_value - slider_data->min_value);
    int thumb_x = box->area.x + (int)(value_ratio * box->area.w);
    int thumb_width = 10; // Fixed thumb width
    SDL_Rect thumb_rect = {
        thumb_x - (thumb_width / 2),
        box->area.y,
        thumb_width,
        box->area.h
    };

    // Draw the thumb
    SDL_SetRenderDrawColor(box->parent_window->renderer, 0, 120, 215, 255); // Blue thumb
    SDL_RenderFillRect(box->parent_window->renderer, &thumb_rect);

    // Draw value label
    if (box->label) {
        char value_str[32];
        snprintf(value_str, sizeof(value_str), "%s: %.2f", box->label, box->value);
        SDL_Texture* value_texture = ui_tool_text2texture(box->parent_window, value_str, COLOR_WHITE);
        if (value_texture) {
            int text_w, text_h;
            SDL_QueryTexture(value_texture, NULL, NULL, &text_w, &text_h);
            SDL_Rect text_rect = {
                box->area.x + (box->area.w - text_w) / 2, // Centered
                box->area.y + box->area.h + 5, // Below the slider
                text_w,
                text_h
            };
            SDL_RenderCopy(box->parent_window->renderer, value_texture, NULL, &text_rect);
            SDL_DestroyTexture(value_texture);
        }
    }
}

// Public function to create a slider UI element
ui_box_t* ui_belem_slider(ui_win_t* win, const char* label, float min_val, float max_val, float initial_val, float step, ui_bhook_fn_t on_change_fn) {
    ui_box_t* slider_box;
    SDL_Rect area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H / 2); // Slider can be thinner

    slider_box = ui_box_create(win, area, (SDL_Color){0,0,0,0}); // Transparent background for the box itself

    // Allocate and initialize slider data
    ui_slider_data_t* slider_data = (ui_slider_data_t*)calloc(1, sizeof(ui_slider_data_t));
    slider_data->min_value = min_val;
    slider_data->max_value = max_val;
    slider_data->step = step;
    slider_data->on_value_change = on_change_fn;
    slider_data->is_dragging = false;

    // Set initial value, clamped to range
    slider_box->value = initial_val;
    if (slider_box->value < min_val) slider_box->value = min_val;
    if (slider_box->value > max_val) slider_box->value = max_val;

    slider_box->label = label; // Use the label field for the slider's name

    // Add hooks
    ui_bhook_add(&slider_box->on_click_down, ui_bhook_slider_mouse_down);
    ui_bhook_add(&slider_box->on_mouse_motion, ui_bhook_slider_mouse_motion);
    ui_bhook_add(&slider_box->on_click_up, ui_bhook_slider_mouse_up);
    // Add the custom render hook
    ui_bhook_add(&slider_box->render, ui_bhook_slider_render);
    // Pass slider_data to all hooks
    ui_boxhandler_t* current_hook = slider_box->on_click_down;
    while(current_hook) { 
		current_hook->data = slider_data; 
		current_hook = current_hook->next; 
	}
    current_hook = slider_box->on_mouse_motion;
    while(current_hook) { current_hook->data = slider_data; current_hook = current_hook->next; }
    current_hook = slider_box->on_click_up;
    while(current_hook) { current_hook->data = slider_data; current_hook = current_hook->next; }
    current_hook = slider_box->render;
    while(current_hook) { current_hook->data = slider_data; current_hook = current_hook->next; }


    return slider_box;
}

// TODO: Add a destroy hook to free slider_data
