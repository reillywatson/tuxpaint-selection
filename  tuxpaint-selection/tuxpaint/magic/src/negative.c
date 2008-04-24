/*
  negative.c

  Negative Magic Tool Plugin
  Tux Paint - A simple drawing program for children.

  Copyright (c) 2002-2007 by Bill Kendrick and others; see AUTHORS.txt
  bill@newbreedsoftware.com
  http://www.tuxpaint.org/

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  (See COPYING.txt)

  Last updated: August 7, 2007
  $Id: negative.c,v 1.7 2007/08/08 06:56:05 wkendrick Exp $
*/

#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include "tp_magic_api.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

Mix_Chunk * negative_snd;

// No setup required:
int negative_init(magic_api * api)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/sounds/magic/negative.wav",
	   api->data_directory);

  negative_snd = Mix_LoadWAV(fname);

  return(1);
}

Uint32 negative_api_version(void) { return(TP_MAGIC_API_VERSION); }

// Only one tool:
int negative_get_tool_count(magic_api * api)
{
  return(1);
}

// Load our icon:
SDL_Surface * negative_get_icon(magic_api * api, int which)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/images/magic/negative.png",
	   api->data_directory);
  return(IMG_Load(fname));
}

// Return our name, localized:
char * negative_get_name(magic_api * api, int which)
{
  return(strdup(gettext("Negative")));
}

// Return our description, localized:
char * negative_get_description(magic_api * api, int which)
{
  return(strdup(
         gettext("Click and move the mouse around to draw a negative.")));
}

// Callback that does the negative color effect on a circle centered around x,y
void do_negative(void * ptr, int which,
	         SDL_Surface * canvas, SDL_Surface * last,
	         int x, int y)
{
  int xx, yy;
  Uint8 r, g, b;
  magic_api * api = (magic_api *) ptr;

  for (yy = y - 16; yy < y + 16; yy++)
  {
    for (xx = x - 16; xx < x + 16; xx++)
    {
      if (api->in_circle(xx - x, yy - y, 16))
      {
        SDL_GetRGB(api->getpixel(last, xx, yy), last->format, &r, &g, &b);

        r = 0xFF - r;
        g = 0xFF - g;
        b = 0xFF - b;

        api->putpixel(canvas, xx, yy, SDL_MapRGB(canvas->format, r, g, b));
      }
    }
  }
}

// Ask Tux Paint to call our 'do_negative()' callback over a line
void negative_drag(magic_api * api, int which, SDL_Surface * canvas,
	           SDL_Surface * last, int ox, int oy, int x, int y,
		   SDL_Rect * update_rect)
{
  SDL_LockSurface(last);
  SDL_LockSurface(canvas);

  api->line((void *) api, which, canvas, last, ox, oy, x, y, 1, do_negative);

  if (ox > x) { int tmp = ox; ox = x; x = tmp; }
  if (oy > y) { int tmp = oy; oy = y; y = tmp; }

  update_rect->x = ox - 16;
  update_rect->y = oy - 16;
  update_rect->w = (x + 16) - update_rect->x;
  update_rect->h = (y + 16) - update_rect->h;

  api->playsound(negative_snd, (x * 255) / canvas->w, 255);
  
  SDL_UnlockSurface(canvas);
  SDL_UnlockSurface(last);
}

// Ask Tux Paint to call our 'do_negative()' callback at a single point
void negative_click(magic_api * api, int which,
	            SDL_Surface * canvas, SDL_Surface * last,
	            int x, int y, SDL_Rect * update_rect)
{
  negative_drag(api, which, canvas, last, x, y, x, y, update_rect);
}


void negative_release(magic_api * api, int which,
	            SDL_Surface * canvas, SDL_Surface * last,
	            int x, int y, SDL_Rect * update_rect)
{
}


void negative_shutdown(magic_api * api)
{
  if (negative_snd != NULL)
    Mix_FreeChunk(negative_snd);
}

// We don't use colors
void negative_set_color(magic_api * api, Uint8 r, Uint8 g, Uint8 b)
{
}

// We don't use colors
int negative_requires_colors(magic_api * api, int which)
{
  return 0;
}

