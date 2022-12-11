#!/bin/lua9

local pt = g.pt
local ZP = g.ZP

local wstart = 2 -- week start on Monday
local year   = nil
local month  = nil
local today  = os.date("*t")

local pyr = nil
local pmr = nil 
local tr  = nil
local nmr = nil
local nyr = nil

function setrects(p)
	local s = draw.stringsize(font, " ")
	local x0, y0, x1, y1 = p.x, p.y, p.x + 2 * s.x, p.y + s.y
	pyr = g.rect(x0, y0, x1, y1)
	x0 = x1 + 2 * s.x
	x1 = x0 + s.x
	pmr = g.rect(x0, y0, x1, y1)
	x0 = x1 + 2 * s.x
	x1 = x0 + 15 * s.x
	tr = g.rect(x0, y0, x1, y1)
	x0 = x1
	x1 = x0 + s.x
	nmr = g.rect(x0, y0, x1, y1)
	x0 = x1 + 2 * s.x
	x1 = x0 + 2 * s.x
	nyr = g.rect(x0, y0, x1, y1)
end

function dprint(p, s, invert)
	if invert then
		return draw.stringbg(screen, p, display.white, ZP, font, s, display.black, ZP)
	end
	return draw.string(screen, p, col or display.black, ZP, font, s)
end

function eresized()
	local bar = "<<  <      TODAY      >  >>"
	local barw = draw.stringwidth(font, bar)
	local time = os.time({year = year, month = month + 1, day = 0})
	local date = os.date("*t", time)
	local days = date.day
	local start = (date.wday - date.day - wstart + 1) % 7
	local header = os.date("%B %Y", time)
	local p = g.addpt(screen.r.min, pt(10, 10))
	local x = p.x
	draw.draw(screen, screen.r, display.white, nil, ZP)
	p = pt(math.floor(x + (barw - draw.stringwidth(font, header)) / 2), p.y)
	dprint(p, header)
	p = pt(x, p.y + font.height + 5)
	dprint(p, bar)
	setrects(p)
	draw.line(screen, pt(x, p.y - 3), pt(x + barw, p.y - 3), 0, 0, 0, display.black, ZP)
	p = pt(x, p.y + font.height + 5)
	dprint(p, "Mon Tue Wed Thu Fri Sat Sun", true)
	p = pt(p.x, p.y + font.height)
	p = dprint(p, string.rep("    ", start))
	for day = 1, days do
		p = dprint(p, string.format("%3d", day))
		if (day + start) % 7 == 0 then
			p = pt(x, p.y + font.height)
		else
			p = dprint(p, " ")
		end
	end
end

function setdate(m, y)
	month = m
	year  = y
	eresized()
end	

draw.init("calendar")
event.init(event.MOUSE|event.KEYBOARD)
setdate(today.month, today.year)
while true do
	local e, ev = event.event()
	if e == event.MOUSE then
		if ev.mouse.buttons == 1 then
			if     g.ptinrect(ev.mouse.xy, pyr) then setdate(month, year - 1)
			elseif g.ptinrect(ev.mouse.xy, pmr) then setdate(month - 1, year)
			elseif g.ptinrect(ev.mouse.xy, nmr) then setdate(month + 1, year)
			elseif g.ptinrect(ev.mouse.xy, nyr) then setdate(month, year + 1)
			elseif g.ptinrect(ev.mouse.xy, tr)  then setdate(today.month, today.year)
			end
		end
	elseif e == event.KEYBOARD then
		if ev.kbdc == key.DEL then
			os.exit()
		end
	end
end
