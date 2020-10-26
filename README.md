# Description

lua9 is a custom version of the [lua](http://lua.org) interpreter including bindings to plan9 libdraw.  

__Disclaimer:__ This is work in progress and is sure to contain bugs.

# Installation

Make sure to have [git9](http://github.com/oridb/git9) installed first.

Install the [lua 5.4 port](https://github.com/staalmannen/lua) for plan9 by [staalmannen](https://github.com/staalmannen).  
```sh
% git/clone git://github.com/staalmannen/lua
% cd lua
% mk install
```

Install lua9:  
```sh
% git/clone git://github.com/telephil9/lua9
% cd lua9
% mk install
```

The binary is installed as `/bin/ape/lua9`.

# Usage

Refer to the [documentation](https://github.com/telephil9/lua9/blob/master/doc/README.md) and to the [demo](https://github.com/telephil9/lua9/blob/master/demo.lua) script.

Minimal example that simply paints the window black:
```lua
#!/bin/ape/lua9

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
