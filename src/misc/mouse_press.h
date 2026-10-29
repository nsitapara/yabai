#ifndef MOUSE_PRESS_H
#define MOUSE_PRESS_H

// NOTE: On mouse-up yabai reads any change in the pressed window's
// frame as the user dragging its edge or title bar, and rewrites split ratios
// or swaps windows. Its own retiling also changes that frame: a window opening
// beside it, or a placement whose AX notification lands after the press. A
// click on a new window then pins its split at the 0.1/0.9 clamp. Treat the
// change as layout when the cursor stayed put, or when yabai retiled the window
// during the press or shortly before it. The fork's own stationary-click guard
// in MOUSE_UP only covers the first case and does not restore the tile.
#define MOUSE_PRESS_CLICK_SLOP      4.0f
#define MOUSE_PRESS_LAYOUT_GRACE_MS 1000

static inline bool mouse_press_frame_change_is_layout(CGPoint down, CGPoint up, uint64_t down_time, uint64_t layout_time, uint64_t ticks_per_second)
{
    float dx = up.x - down.x;
    float dy = up.y - down.y;
    if (dx * dx + dy * dy <= MOUSE_PRESS_CLICK_SLOP * MOUSE_PRESS_CLICK_SLOP) return true;

    uint64_t grace = ticks_per_second * MOUSE_PRESS_LAYOUT_GRACE_MS / 1000;
    return layout_time && layout_time + grace >= down_time;
}

#endif
