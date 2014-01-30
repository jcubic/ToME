-- The net subsystem
--
-- Load it with: load_subsystem("net")
--
-- This allows for HTTP requests

-- Default http user agent
net.HTTP_USER_AGENT = "T-Engine/"..game_engine.version[1].."."..game_engine.version[2].."."..game_engine.version[3].." Lua Http Library"

-- Process a GET request and return a header table a data buffer
net.http_get = function(ua, host, port, url, callback, call_step)
	local ip = net.resolve_host(host, port)
	if not ip then return nil, "Could not resolve host "..host end
	local conn = net.open(ip)
	if not conn then net.free_ip(ip) return nil, "Could not connect to "..host..":"..port end
	net.send(conn, "GET "..url.." HTTP/1.0\r\n")
	net.send(conn, "Host: "..host.."\r\n")
	net.send(conn, "User-Agent: "..net.HTTP_USER_AGENT.."/"..(ua or "Unknown Script").." "..game_engine.version[1].."."..game_engine.version[2].."."..game_engine.version[3].."\r\n")
	net.send(conn, "Connection: close\r\n\r\n")

	-- If no step size for download is requested then use 500
	local default_step = 500
	local using_default = false

	if not call_step then
		call_step = default_step
		using_default = true
	end

	-- The table that will hold the headers
	local header = {}

	local len, buf = 0, ""
	local _, header_end = nil, nil
	while not nil do
		local read_len, bbuf = net.read(conn, call_step)
		if read_len > 0 then buf = buf..bbuf; len = len + read_len
		else break end

		-- Look for the header end, only when we havent found it yet
		if not header_end then
			_, header_end = strfind(buf, "\r\n\r\n")
			-- Finnaly found it, analize it
			if header_end then
				if using_default then
					-- 500 is too inefficient for large amounts of data
					call_step = 2048
				end

				local head = strsub(buf, 1, header_end)

				-- Extract all header info
				local idx_start, idx_end, line = 0, 0, ""
				while not nil do
					local old_idx = idx_end + 1
					idx_start, idx_end = strfind(head, "\r\n", idx_end + 1)
					if not idx_start then break end
					line = strsub(head, old_idx, idx_start)
					local i, j, name, val = strfind(line, "(.*): (.*)")
					if i then header[name] = val end
				end

				-- Extract the http code too
				local i, j, code = strfind(head, "HTTP/... (...) ")
				if i then header.code = code else header.code = "404" end
			end
		else
			callback(len - header_end, header['Content-Length'] and strsub(header['Content-Length'], 1, strlen(header['Content-Length']) - 1))
		end
	end

	net.close(conn)
	net.free_ip(ip)

	local start, endd = strfind(buf, "\r\n\r\n")
	return header, strsub(buf, endd + 1)
end

-- Process a POST request and return a header table a data buffer
net.http_post = function(ua, host, port, url, data_table, callback, call_step)
	local boundary = "---------------------------1551256570188184812105457895"
	local ip = net.resolve_host(host, port)
	if not ip then return nil, "Could not resolve host "..host end
	local conn = net.open(ip)
	if not conn then net.free_ip(ip) return nil, "Could not connect to "..host..":"..port end

	-- Make up the parts
	local data = ""
	for k, e in data_table do
		data = data .. boundary .. "\r\n"
		if type(e) == "string" then
			data = data .. "Content-Disposition: form-data; name=\""..k.."\"\r\n\r\n"
			data = data .. e .."\r\n"
		else
			data = data .. "Content-Disposition: form-data; name=\""..k.."\"; filename=\""..e.filename.."\"\r\n"
			data = data .. "Content-Type: application/octet-stream\r\n\r\n"
			data = data .. e.data .."\r\n"
		end
	end
	data = data..boundary.."--\r\n"

	net.send(conn, "POST "..url.." HTTP/1.0\r\n")
	net.send(conn, "Host: "..host.."\r\n")
	net.send(conn, "User-Agent: "..net.HTTP_USER_AGENT.."/"..(ua or "Unknown Script").." "..game_engine.version[1].."."..game_engine.version[2].."."..game_engine.version[3].."\r\n")
	net.send(conn, "Connection: close\r\n")
	net.send(conn, "Content-Length: "..strlen(data).."\r\n")
	net.send(conn, "Content-Type: multipart/form-data; boundary="..boundary.."\r\n")
	net.send(conn, "\r\n")
	net.send(conn, data)

	-- If no step size for download is requested then use 500
	call_step = call_step or 500

	-- The table that will hold the headers
	local header = {}

	local len, buf = 0, ""
	local _, header_end = nil, nil
	while not nil do
		local read_len, bbuf = net.read(conn, call_step)
		if read_len > 0 then buf = buf..bbuf; len = len + read_len
		else break end

		-- Look for the header end, only when we havent found it yet
		if not header_end then
			_, header_end = strfind(buf, "\r\n\r\n")
			-- Finnaly found it, analize it
			if header_end then
				local head = strsub(buf, 1, header_end)

				-- Extract all header info
				local idx_start, idx_end, line = 0, 0, ""
				while not nil do
					local old_idx = idx_end + 1
					idx_start, idx_end = strfind(head, "\r\n", idx_end + 1)
					if not idx_start then break end
					line = strsub(head, old_idx, idx_start)
					local i, j, name, val = strfind(line, "(.*): (.*)")
					if i then header[name] = val end
				end

				-- Extract the http code too
				local i, j, code = strfind(head, "HTTP/... (...) ")
				if i then header.code = code else header.code = "404" end
			end
		else
			callback(len - header_end, strsub(header['Content-Length'], 1, strlen(header['Content-Length']) - 1))
		end
	end

	net.close(conn)
	net.free_ip(ip)

	local start, endd = strfind(buf, "\r\n\r\n")
	return header, strsub(buf, endd + 1)
end

function net.http_download(screen_name, ua, host, file, callback, pause_at_end)
	local screen = term.save_to()
	local ret, wid, hgt = term.get_size()
	term.clear()

	term.c_put_str(color.WHITE, screen_name, 0, (wid / 2) - 15);
	term.c_put_str(color.WHITE, "File: "..file, 2, 0);

	term.c_put_str(color.YELLOW, "["..strrep("-", 50).."]", hgt / 2, (wid / 2) - 25);
	term.redraw()

	term.c_put_str(color.WHITE, "Cur Size: "..0, 3, 0)
	term.c_put_str(color.WHITE, "Max Size: Unknown", 4, 0)
	term.redraw()

	local header, data = net.http_get(ua, host, 80, file, callback)

	if not header then
		term.put_str("Download failed!", hgt - 2, (wid / 2) - 7)
		if data then term.put_str(data, hgt - 1, (wid / 2) - (strlen(data) / 2)) end
	else
		term.put_str("Download successfull!", hgt - 1, (wid / 2) - 10)
	end

	if pause_at_end then term.inkey() end

	term.load_from(screen, true)

	if header then return data else return nil end
end
