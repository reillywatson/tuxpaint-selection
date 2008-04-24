/*
  emboss.c

  Emboss Magic Tool Plugin
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
  $Id: emboss.c,v 1.5 2007/08/08 06:56:05 wkendrick Exp $
*/

#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include "tp_magic_api.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

/* Our globals: */

Mix_Chunk * emboss_snd;


Uint32 emboss_api_version(void) { return(TP_MAGIC_API_VERSION); }


// No setup required:
int emboss_init(magic_api * api)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/sounds/magic/emboss.ogg",
	    api->data_directory);
  emboss_snd = Mix_LoadWAV(fname);

  return(1);
}

// We have multiple tools:
int emboss_get_tool_count(magic_api * api)
{
  return(1);
}

// Load our icons:
SDL_Surface * emboss_get_icon(magic_api * api, int which)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/images/magic/emboss.png",
	   api->data_directory);

  return(IMG_Load(fname));
}

// Return our names, localized:
char * emboss_get_name(magic_api * api, int which)
{
  return(strdup(gettext("Emboss")));
}

// Return our descriptions, localized:
char * emboss_get_description(magic_api * api, int which)
{
  return(strdup(gettext("Click and drag the mouse to emboss the picture.")));
}

// Do the effect:

void do_emboss(void * ptr, int which, SDL_Surface * canvas, SDL_Surface * last,
                int x, int y)
{
  magic_api * api = (magic_api *) ptr;
  int xx, yy;
  Uint8 r1, g1, b1,
        r2, g2, b2;
  int r, g, b;
  float h, s, v;
  int avg1, avg2;

  for (yy = -16; yy < 16; yy++)
  {
    for (xx = -16; xx < 16; xx++)
    {
      if (api->in_circle(xx, yy, 16))
      {
        if (!api->touched(x + xx, y + yy))
        {
          SDL_GetRGB(api->getpixel(last, x + xx, y + yy), last->format, &r1, &g1, &b1);
          SDL_GetRGB(api->getpixel(last, x + xx + 2, y + yy + 2), last->format, &r2, &g2, &b2);

          avg1 = (r1 + g1 + b1) / 3;
          avg2 = (r2 + g2 + b2) / 3;

	  api->rgbtohsv(r1, g1, b1, &h, &s, &v);

          r = 128 + (((avg1 - avg2) * 3) / 2);
          if (r < 0) r = 0;
          if (r > 255) r = 255;
          g = b = r;

	  v = (r / 255.0);

	  api->hsvtorgb(h, s, v, &r1, &g1, &b1);

          api->putpixel(canvas, x + xx, y + yy, SDL_MapRGB(canvas->format, r1, g1, b1));
        }
      }
    }
  }
}

// Affect the canvas on drag:
void emboss_drag(magic_api * api, int which, SDL_Surface * canvas,
	          SDL_Surface * last, int ox, int oy, int x, int y,
		  SDL_Rect * update_rect)
{
  api->line((void *) api, which, canvas, last, ox, oy, x, y, 1, do_emboss);

  if (ox > x) { int tmp = ox; ox = x; x = tmp; }
  if (oy > y) { int tmp = oy; oy = y; y = tmp; }

  update_rect->x = ox - 16;
  update_rect->y = oy - 16;
  update_rect->w = (x + 16) - update_rect->x;
  update_rect->h = (y + 16) - update_rect->h;

  api->playsound(emboss_snd,
                 (x * 255) / canvas->w, 255);
}

// Affect the canvas on click:
void emboss_click(magic_api * api, int which,
	           SDL_Surface * canvas, SDL_Surface * last,
	           int x, int y, SDL_Rect * update_rect)
{
  emboss_drag(api, which, canvas, last, x, y, x, y, update_rect);
}

// Affect the canvas on release:
void emboss_release(magic_api * api, int which,
	           SDL_Surface * canvas, SDL_Surface * last,
	           int x, int y, SDL_Rect * update_rect)
{
}

// No setup happened:
void emboss_shutdown(magic_api * api)
{
  if (emboss_snd != NULL)
    Mix_FreeChunk(emboss_snd);
}

// Record the color from Tux Paint:
void emboss_set_color(magic_api * api, Uint8 r, Uint8 g, Uint8 b)
{
}

// Use colors:
int emboss_requires_colors(magic_api * api, int which)
{
  return 0;
}

