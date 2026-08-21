#include "libui.h"

frame_t ui_frame(int left, int right, int top, int bottom)
{
	frame_t frame;

	frame.left = left;
	frame.right = right;
	frame.top = top;
	frame.bottom = bottom;
	return frame;
}
frame_t ui_frame_add(const frame_t f1, const frame_t f2)
{
	frame_t res;

	res.left = f1.left + f2.left;
	res.right = f1.right + f2.right;
	res.top = f1.top + f2.top;
	res.bottom = f1.bottom + f2.bottom;
	return res;
}
