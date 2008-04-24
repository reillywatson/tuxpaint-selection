/*
  fade_darken.c

  Fade and Darken Magic Tools Plugin
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
  $Id: fade_darken.c,v 1.6 2007/08/08 06:56:05 wkendrick Exp $
*/

#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include "tp_magic_api.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

enum {
  TOOL_FADE,
  TOOL_DARKEN,
  NUM_TOOLS
};

Mix_Chunk * snd_effects[NUM_TOOLS];


int fade_darken_init(magic_api * api)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/sounds/magic/fade.wav",
	   api->data_directory);
  snd_effects[TOOL_FADE] = Mix_LoadWAV(fname);  

  snprintf(fname, sizeof(fname), "%s/sounds/magic/darken.wav",
	   api->data_directory);
  snd_effects[TOOL_DARKEN] = Mix_LoadWAV(fname);  

  return(1);
}

Uint32 fade_darken_api_version(void) { return(TP_MAGIC_API_VERSION); }

// Multiple tools:
int fade_darken_get_tool_count(magic_api * api)
{
  return(NUM_TOOLS);
}

// Load our icon:
SDL_Surface * fade_darken_get_icon(magic_api * api, int which)
{
  char fname[1024];

  if (which == TOOL_FADE)
  {
    snprintf(fname, sizeof(fname), "%s/images/magic/fade.png",
	     api->data_directory);
  }
  else if (which == TOOL_DARKEN)
  {
    snprintf(fname, sizeof(fname), "%s/images/magic/darken.png",
	     api->data_directory);
  }

  return(IMG_Load(fname));
}

// Return our name, localized:
char * fade_darken_get_name(magic_api * api, int which)
{
  if (which == TOOL_FADE)
    return(strdup(gettext("Lighten")));
  else if (which == TOOL_DARKEN)
    return(strdup(gettext("Darken")));

  return(NULL);
}

// Return our description, localized:
char * fade_darken_get_description(magic_api * api, int which)
{
  if (which == TOOL_FADE)
    return(strdup(
           gettext("Click and move to fade the colors.")));
  else if (which == TOOL_DARKEN)
    return(strdup(
           gettext("Click and move to darken the colors.")));

  return(NULL);
}

// Callback that does the fade_darken color effect on a circle centered around x,y
void do_fade_darken(void * ptr, int which,
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
      if (api->in_circle(xx - x, yy - y, 16) &&
	  !api->touched(xx, yy))
      {
        SDL_GetRGB(api->getpixel(last, xx, yy), last->format, &r, &g, &b);

        if (which == TOOL_FADE)
        {
          r = min(r + 48, 255);
          g = min(g + 48, 255);
          b = min(b + 48, 255);
        }
        else if (which == TOOL_DARKEN)
        {
          r = max(r - 48, 0);
          g = max(g - 48, 0);
          b = max(b - 48, 0);
        }

        api->putpixel(canvas, xx, yy, SDL_MapRGB(canvas->format, r, g, b));
      }
    }
  }
}

// Ask Tux Paint to call our 'do_fade_darken()' callback over a line
void fade_darken_drag(magic_api * api, int which, SDL_Surface * canvas,
	           SDL_Surface * last, int ox, int oy, int x, int y,
                   SDL_Rect * update_rect)
{
  SDL_LockSurface(last);
  SDL_LockSurface(canvas);

  api->line((void *) api, which, canvas, last, ox, oy, x, y, 1, do_fade_darken);
  
  SDL_UnlockSurface(canvas);
  SDL_UnlockSurface(last);

  api->playsound(snd_effects[which], (x * 255) / canvas->w, 255);

  if (ox > x) { int tmp = ox; ox = x; x = tmp; }
  if (oy > y) { int tmp = oy; oy = y; y = tmp; }

  update_rect->x = ox - 16;
  update_rect->y = oy - 16;
  update_rect->w = (x + 16) - update_rect->x;
  update_rect->h = (y + 16) - update_rect->y;
}

// Ask Tux Paint to call our 'do_fade_darken()' callback at a single point
void fade_darken_click(magic_api * api, int which,
	            SDL_Surface * canvas, SDL_Surface * last,
	            int x, int y, SDL_Rect * update_rect)
{
  fade_darken_drag(api, which, canvas, last, x, y, x, y, update_rect);
}

// Release
void fade_darken_release(magic_api * api, int which,
	            SDL_Surface * canvas, SDL_Surface * last,
	            int x, int y, SDL_Rect * update_rect)
{
}


// No setup happened:
void fade_darken_shutdown(magic_api * api)
{
  if (snd_effects[0] != NULL)
    Mix_FreeChunk(snd_effects[0]);
  if (snd_effects[1] != NULL)
    Mix_FreeChunk(snd_effects[1]);
}

// We don't use colors
void fade_darken_set_color(magic_api * api, Uint8 r, Uint8 g, Uint8 b)
{
}

// We don't use colors
int fade_darken_requires_colors(magic_api * api, int which)
{
  return 0;
}

