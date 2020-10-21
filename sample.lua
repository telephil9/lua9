#!/bin/ape/lua9

-- lua libdraw sample

function rect(x0, y0, x1, y1)
	return { min = { x = x0, y = y0 }, max = { x = x1, y = y1 } }
end

function pt(x, y)
	return { x = x, y = y }
end

local ZP = pt(0, 0)

function eresized()
	draw.draw(screen, screen.r, display.white, nil, ZP)
	draw.draw(screen, rect(50, 150, 100, 200), display.black, nil, ZP)
	draw.string(screen, pt(110, 160), display.black, nil, font, 'Hello LUA')
	draw.line(screen, pt(50, 210), pt(200, 210), draw.Endsquare, draw.Endarrow, 1, display.black, ZP)
end

draw.initdraw('lua sample')
draw.einit(draw.Emouse|draw.Ekeyboard)
eresized()
while true do
	local e, ev = draw.event()
	if e == draw.Emouse then
		--print 'Mouse event'
	elseif e == draw.Ekeyboard then
		if string.char(ev.kbdc) == 'q' then
			os.exit()
		end
	end
end
