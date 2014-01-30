-- Monster combat
load_subsystem("combat_monster_default")

-- All the monster attack methods
add_monster_attack_method
{
	["name"]	= "HIT",
	["desc"]	= "hit",
	["action"]	= "hits @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
	end,
}
add_monster_attack_method
{
	["name"]	= "STING",
	["desc"]	= "sting",
	["action"]	= "stings @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
	end,
}

-- All monster attack types
add_monster_attack_type
{
	["name"]	= "HURT",
	["desc"]	= "hurt",
	["power"]	= 60,
	["type"]	= dam.MELEE,
	["obvious"]	= true,
	["fct"]		= function(params, special, method, m_idx)
		local ac = player.ac + player.to_a
		params.dam = params.dam - rng(ac / 3, ac)
		if params.dam < 0 then params.dam = 0 end
	end,
}
add_monster_attack_type
{
	["name"]	= "STR_POIS",
	["desc"]	= "poison strength",
	["power"]	= 60,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["fct"]		= function(params, special, method, m_idx)
		local need = 14 - (player.lev / 2)
		if rng.roll(1, 20) >= need then
			message("You feel a sting in your arm!")
			do_dec_stat(A_STR, STAT_DEC_NORMAL)
		end
	end,
}
add_monster_attack_type
{
	["name"]	= "STEAL_GOLD",
	["desc"]	= "steal gold",
	["power"]	= 60,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["fct"]		= function(params, special, method, m_idx)
		if rng.percent(40 - player.lev * 4) then
			message(monster_desc(monster(m_idx), 0) .. " steals some of your gold!")
			player.au = player.au / 2
			teleport_away(m_idx, 10)
		end
	end,
}
add_monster_attack_type
{
	["name"]	= "FIRE",
	["desc"]	= "burn",
	["power"]	= 60,
	["type"]	= dam.FIRE,
	["obvious"]	= true,
}
add_monster_attack_type
{
	["name"]	= "COLD",
	["desc"]	= "freeze",
	["power"]	= 60,
	["type"]	= dam.COLD,
	["obvious"]	= true,
}

function default_monster_drop(m_idx, m_ptr, r_ptr)
	-- No drops
end
