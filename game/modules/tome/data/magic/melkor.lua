-- handle the melkor school

declare_global_constants {
	"MELKOR_CORPSE_EXPLOSION",
	"MELKOR_CURSE",
	"MELKOR_MIND_STEAL",
	"do_melkor_curse",
}

-- Not included in the spell code directly because I need to call it from somewhere else too
function do_melkor_curse(who)
	local m_ptr = monster(who)

	if get_level(MELKOR_CURSE) >= 70 then
		local r_ptr = race_info_idx(m_ptr.r_idx, m_ptr.ego)

		m_ptr.maxhp = m_ptr.maxhp - r_ptr.hside;
		if m_ptr.maxhp < 1 then m_ptr.maxhp = 1 end
		if m_ptr.hp > m_ptr.maxhp then m_ptr.hp = m_ptr.maxhp end
		flag_set(player.redraw, PR_HEALTH,1)
	end
	if get_level(MELKOR_CURSE) >= 50 then
		m_ptr.speed = m_ptr.speed - get_level(MELKOR_CURSE, 7)
		m_ptr.mspeed = m_ptr.mspeed - get_level(MELKOR_CURSE, 7)

		if m_ptr.speed < 70 then m_ptr.speed = 70 end
		if m_ptr.mspeed < 70 then m_ptr.mspeed = 70 end
	end
	if get_level(MELKOR_CURSE) >= 30 then
		m_ptr.ac = m_ptr.ac - get_level(MELKOR_CURSE, 50)

		if m_ptr.ac < -70 then m_ptr.ac = -70 end
	end

	local i, pow
	i = 1
	pow = get_level(MELKOR_CURSE, 2)
	while (i <= 4) do
		if m_ptr.blow[i].d_dice > 0 then
			if m_ptr.blow[i].d_dice < pow then
				pow = m_ptr.blow[i].d_dice
			end
			if m_ptr.blow[i].d_side < pow then
				pow = m_ptr.blow[i].d_side
			end
			m_ptr.blow[i].d_dice = m_ptr.blow[i].d_dice - pow
		end
		i = i + 1
	end

	local m_name = monster_desc(m_ptr, 0).." looks weaker."
	message(strupper(strsub(m_name, 0, 1))..strsub(m_name, 2))

	-- wake it
	m_ptr.csleep = 0;
end

MELKOR_CURSE = add_spell
{
	["name"] =      "Curse",
	["school"] =    {SCHOOL_MELKOR},
	["level"] =     1,
	["mana"] =      50,
	["mana_max"] =  300,
	["fail"] = 	20,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end

				if target_who == -1 then
					message("You must target a monster.")
				else
					do_melkor_curse(target_who)
				end
				return true
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"It curses a monster, reducing its melee power",
			"At level 10 it can be auto-casted (with no piety cost) while fighting",
			"At level 30 it also reduces armor",
			"At level 50 it also reduces speed",
			"At level 70 it also reduces max life (but it is never fatal)",
	}
}

MELKOR_CORPSE_EXPLOSION = add_spell
{
	["name"] =      "Corpse Explosion",
	["school"] =    {SCHOOL_MELKOR},
	["level"] =     19,
	["mana"] =      100,
	["mana_max"] =  500,
	["fail"] = 	45,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			return fire_ball(dam.CORPSE_EXPL, 0, 20 + get_cast_level(70), 2 + get_cast_level(5))
	end,
	["info"] = 	function()
			return "dam "..(20 + get_cast_level(70)).."%"
	end,
	["desc"] =	{
			"It makes corpses in an area around you explode for a percent of their",
			"hit points as damage",
	}
}

MELKOR_MIND_STEAL = add_spell
{
	["name"] =      "Mind Steal",
	["school"] =    {SCHOOL_MELKOR},
	["level"] =     39,
	["mana"] =      1000,
	["mana_max"] =  3000,
	["fail"] = 	90,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end

				if target_who == -1 then
					message("You must target a monster.")
				else
					local chance, m_ptr, r_ptr

					m_ptr = monster(target_who)
					r_ptr = race_info_idx(m_ptr.r_idx, m_ptr.ego)
					chance = get_cast_level(50)
					if (rng(m_ptr.level) < chance) and ((r_ptr.flags1 & RF1_UNIQUE) == 0) then
						player.control = target_who
						m_ptr.mflag = m_ptr.mflag | MFLAG_CONTROL

						local m_name = monster_desc(m_ptr, 0).." falls under your control."
						message(strupper(strsub(m_name, 0, 1))..strsub(m_name, 2))
					else
						local m_name = monster_desc(m_ptr, 0).." resists."
						message(strupper(strsub(m_name, 0, 1))..strsub(m_name, 2))
					end
					return true
				end
	end,
	["info"] = 	function()
			return "chance 1d(mlvl)<"..(get_cast_level(50))
	end,
	["desc"] =	{
			"It allows your spirit to temporarily leave your own body, which will",
			"be vulnerable, to control one of your enemies body."
	}
}
