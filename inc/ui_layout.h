#ifndef UI_STYLE
# define UI_STYLE
#include "libui.h"  // gets ui_pos_t fully defined, forward decl handles ui_win_t
// Horizontal alignment (mutually exclusive)
#define UI_LAYOUT_ALIGN_LEFT    (1 << 0)
#define UI_LAYOUT_ALIGN_RIGHT   (1 << 1)
#define UI_LAYOUT_ALIGN_CENTER_X (1 << 2)

// Vertical alignment (mutually exclusive)
#define UI_LAYOUT_ALIGN_TOP     (1 << 3)
#define UI_LAYOUT_ALIGN_BOTTOM  (1 << 4)
#define UI_LAYOUT_ALIGN_CENTER_Y (1 << 5)

// Sizing behavior
#define UI_LAYOUT_FILL_X        (1 << 6)  // stretch to fill parent width
#define UI_LAYOUT_FILL_Y        (1 << 7)  // stretch to fill parent height
#define UI_LAYOUT_FIT_CONTENT   (1 << 8)  // shrink to fit children

// Child distribution (when layout_direction is row/column)
#define UI_LAYOUT_SPACE_BETWEEN (1 << 9)  // equal space between children
#define UI_LAYOUT_SPACE_AROUND  (1 << 10) // equal space around children

// Overflow
#define UI_LAYOUT_WRAP          (1 << 11)
#define UI_LAYOUT_CLIP          (1 << 12) // clip children outside bounds
#define UI_LAYOUT_SCROLL_X      (1 << 13)
#define UI_LAYOUT_SCROLL_Y      (1 << 14)

// Misc
// #define UI_LAYOUT_HIDDEN        (1 << 15) // exclude from layout calculation
#define UI_LAYOUT_ABSOLUTE      (1 << 16) // ignore parent layout, use area directly

#endif
