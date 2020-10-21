# About

lua9 is a custom version of the [lua](http://lua.org) interpreter including bindings to plan9 libdraw.  
This relies on the [lua 5.4 port](https://github.com/staalmannen/lua) for plan9 by @staalmannen.

Disclaimer: This is work in progress and is sure to contain bugs.

# Installation

First, install the lua 5.4 port for plan9.  

Clone the sources using the [git9](https://github.com/oridb/git9) client:
```sh
% git/clone git://github.com/telephil9/lua9
```

Install the lua9 interpreter:
```sh
% cd lua9
% mk install
```

# Running

```sh
% ape/lua9 <script.lua>
```

**Note:** shebang scripts do not currently work.

# Credits

- Philippe (@telephil9)
