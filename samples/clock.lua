#!/bin/ape/lua9

local pt = g.pt
local ZP = g.ZP

function eresized()
	local p = g.addpt(screen.r.min, pt(10, 10))
	local s = os.date()
	draw.draw(screen, screen.r, display.white, nil, ZP)
	draw.string(screen, p, display.black, ZP, font, s)
end

draw.init("clock")
event.init(event.KEYBOARD)
local t = event.timer(0, 1000)
eresized()
while true do
	local e, ev = event.event()
	if e == t then
		eresized()
	elseif e == event.KEYBOARD then
		if ev.kbdc == key.DEL then
			os.exit()
		end
	end
end
