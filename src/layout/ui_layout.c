#include "ui_box.h"
#include "ui_win.h"
#include "ui_layout.h"

static void arrange_row(ui_box_t *parent)
{
	uint32_t pl = parent->layout;
	ui_box_t *child;

	int visible = 0;
	int fixed_w = 0;
	int grow_x_count = 0;
	child = parent->childs;
	while (child) {
		if (!(child->flags & BOX_HIDDEN)) {
			visible++;
			if (child->layout & UI_LAYOUT_GROW_X)
				grow_x_count++;
			else
				fixed_w += child->area.w;
		}
		child = child->next;
	}

	if (visible == 0)
		return;

	int parent_w = parent->area.w;
	int gap = 0;
	int start_offset = 0;

	if (grow_x_count > 0) {
		int remaining = parent_w - fixed_w;
		if (remaining < 0)
			remaining = 0;
		int grow_w = remaining / grow_x_count;
		child = parent->childs;
		while (child) {
			if (!(child->flags & BOX_HIDDEN) && (child->layout & UI_LAYOUT_GROW_X))
				child->area.w = grow_w;
			child = child->next;
		}
	} else {
		bool space_between = pl & UI_LAYOUT_SPACE_BETWEEN;
		bool space_around = pl & UI_LAYOUT_SPACE_AROUND;
		int total_w = fixed_w;
		if (space_between && visible > 1)
			gap = (parent_w - total_w) / (visible - 1);
		else if (space_around) {
			gap = (parent_w - total_w) / visible;
			start_offset = gap / 2;
		}
	}

	int x = (pl & UI_LAYOUT_BELOW)
		? (parent->area.x + parent->area.w + start_offset)
		: (parent->area.x + start_offset);
	child = parent->childs;
	while (child) {
		if (child->flags & BOX_HIDDEN) {
			child = child->next;
			continue;
		}

		uint32_t cl = child->layout;

		child->area.x = x;

		if (cl & UI_LAYOUT_GROW_Y) {
			child->area.y = parent->area.y;
			child->area.h = parent->area.h;
		} else if (cl & UI_LAYOUT_ALIGN_BOTTOM)
			child->area.y = parent->area.y + parent->area.h - child->area.h;
		else if (cl & UI_LAYOUT_ALIGN_CENTER_Y)
			child->area.y = parent->area.y + (parent->area.h - child->area.h) / 2;
		else
			child->area.y = parent->area.y;

		x += child->area.w + gap;
		child = child->next;
	}
}

static void arrange_col(ui_box_t *parent)
{
	uint32_t pl = parent->layout;
	ui_box_t *child;

	int visible = 0;
	int fixed_h = 0;
	int grow_y_count = 0;
	child = parent->childs;
	while (child) {
		if (!(child->flags & BOX_HIDDEN)) {
			visible++;
			if (child->layout & UI_LAYOUT_GROW_Y)
				grow_y_count++;
			else
				fixed_h += child->area.h;
		}
		child = child->next;
	}

	if (visible == 0)
		return;

	int parent_h = parent->area.h;
	int gap = 0;
	int start_offset = 0;

	if (grow_y_count > 0) {
		int remaining = parent_h - fixed_h;
		if (remaining < 0)
			remaining = 0;
		int grow_h = remaining / grow_y_count;
		child = parent->childs;
		while (child) {
			if (!(child->flags & BOX_HIDDEN) && (child->layout & UI_LAYOUT_GROW_Y))
				child->area.h = grow_h;
			child = child->next;
		}
	} else {
		bool space_between = pl & UI_LAYOUT_SPACE_BETWEEN;
		bool space_around = pl & UI_LAYOUT_SPACE_AROUND;
		int total_h = fixed_h;
		if (space_between && visible > 1)
			gap = (parent_h - total_h) / (visible - 1);
		else if (space_around) {
			gap = (parent_h - total_h) / visible;
			start_offset = gap / 2;
		}
	}

	int y = (pl & UI_LAYOUT_BELOW)
		? (parent->area.y + parent->area.h + start_offset)
		: (parent->area.y + start_offset);
	child = parent->childs;
	while (child) {
		if (child->flags & BOX_HIDDEN) {
			child = child->next;
			continue;
		}

		uint32_t cl = child->layout;

		child->area.y = y;

		if (cl & UI_LAYOUT_GROW_X) {
			child->area.x = parent->area.x;
			child->area.w = parent->area.w;
		} else if (cl & UI_LAYOUT_ALIGN_RIGHT)
			child->area.x = parent->area.x + parent->area.w - child->area.w;
		else if (cl & UI_LAYOUT_ALIGN_CENTER_X)
			child->area.x = parent->area.x + (parent->area.w - child->area.w) / 2;
		else
			child->area.x = parent->area.x;

		y += child->area.h + gap;
		child = child->next;
	}
}

static void apply_content_align(ui_box_t *b)
{
	uint32_t l = b->layout;
	bool h_set = l & (UI_LAYOUT_CONTENT_ALIGN_LEFT | UI_LAYOUT_CONTENT_ALIGN_RIGHT | UI_LAYOUT_CONTENT_ALIGN_CENTER_X);
	bool v_set = l & (UI_LAYOUT_CONTENT_ALIGN_TOP | UI_LAYOUT_CONTENT_ALIGN_BOTTOM | UI_LAYOUT_CONTENT_ALIGN_CENTER_Y);

	if (!h_set && !v_set)
        return; // no alignment flags — leave layers untouched
	ui_layer_t *layer = b->layers;
	while (layer) {
		if (!h_set || (l & UI_LAYOUT_CONTENT_ALIGN_CENTER_X))
			layer->area.x = b->area.x + (b->area.w - layer->area.w) / 2;
		else if (l & UI_LAYOUT_CONTENT_ALIGN_RIGHT)
			layer->area.x = b->area.x + b->area.w - layer->area.w;
		else if (l & UI_LAYOUT_CONTENT_ALIGN_LEFT)
			layer->area.x = b->area.x;

		if (!v_set || (l & UI_LAYOUT_CONTENT_ALIGN_CENTER_Y))
			layer->area.y = b->area.y + (b->area.h - layer->area.h) / 2;
		else if (l & UI_LAYOUT_CONTENT_ALIGN_BOTTOM)
			layer->area.y = b->area.y + b->area.h - layer->area.h;
		else if (l & UI_LAYOUT_CONTENT_ALIGN_TOP)
			layer->area.y = b->area.y;

		layer = layer->next;
	}
}

static void layout_one_box(ui_box_t *b)
{
	if (!b || (b->flags & BOX_HIDDEN))
		return;

	SDL_Rect container; 
	if (b->parent)
		container = b->parent->area;
	else {
		container = b->parent_window->area;
		container.x = 0;
		container.y = 0;
	}
	uint32_t layout = b->layout;
	if (!(layout & UI_LAYOUT_ABSOLUTE)) {
		bool parent_is_row = b->parent && (b->parent->layout & UI_LAYOUT_DIR_ROW);
		bool parent_is_col = b->parent && (b->parent->layout & UI_LAYOUT_DIR_COL);

		if (layout & UI_LAYOUT_FILL_X) {
			b->area.x = container.x;
			b->area.w = container.w;
		}
		if (layout & UI_LAYOUT_FILL_Y) {
			b->area.y = container.y;
			b->area.h = container.h;
		}

		if (!parent_is_row) {
			if (layout & UI_LAYOUT_ALIGN_LEFT)
				b->area.x = container.x;
			else if (layout & UI_LAYOUT_ALIGN_RIGHT)
				b->area.x = container.x + container.w - b->area.w;
			else if (layout & UI_LAYOUT_ALIGN_CENTER_X)
				b->area.x = container.x + (container.w - b->area.w) / 2;
		}

		if (!parent_is_col) {
			if (layout & UI_LAYOUT_ALIGN_TOP)
				b->area.y = container.y;
			else if (layout & UI_LAYOUT_ALIGN_BOTTOM)
				b->area.y = container.y + container.h - b->area.h;
			else if (layout & UI_LAYOUT_ALIGN_CENTER_Y)
				b->area.y = container.y + (container.h - b->area.h) / 2;
		}
	}

	if (layout & UI_LAYOUT_DIR_ROW)
		arrange_row(b);
	else if (layout & UI_LAYOUT_DIR_COL)
		arrange_col(b);

	apply_content_align(b);
	b->layout &= ~UI_LAYOUT_DIRTY;
	ui_box_t *child = b->childs;
	while (child) {
		layout_one_box(child);
		child = child->next;
	}
}

void ui_layout_pass(ui_win_t *win)
{
	if (!win)
		return;

	// SDL_Rect win_area = {0, 0, win->area.w, win->area.h};

	// if (win->canvas)
		// layout_one_box(win->canvas);
	ui_box_t *b = win->boxes;
	while (b) {
		layout_one_box(b);
		b = b->next;
	}
}
