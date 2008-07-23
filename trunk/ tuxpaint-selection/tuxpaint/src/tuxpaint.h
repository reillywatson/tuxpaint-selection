#ifndef TUXPAINT_H
#define TUXPAINT_H

// Draw a XOR rect onto the screen (does not modify canvas)
// screencoords is true if coordinates are screen-based, false if
// they are canvas-based
void rect_xor_to_screen(int x1, int y1, int x2, int y2, int screencoords);
void ellipse_xor_to_screen(int x1, int y1, int x2, int y2, int screencoords);
// Draw a XOR line onto the screen (does not modify canvas)
void line_xor_to_screen(int x1, int y1, int x2, int y2, int screencoords);
// Draw a XOR pixel onto the screen (does not modify canvas)
void xorpixel_to_screen(int x, int y, int screencoords);

// Erase a rectangle from a surface, replacing it with the background.
void erase_rect_from_surface(SDL_Surface *surface, SDL_Rect *rect, int screencoords);

void update_canvas(int x1, int y1, int x2, int y2);
void update_screen(int x1, int y1, int x2, int y2);

// update the undo stack
void rec_undo_buffer();

// A tool change listener gets called when the current tool is changed.
typedef void (*ToolChangeCallback) (int, int);

// Add a tool change listener (duplicate callbacks are allowed, but that function will then be called twice)
void add_tool_changed_listener(ToolChangeCallback cb);
// Remove a tool change listener (removes the first matching ToolChangeCallback in the set of listeners)
void remove_tool_changed_listener(ToolChangeCallback cb);

#endif
