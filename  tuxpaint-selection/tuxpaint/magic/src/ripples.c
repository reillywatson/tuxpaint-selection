/*
  ripples.c

  Ripples Magic Tool Plugin
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
  $Id: ripples.c,v 1.2 2007/08/08 06:56:05 wkendrick Exp $
*/

#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include "tp_magic_api.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include <math.h>

/* Our globals: */

Mix_Chunk * ripples_snd;

int ripples_z, ripples_brite;

Uint32 ripples_api_version(void) { return(TP_MAGIC_API_VERSION); }

#define deg_cos(x) cos((x) * M_PI / 180.0)
#define deg_sin(x) sin((x) * M_PI / 180.0)

// No setup required:
int ripples_init(magic_api * api)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/sounds/magic/ripples.ogg",
	    api->data_directory);
  ripples_snd = Mix_LoadWAV(fname);

  return(1);
}

// We have multiple tools:
int ripples_get_tool_count(magic_api * api)
{
  return(1);
}

// Load our icons:
SDL_Surface * ripples_get_icon(magic_api * api, int which)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/images/magic/ripples.png",
	   api->data_directory);

  return(IMG_Load(fname));
}

// Return our names, localized:
char * ripples_get_name(magic_api * api, int which)
{
  return(strdup(gettext("Ripples")));
}

// Return our descriptions, localized:
char * ripples_get_description(magic_api * api, int which)
{
  return(strdup(gettext("Click to make ripples appear over your picture.")));
}

// Affect the canvas on drag:
void ripples_drag(magic_api * api, int which, SDL_Surface * canvas,
	          SDL_Surface * last, int ox, int oy, int x, int y,
		  SDL_Rect * update_rect)
{
}

void ripples_linecb(void * ptr, int which,
                    SDL_Surface * canvas, SDL_Surface * last,
                    int x, int y)
{
  magic_api * api = (magic_api *) ptr;
  Uint8 r, g, b;
  Uint32 pix;

  pix = api->getpixel(last, x + ripples_z, y + ripples_z);
  SDL_GetRGB(pix, last->format, &r, &g, &b);

  r = max(0, min(255, r + ripples_brite));
  g = max(0, min(255, g + ripples_brite));
  b = max(0, min(255, b + ripples_brite));

  api->putpixel(canvas, x, y, SDL_MapRGB(canvas->format, r, g, b));
}

// Affect the canvas on click:
void ripples_click(magic_api * api, int which,
	           SDL_Surface * canvas, SDL_Surface * last,
	           int x, int y, SDL_Rect * update_rect)
{
  float radius;
  float fli;
  int ox, oy, nx, ny, d;

  radius = 100;

  for (fli = 0; fli < radius; fli = fli + .25)
  {
    ripples_z = (10 * deg_sin(((50 * 50) / (fli + 4)) * 10));
	  
    ox = fli * deg_cos(0) + x;
    oy = -fli * deg_sin(0) + y;
	  
    for (d = 0; d <= 360 + (360 / (fli + 1)); d = d + 360 / (fli + 1))
    {
      nx = fli * deg_cos(d) + x;
      ny = -fli * deg_sin(d) + y;
	      
      ripples_brite = (ripples_z * 20 * deg_sin(d + 45)) / ((fli / 4) + 1);

      api->line((void *) api, which, canvas, last, ox, oy, nx, ny, 1, ripples_linecb);
	      
      ox = nx;
      oy = ny;
    }
  }

  update_rect->x = x - 100;
  update_rect->y = y - 100;
  update_rect->w = 200;
  update_rect->h = 200;

  api->playsound(ripples_snd, (x * 255) / api->canvas_w, 255);
}

// Affect the canvas on release:
void ripples_release(magic_api * api, int which,
	           SDL_Surface * canvas, SDL_Surface * last,
	           int x, int y, SDL_Rect * update_rect)
{
}

// No setup happened:
void ripples_shutdown(magic_api * api)
{
  if (ripples_snd != NULL)
    Mix_FreeChunk(ripples_snd);
}

// Record the color from Tux Paint:
void ripples_set_color(magic_api * api, Uint8 r, Uint8 g, Uint8 b)
{
}

// Use colors:
int ripples_requires_colors(magic_api * api, int which)
{
  return 0;
}

