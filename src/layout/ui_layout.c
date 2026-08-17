#include "ui_box.h"
#include "ui_win.h"
#include "ui_layout.h"

void arrange_row(ui_box_t *parent)
{
	uint32_t pl = parent->layout;
	ui_box_t *child;

	padding_t pad = parent->padding;
	int visible = 0;
	int fixed_w = 0;
	int fill_x_count = 0;
	pad.left += parent->border;
	pad.top += parent->border;
	pad.bottom += parent->border;
	pad.right += parent->border;
	child = parent->childs;
	while (child) {
		if (!(child->state & BOX_HIDDEN)) {
			visible++;
			if (child->layout & UI_LAYOUT_FILL_X) {
				fill_x_count++;
			} else {
				fixed_w += child->area.w;
			}
		}
		child = child->next;
	}

	if (visible == 0)
		return;

	int parent_w = parent->area.w;
	int gap = 0;
	int start_offset = 0;

	if (fill_x_count > 0) {
		int remaining = parent_w - fixed_w;
		if (remaining < 0)
			remaining = 0;
		int fill_w = remaining / fill_x_count;
		child = parent->childs;
		while (child) {
			if (!(child->state & BOX_HIDDEN) && (child->layout & UI_LAYOUT_FILL_X))
				child->area.w = fill_w;
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
		if (child->state & BOX_HIDDEN) {
			child = child->next;
			continue;
		}

		uint32_t cl = child->layout;

		child->area.x = x + pad.left;

		if (cl & UI_LAYOUT_FILL_Y) {
			child->area.y = parent->area.y + pad.top;
			child->area.h = parent->area.h - (pad.top + pad.bottom);
		} else if (cl & UI_LAYOUT_ALIGN_BOTTOM) {
			child->area.y = parent->area.y + parent->area.h - child->area.h;
			child->area.y += pad.top;
		} else if (cl & UI_LAYOUT_ALIGN_CENTER_Y) {
			child->area.y = parent->area.y + (parent->area.h - child->area.h) / 2;
			child->area.y += pad.top;
		} else {
			child->area.y = parent->area.y;
			child->area.y += pad.top;
		}
		x += child->area.w + gap;
		child = child->next;
	}
}

void arrange_col(ui_box_t *parent)
{
	uint32_t pl = parent->layout;
	ui_box_t *child;

	int visible = 0;
	int fixed_h = 0;
	int fill_y_count = 0;
	padding_t pad = parent->padding;
	pad.left += parent->border;
	pad.top += parent->border;
	pad.bottom += parent->border;
	pad.right += parent->border;
	child = parent->childs;
	// mesure la hauteur total des enfants
	while (child) {
		if (!(child->state & BOX_HIDDEN)) {
			visible++;
			if (child->layout & UI_LAYOUT_FILL_Y)
				fill_y_count++;
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

	if (fill_y_count > 0) {
		int remaining = parent_h - fixed_h;
		if (remaining < 0)
			remaining = 0;
		int fill_h = remaining / fill_y_count;
		child = parent->childs;
		while (child) {
			if (!(child->state & BOX_HIDDEN) && (child->layout & UI_LAYOUT_FILL_Y))
				child->area.h = fill_h;
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
		if (child->state & BOX_HIDDEN) {
			child = child->next;
			continue;
		}

		uint32_t cl = child->layout;

		child->area.y = y + pad.top;

		if (cl & UI_LAYOUT_FILL_X) {
			child->area.x = parent->area.x + pad.left;
			child->area.w = parent->area.w - (pad.left + pad.right);
		} else if (cl & UI_LAYOUT_ALIGN_RIGHT) {
			child->area.x = parent->area.x + parent->area.w - child->area.w;
			child->area.x += pad.left;
		} else if (cl & UI_LAYOUT_ALIGN_CENTER_X) {
			child->area.x = parent->area.x + (parent->area.w - child->area.w) / 2;
			child->area.x += pad.left;
		} else {
			child->area.x = parent->area.x;
			child->area.x += pad.left;
		}
		y += child->area.h + gap;
		child = child->next;
	}
}

void apply_content_align(ui_box_t *b)
{
	uint32_t l = b->layout;
	bool h_set = l & (UI_LAYOUT_CONTENT_ALIGN_LEFT | UI_LAYOUT_CONTENT_ALIGN_RIGHT | UI_LAYOUT_CONTENT_ALIGN_CENTER_X);
	bool v_set = l & (UI_LAYOUT_CONTENT_ALIGN_TOP | UI_LAYOUT_CONTENT_ALIGN_BOTTOM | UI_LAYOUT_CONTENT_ALIGN_CENTER_Y);

	if (!h_set && !v_set)
        return; // no alignment flags — leave layers untouched
	ui_layer_t *layer = b->layers;
	while (layer) {
		if (!h_set || (l & UI_LAYOUT_CONTENT_ALIGN_CENTER_X))
			layer->area.x = (b->area.w - layer->area.w) / 2;
		else if (l & UI_LAYOUT_CONTENT_ALIGN_RIGHT)
			layer->area.x = b->area.w - layer->area.w;
		else if (l & UI_LAYOUT_CONTENT_ALIGN_LEFT)
			layer->area.x = 0;

		if (!v_set || (l & UI_LAYOUT_CONTENT_ALIGN_CENTER_Y))
			layer->area.y = (b->area.h - layer->area.h) / 2;
		else if (l & UI_LAYOUT_CONTENT_ALIGN_BOTTOM)
			layer->area.y = b->area.h - layer->area.h;
		else if (l & UI_LAYOUT_CONTENT_ALIGN_TOP)
			layer->area.y = 0;

		layer = layer->next;
	}
}

void apply_sizing(ui_box_t *b)
{
	uint32_t l = b->layout;
	if (!(l & UI_LAYOUT_FIT_CONTENT))
		return;

	int need_w = 0;
	int need_h = 0;
	bool row = l & UI_LAYOUT_DIR_ROW;
	bool col = l & UI_LAYOUT_DIR_COL;
	ui_box_t *child = b->childs;
	while (child) {
		if (!(child->state & BOX_HIDDEN)) {
			int cw = child->area.w;
			int ch = child->area.h;
			if (row) {
				if (!(child->layout & UI_LAYOUT_FILL_X))
					need_w += cw;
				if (ch > need_h)
					need_h = ch;
			} else if (col) {
				if (!(child->layout & UI_LAYOUT_FILL_Y))
					need_h += ch;
				if (cw > need_w)
					need_w = cw;
			} else {
				if (cw > need_w)
					need_w = cw;
				if (ch > need_h)
					need_h = ch;
			}
		}
		child = child->next;
	}
	ui_layer_t *layer = b->layers;
	while (layer) {
		int lw = layer->area.w;
		int lh = layer->area.h;
		if (lw > need_w)
			need_w = lw;
		if (lh > need_h)
			need_h = lh;
		layer = layer->next;
	}
	if (!(l & UI_LAYOUT_FILL_X))
		b->area.w = need_w + b->padding.left + b->padding.right + 2 * b->border;
	if (!(l & UI_LAYOUT_FILL_Y))
		b->area.h = need_h + b->padding.top + b->padding.bottom + 2 * b->border;
}

static void layout_one_box(ui_box_t *b)
{
	if (!b || (b->state & BOX_HIDDEN))
		return;

	SDL_Rect container; 
	ui_box_t* parent;

	parent = b->parent;
	padding_t pad = {};
	if (parent) {
		container = parent->area;
		pad = parent->padding;
		pad.left += parent->border;
		pad.right += parent->border;
		pad.top += parent->border;
		pad.bottom += parent->border;
	} else {
		container = b->parent_window->area;
		container.x = 0;
		container.y = 0;
	}
	uint32_t layout = b->layout;
	if (!(layout & UI_LAYOUT_ABSOLUTE)) {
		bool parent_row = parent && (parent->layout & UI_LAYOUT_DIR_ROW);
		bool parent_col = parent && (parent->layout & UI_LAYOUT_DIR_COL);

		if (layout & UI_LAYOUT_FILL_X && !parent_row) {
			b->area.x = container.x + pad.left;
			b->area.w = container.w - (pad.left + pad.right);
		}
		if (layout & UI_LAYOUT_FILL_Y && !parent_col) {
			b->area.y = container.y + pad.top;
			b->area.h = container.h - (pad.top + pad.bottom);
		}

		if (!parent_row) {
			if (layout & UI_LAYOUT_ALIGN_LEFT)
				b->area.x = container.x + pad.left;
			else if (layout & UI_LAYOUT_ALIGN_RIGHT)
				b->area.x = container.x + (container.w - pad.right) - b->area.w;
			else if (layout & UI_LAYOUT_ALIGN_CENTER_X) // padding has no effect
				b->area.x = container.x + (container.w - b->area.w) / 2;
		}

		if (!parent_col) {
			if (layout & UI_LAYOUT_ALIGN_TOP)
				b->area.y = container.y + pad.top;
			else if (layout & UI_LAYOUT_ALIGN_BOTTOM)
				b->area.y = container.y + (container.h - pad.bottom) - b->area.h;
			else if (layout & UI_LAYOUT_ALIGN_CENTER_Y) // padding has no effect
				b->area.y = container.y + (container.h - b->area.h) / 2;
		}
	}

	apply_sizing(b);
	if (layout & UI_LAYOUT_DIR_ROW)
		arrange_row(b);
	else if (layout & UI_LAYOUT_DIR_COL)
		arrange_col(b);

	b->layout &= ~UI_LAYOUT_DIRTY;
	ui_box_t *child = b->childs;
	while (child) {
		layout_one_box(child);
		child = child->next;
	}
}

void ui_layout_pass(ui_win_t *win)
{
	ui_box_t *b;

	if (!win) {
		return;
	}
	b = win->boxes;
	while (b) {
		layout_one_box(b);
		b = b->next;
	}
}
