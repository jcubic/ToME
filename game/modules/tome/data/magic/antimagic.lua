--
-- Antimagic "spells"
--

declare_global_constants
{
	"SCHOOL_ANTIMAGIC",
	"ANTIMAGIC_EXPLODE",
	"ANTIMAGIC_DETECT_TRAPS",
	"ANTIMAGIC_DESTROY_TRAPS",
	"antimagic",
}

antimagic = {}

player.max_antimagic_levels = function()
	if player.inventory[INVEN_MAINHAND][1] and player.inventory[INVEN_MAINHAND][1].flags[FLAG_ANTIMAGIC] then
		local obj = player.inventory[INVEN_MAINHAND][1]

		return 5 + get_skill_scale(SKILL_ANTIMAGIC, obj.flags[FLAG_ANTIMAGIC] - 5)
	end
	return 0
end

player.cur_antimagic_levels = function()
	if player.inventory[INVEN_MAINHAND][1] and player.inventory[INVEN_MAINHAND][1].flags[FLAG_ANTIMAGIC] then
		return player.inventory[INVEN_MAINHAND][1].flags[FLAG_ABSORBED_LEVELS] or 0
	end
	return 0
end

-- Absorb spell levels
hook(hook.ANTIMAGIC_ABSORB, function(m_idx, spl, level)
	if player.inventory[INVEN_MAINHAND][1] and player.inventory[INVEN_MAINHAND][1].flags[FLAG_ANTIMAGIC] then
		local obj = player.inventory[INVEN_MAINHAND][1]
		player.redraw[FLAG_PR_MANA] = true
		player.update = player.update | PU_BONUS
		obj.flags[FLAG_ABSORBED_LEVELS] = (obj.flags[FLAG_ABSORBED_LEVELS] or 0) + max(1, rng(1, level))
		if obj.flags[FLAG_ABSORBED_LEVELS] >= player.max_antimagic_levels() then
			obj.flags[FLAG_ABSORBED_LEVELS] = player.max_antimagic_levels()

			if rng.percent(51 - get_skill(SKILL_ANTIMAGIC, 50)) then
				message(color.LIGHT_RED, "Your "..object_desc(obj, 0, 0).." antimagic field fickles for an instant and then explodes!")
				local damage = obj.flags[FLAG_ABSORBED_LEVELS]
				project(WHO_PLAYER, 3, player.py, player.px, damage, dam.PURE, PROJECT_HIDE + PROJECT_JUMP + PROJECT_KILL + PROJECT_CASTER + PROJECT_NO_RADIUS_DAM)
				obj.flags[FLAG_ABSORBED_LEVELS] = 0
			end
		end
	end
end)

antimagic.guss_custom_power = function(type, amt)
	if type == "get" then return player.cur_antimagic_levels()
	elseif type == "name" then return "antimagic"
	elseif type == "set" then
		if player.inventory[INVEN_MAINHAND][1] and player.inventory[INVEN_MAINHAND][1].flags[FLAG_ANTIMAGIC] then
			local obj = player.inventory[INVEN_MAINHAND][1]
			player.redraw[FLAG_PR_MANA] = true
			player.update = player.update | PU_BONUS
			obj.flags[FLAG_ABSORBED_LEVELS] = (obj.flags[FLAG_ABSORBED_LEVELS] or 0) + amt
			if obj.flags[FLAG_ABSORBED_LEVELS] < 0 then obj.flags[FLAG_ABSORBED_LEVELS] = 0
			elseif obj.flags[FLAG_ABSORBED_LEVELS] > player.max_antimagic_levels() then obj.flags[FLAG_ABSORBED_LEVELS] = player.max_antimagic_levels() end
		end
	end
end

add_mkey
{
	mkey 	= "Antimagic",
	type 	= "skill",
	fct 	= function()
		local book = make_book
		(
			ANTIMAGIC_DETECT_TRAPS,
			ANTIMAGIC_DESTROY_TRAPS,
			ANTIMAGIC_EXPLODE
		)

		local obj = player.inventory[INVEN_MAINHAND][1]
		if not obj or obj.k_idx <= 0 or not obj.flags[FLAG_ANTIMAGIC] then
			message('You must wield darkstone weapon to use antimagic.')
			return
		end

		local spl = nil
		local ret, rep = repeat_keys.pull()
		if ret then
			if is_ok_spell(rep, book) then
				spl = rep
			end
		end

		spl = spl or get_school_spell("cast", "is_ok_spell", book)
		if spl ~= -1 then
			cast_school_spell(spl, spell(spl))
		end

		delete_object(book)
	end,
}

SCHOOL_ANTIMAGIC = add_school
{
	name = "Antimagic",
	skill = "SKILL_ANTIMAGIC",
}

ANTIMAGIC_EXPLODE = add_spell
{
	name = 		"Explosion",
	school = 	{SCHOOL_ANTIMAGIC},
	level = 	20,
	custom_mana = 	player.cur_antimagic_levels,
	custom_power =  antimagic.guss_custom_power,
	fail = 		0,
	blind = 	FALSE,
	antimagic = 	FALSE,
	spell = function()
			message(color.LIGHT_RED, "Your "..object_desc(player.inventory[INVEN_MAINHAND][1], 0, 0).." antimagic field fickles for an instant and then explodes!")
			local damage = player.cur_antimagic_levels()
			
			project(WHO_PLAYER, 3, player.py, player.px, damage, dam.PURE, PROJECT_HIDE + PROJECT_JUMP + PROJECT_KILL + PROJECT_NO_RADIUS_DAM)
			return true
	end,
	info = function()
			return "dam "..player.cur_antimagic_levels()
	end,
	desc =	{
			"Releases the magic entraped in your weapon into a powerful blast.",
	}
}

ANTIMAGIC_DETECT_TRAPS = add_spell
{
	name = 		"Sense traps",
	school = 	{SCHOOL_ANTIMAGIC},
	level = 	5,
	mana = 		1,
	custom_power =  antimagic.guss_custom_power,
	fail = 		0,
	blind = 	FALSE,
	antimagic = 	FALSE,
	spell = function()
			return traps.detect(10 + get_cast_level(40, 0))
	end,
	info = function()
			return "rad "..(10 + get_cast_level(40, 0))
	end,
	desc =	{
			"Sense traps and other such magic contraptions around you.",
	}
}

ANTIMAGIC_DESTROY_TRAPS = add_spell
{
	name = 		"Destroy traps",
	school = 	{SCHOOL_ANTIMAGIC},
	level = 	10,
	mana = 		5,
	custom_power =  antimagic.guss_custom_power,
	fail = 		0,
	blind = 	FALSE,
	antimagic = 	FALSE,
	spell = function()
        		local obvious = false
                        for y = player.py - 1, player.py + 1 do
	                        for x = player.px - 1, player.px + 1 do
                                	if in_bounds(y, x) then
						obvious = is_obvious(traps.disarm_all_location_traps(false, WHO_PLAYER, y, x), obvious)
                                        end
                		end
                        end
			return obvious
	end,
	info = function()
			return ""
	end,
	desc =	{
			"Destroy traps and other such magic contraptions around you.",
	}
}
