#!/bin/ape/lua9

local pt = g.pt
local rect = g.rect
local ZP = g.ZP

function intro()
	local p = g.addpt(screen.r.min, pt(10, 10))
	draw.draw(screen, screen.r, display.white, nil, ZP)
	draw.string(screen, p, display.black, nil, font, "lua9 libdraw demo")
	p = g.addpt(p, pt(0, font.height + 2))
	draw.string(screen, p, display.black, nil, font, "Select a demo from the right-click menu")
end

function lines()
	local p = g.addpt(screen.r.min, pt(10, 10))
	draw.draw(screen, screen.r, display.white, nil, ZP)
	draw.string(screen, p, display.black, nil, font, "draw.line(screen, p0, p1, draw.END_SQUARE, draw.END_ARROW, 2, display.black, ZP)")
	p = g.addpt(p, pt(0, 50))
	draw.line(screen, p, g.addpt(p, pt(200, 0)), draw.END_SQUARE, draw.END_ARROW, 2, display.black, ZP)
end

function polys()
	local p = g.addpt(screen.r.min, pt(10, 10))
	draw.draw(screen, screen.r, display.white, nil, ZP)
	draw.string(screen, p, display.black, nil, font, "draw.poly(screen, { p0, p1, p2, p0 }, draw.END_SQUARE, draw.END_SQUARE, 1, display.black, ZP)")
	p = g.addpt(p, pt(0, 50))
	p1 = g.addpt(p, pt(200, 200))
	p2 = g.addpt(p, pt(0, 200))
	draw.poly(screen, { p, p1, p2, p }, draw.END_SQUARE, draw.END_SQUARE, 0, display.black, ZP)
	p = g.addpt(p, pt(0, 250))
	draw.string(screen, p, display.black, nil, font, "draw.fillpoly(screen, { p0, p1, p2, p3, p4, p0 }, 3, display.black, ZP)")
	p = g.addpt(p, pt(50, 50))
	p1 = g.addpt(p, pt(200, 100))
	p2 = g.addpt(p, pt(100, 150))
	p3 = g.addpt(p, pt(50, 200))
	p4 = g.addpt(p, pt(-50, 50))
	draw.fillpoly(screen, { p, p1, p2, p3, p4, p }, 3, display.black, ZP)
end

function strings()
	local p = g.addpt(screen.r.min, pt(10, 10))
	draw.draw(screen, screen.r, display.white, nil, ZP)
	draw.string(screen, p, display.black, nil, font, "draw.string(screen, p, display.black, ZP, font, 'Hello LUA')")
	p = g.addpt(p, pt(0, font.height + 2))
	draw.string(screen, p, display.black, ZP, font, 'Hello LUA')
	p = g.addpt(p, pt(0, 2 * font.height + 2))
	draw.string(screen, p, display.black, nil, font, "draw.stringn(screen, p, display.black, ZP, font, 'Hello LUA', 5)")
	p = g.addpt(p, pt(0, font.height + 2))
	draw.stringn(screen, p, display.black, ZP, font, 'Hello LUA', 5)
	p = g.addpt(p, pt(0, 2 * font.height + 2))
	draw.string(screen, p, display.black, nil, font, "draw.stringbg(screen, p, display.white, ZP, font, 'Hello LUA', display.black, ZP)")
	p = g.addpt(p, pt(0, font.height + 2))
	draw.stringbg(screen, p, display.white, ZP, font, 'Hello LUA', display.black, ZP)
	p = g.addpt(p, pt(0, 2 * font.height + 2))
	draw.string(screen, p, display.black, nil, font, "draw.stringnbg(screen, p, display.white, ZP, font, 'Hello LUA', 5, display.black, ZP)")
	p = g.addpt(p, pt(0, font.height + 2))
	draw.stringnbg(screen, p, display.white, ZP, font, 'Hello LUA', 5, display.black, ZP)
end

function colors()
	local p = g.addpt(screen.r.min, pt(10, 10))
	draw.draw(screen, screen.r, display.white, nil, ZP)
	draw.string(screen, p, display.black, nil, font, "red = draw.allocimage(display, rect(0,0,1,1), screen.chan, 1, color.RED)")
	p = g.addpt(p, pt(0, font.height + 2))
	draw.string(screen, p, display.black, nil, font, "draw.draw(screen, r, red, nil, ZP)")
	local red = draw.allocimage(display, rect(0,0,1,1), screen.chan, 1, color.RED)
	p = g.addpt(p, pt(0, font.height + 2))
	draw.draw(screen, g.rectaddpt(rect(0,0,50,50), p), red, nil, ZP)
	p = g.addpt(p, pt(0, font.height + 2 + 50))
	draw.string(screen, p, display.black, nil, font, "draw.allocimagemix(display, color.BLUE, color.YELLOW)")
	local c = draw.allocimagemix(display, color.BLUE, color.YELLOW)
	p = g.addpt(p, pt(0, font.height + 2))
	draw.draw(screen, g.rectaddpt(rect(0,0,50,50), p), c, nil, ZP)
end

local index = nil
local menu = { "line", "poly", "string", "colors", "exit" }
local demos = { lines, polys, strings, colors, os.exit }

function eresized()
	if index then
		demos[index]()
	else
		intro()
	end
end

draw.init("lua9 demo")
event.init(event.MOUSE|event.KEYBOARD)
eresized()
while true do
	local e, ev = event.event()
	if e == event.MOUSE then
		if ev.mouse.buttons == 4 then
			index = event.menuhit(3, ev.mouse, menu)
			if index then
				demos[index]()
			end
		end	
	elseif e == event.KEYBOARD then
		if key.eq(ev.kbdc, 'q') or ev.kbdc == key.DEL then
			os.exit()
		end
	end
end
