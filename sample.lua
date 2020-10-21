#!/bin/ape/lua

-- lua libdraw sample

function rect(x0, y0, x1, y1)
	r = { min={}, max={} }
	r.min.x = x0
	r.min.y = y0
	r.max.x = x1
	r.max.y = y1
	return r
end

function eresized()
	draw.draw(screen, screen.r, display.white, nil, {x=0, y=0})
	draw.draw(screen, rect(50, 150, 100, 200), display.black, nil, {x=0, y=0})
	draw.string(screen, {x=110, y=160}, display.black, nil, font, 'Hello LUA')
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
