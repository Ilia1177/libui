#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include "ui_box.h"

#define UI_LAYOUT_ABSOLUTE (1 << 16)
// Horizontal alignment (mutually exclusive per axis)
#define UI_LAYOUT_ALIGN_LEFT     (1 << 0)
#define UI_LAYOUT_ALIGN_RIGHT    (1 << 1)
#define UI_LAYOUT_ALIGN_CENTER_X (1 << 2)

// Vertical alignment (mutually exclusive per axis)
#define UI_LAYOUT_ALIGN_TOP      (1 << 3)
#define UI_LAYOUT_ALIGN_BOTTOM   (1 << 4)
#define UI_LAYOUT_ALIGN_CENTER_Y (1 << 5)

// Sizing behavior
#define UI_LAYOUT_FILL_X      (1 << 6)
#define UI_LAYOUT_FILL_Y      (1 << 7)
#define UI_LAYOUT_FIT_CONTENT (1 << 8)

// Child distribution (when layout direction is row/column)
#define UI_LAYOUT_SPACE_BETWEEN (1 << 9)
#define UI_LAYOUT_SPACE_AROUND  (1 << 10)

// Overflow
#define UI_LAYOUT_WRAP          (1 << 11) // not tested
#define UI_LAYOUT_CLIP          (1 << 12)
#define UI_LAYOUT_SCROLL_X      (1 << 13) // not tested
#define UI_LAYOUT_SCROLL_Y      (1 << 14) // not tested

// Auto-size parent to children
#define UI_LAYOUT_FIT_CHILDREN  (1 << 15) // not tested

// Render order
#define UI_LAYOUT_OVERLAY (1 << 20) // render in overlay pass (always on top)

// Direction (how children are arranged)
#define UI_LAYOUT_DIR_ROW (1 << 17)
#define UI_LAYOUT_DIR_COL (1 << 18)

// Child position relative to parent
#define UI_LAYOUT_BELOW (1 << 19) // children start after parent (below for col, right for row)

// Child sizing
#define UI_LAYOUT_GROW_X (1 << 22) // expand to fill remaining horizontal space of parent box
#define UI_LAYOUT_GROW_Y (1 << 23) // expand to fill remaining vertical space of parent box


#define UI_LAYOUT_CONTENT_ALIGN_LEFT     (1 << 24)
#define UI_LAYOUT_CONTENT_ALIGN_RIGHT    (1 << 25)
#define UI_LAYOUT_CONTENT_ALIGN_CENTER_X (1 << 26)


#define UI_LAYOUT_CONTENT_ALIGN_TOP      (1 << 27)
#define UI_LAYOUT_CONTENT_ALIGN_BOTTOM   (1 << 28)
#define UI_LAYOUT_CONTENT_ALIGN_CENTER_Y (1 << 29)
// Misc

#define UI_LAYOUT_DIRTY (1 << 31)

typedef struct ui_win_s ui_win_t;
void                    ui_layout_pass(ui_win_t* win);
ui_box_t*               ui_layout_menu(ui_box_t* menu);

#endif
