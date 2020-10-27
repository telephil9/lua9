# lua9 Documentation

lua9 is a custom [lua](http://lua.org) interpreter including bindings to various plan9 libraries.
This document is not meant to replace the plan9 documentation, please refer to the man pages in [section 2](http://man.9front.org/2/) for the documentation of individual functions.

## Notes

The API is mostly a one-for-one translation of the C API with some slight changes to make the API feel more "lua-ish".
The most significant differences are:
* The API is split across several modules (draw, event, etc.).
* Naming respects lua conventions instead of their original C name.
* Functions that take pointers as out parameters (e.g. event) instead return the value and may return multiple values (e.g. icossin).
* In lua, indices start at one not zero. The API respects the lua convention for function returning indices (e.g. emenuhit)
* Value types (e.g. Point, Rectangle) are mapped as lua tables, reference types (e.g. Image, Font) as userdata.

## API Reference

### Module `draw`

#### `draw.init(title)`
This is the C `initdraw` function.  
The important difference is that it does not handle the additional parameters allowed by `initdraw`.
When called, `init` will check for the existence of an `eresized` function as with the C API.  
Upon success, three global variables are defined: `display`, `screen` and `font`.

#### `draw.draw(dst, r, src, mask, p)`
#### `draw.replxy(min, max, x)`
#### `draw.repl(r, p)`
#### `draw.replclipr(i, r, clipr)`
#### `draw.line(dst, p0, p1, end0, end1, thick, src, sp)`
#### `draw.poly(dst, pts, end0, end1, radius, src, sp)`
#### `draw.fillpoly(dst, pts, wind, src, sp)`
#### `draw.bezier(dst, p0, p1, p2, p3, end0, end1, radius, src, sp)`
#### `draw.fillbezier(dst, p0, p1, p2, p3, w, src, sp)`
#### `draw.bezspline(dst, pts, end0, end1, radius, src, sp)`
#### `draw.fillbezspline(dst, pts, w, src, sp)`
#### `draw.ellipse(dst, c, a, b, thick, src, sp)`
#### `draw.fillellipse(dst, c, a, b, src, sp)`
#### `draw.arc(dst, c, a, b, thick, src, sp, alpha, phi)`
#### `draw.fillarc(dst, c, a, b, src, sp, alpha, phi)`
#### `draw.icossin(deg)`
Returns two values: `cosp` and `sinp`
#### `draw.icossin2(x, y)`
Returns two values: `cosp` and `sinp`
#### `draw.border(dst, r, i, color, sp)`
#### `draw.string(dst, p, src, sp, f, s)`
#### `draw.stringn(dst, p, src, sp, f, s, len)`
#### `draw.stringbg(dst, p, src, sp, f, s, bg, bgp)`
#### `draw.stringnbg(dst, p, src, sp, f, s, len, bg, bgp)`
#### `draw.allocimage(d, r, chan, repl, col)`
#### `draw.allocimagemix(d, one, three)`

#### Constants
The following constants are exported:
* `END_SQUARE`, `END_DISC`, `END_ARROW`, `END_MASK`

### Module `event`

#### `event.init(keys)`
Initialize event handling. This is the `einit` C function.
#### `event.event()`
Returns:
* The event key
* The event structure
#### `event.kbd()`
#### `event.canmouse()`
#### `event.cankbd()`
#### `event.timer(key, n)`
#### `event.enter(ask, s, m)`
The `s` parameter is used as an initial value in the text field.
#### `event.menuhit(but, m, menu)`
The `menu` is expected to be a table of strings. The generation of menu items is not supported currently.  
Returns:
- Selected index (starting at 1) or `nil`

#### Constants
The following constants are exported:
* `MOUSE`, `KEYBOARD`

### Module `g`
Geometry related functions.

#### `g.pt(x, y)`
#### `g.rect(x0, y0, x1, y1)`
#### `g.rpt(p, q)`
#### `g.addpt(p, q)`
#### `g.subpt(p, q)`
#### `g.mulpt(p, a)`
#### `g.divpt(p, a)`
#### `g.rectaddpt(r, p)`
#### `g.rectsubpt(r, p)`
#### `g.insetrect(r, n)`
#### `g.canonrect(r)`
#### `g.eqpt(p, q)`
#### `g.eqrect(r, s)`
#### `g.ptinrect(p, r)`
#### `g.rectinrect(r, s)`
#### `g.rectxrect(r, s)`
#### `g.badrect(r)`
#### `g.dx(r)`
#### `g.dy(r)`

### Module `key`

#### `key.eq(k, str)`
Checks whether k is equal to str. This is a convenience function to help with lua not having a character type.

#### Constants
The following constants are exported:
* `HOME`, `UP`, `DOWN`, `PGUP`, `PRINT`, `LEFT`, `RIGHT`, `PGDOWN`, `INS`, `END`, `SOH`, `STX`, `ETX`, `EOF`, `ENQ`, `ACK`, `BS`, `NACK`, `ETB`, `DEL`, `ESC`.

### Module `color`

#### Constants
The following constants are exported:
* `OPAQUE`, `TRANSPARENT`, `BLACK`, `WHITE`, `RED`, `GREEN`, `BLUE`, `CYAN`, `MAGENTA`, `YELLOW`, `PALE_YELLOW`, `DARK_YELLOW`, `PALE_GREEN`, `DARK_GREEN`, `MED_GREEN`, `DARK_BLUE`, `PALE_BLUE_GREEN`, `PALE_BLUE`, `BLUE_GREEN`, `GREY_GREEN`, `PALE_GREY_GREEN`, `YELLOW_GREEN`, `MED_BLUE`, `GREY_BLUE`, `PALE_GREY_BLUE`, `PURPLE_BLUE`, `NOT_A_COLOR`, `NO_FILL`

