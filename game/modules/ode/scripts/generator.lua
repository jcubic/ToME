-- Dungeon generator
-- The algorithm is mostly based on rogue dungeon algorithm

constant("doom_gen", {})

doom_gen.rooms = { {}, {}, {}, {}, {}, {}, {}, {}, {}, }

function doom_gen.draw_room(rp)
	local x, y, w, h = rp.x, rp.y, rp.w, rp.h
	for i = x, x + w - 1 do
		for j = y, y + h - 1 do
			local feat = FEAT_FLOOR_ROOM
			if j == y or j == y + h - 1 then
				feat = FEAT_WALL_LR
			elseif i == x or i == x + w - 1 then
				feat = FEAT_WALL_UD
			end
			cave_set_feat(j, i, feat)
			if not rp.dark then cave(j, i).info = cave(j, i).info | CAVE_ROOM | CAVE_GLOW end
		end
	end
end

function doom_gen.rnd_room()
	while not nil do
		local rm = rng(getn(doom_gen.rooms))
		if not doom_gen.rooms[rm].gone then return rm end
    	end
end


function doom_gen.do_rooms()
	-- bsze is the maximum room size
	local bsze = { x = cur_wid / 3, y = cur_hgt / 3 }

	-- Put the gone rooms, if any, on the level
	for i = 1, rng(3) do
--		doom_gen.rooms[doom_gen.rnd_room()].gone = true
	end
	
	-- dig and populate all the rooms on the level
	for  i = 1, getn(doom_gen.rooms) do
		local rp = doom_gen.rooms[i]
		local top = {}
		
		-- Find upper left corner of box that this room goes in
		top.x = imod(i - 1, 3) * bsze.x + 1
		top.y = (i - 1) / 3 * bsze.y
		
		-- Missing room!
		if rp.gone then
			-- Place a gone room.  Make certain that there is a blank line
			-- for passage drawing.
			repeat
				rp.x = top.x + rng(bsze.x - 1)
				rp.y = top.y + rng(bsze.y - 1)
				rp.w = -cur_wid
				rp.h = -cur_hgt
			until rp.y > 0 and rp.y < cur_hgt - 1
			
		-- Complete room
		else
			-- Make it dark! .. boooooh scary!
			if rng(10) < dun_level then rp.dark = true end

			-- Find a place and size for a random room
			repeat
				rp.w = rng(4, bsze.x - 1)
				rp.h = rng(4, bsze.y - 1)
				rp.x = top.x + rng(0, bsze.x - rp.w - 1)
				rp.y = top.y + rng(0, bsze.y - rp.h - 1)
			until rp.y ~= 0

			doom_gen.draw_room(rp)

			local monst_chance = 25

			-- Put the gold in
			if rng.percent(60) then
				place_gold(rng(rp.y + 1, rp.y + rp.h - 1), rng(rp.x + 1, rp.x + rp.w - 1))
				monst_chance = 85
			end

			-- Put the object in
			if rng.percent(40 + dun_level * 7) then
				place_object(rng(rp.y + 1, rp.y + rp.h - 1), rng(rp.x + 1, rp.x + rp.w - 1), false, false, OBJ_FOUND_FLOOR)
				if monst_chance == 85 then
					monst_chance = 98
				else
					monst_chance = 75
				end
			end

			-- Put the monster in
			if rng.percent(monst_chance) then
				local y, x = rng(rp.y + 1, rp.y + rp.h - 1), rng(rp.x + 1, rp.x + rp.w - 1)
				place_monster(y, x, false, false)
				
				-- Double fun!
				if rng.percent(20) then
					y, x = rng(rp.y + 1, rp.y + rp.h - 1), rng(rp.x + 1, rp.x + rp.w - 1)
					place_monster(y, x, false, false)
				end
			end
		end
	end
end


function doom_gen.connect(r1, r2)
	local rpf, rpt
	local rmt
	local distance, turn_spot, turn_distance
	local rm
	local direc
	local delta, curr, turn_delta, spos, epos = {x=0,y=0}, {x=0,y=0}, {x=0,y=0}, {x=0,y=0}, {x=0,y=0}
	
	if r1 < r2 then
		rm = r1
		if r1 + 1 == r2 then
			direc = 'r'
		else
			direc = 'd'
		end
	else
		rm = r2
		if r2 + 1 == r1 then
			direc = 'r'
		else
			direc = 'd'
		end
	end
	rpf = doom_gen.rooms[rm]
	
	-- Set up the movement variables, in two cases:
	-- first drawing one down.
	if direc == 'd' then
		rmt = rm + 3				-- room # of dest
		rpt = doom_gen.rooms[rmt]		-- room pointer of dest
		delta.x = 0				-- direction of move
		delta.y = 1
		spos.x = rpf.x				-- start of move
		spos.y = rpf.y
		epos.x = rpt.x				-- end of move
		epos.y = rpt.y
		if not rpf.gone then			-- if not gone pick door pos
			spos.x = spos.x + rng(1, rpf.w - 1)
			spos.y = spos.y + rpf.h - 1
		end
		if not rpt.gone then
			epos.x = epos.x + rng(1, rpt.w - 1)
		end
		distance = abs(spos.y - epos.y) - 1	-- distance to move
		turn_delta.y = 0			-- direction to turn
		turn_delta.x = iif(spos.x < epos.x, 1, -1)
		turn_distance = abs(spos.x - epos.x)	-- how far to turn
		turn_spot = rng(distance)		-- where turn starts
	elseif (direc == 'r') then			-- setup for moving right
		rmt = rm + 1				-- room # of dest
		rpt = doom_gen.rooms[rmt]		-- room pointer of dest
		delta.x = 1				-- direction of move
		delta.y = 0
		spos.x = rpf.x				-- start of move
		spos.y = rpf.y
		epos.x = rpt.x				-- end of move
		epos.y = rpt.y

		if not rpf.gone then
			spos.x = spos.x + rpf.w - 1
			spos.y = spos.y + rng(1, rpf.h - 1)
		end
		if not rpt.gone then
			epos.y = epos.y + rng(1, rpt.h - 1)
		end
		distance = abs(spos.x - epos.x) - 1
		turn_delta.y = iif(spos.y < epos.y, 1, -1)
		turn_delta.x = 0
		turn_distance = abs(spos.y - epos.y)
		turn_spot = rng(distance)
	else
		error("error in connection tables")
	end

	-- Draw in the doors on either side of the passage or just put #'s
	-- if the rooms are gone.
	if not rpf.gone then
		doom_gen.door(spos)
	else
		doom_gen.tunnel(spos)
	end
	if not rpt.gone then
		doom_gen.door(epos)
	else
		doom_gen.tunnel(epos)
	end
	
	-- Get ready to move...
	curr.x = spos.x
	curr.y = spos.y
	while distance > 0 do
		-- Move to new position
		curr.x = curr.x + delta.x
		curr.y = curr.y + delta.y
		
		-- Check if we are at the turn place, if so do the turn
		if distance == turn_spot and turn_distance > 0 then
			while turn_distance > 0 do
				doom_gen.tunnel(curr)
				curr.x = curr.x + turn_delta.x
				curr.y = curr.y + turn_delta.y
				turn_distance = turn_distance - 1
			end
		end
		
		-- Continue digging along
		doom_gen.tunnel(curr)
		distance = distance - 1
	end
	
	curr.x = curr.x + delta.x
	curr.y = curr.y + delta.y
	
	assert(curr.x == epos.x and curr.y == epos.y, "Warning, connectivity problem on this level.")
end

function doom_gen.door(pos)
	cave_set_feat(pos.y, pos.x, iif(rng(10) < dun_level and rng.percent(20), FEAT_DOOR_LOCKED_HEAD, FEAT_DOOR))
	cave(pos.y, pos.x).info = cave(pos.y, pos.x).info | CAVE_GLOW
end
function doom_gen.tunnel(pos)
	cave_set_feat(pos.y, pos.x, FEAT_FLOOR_TUNNEL)
	cave(pos.y, pos.x).info = cave(pos.y, pos.x).info | CAVE_GLOW
end

function doom_gen.do_tunnels()
	local r1, r2, r1_idx, r2_idx
	local i, j
	local roomcount
	local rdes =
	{
		{ conn = { 0, 1, 0, 1, 0, 0, 0, 0, 0 }, isconn = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, ingraph = 0 },
		{ conn = { 1, 0, 1, 0, 1, 0, 0, 0, 0 }, isconn = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, ingraph = 0 },
		{ conn = { 0, 1, 0, 0, 0, 1, 0, 0, 0 }, isconn = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, ingraph = 0 },
		{ conn = { 1, 0, 0, 0, 1, 0, 1, 0, 0 }, isconn = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, ingraph = 0 },
		{ conn = { 0, 1, 0, 1, 0, 1, 0, 1, 0 }, isconn = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, ingraph = 0 },
		{ conn = { 0, 0, 1, 0, 1, 0, 0, 0, 1 }, isconn = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, ingraph = 0 },
		{ conn = { 0, 0, 0, 1, 0, 0, 0, 1, 0 }, isconn = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, ingraph = 0 },
		{ conn = { 0, 0, 0, 0, 1, 0, 1, 0, 1 }, isconn = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, ingraph = 0 },
		{ conn = { 0, 0, 0, 0, 0, 1, 0, 1, 0 }, isconn = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, ingraph = 0 },
	}
	
	-- Starting with one room, connect it to a random adjacent room and
	-- then pick a new room to start with.
	roomcount = 1
	r1_idx = rng(getn(doom_gen.rooms))
	r1 = rdes[r1_idx]
	r1.ingraph = 1
	repeat
		-- Find a room to connect with
		j = 1
		for i = 1, getn(doom_gen.rooms) do
			if r1.conn[i] == 1 and rdes[i].ingraph == 0 then
				if rng(j) == 1 then
					r2_idx = i
					r2 = rdes[i]
					j = j + 1
				end
			end
		end
		
		-- If no adjacent rooms are outside the graph, pick a new room
		-- to look from
		if j == 1 then
			repeat
				r1_idx = rng(getn(doom_gen.rooms))
				r1 = rdes[r1_idx]
			until r1.ingraph == 1
		
		-- Otherwise, connect new room to the graph, and draw a tunnel
		-- to it
		else
			r2.ingraph = 1
			doom_gen.connect(r1_idx, r2_idx)
			r1.isconn[r2_idx] = 1
			r2.isconn[r1_idx] = 1
			roomcount = roomcount + 1
		end
	until roomcount >= getn(doom_gen.rooms)
	
	-- Attempt to add passages to the graph a random number of times so
	-- that there isn't just one unique passage through it.
	for roomcount = rng(5), 1, -1 do
		r1_idx = rng(getn(doom_gen.rooms))
		r1 = rdes[r1_idx]	-- A random room to look from
		
		-- Find an adjacent room not already connected
		j = 0
		for i = 1, getn(doom_gen.rooms) do
			if r1.conn[i] == 1 and r1.isconn[i] == 0 then
				if rng(j) == 1 then
					r2_idx = i
					r2 = rdes[i]
					j = j + 1
				end
			end
		end
		
		-- If there is one, connect it and look for the next added
		-- passage
		if j ~= 0 then
			doom_gen.connect(r1_idx, r2_idx)
			r1.isconn[r2_idx] = 1
			r2.isconn[r1_idx] = 1
		end
	end
end


level_generator
{
	name = "dungeon_doom"
	stairs = FALSE
	monsters = FALSE
	objects = FALSE
	miscs = FALSE
	
	gen = function()
		for i = 0, MAX_WID - 1 do
			for j = 0, MAX_HGT - 1 do
				cave(j, i).info = 0
				cave_set_feat(j, i, FEAT_WALL)
				cave(j, i).mimic = 0
			end
		end
		
		-- Make up the rooms
		doom_gen.do_rooms()
		
		-- Make up the tunnels
		doom_gen.do_tunnels()
		
		-- Place the stair down
		if dun_level < dungeon(current_dungeon_idx).maxdepth then
			while not nil do
				local rp = doom_gen.rooms[rng(getn(doom_gen.rooms))]
				local y, x = rng(rp.y, rp.y + rp.h), rng(rp.x, rp.x + rp.w)
				if cave(y, x).feat == FEAT_FLOOR_ROOM then
					cave(y, x).feat = FEAT_MORE
					break
				end
			end
		end
		
		-- Place the player
		while not nil do
			local rp = doom_gen.rooms[rng(getn(doom_gen.rooms))]
			local y, x = rng(rp.y, rp.y + rp.h), rng(rp.x, rp.x + rp.w)
			if cave(y, x).feat == FEAT_FLOOR_ROOM then
				player.py = y
				player.px = x
				break
			end
		end
		return true
	end
}
