-- handle the fire school
-- handle the fire school

declare_global_constants {
	"FIERYAURA",
	"FIREFLASH",
	"FIREGOLEM",
	"FIREWALL",
	"GLOBELIGHT",
}

GLOBELIGHT = add_spell
{
	name =      "Globe of Light",
	school =    {SCHOOL_FIRE},
	level =     1,
	mana =      2,
	mana_max =  15,
	fail =      10,
	stick =
	{
			charge =    { 10, 5 },
			[TV_ORB] =
			{
				rarity =	    7,
				base_level =	{ 1, 15 },
				max_level =		{ 10, 45 },
			},
	},
	inertia = 	{ 1, 40 },
	tactical =
	{
		[ai.tactics.ATTACK] = 6
	}
	spell =     function()
		local obvious
		if get_cast_level(50) >= 3 then
			lite_room(player.py, player.px)
			obvious = true
			if get_cast_level(50) < 15 then
				fire_ball(dam.LITE, 0, 5 + get_cast_level(50), 5 + get_cast_level(6))
			end
		else
			lite_room(player.py, player.px)
			obvious = true
		end
		if get_cast_level(50) >= 15 then
			obvious = is_obvious(fire_ball(dam.LITE, 0, 10 + get_cast_level(100), 5 + get_cast_level(6)), obvious)
		end
		return obvious
	end,
	info =      function()
		if get_cast_level(50) >= 15 then
			return "dam "..(10 + get_cast_level(100)).." rad "..(5 + get_cast_level(6))
		else
			return ""
		end
	end,
	desc =      {
			"Creates a globe of pure light",
			"At level 6 it starts damaging monsters",
			"At level 30 it starts creating a more powerful kind of light",
	}
}

FIREFLASH = add_spell
{
	name =      "Fireflash",
	school =    {SCHOOL_FIRE},
	level =     19,
	mana =      5,
	mana_max =  70,
	fail =      35,
	stick =
	{
			charge =    { 5, 5 },
			[TV_ORB] =
			{
				rarity =	    35,
				base_level =	{ 1, 15 },
				max_level =		{ 15, 35 },
			},
	},
	tactical =
	{
		[ai.tactics.ATTACK] = 9
		[ai.tactics.ANNOY] = 1
	}
	spell =     function()
		local ret, dir, type
		if (get_cast_level(50) >= 20) then
			type = dam.VALINOREAN_FIRE
		else
			type = dam.FIRE
		end
		ret, dir = get_aim_dir()
		if not ret then return SPELL_NOTHING end
		return fire_ball(type, dir, 20 + get_cast_level(500), 2 + get_cast_level(5))
	end,
	info =      function()
		return "dam "..(20 + get_cast_level(500)).." rad "..(2 + get_cast_level(5))
	end,
	desc =      {
			"Conjures a ball of fire to burn your foes to ashes",
			"At level 40 it turns into a ball of valinorean fire"
	}
}

FIERYAURA = add_spell
{
	name =      "Fiery Shield",
	school =    {SCHOOL_FIRE},
	level =     39,
	mana =      20,
	mana_max =  60,
	fail =      50,
	stick =
	{
			charge =    { 3, 5 },
			[TV_ORB] =
			{
				rarity =	    50,
				base_level =	{ 1, 10 },
				max_level =		{ 5, 40 },
			},
	},
	inertia = 	{ 2, 15 },
	tactical =
	{
		[ai.tactics.ENHANCEMENT] = 10
		[ai.tactics.ANNOY] = 3
	}
	spell =     function()
		local type
		if (get_cast_level(50) >= 8) then
			type = AURA_VAL_FIRE
		else
			type = AURA_FIRE
		end
		return timed_effect(timed_effect.AURA, rng(20) + 10 + get_cast_level(70), type, 5 + get_cast_level(15), 5 + get_cast_level(7))
	end,
	info =      function()
		return "dam "..rng.desc_roll(5 + get_cast_level(15), 5 + get_cast_level(7)).." dur "..(10 + get_cast_level(70)).."+d20"
	end,
	desc =      {
			"Creates a shield of fierce flames around you",
			"At level 16 it turns into a greater kind of flame that can not be resisted"
	}
}

FIREWALL = add_spell
{
	name =      "Firewall",
	school =    {SCHOOL_FIRE},
	level =     29,
	mana =      25,
	mana_max =  100,
	fail =      40,
	stick =
	{
			charge =    { 4, 5 },
			[TV_ORB] =
			{
				rarity =	    55,
				base_level =	{ 1, 10 },
				max_level =		{ 5, 40 },
			},
	},
	tactical =
	{
		[ai.tactics.ATTACK] = 6
		[ai.tactics.ANNOY] = 3
	}
	spell =     function()
		local ret, dir, type
		if (get_cast_level(50) >= 6) then
			type = dam.DARK_FIRE
		else
			type = dam.FIRE
		end
		ret, dir = get_aim_dir()
		if not ret then return SPELL_NOTHING end
		fire_wall(type, dir, 40 + get_cast_level(150), 10 + get_cast_level(14), player.speed())
		return true
	end,
	info =      function()
		return "dam "..(40 + get_cast_level(150)).." dur "..(10 + get_cast_level(14))
	end,
	desc =      {
			"Creates a fiery wall to incinerate monsters stupid enough to attack you",
			"At level 12 it turns into a wall of darkfire"
	}
}


FIREGOLEM = add_spell
{
	name =      "Fire Golem",
	school =    {SCHOOL_FIRE, SCHOOL_MIND},
	level =     13,
	mana =      16,
	mana_max =  70,
	fail =      40,
	spell =     function()
			local m_idx, y, x, ret, item

			-- Can we reconnect ?
			if do_control_reconnect() then
				message("Control re-established.")
				monster(player.control).speed = player.pspeed
				return true
			end

			ret, item = get_item("Which light source do you want to use to create the golem?",
					     "You have no light source for the golem",
					     USE_INVEN | USE_EQUIP,
					     function (obj)
						if (obj.tval == TV_LITE) and ((obj.sval == SV_LITE_TORCH) or (obj.sval == SV_LITE_LANTERN)) then
							return true
							end
							return FALSE
					     end
			)
			if not ret then return SPELL_NOTHING end
			item_increase(item, -1)
			item_describe(item)
			item_optimize(item)

			-- Summon it
			set_monster_generation(MONSTER_FIRE_GOLEM, true)
			y, x = find_position(player.py, player.px)
			m_idx = place_monster_one(y, x, MONSTER_FIRE_GOLEM, 0, FALSE, FACTION_PLAYER)
			set_monster_generation(MONSTER_FIRE_GOLEM, true)

			-- level it
			if m_idx ~= 0 then
				monster_set_level(m_idx, 7 + get_cast_level(70))
				player.control = m_idx
				monster(m_idx).speed = player.pspeed
				monster(m_idx).mflag = monster(m_idx).mflag | MFLAG_CONTROL
			end
			return true
	end,
	info =      function()
			return "golem level "..(7 + get_cast_level(70))
	end,
	desc =      {
			"Creates a fiery golem and controls it",
			"During the control the available keylist is:",
			"Movement keys: movement of the golem(depending on its speed",
			"               it can move more than one square)",
			", : pickup all items on the floor",
			"d : drop all carried items",
			"i : list all carried items",
			"m : end the possession/use golem powers",
			"Most of the other keys are disabled, you cannot interact with your",
			"real body while controlling the golem",
			"But to cast the spell you will need a lantern or a wooden torch to",
			"Create the golem from"
	}
}
