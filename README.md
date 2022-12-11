# Description

lua9 is a custom version of the [lua](http://lua.org) interpreter including bindings to some plan9 libraries.
The supported libraries are:
* libdraw
* libplumb
* lib9 (very partial support)  

__Disclaimer:__ This is work in progress and is sure to contain bugs.

# Installation

lua9 uses kvik's lu9 native port of lua.

```sh
% git/clone git://github.com/telephil9/lua9
% cd lua9
% mk pull
% mk install
```

The binary is installed as `/bin/lua9`.

# Usage

Refer to the [documentation](https://github.com/telephil9/lua9/blob/master/doc/README.md) and to the [samples](https://github.com/telephil9/lua9/tree/master/samples). Most notably look at the [demo](https://github.com/telephil9/lua9/blob/master/samples/demo.lua) script.

Minimal example that simply paints the window black:
```lua
#!/bin/lua9

function eresized()
  draw.draw(screen, screen.r, display.black, nil, g.ZP)
end

draw.init("sample")
event.init(event.KEYBOARD)
eresized()
while true do
  local e, ev = event.event()
  if e == event.KEYBOARD then
    if ev.kbdc == key.DEL then
      os.exit()
    end
  end
end
```

# Credits

* [phil9](http://github.com/telephil9)

# License

[MIT](https://github.com/telephil9/lua9/blob/master/LICENSE)
