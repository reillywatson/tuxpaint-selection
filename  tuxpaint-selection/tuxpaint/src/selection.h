#ifndef SELECTION_H
#define SELECTION_H

#include "SDL.h"

enum
{
	SELECTTOOL_RECT,
	SELECTTOOL_ELLIPSE,
	SELECTTOOL_LASSO
};

void selectionInit(SDL_Surface *canvas);
void selectionCleanup();

void begin_selection(SDL_Surface *canvas, SDL_Surface *screen, int x, int y);
void update_selection(SDL_Surface *canvas, SDL_Surface *screen, int x, int y);
void finish_selection(SDL_Surface *canvas, SDL_Surface *screen, int x, int y);

void set_selection_tool(int tool);

void copy_selection_to_clipboard();

// Called when the select tool is no longer the current tool,
// so we can remove our XOR shape and free our selection points
// memory.  Doesn't exist just yet.
// Plan: replace all instances of "cur_tool = x" in the code
// with set_tool(x), which will inform a set of listeners
// that the tool has changed.
void select_tool_done();

#endif // SELECTION_H
