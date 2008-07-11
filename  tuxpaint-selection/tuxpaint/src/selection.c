#include "selection.h"
#include "pixels.h"
//#include "shapes.h"

static int selectTool = SELECTTOOL_RECT;
static int startx = 0;
static int starty = 0;
static int oldx = -1;
static int oldy = -1;

typedef struct
{
	void (*addSelectionPointCallback)(int,int);
	int (*isSelectedCallback)(int,int);
	void (*drawOutlineCallback)();
} selectionCallbacks;

static selectionCallbacks callbacks;

typedef struct
{
	int x;
	int y;
} Point;

static Point *points;
static int numPoints = 0;
static int pointArraySize = 0;
static int minx;
static int miny;
static int maxx;
static int maxy;

int min(int a, int b)
{
	if (a < b) return a;
	return b;
}

int max(int a, int b)
{
	if (a > b) return a;
	return b;
}

static void initSelBounds()
{
	minx = 10000;
	maxx = -1;
	miny = 10000;
	maxy = -1;
}

static void initPoints()
{
	pointArraySize = 16;
	points = malloc(sizeof(Point) * pointArraySize);
	initSelBounds();
	numPoints = 0;
}

// Add the point (x,y) to the current selection
static void addPolygonalSelectionPoint(int x, int y)
{
	if (pointArraySize == 0)
	{
		initPoints();
	}
	if (x < minx) minx = x;
	if (x > maxx) maxx = x;
	if (y < miny) miny = y;
	if (y > maxy) maxy = y;
	Point p = {x, y};
	points[numPoints] = p;
	numPoints++;
	if (numPoints >= pointArraySize)
	{
		pointArraySize = pointArraySize * 2;
		points = realloc(points, sizeof(Point) * pointArraySize);
	}
}

// Move all points in the current selection
// dx pixels right and dy pixels down
static void moveSelection(int dx, int dy)
{
	minx += dx;
	maxx += dx;
	miny += dy;
	maxy += dy;
	int i;
	for (i = 0; i < numPoints; i++)
	{
		points[i].x += dx;
		points[i].y += dy;
	}
}

// Remove all points from the current selection
static void clearSelectionPoints()
{
	numPoints = 0;
	initSelBounds();
	int i;
	for (i = 0; i < pointArraySize; i++)
	{
		// [FIXME] CHECK IF THIS IS HOW IT WORKS!
		free(&points[i]);
	}
	pointArraySize = 0;
	// [FIXME] when should we free the points array?
	// It seems wasteful to free it and then malloc
	// it again every time the user make a new selection.
	// We really want to do it when the user switches away
	// from the selection tools, but we don't get notified about that (yet...)
}

static void rectDrawOutline()
{
	if (numPoints == 2)
	{
		rect_xor_to_screen(points[0].x, points[0].y, points[1].x, points[1].y, 1);
	}
}

// update the selection boundaries for an elliptical or
// rectangular selection tool
static void addBoundaryToolPoint(int x, int y)
{
	// if we've already got a rectangle,
	// we want to leave the first point fixed
	if (numPoints == 4)
	{
		numPoints = 1;
		int x1 = points[0].x;
		int y1 = points[0].y;
		minx = x1;
		maxx = x1;
		miny = y1;
		maxy = y1;
		addPolygonalSelectionPoint(x1, y);
		addPolygonalSelectionPoint(x, y);
		addPolygonalSelectionPoint(x, y1);
	}
	else
	{
		// this is a new selection; make the first point
		addPolygonalSelectionPoint(x, y);
	}
}

int rectIsSelected(int x, int y)
{
	if (x >= minx &&
		x <= maxx &&
		y >= miny &&
		y <= maxy)
	{
		return 1;
	}
	return 0;
}

int ellipseIsSelected(int x, int y)
{
	// a point (x,y) is in an ellipse when
	// x^2     y^2
	// ---  +  ---  < 1
	// a^2     b^2
	//
	// where a is the major axis radius
	// and b is the minor one.
	x = x - minx;
	y = y - miny;
	if (x < 0 || y < 0)
	{
		return 0;
	}

	int w = maxx - minx;
	int h = maxy - miny;
	int a = w / 2;
	int b = h / 2;
	if (a > b)
	{
		// our major axis is vertical; swap
		int c = a;
		a = b;
		b = c;
	}

	if (((double)(x^2) / (double)(a^2)) + ((double)(y^2) / (double)(b^2)) < 1)
	{
		return 1;
	}
	return 0;
}

int lassoIsSelected(int x, int y)
{
	// We need this function to be
	// really fast for answering this question over many points,
	// for the purpose of pasting the selection.
	// The standard "ray firing" winding point-in-polygon algorithm needs to be modified
	// to flag each pixel along the ray as inside or outside, and then have this
	// answer stored in some array
	
	// [FIXME] placeholder (not even a little bit correct):
	if (x > minx && x < maxx && y > miny >> y < maxy) return 1;
	return 0;
}


static void drawPolygonalOutlineXOR()
{
	if (numPoints > 1)
	{
		int i;
		Point p1, p2;
		p2 = points[0];
		for (i = 1; i < numPoints; i++)
		{
			p1 = p2;
			p2 = points[i];
			line_xor_to_screen(p1.x, p1.y, p2.x, p2.y, 1);
		}
		line_xor_to_screen(points[0].x, points[0].y, p2.x, p2.y, 1);
		// we've now XORed this pixel twice, so we need
		// to do it again for it to show up
		xorpixel(points[0].x, points[0].y);
	}
}


static void drawEllipseOutlineXOR()
{
	// [FIXME] Shapes.h can't be included properly from selection.c
	// (dependencies in shapes.h on gettext_noop), so we are
	// hardcoding SHAPE_ELLIPSE to be 6.
	// This function calls do_shape(), which is not exactly what we want,
	// since it has a minimum diameter of 15 (should this be changed?)
	shape_xor_to_screen(minx, miny, maxx, maxy, 6, 1);
}

	// alternate ellipse drawing code (doesn't seem to work...)
	/*
	int a = maxx - minx;
	int b = maxy - miny;
	if (a < b)
	{
		int c = a;
		a = b;
		b = c;
	}
	int x = floor(a + .5);
	int y = 0;
	int t1 = a * a;
	int t2 = 2 * t1;
	int t3 = 2 * t2;
	int t4 = b * b;
	int t5 = 2 * t4;
	int t6 = 2 * t5;
	int t7 = a * t5;
	int t8 = 2 * t7;
	int t9 = 0;
	int d1 = t2 - t7 + t4 / 2;
	int d2 = t1 / 2 - t8 + t5;
	while (d2 < 0)					// region 1 of ellipse
	{
		xorpixel(x + minx,y + miny);
		y = y + 1;
		t9 = t9 + t3;
		if (d1 < 0)
		{
			d1 = d1 + t9 + t2;
			d2 = d2 + t9;
		}
		else
		{
			x = x - 1;
			t8 = t8 - t6;
			d1 = d1 - t8 + t9 + t2;
			d2 = d2 - t8 + t5 + t9;
		}
	}
	do
	{
		xorpixel(x + minx,y + miny);
		x = x - 1;
		t8 = t8 - t6;
		if (d2 < 0)
		{
			y = y + 1;
			t9 = t9 + t3;
			d2 = d2 - t8 + t5 + t9;
		}
		else
		{
			d2 = d2 - t8 + t5;
		}
	}
	while ( x >= 0);*/

static selectionCallbacks rectangleCalls = 
{
	addBoundaryToolPoint,
	rectIsSelected,
	drawPolygonalOutlineXOR
};

static selectionCallbacks ellipseCalls = 
{
	addBoundaryToolPoint,
	ellipseIsSelected,
	drawEllipseOutlineXOR
};

static selectionCallbacks lassoCalls = 
{
	addPolygonalSelectionPoint,
	lassoIsSelected,
	drawPolygonalOutlineXOR
};

void set_selection_tool(int tool)
{
	selectTool = tool;
	switch(tool)
	{
		case SELECTTOOL_RECT:
			callbacks = rectangleCalls;
			break;
		case SELECTTOOL_ELLIPSE:
			callbacks = ellipseCalls;
			break;
		case SELECTTOOL_LASSO:
			callbacks = lassoCalls;
			break;
	}
}

static int dragmode = 0;
static int lastx;
static int lasty;

static int isSelected(int x, int y)
{
	return (*callbacks.isSelectedCallback)(x, y);
}

static void addSelectionPoint(int x, int y)
{
	(*callbacks.addSelectionPointCallback)(x, y);
}

static void drawOutlineXOR()
{
	(*callbacks.drawOutlineCallback)();
}

static Point* origPoints;
static int oldminx = 10000;
static int oldmaxx = -1;
static int oldminy = 10000;
static int oldmaxy = -1;

// remember the original selection so it can be copied
// while moving it around
static void storeSelection()
{
	// [FIXME] I don't free old origPoints memory!
	origPoints = malloc(numPoints * sizeof(Point));
	int i;
	oldminx = 10000;
	oldmaxx = -1;
	oldminy = 10000;
	oldmaxy = -1;
	for (i = 0; i < numPoints; i++)
	{
		origPoints[i] = points[i];
		int x = origPoints[i].x;
		int y = origPoints[i].y;
		if (x < oldminx) oldminx = x;
		if (x > oldmaxx) oldmaxx = x;
		if (y < oldminy) oldminy = y;
		if (y > oldmaxy) oldmaxy = y;
	}
}

SDL_Surface *clipboard = 0;

// copy to clipboard
void copy_selection(SDL_Surface *surface)
{
	if (clipboard != 0)
	{
		SDL_FreeSurface(clipboard);
		clipboard = 0;
	}
	clipboard = SDL_CreateRGBSurface(surface->flags,
		maxx - minx, maxy - miny,
		surface->format->BitsPerPixel,
		surface->format->Rmask,
		surface->format->Gmask,
		surface->format->Bmask,
		surface->format->Amask);
	int i, j;
	SDL_Rect src = {0, 0, 1, 1};
	SDL_Rect dst = {0, 0, 1, 1};
	for (j = miny; j <= maxy; j++)
	{
		for (i = minx; i <= maxx; i++)
		{
			src.x = i;
			src.y = j;
			dst.x = i - minx;
			dst.y = j - miny;
			if (isSelected(i, j))
			{
				SDL_BlitSurface(surface, &src, clipboard, &dst);
				//putpixels[clipboard->format->BitsPerPixel](clipboard, i, j, getpixels[surface->format->BitsPerPixel](surface, i, j));
			}
		}
	}
}

void paste_from_clipboard(SDL_Surface *dst, int x, int y)
{
	if (clipboard == 0)
	{
		//debug("pasting when clipboard contents are empty!");
		return;
	}
	SDL_Rect srcrect = {0, 0, clipboard->w, clipboard->h};
	SDL_Rect dstrect = {x, y, clipboard->w, clipboard->h};
	SDL_BlitSurface(clipboard, &srcrect, dst, &dstrect);
	SDL_UpdateRect(dst, 0, 0, 0, 0);
}

// dstsurface can be either canvas or screen
void pasteSelection(SDL_Surface *srcsurface, SDL_Surface *dstsurface, int x, int y, int canvastoscreen, int cut)
{
	// [HACK] these 96s shouldn't be here
	SDL_Rect src = {max(oldminx - 96, 0), oldminy, oldmaxx - oldminx + 1, oldmaxy - oldminy + 1};
	int diff = 0;
	if (canvastoscreen == 0) diff = 96;
	SDL_Rect dst = {max(minx - diff, 0), miny, maxx - minx + 1, maxy - miny + 1};
	SDL_BlitSurface(srcsurface, &src, dstsurface, &dst);
	if (cut == 1)
	{
		// instead of doing this, i think we should be using the screen surface
		// as a "preview" layer, and then only committing the change when the selection is cleared
		// (ie a new selection is made or the user goes to do something else)
		erase_rect_from_surface(srcsurface, &src, 0);
	}

	rec_undo_buffer();
	if (cut == 1) update_canvas(0,0,srcsurface->w,srcsurface->h);
	//update_canvas(min(minx, oldminx) - 96, min(miny, oldminy), max(maxx, oldmaxx) - 96, max(maxy, oldmaxy));
	SDL_UpdateRect(srcsurface, 0,0,0,0);
	SDL_UpdateRect(dstsurface, 0,0,0,0);//update_screen(min(minx, oldminx), min(miny, oldminy), max(maxx, oldmaxx), max(maxy, oldmaxy));
}



void begin_selection(SDL_Surface *canvas, SDL_Surface *screen, int x, int y)
{
	startx = x;
	starty = y;
	lastx = x;
	lasty = y;
	dragmode = 0;

	if (isSelected(x,y) == 1)
	{
		dragmode = 1;
		storeSelection();
		copy_selection(canvas);
	}
	else
	{
		drawOutlineXOR();
		clearSelectionPoints();
		addSelectionPoint(x,y);
	}
}


void update_selection(SDL_Surface *canvas, SDL_Surface *screen, int x, int y)
{
	drawOutlineXOR();
	if (dragmode == 1 && lastx > -1)
	{
		moveSelection(x - lastx, y - lasty);
		// [HACK] i shouldn't be adding 96 here at all
	//	pasteSelection(canvas, screen, x + 96, y, 1, 0);
	}
	else
	{
		addSelectionPoint(x, y);
	}
	drawOutlineXOR();
	lastx = x;
	lasty = y;
}



void finish_selection(SDL_Surface *canvas, SDL_Surface *screen, int x, int y)
{
	if (dragmode == 1)
	{
		paste_from_clipboard(canvas, x - 96, y);
		update_canvas(0, 0, canvas->w, canvas->h);
	//	pasteSelection(canvas, canvas, x, y, 0, 1);
	}
	dragmode = 0;
}


void selectionInit(SDL_Surface *canvas)
{
	set_selection_tool(SELECTTOOL_RECT);
}

void selectionCleanup()
{
	SDL_FreeSurface(clipboard);
	clipboard = NULL;
}

void shrinkToFit(Point *pointArray, int numPoints, SDL_Surface *surface)
{
	
}
