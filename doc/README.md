# lua9 API Documentation

lua9 API for plan9 libraries is mostly a one-to-one translation of the C functions. To get documentation for said function please refer to their respective man page in section 2.

## Differences with plan9 C API

The major difference is that exported symbols respect lua naming conventions so all constants are uppercase and words separated by an underscore (e.g. EMouse becomes MOUSE).  
The `initdraw()` function is only called `init()` and only accept a single argument which is the name of the window. The C function allows to specify a custom error function and a font but this is not implemented.

## `draw` module

The `draw` module exposes drawing operations from libdraw.  
The first step is to call `init()`. This will check that an `eresized()` function is defined, do the actual `initdraw()` call and then define the global variables `display`, `screen` and `font` upon success.

### Functions

* init(title)
* draw(dst, r, src, mask, p)
* line(dst, p0, p1, end0, end1, thick, src, sp)
* ellipse(dst, c, a, b, thick, src, sp)
* fillellipse(dst, c, a, b, src, sp)
* string(dst, p, src, sp, f, s, r) -> Point
* allocimage(d, r, chan, repl, col) -> Image

### Constants

* END_SQUARE
* END_DISC
* END_ARROW
* END_MASK

## `event` module

The `event` module exposes all event related functions. The `einit()` function from libdraw is thus `event.init()`.

### Functions

* init(keys)
* event() -> Integer, Event

### Constants

* MOUSE
* KEYBOARD

## `key` module

The `key` module exposes mostly some key constants. It also adds an helper function to check equality with an lua string.

### Functions

* eq(key, str) -> Boolean

### Constants

* HOME
* UP
* DOWN
* PGUP
* PRINT
* LEFT
* RIGHT
* PGDOWN
* INS
* END
* SOH
* STX
* ETX
* EOF
* ENQ
* ACK
* BS
* NACK
* ETB
* DEL
* ESC

## `color` module

The `color` module only exposes color constants.

### Constants

* OPAQUE
* TRANSPARENT
* BLACK
* WHITE
* RED
* GREEN
* BLUE
* CYAN
* MAGENTA
* YELLOW
* PALE_YELLOW
* DARK_YELLOW
* PALE_GREEN
* DARK_GREEN
* MED_GREEN
* DARK_BLUE
* PALE_BLUE_GREEN
* PALE_BLUE
* BLUE_GREEN
* GREY_GREEN
* PALE_GREY_GREEN
* YELLOW_GREEN
* MED_BLUE
* GREY_BLUE
* PALE_GREY_BLUE
* PURPLE_BLUE
* NOT_A_COLOR
* NO_FILL

