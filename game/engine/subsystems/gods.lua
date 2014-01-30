-- Gods helper files

declare_global_constants {
	"god",
}

-- Gods structs
god = {}
god.__next = 1

god.NONE = 0
god.ALL = -1

function god.new(q)
	local i, index, d, z, qq

	assert(q.name, "No god name")
	assert(q.desc, "No god desc")

        q.hooks = q.hooks or {}

	i = god.__next
        god.__next = god.__next + 1
        god[i] = q
        if q.define then god[q.define] = i end

	for index, d in q.hooks do
		hook(index, d)
	end
	if q.data then
		for index, d in q.data do
			-- Besure it exists
			setglobal(index, d)

			-- Make it save & load
	     		add_loadsave(index, d)
		end
	end
	return i
end

function god.inc_piety(g, amt)
	if g == god.ALL or player.pgod == g then
        	player.grace(player.grace(amt))
        end
end

function god.follow(g, silent)
	if hook.process(hook.FOLLOW_GOD, g, "ask", silent) then return end
        if player.god(god.NONE) then
        	player.pgod = g
                player.grace(0)
                if not silent then message("You are now a follower of "..god[g].name..".") end
                hook.process(hook.FOLLOW_GOD, g, "done", silent)
        end
end

function god.abandon()
	player.pgod = god.NONE
        player.grace(0)
end

function god.pray()
	local altar = cave(player.py, player.px)
	-- On an altar, either get a god or sacrifice
	if cave_feat_is(altar, FLAG_ALTAR) then
		if player.god(god.NONE) then
			local g = cave_feat_get(altar, FLAG_ALTAR)
			term.save()
			term.text_out(god[g].name..":\n")
			for i = 1, getn(god[g].desc) do term.text_out(god[g].desc[i]) end
			term.text_out(color.LIGHT_GREEN, "\nWorship "..god[g].name.."? (y/N)\n")
			local key = term.inkey()
			if key == strbyte('y') then
				god.follow(g)
			end
			term.load()
		else
			hook.process(hook.SACRIFICE_GOD, cave_feat_get(altar, FLAG_ALTAR))
		end
	-- Otherwise pray
	else
		if player.god(god.NONE) then
			message("You do not worship a god.")
		else
			if not player.praying then
				message("You start praying to "..god[player.pgod].name..".")
			else
				message("You stop praying to "..god[player.pgod].name..".")
			end
			player.praying = not player.praying
				-- Update stuffs
			player.update = player.update | PU_BONUS | PU_HP | PU_MANA | PU_SANITY | PU_BODY;
			player.redraw[FLAG_PR_PIETY] = true
			energy_use = 100
		end
	end
end
if not get_subsystem_param("gods", "no_key_bind") then
	hook(hook.KEYPRESS, function(key)
		if key == strbyte('p') then
			return true
		end
	end)
end

-- Grace counter
counter.create
{
	name = "GRACE"
	min = -300000
	max = 300000
	reset = "none"
	on_decrease = function(value, modif)
		player.redraw[FLAG_PR_PIETY] = true
		player.update = player.update | PU_BONUS
		player.window[FLAG_PW_PLAYER] = true
		return true, modif
	end
	on_increase = function(value, modif)
		player.redraw[FLAG_PR_PIETY] = true
		player.update = player.update | PU_BONUS
		player.window[FLAG_PW_PLAYER] = true
		return true, modif
	end
}

function player.grace(val)
	if val then counter.set(counter.GRACE, val) end
	return counter.get(counter.GRACE)
end

player.pgod = god.NONE
player.praying = false
add_loadsave("player.pgod", god.NONE)
add_loadsave("player.praying", false)
hook(hook.BIRTH, function() player.pgod = god.NONE; player.praying = false; player.grace(0) end)

-- Utility
function player.god(god)
	if (player.pgod == god) then return true end
	return nil
end
function player.pray_god(god)
	if (player.praying) and (player.pgod == god) then return true end
	return nil
end
