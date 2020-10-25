# About

lua9 is a custom version of the [lua](http://lua.org) interpreter including bindings to plan9 libdraw.  

__Disclaimer:__ This is work in progress and is sure to contain bugs.

# Installation

Make sure to have [git9](http://github.com/oridb/git9] installed first.

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

# Running

```sh
% ape/lua9 <script.lua>
```

You can also start your lua script with `#!/bin/ape/lua9`.

# Documentation

TODO

