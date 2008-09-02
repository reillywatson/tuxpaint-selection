#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "SDL.h"

// initialize the clipboard
void clipboard_init(void);
// program is closing; perform any necessary cleanup
void clipboard_shutdown(void);
// returns the clipboard contents in an SDL_Surface, NULL if there
// is no image in the clipboard
SDL_Surface *get_clipboard_contents(void);
// sets the current clipboard contents to be the specified SDL_Surface image
void set_clipboard_contents(SDL_Surface *surface);

#endif
