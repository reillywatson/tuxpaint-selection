/*
  cartoon.c

  Cartoon Magic Tool Plugin
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

  Last updated: August 21, 2007
  $Id: cartoon.c,v 1.5 2007/08/22 06:27:42 wkendrick Exp $
*/

#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include <stdlib.h>
#include <math.h>
#include "tp_magic_api.h"
#include "SDL_image.h"
#include "SDL_mixer.h"


/* Our globals: */

Mix_Chunk * cartoon_snd;

#define OUTLINE_THRESH 48


// No setup required:
int cartoon_init(magic_api * api)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/sounds/magic/cartoon.wav",
	    api->data_directory);
  cartoon_snd = Mix_LoadWAV(fname);

  return(1);
}

Uint32 cartoon_api_version(void) { return(TP_MAGIC_API_VERSION); }

// We have multiple tools:
int cartoon_get_tool_count(magic_api * api)
{
  return(1);
}

// Load our icons:
SDL_Surface * cartoon_get_icon(magic_api * api, int which)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/images/magic/cartoon.png",
	   api->data_directory);

  return(IMG_Load(fname));
}

// Return our names, localized:
char * cartoon_get_name(magic_api * api, int which)
{
  return(strdup(gettext("Cartoon")));
}

// Return our descriptions, localized:
char * cartoon_get_description(magic_api * api, int which)
{
  return(strdup(gettext(
"Click and move the mouse around to turn the picture into a cartoon.")));
}

// Do the effect:

void do_cartoon(void * ptr, int which, SDL_Surface * canvas, SDL_Surface * last,
                int x, int y)
{
  magic_api * api = (magic_api *) ptr;
  int xx, yy;
  Uint8 r1, g1, b1, r2, g2, b2;
  Uint8 r, g, b;
  float hue, sat, val;

  /* First, convert colors to more cartoony ones: */

  for (yy = y - 16; yy < y + 16; yy = yy + 1)
  {
    for (xx = x - 16; xx < x + 16; xx = xx + 1)
    {
      if (api->in_circle(xx - x, yy - y, 16))
      {
        /* Get original color: */

        SDL_GetRGB(api->getpixel(last, xx, yy), last->format, &r, &g, &b);

        api->rgbtohsv(r, g, b, &hue, &sat, &val);

        val = val - 0.5;
        val = val * 4;
        val = val + 0.5;

        if (val < 0)
          val = 0;
        else if (val > 1.0)
          val = 1.0;

        val = floor(val * 4) / 4;
        hue = floor(hue * 4) / 4;

        sat = floor(sat * 4) / 4;

        api->hsvtorgb(hue, sat, val, &r, &g, &b);

        api->putpixel(canvas, xx, yy, SDL_MapRGB(canvas->format, r, g, b));
      }
    }
  }

  /* Then, draw dark outlines where there's a large contrast change */

  for (yy = y - 16; yy < y + 16; yy++)
  {
    for (xx = x - 16; xx < x + 16; xx++)
    {
      if (api->in_circle(xx - x, yy - y, 16))
      {
        /* Get original color: */

        SDL_GetRGB(api->getpixel(last, xx, yy), last->format, &r, &g, &b);

        SDL_GetRGB(api->getpixel(last, xx + 1, yy),
                   last->format, &r1, &g1, &b1);

        SDL_GetRGB(api->getpixel(last, xx + 1, yy + 1),
                   last->format, &r2, &g2, &b2);

        if (abs(((r + g + b) / 3) - (r1 + g1 + b1) / 3) > OUTLINE_THRESH
            || abs(((r + g + b) / 3) - (r2 + g2 + b2) / 3) >
            OUTLINE_THRESH || abs(r - r1) > OUTLINE_THRESH
            || abs(g - g1) > OUTLINE_THRESH
            || abs(b - b1) > OUTLINE_THRESH
            || abs(r - r2) > OUTLINE_THRESH
            || abs(g - g2) > OUTLINE_THRESH
            || abs(b - b2) > OUTLINE_THRESH)
        {
          api->putpixel(canvas, xx - 1, yy,
                   SDL_MapRGB(canvas->format, 0, 0, 0));
          api->putpixel(canvas, xx, yy - 1,
                   SDL_MapRGB(canvas->format, 0, 0, 0));
          api->putpixel(canvas, xx - 1, yy - 1,
                   SDL_MapRGB(canvas->format, 0, 0, 0));
        }
      }
    }
  }
}

// Affect the canvas on drag:
void cartoon_drag(magic_api * api, int which, SDL_Surface * canvas,
	          SDL_Surface * last, int ox, int oy, int x, int y,
		  SDL_Rect * update_rect)
{
  api->line((void *) api, which, canvas, last, ox, oy, x, y, 1, do_cartoon);

  if (ox > x) { int tmp = ox; ox = x; x = tmp; }
  if (oy > y) { int tmp = oy; oy = y; y = tmp; }

  update_rect->x = ox - 16;
  update_rect->y = oy - 16;
  update_rect->w = (x + 16) - update_rect->x;
  update_rect->h = (y + 16) - update_rect->h;

  api->playsound(cartoon_snd, (x * 255) / canvas->w, 255);
}

// Affect the canvas on click:
void cartoon_click(magic_api * api, int which,
	           SDL_Surface * canvas, SDL_Surface * last,
	           int x, int y, SDL_Rect * update_rect)
{
  cartoon_drag(api, which, canvas, last, x, y, x, y, update_rect);
}

// Affect the canvas on release:
void cartoon_release(magic_api * api, int which,
	           SDL_Surface * canvas, SDL_Surface * last,
	           int x, int y, SDL_Rect * update_rect)
{
}

// No setup happened:
void cartoon_shutdown(magic_api * api)
{
  if (cartoon_snd != NULL)
    Mix_FreeChunk(cartoon_snd);
}

// Record the color from Tux Paint:
void cartoon_set_color(magic_api * api, Uint8 r, Uint8 g, Uint8 b)
{
}

// Use colors:
int cartoon_requires_colors(magic_api * api, int which)
{
  return 0;
}

