#include "clipboard.h"
#include "SDL_Image.h"
#include "tiffio.h"

#ifdef __APPLE__
#include <carbon/carbon.h>
static PasteboardRef pasteboard;
static int initialized = 0;
void clipboard_init()
{
	initialized = 1;
	PasteboardCreate(kPasteboardClipboard, &pasteboard);
}

void clipboard_cleanup()
{
}

SDL_Surface *get_clipboard_contents()
{
	if (initialized == 0)
	{
		clipboard_init();
	}
	CFStringRef tifftype = CFSTR("public.tiff");
	ItemCount count;
	PasteboardGetItemCount(pasteboard, &count);
	if (count == 1)
	{
		PasteboardItemID item;
		PasteboardGetItemIdentifier(pasteboard, 1, &item);
		CFArrayRef types;
		PasteboardCopyItemFlavors(pasteboard, item, &types);
		int flavorcount = CFArrayGetCount(types);
		CFIndex i;
		for (i = 0; i < flavorcount; i++)
		{
			CFStringRef flavor;
			flavor = (CFStringRef)CFArrayGetValueAtIndex(types, i);
			if (CFStringCompare(flavor, tifftype, 0) == kCFCompareEqualTo)
			{
				CFDataRef data;
				PasteboardCopyItemFlavorData(pasteboard, item, flavor, &data);
				Uint8 *buffer;
				CFIndex size = CFDataGetLength(data);
				buffer = malloc(size);
				CFDataGetBytes(data, CFRangeMake(0, size), buffer);
				/*FILE *file;
				file = fopen("clip.tif","a+");
				Uint8 *c;
				for (c = buffer; c <= buffer + size; c++)
				{
					fputc(*c, file);
				}
				fclose(file);*/

				//Loading from memory works like this:
				SDL_RWops *rw = SDL_RWFromMem(buffer, size);
				SDL_Surface *surface = (SDL_Surface*)IMG_Load_RW(rw, 1); // this will free 'buffer' for me
				
				//SDL_Surface *surface = (SDL_Surface*)IMG_Load("clipb.tif");
				if (!surface)
				{
					char* error = IMG_GetError();
					printf("Oh My Goodness, an error : %s", error);
				}
				CFRelease(data);
				return surface;
			}
		}
	}
	return NULL;
}

void set_clipboard_contents(SDL_Surface *surface)
{
	PasteboardClear(pasteboard);
}
#else
#ifdef WIN32
SDL_Surface *get_clipboard_contents()
{
	return NULL;
}

void set_clipboard_contents(SDL_Surface *surface)
{
}
#else
// this platform has no clipoard support
SDL_Surface *get_clipboard_contents()
{
	return NULL;
}

void set_clipboard_contents(SDL_Surface *surface)
{
}
#endif
#endif
