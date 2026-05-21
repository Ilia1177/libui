#include "ui_tool.h"
#include "ui_global.h"
#include "ui_box.h"

void ui_tool_init(ui_tool_t *tool, const char *name, void *data) {
	printf("init tool %s\n", name);
	fflush(stdout);
    tool->name = name;
    tool->data = data;
    tool->on_activate = NULL;
    tool->on_deactivate = NULL;
    tool->on_click_down = NULL;
    tool->on_click_up = NULL;
    tool->on_mouse_motion = NULL;
    tool->on_key_down = NULL;
    tool->on_update = NULL;
    tool->on_render = NULL;
    tool->on_cleanup = NULL;
}

static void bridge_click_down(ui_box_t *box, SDL_Event *e, void *unused) {
    (void)unused;
	printf("bridge click down\n");
	fflush(stdout);
    ui_tool_t *t = box->parent_window->global->active_tool;
    if (t && t->on_click_down) 
		t->on_click_down(box, e, t->data);
}

static void bridge_click_up(ui_box_t *box, SDL_Event *e, void *unused) {
    (void)unused;
    ui_tool_t *t = box->parent_window->global->active_tool;
    if (t && t->on_click_up) 
		t->on_click_up(box, e, t->data);
}

static void bridge_mouse_motion(ui_box_t *box, SDL_Event *e, void *unused) {
    (void)unused;
    ui_tool_t *t = box->parent_window->global->active_tool;
    if (t && t->on_mouse_motion) 
		t->on_mouse_motion(box, e, t->data);
}

static void bridge_key_down(ui_box_t *box, SDL_Event *e, void *unused) {
    (void)unused;
    ui_tool_t *t = box->parent_window->global->active_tool;
    if (t && t->on_key_down) 
		t->on_key_down(box, e, t->data);
}

static void bridge_update(ui_box_t *box, SDL_Event *e, void *unused) {
    (void)unused;
    (void)e;
    ui_tool_t *t = box->parent_window->global->active_tool;
    if (t && t->on_update) 
		t->on_update(box, t->data);
}

static void bridge_render(ui_box_t *box, SDL_Event *e, void *unused) {
    (void)unused;
    (void)e;
    ui_tool_t *t = box->parent_window->global->active_tool;
    if (t && t->on_render) 
		t->on_render(box, box->parent_window->renderer, t->data);
}

void ui_tool_activate(ui_globalApp_t *app, ui_tool_t *tool, ui_box_t *canvas)
{
	(void)app;
	(void)tool;
	(void)canvas;
	printf("tool activate\n");
	fflush(stdout);
	if (!app || !tool || !canvas)
		return;
	if (app->active_tool)
	   ui_tool_deactivate(app, canvas);
	app->active_tool = tool;
   if (tool->on_activate)
	   tool->on_activate(canvas, tool->data);
	printf("Clickdown of %s tool is: %p\n", tool->name, tool->on_click_down);
	fflush(stdout);
	if (tool->on_click_down) {
		printf("add clickup bridge\n");
		fflush(stdout);
	    ui_bhook_append(&canvas->on_click_down, bridge_click_down);
	}
	if (tool->on_click_up) {
	    ui_bhook_append(&canvas->on_click_up, bridge_click_up);
	}
	if (tool->on_mouse_motion)
		ui_bhook_append(&canvas->on_mouse_motion, bridge_mouse_motion);
	if (tool->on_key_down)
		ui_bhook_append(&canvas->on_key_down, bridge_key_down);
	if (tool->on_update)
		ui_bhook_append(&canvas->update, bridge_update);
	if (tool->on_render)
		ui_bhook_append(&canvas->render, bridge_render);
	// ui_boxhandler_t* curr = canvas->on_click_down;
	// while(curr) {
	// 	printf("box handler click down is : %p\n", curr->fn);
	// 	fflush(stdout);
	// 	curr = curr->next;
	// }
	printf("end tool activate\n");
	fflush(stdout);
}

void ui_tool_deactivate(ui_globalApp_t *app, ui_box_t *canvas) {
    if (!app || !app->active_tool || !canvas) return;

	printf("deactivate tool\n");
	fflush(stdout);
    ui_tool_t *tool = app->active_tool;

    ui_bhook_remove(&canvas->on_click_down, bridge_click_down);
    ui_bhook_remove(&canvas->on_click_up, bridge_click_up);
    ui_bhook_remove(&canvas->on_mouse_motion, bridge_mouse_motion);
    ui_bhook_remove(&canvas->on_key_down, bridge_key_down);
    ui_bhook_remove(&canvas->update, bridge_update);
    ui_bhook_remove(&canvas->render, bridge_render);

    if (tool->on_deactivate)
        tool->on_deactivate(canvas, tool->data);

    if (tool->on_cleanup)
        tool->on_cleanup(tool->data);

    app->active_tool = NULL;
}
