#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include "ui_box.h"

/*  -------------------
 *	|                 |
 *	|
 *	|
 *	|
 *	|
 *	-------------------
 *
 *
 *
 *
 *
 * */

#define UI_LAYOUT_ALIGN_X_MASK (UI_LAYOUT_ALIGN_LEFT | UI_LAYOUT_ALIGN_RIGHT | UI_LAYOUT_ALIGN_CENTER_X)
#define UI_LAYOUT_ALIGN_Y_MASK (UI_LAYOUT_ALIGN_TOP | UI_LAYOUT_ALIGN_BOTTOM | UI_LAYOUT_ALIGN_CENTER_Y)
#define UI_LAYOUT_DIR_MASK     (UI_LAYOUT_DIR_ROW | UI_LAYOUT_DIR_COL)
#define UI_LAYOUT_SPACE_MASK   (UI_LAYOUT_SPACE_BETWEEN | UI_LAYOUT_SPACE_AROUND)
#define UI_LAYOUT_CONTENT_X_MASK (UI_LAYOUT_CONTENT_ALIGN_LEFT | UI_LAYOUT_CONTENT_ALIGN_RIGHT | UI_LAYOUT_CONTENT_ALIGN_CENTER_X)
#define UI_LAYOUT_CONTENT_Y_MASK (UI_LAYOUT_CONTENT_ALIGN_TOP | UI_LAYOUT_CONTENT_ALIGN_BOTTOM | UI_LAYOUT_CONTENT_ALIGN_CENTER_Y)


#define UI_LAYOUT_ABSOLUTE (1 << 16)

// Horizontal alignment (mutually exclusive per axis)
#define UI_LAYOUT_ALIGN_LEFT     (1 << 0) // ok!
#define UI_LAYOUT_ALIGN_RIGHT    (1 << 1) // ok!
#define UI_LAYOUT_ALIGN_CENTER_X (1 << 2) // ok!

// Vertical alignment (mutually exclusive per axis)
#define UI_LAYOUT_ALIGN_TOP      (1 << 3) // ok!
#define UI_LAYOUT_ALIGN_BOTTOM   (1 << 4) // ok!
#define UI_LAYOUT_ALIGN_CENTER_Y (1 << 5) // ok!

// Sizing behavior
#define UI_LAYOUT_FILL_X      (1 << 6) // ok...
#define UI_LAYOUT_FILL_Y      (1 << 7) // ok...
#define UI_LAYOUT_FIT_CONTENT (1 << 8) // ok...

// Child distribution (when layout direction is row/column)
#define UI_LAYOUT_SPACE_BETWEEN (1 << 9) // buggy
#define UI_LAYOUT_SPACE_AROUND  (1 << 10) // not tested

// Overflow
#define UI_LAYOUT_WRAP          (1 << 11) // not tested
#define UI_LAYOUT_CLIP          (1 << 12) // not tested
#define UI_LAYOUT_SCROLL_X      (1 << 13) // not tested
#define UI_LAYOUT_SCROLL_Y      (1 << 14) // not tested

// Render order (allways on top)
#define UI_LAYOUT_OVERLAY (1 << 20) // ok!

// Direction (how children are arranged)
#define UI_LAYOUT_DIR_ROW (1 << 17) // ok!
#define UI_LAYOUT_DIR_COL (1 << 18) // ok!

// Child position relative to parent
// children start after parent (below for col, right for row)
#define UI_LAYOUT_BELOW (1 << 19) // ok...

// Child sizing expand to fill remaining vertical/horizontal space of parent box
#define UI_LAYOUT_GROW_X (1 << 22) // ok...
#define UI_LAYOUT_GROW_Y (1 << 23) // ok...


#define UI_LAYOUT_CONTENT_ALIGN_LEFT     (1 << 24) // not tested
#define UI_LAYOUT_CONTENT_ALIGN_RIGHT    (1 << 25) // not tested
#define UI_LAYOUT_CONTENT_ALIGN_CENTER_X (1 << 26) // not tested


#define UI_LAYOUT_CONTENT_ALIGN_TOP      (1 << 27) // not tested
#define UI_LAYOUT_CONTENT_ALIGN_BOTTOM   (1 << 28) // not tested
#define UI_LAYOUT_CONTENT_ALIGN_CENTER_Y (1 << 29) // not tested
// Misc

#define UI_LAYOUT_DIRTY (1 << 31)

typedef struct ui_win_s ui_win_t;
void                    ui_layout_pass(ui_win_t* win);
ui_box_t*               ui_layout_menu(ui_box_t* menu);

#endif
