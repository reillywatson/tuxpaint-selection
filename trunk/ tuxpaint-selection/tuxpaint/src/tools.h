/*
  tools.h

  For Tux Paint
  List of available tools.

  Copyright (c) 2002-2007 by Bill Kendrick
  bill@newbreedsoftware.com
  http://www.tuxpaint.org/

  June 14, 2002 - July 17, 2007
  $Id: tools.h,v 1.8 2007/07/26 16:30:36 wkendrick Exp $
*/


#include "tip_tux.h"


/* What tools are available: */

enum
{
  TOOL_BRUSH,
  TOOL_STAMP,
  TOOL_LINES,
  TOOL_SHAPES,
  TOOL_TEXT,
  TOOL_MAGIC,
  TOOL_UNDO,
  TOOL_REDO,
  TOOL_ERASER,
  TOOL_NEW,
  TOOL_OPEN,
  TOOL_SAVE,
  TOOL_PRINT,
  TOOL_QUIT,
  NUM_TOOLS
};


/* Tool names: */

const char *const tool_names[NUM_TOOLS] = {
  // Freehand painting tool
  gettext_noop("Paint"),

  // Stamp tool (aka Rubber Stamps)
  gettext_noop("Stamp"),

  // Line drawing tool
  gettext_noop("Lines"),

  // Shape creation tool (square, circle, etc.)
  gettext_noop("Shapes"),

  // Text tool
  gettext_noop("Text"),

  // "Magic" effects tools (blur, flip image, etc.)
  gettext_noop("Magic"),

  // Undo last action
  gettext_noop("Undo"),

  // Redo undone action
  gettext_noop("Redo"),

  // Eraser tool
  gettext_noop("Eraser"),

  // Start a new picture
  gettext_noop("New"),

  // Open a saved picture
  gettext_noop("Open"),

  // Save the current picture
  gettext_noop("Save"),

  // Print the current picture
  gettext_noop("Print"),

  // Quit/exit Tux Paint application
  gettext_noop("Quit")
};


/* Some text to write when each tool is selected: */

const char *const tool_tips[NUM_TOOLS] = {
  // Paint tool instructions
  gettext_noop("Pick a color and a brush shape to draw with."),

  // Stamp tool instructions
  gettext_noop("Pick a picture to stamp around your drawing."),

  // Line tool instructions
  gettext_noop("Click to start drawing a line. Let go to complete it."),

  // Shape tool instructions
  gettext_noop("Pick a shape. Click to pick the center, drag, then let go when it is the size you want. Move around to rotate it, and click to draw it."),

  // Text tool instructions
  gettext_noop("Choose a style of text. Click on your drawing and you can start typing."),

  // Magic tool instruction
  gettext_noop("Pick a magical effect to use on your drawing!"),

  // Response to 'undo' action
  gettext_noop("Undo!"),

  // Response to 'redo' action
  gettext_noop("Redo!"),

  // Eraser tool
  gettext_noop("Eraser!"),

  // Response to 'start a new image' action
  gettext_noop("Pick a color or picture with which to start a new drawing."),

  // Response to 'open' action (while file dialog is being constructed)
  gettext_noop("Open…"),

  // Response to 'save' action
  gettext_noop("Your image has been saved!"),

  // Response to 'print' action (while printing, or print dialog is being used)
  gettext_noop("Printing…"),

  // Response to 'quit' (exit) action
  gettext_noop("Bye bye!")
};

// Instruction while using Line tool (after click, before release)
#define TIP_LINE_START gettext_noop("Let go of the button to complete the line.")

// Instruction while using Shape tool (after first click, before release)
#define TIP_SHAPE_START gettext_noop("Hold the button to stretch the shape.")

// Instruction while finishing Shape tool (after release, during rotation step before second click)
#define TIP_SHAPE_NEXT gettext_noop("Move the mouse to rotate the shape. Click to draw it.")

// Notification that 'New' action was aborted (current image would have been lost)
#define TIP_NEW_ABORT gettext_noop("OK then… Let’s keep drawing this one!")


/* Tool icon filenames: */

const char *const tool_img_fnames[NUM_TOOLS] = {
  DATA_PREFIX "images/tools/brush.png",
  DATA_PREFIX "images/tools/stamp.png",
  DATA_PREFIX "images/tools/lines.png",
  DATA_PREFIX "images/tools/shapes.png",
  DATA_PREFIX "images/tools/text.png",
  DATA_PREFIX "images/tools/magic.png",
  DATA_PREFIX "images/tools/undo.png",
  DATA_PREFIX "images/tools/redo.png",
  DATA_PREFIX "images/tools/eraser.png",
  DATA_PREFIX "images/tools/new.png",
  DATA_PREFIX "images/tools/open.png",
  DATA_PREFIX "images/tools/save.png",
  DATA_PREFIX "images/tools/print.png",
  DATA_PREFIX "images/tools/quit.png"
};


/* Tux icons to use: */

const int tool_tux[NUM_TOOLS] = {
  TUX_DEFAULT,
  TUX_DEFAULT,
  TUX_DEFAULT,
  TUX_DEFAULT,
  TUX_DEFAULT,
  TUX_DEFAULT,
  TUX_OOPS,
  TUX_WAIT,
  TUX_DEFAULT,
  TUX_DEFAULT,
  TUX_DEFAULT,
  TUX_GREAT,
  TUX_GREAT,
  TUX_DEFAULT
};
