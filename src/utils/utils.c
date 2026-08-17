#include "libui.h"

void ui_padding(ui_box_t* b, int left, int right, int top, int bottom)
{
	if (!b)
		return;

	b->padding.left = left;
	b->padding.right = right;
	b->padding.top = top;
	b->padding.bottom = bottom;
}

void ui_layout_set(ui_box_t *b, uint32_t flag, bool set)
{
	if (!b)
		return;
	if (!set) {
		b->layout &= ~flag;
		return;
	}
	if (flag & UI_LAYOUT_DIR_MASK)     b->layout &= ~UI_LAYOUT_DIR_MASK;
	if (flag & UI_LAYOUT_ALIGN_X_MASK) b->layout &= ~UI_LAYOUT_ALIGN_X_MASK;
	if (flag & UI_LAYOUT_ALIGN_Y_MASK) b->layout &= ~UI_LAYOUT_ALIGN_Y_MASK;
	if (flag & UI_LAYOUT_SPACE_MASK)   b->layout &= ~UI_LAYOUT_SPACE_MASK;
	if (flag & UI_LAYOUT_CONTENT_X_MASK) b->layout &= ~UI_LAYOUT_CONTENT_X_MASK;
	if (flag & UI_LAYOUT_CONTENT_Y_MASK) b->layout &= ~UI_LAYOUT_CONTENT_Y_MASK;
	b->layout |= flag;
}
