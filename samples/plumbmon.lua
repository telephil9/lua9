#!/bin/ape/lua9

-- lua rewrite of plumbmon by sirjofri (see http://github.com/sirjofri/plumbmon)

function log_message(port, data)
	local s = string.format("%s [%s] %s", os.date("%Y-%m-%d %H:%M"), port, data)
	print(s)
end

function listen(port)
	fd = plumb.open(port, plan9.OREAD)
	while true do
		local m = plumb.recv(fd)
		log_message(port, m.data)
	end
end

local ports = { "edit", "web", "image", "seemail" }
for k,v in ipairs(ports) do
	plan9.fork(listen, v)
end
