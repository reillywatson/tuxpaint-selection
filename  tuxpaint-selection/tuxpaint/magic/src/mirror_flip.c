/*
  mirror_flip.c

  Mirror and Flip Magic Tools Plugin
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

  Last updated: October 29, 2007
  $Id: mirror_flip.c,v 1.6 2007/10/29 15:58:06 wkendrick Exp $
*/

#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include "tp_magic_api.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

/* What tools we contain: */

enum {
  TOOL_MIRROR,
  TOOL_FLIP,
  NUM_TOOLS
};

Mix_Chunk * snd_effects[NUM_TOOLS];


// No setup required:
int mirror_flip_init(magic_api * api)
{
  char fname[1024];

  snprintf(fname, sizeof(fname), "%s/sounds/magic/mirror.wav",
           api->data_directory);
  snd_effects[TOOL_MIRROR] = Mix_LoadWAV(fname);

  snprintf(fname, sizeof(fname), "%s/sounds/magic/flip.wav",
           api->data_directory);
  snd_effects[TOOL_FLIP] = Mix_LoadWAV(fname);

  return(1);
}

Uint32 mirror_flip_api_version(void) { return(TP_MAGIC_API_VERSION); }

// We have multiple tools:
int mirror_flip_get_tool_count(magic_api * api)
{
  return(NUM_TOOLS);
}

// Load our icons:
SDL_Surface * mirror_flip_get_icon(magic_api * api, int which)
{
  char fname[1024];

  if (which == TOOL_MIRROR)
  {
    snprintf(fname, sizeof(fname), "%s/images/magic/mirror.png",
	     api->data_directory);
  }
  else if (which == TOOL_FLIP)
  {
    snprintf(fname, sizeof(fname), "%s/images/magic/flip.png",
	     api->data_directory);
  }

  return(IMG_Load(fname));
}

// Return our names, localized:
char * mirror_flip_get_name(magic_api * api, int which)
{
  if (which == TOOL_MIRROR)
    return(strdup(gettext("Mirror")));
  else if (which == TOOL_FLIP)
    return(strdup(gettext("Flip")));

  return(NULL);
}

// Return our descriptions, localized:
char * mirror_flip_get_description(magic_api * api, int which)
{
  if (which == TOOL_MIRROR)
    return(strdup(
           gettext("Click to make a mirror image.")));
  else
    return(strdup(
           gettext("Click to flip the picture upside-down.")));

  return(NULL);
}

// We affect the whole canvas, so only do things on click, not drag:
void mirror_flip_drag(magic_api * api, int which, SDL_Surface * canvas,
	  SDL_Surface * last, int ox, int oy, int x, int y,
	  SDL_Rect * update_rect)
{
  // No-op
}

void mirror_flip_release(magic_api * api, int which, SDL_Surface * canvas,
	  SDL_Surface * last, int ox, int oy, int x, int y,
	  SDL_Rect * update_rect)
{
  // No-op
}

// Affect the canvas on click:
void mirror_flip_click(magic_api * api, int which,
	   SDL_Surface * canvas, SDL_Surface * last,
	   int x, int y,
	   SDL_Rect * update_rect)
{
  int xx, yy;
  SDL_Rect src, dest;

  if (which == TOOL_MIRROR)
  {
    for (xx = 0; xx < canvas->w; xx++)
    {
      src.x = xx;
      src.y = 0;
      src.w = 1;
      src.h = canvas->h;

      dest.x = canvas->w - xx - 1;
      dest.y = 0;

      SDL_BlitSurface(last, &src, canvas, &dest);
    }

    api->special_notify(SPECIAL_MIRROR);
  }
  else if (which == TOOL_FLIP)
  {
    for (yy = 0; yy < canvas->h; yy++)
    {
      src.x = 0;
      src.y = yy;
      src.w = canvas->w;
      src.h = 1;

      dest.x = 0;
      dest.y = canvas->h - yy - 1;

      SDL_BlitSurface(last, &src, canvas, &dest);
    }

    api->special_notify(SPECIAL_FLIP);
  }

  update_rect->x = 0;
  update_rect->y = 0;
  update_rect->w = canvas->w;
  update_rect->h = canvas->h;

  api->playsound(snd_effects[which], 128, 255);
}

// No setup happened:
void mirror_flip_shutdown(magic_api * api)
{
  if (snd_effects[0] != NULL)
    Mix_FreeChunk(snd_effects[0]);
  if (snd_effects[1] != NULL)
    Mix_FreeChunk(snd_effects[1]);
}

// We don't use colors:
void mirror_flip_set_color(magic_api * api, Uint8 r, Uint8 g, Uint8 b)
{
}

// We don't use colors:
int mirror_flip_requires_colors(magic_api * api, int which)
{
  return 0;
}

