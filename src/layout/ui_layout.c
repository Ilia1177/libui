#include "ui_layout.h"
#include "ui_box.h"
#include "ui_win.h"

static int main_size(ui_box_t* c, bool row) { return row ? c->area.w : c->area.h; }

static void main_size_set(ui_box_t* c, int v, bool row)
{
    if (row)
        c->area.w = v;
    else
        c->area.h = v;
}

static int main_pos(ui_box_t* c, bool row) { return row ? c->area.x : c->area.y; }

static void main_pos_set(ui_box_t* c, int v, bool row)
{
    if (row)
        c->area.x = v;
    else
        c->area.y = v;
}

static int cross_size(ui_box_t* c, bool row) { return row ? c->area.h : c->area.w; }

static void cross_size_set(ui_box_t* c, int v, bool row)
{
    if (row)
        c->area.h = v;
    else
        c->area.w = v;
}

static int cross_pos(ui_box_t* c, bool row) { return row ? c->area.y : c->area.x; }

static void cross_pos_set(ui_box_t* c, int v, bool row)
{
    if (row)
        c->area.y = v;
    else
        c->area.x = v;
}

static frame_t content_padding(ui_box_t* parent)
{
    frame_t pad;

    if (!parent)
        return ui_frame(0, 0, 0, 0);
    pad = ui_frame_add(parent->padding, parent->border);
    return pad;
}

static void arrange_childs(ui_box_t* parent, bool row)
{
    uint32_t  pl = parent->layout;
    uint32_t  fill_main = row ? UI_LAYOUT_FILL_X : UI_LAYOUT_FILL_Y;
    uint32_t  fill_cross = row ? UI_LAYOUT_FILL_Y : UI_LAYOUT_FILL_X;
    uint32_t  align_end = row ? UI_LAYOUT_ALIGN_BOTTOM : UI_LAYOUT_ALIGN_RIGHT;
    uint32_t  align_center = row ? UI_LAYOUT_ALIGN_CENTER_Y : UI_LAYOUT_ALIGN_CENTER_X;
    frame_t   pad = content_padding(parent);
    int       pad_lead = row ? pad.left : pad.top;
    int       cross_lead = row ? pad.top : pad.left;
    int       cross_trail = row ? pad.bottom : pad.right;
    ui_box_t* child;
    int       visible = 0;
    int       fixed = 0;
    int       fill_count = 0;

    child = parent->childs;
    while (child) {
        if (!(child->state & BOX_HIDDEN)) {
            visible++;
            if (child->layout & fill_main)
                fill_count++;
            else
                fixed += main_size(child, row);
        }
        child = child->next;
    }

    if (visible == 0)
        return;

    int parent_main = main_size(parent, row);
    int gap = 0;
    int start_offset = 0;

    // size childs
    if (fill_count > 0) {
        int remaining = parent_main - fixed;
        if (remaining < 0)
            remaining = 0;
        int fill_size = remaining / fill_count;
        child = parent->childs;
        while (child) {
            if (!(child->state & BOX_HIDDEN) && (child->layout & fill_main))
                main_size_set(child, fill_size, row);
            child = child->next;
        }
    } else {
        bool space_between = pl & UI_LAYOUT_SPACE_BETWEEN;
        bool space_around = pl & UI_LAYOUT_SPACE_AROUND;
        if (space_between && visible > 1)
            gap = (parent_main - fixed) / (visible - 1);
        else if (space_around) {
            gap = (parent_main - fixed) / visible;
            start_offset = gap / 2;
        }
    }

    int pos = (pl & UI_LAYOUT_BELOW) ? (main_pos(parent, row) + parent_main + start_offset)
                                     : (main_pos(parent, row) + start_offset);
    child = parent->childs;
    while (child) {
        if (child->state & BOX_HIDDEN) {
            child = child->next;
            continue;
        }

        uint32_t cl = child->layout;

        main_pos_set(child, pos + pad_lead, row);

        if (cl & fill_cross) {
            cross_pos_set(child, cross_pos(parent, row) + cross_lead, row);
            cross_size_set(child, cross_size(parent, row) - (cross_lead + cross_trail), row);
        } else if (cl & align_end) {
            cross_pos_set(child,
                          cross_pos(parent, row) + cross_size(parent, row) -
                              cross_size(child, row) + cross_lead,
                          row);
        } else if (cl & align_center) {
            cross_pos_set(child,
                          cross_pos(parent, row) +
                              (cross_size(parent, row) - cross_size(child, row)) / 2 + cross_lead,
                          row);
        } else {
            cross_pos_set(child, cross_pos(parent, row) + cross_lead, row);
        }
        pos += main_size(child, row) + gap;
        child = child->next;
    }
}

void apply_align_content(ui_box_t* b)
{
    uint32_t l = b->layout;

    bool h_set = l & UI_LAYOUT_CONTENT_X_MASK;
    bool v_set = l & UI_LAYOUT_CONTENT_Y_MASK;

    if (!h_set && !v_set)
        return; // no alignment flags — leave layers untouched
    ui_layer_t* layer = b->layers;
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

void apply_sizing(ui_box_t* b)
{
    const uint32_t l = b->layout;

    if (!(l & UI_LAYOUT_FIT_CONTENT))
        return;

    dimension_t needed;
    dimension_t curr_child;

    needed = ui_dim(0, 0);
    bool      row = l & UI_LAYOUT_DIR_ROW;
    bool      col = l & UI_LAYOUT_DIR_COL;
    ui_box_t* child = b->childs;
    while (child) {
        if (!(child->state & BOX_HIDDEN)) {
            curr_child = ui_dim(child->area.w, child->area.h);
            // int current_child_w = child->area.w;
            // int current_child_h = child->area.h;
            if (row) {
                if (!(child->layout & UI_LAYOUT_FILL_X))
                    needed.width += curr_child.width;
                if (curr_child.height > needed.height)
                    needed.height = curr_child.height;
            } else if (col) {
                if (!(child->layout & UI_LAYOUT_FILL_Y))
                    needed.height += curr_child.height;
                if (curr_child.width > needed.width)
                    needed.width = curr_child.width;
            } else {
                if (curr_child.width > needed.width)
                    needed.width = curr_child.width;
                if (curr_child.height > needed.height)
                    needed.height = curr_child.height;
            }
        }
        child = child->next;
    }
    ui_layer_t* layer = b->layers;
    while (layer) {
        dimension_t curr_lay = ui_dim(layer->area.w, layer->area.h);
        if (curr_lay.width > needed.width)
            needed.width = curr_lay.width;
        if (curr_lay.height > needed.height)
            needed.height = curr_lay.height;
        layer = layer->next;
    }
    if (!(l & UI_LAYOUT_FILL_X))
        b->area.w =
            needed.width + b->padding.left + b->padding.right + b->border.left + b->border.right;
    if (!(l & UI_LAYOUT_FILL_Y))
        b->area.h =
            needed.height + b->padding.top + b->padding.bottom + b->border.top + b->border.bottom;
}

static void place_self_in_parent(ui_box_t* b, SDL_Rect container, frame_t pad, bool parent_row,
                                 bool parent_col)
{
    uint32_t layout = b->layout;

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

static void layout_one_box(ui_box_t* b)
{
    if (!b || (b->state & BOX_HIDDEN))
        return;

    SDL_Rect      container;
    ui_box_t*     parent = b->parent;
    const frame_t pad = content_padding(parent);
    bool          parent_row = false;
    bool          parent_col = false;

    if (parent) {
        container = parent->area;
        parent_row = parent->layout & UI_LAYOUT_DIR_ROW;
        parent_col = parent->layout & UI_LAYOUT_DIR_COL;
    } else {
        container = ui_area(0, 0, b->win->area.w, b->win->area.h);
        // container = b->win->area;
        // container.x = 0;
        // container.y = 0;
    }

    if (!(b->layout & UI_LAYOUT_ABSOLUTE))
        place_self_in_parent(b, container, pad, parent_row, parent_col);

    apply_sizing(b);
    if (b->layout & UI_LAYOUT_DIR_MASK)
        arrange_childs(b, b->layout & UI_LAYOUT_DIR_ROW);
    apply_align_content(b);

    b->layout &= ~UI_LAYOUT_DIRTY;
    ui_box_t* child = b->childs;
    while (child) {
        layout_one_box(child);
        child = child->next;
    }
}

void ui_layout_pass(ui_win_t* win)
{
    ui_box_t* b;

    if (!win) {
        return;
    }
    b = win->boxes;
    while (b) {
        layout_one_box(b);
        b = b->next;
    }
}
