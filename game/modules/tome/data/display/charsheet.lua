--	print(num, i, (i * 100) / 5000, imod(i * 2, 100))

-- Character sheet display
set_character_sheet
{
	name = "Character Basic Informations"
	page = 1
	display =
	{
		-- basic char info
		[{ 1, 0, "s", 1 }] = { title=character_sheet.section_title("Character", 26) }
		[{ 2, 0, "s", 1 }] = { title="Name  :", data="player_name()" color=color.LIGHT_BLUE }
		[{ 3, 0, "s", 1 }] = { title="Sex   :", data="player.get_sex().title" color=color.LIGHT_BLUE }
		[{ 4, 0, "s", 1 }] = {
					title="Race  :"
					data=function()
						if player.body_monster.r_idx == 0 then
							return player.get_race_and_background()
						else
							return monster_desc(player.body_monster,
												512 | 256 | 128)
						end
					end
					color=color.LIGHT_BLUE
				     }
		[{ 5, 0, "s", 1 }] = { title="Class :", data="player.get_class().title" color=color.LIGHT_BLUE }
		[{ 6, 0, "s", 1 }] = { title="God   :", condition=function() return player.pgod > 0 end data="god[player.pgod].name" color=color.LIGHT_BLUE }
		[{ 7, 0, "s", 1 }] = { title="Form  :", condition=function() return player.mimic_form ~= 0 end data="mimic.get_info(player.mimic_form, 'show_name')" color=color.LIGHT_BLUE }

		-- Midscreen info
		[{ 1, 30, "s", 1 }] = { title=character_sheet.section_title("Progression", 20) }
		[{ 2, 30, "d", -20 }] = { title="Level", data="player.lev" color=color.LIGHT_GREEN }
		[{ 3, 30, "d", -20 }] = { title="Exp", data="player.exp" color=color.LIGHT_GREEN }
		[{ 4, 30, "s", -20 }] = { title="Exp to Adv", data=function() if player.lev < 50 then return (player_exp[player.lev] * player.expfact) / 100 else return "****" end end color=color.LIGHT_GREEN }

		-- stats
		[{ 1, 55, "s", 1 }] = { title=character_sheet.section_title("Stats", 24) }
		[{ 2, 55, "s", 0 }] = { title="STR", data="character_sheet.display_stat(A_STR)" color=color.WHITE }
		[{ 3, 55, "s", 0 }] = { title="INT", data="character_sheet.display_stat(A_INT)" color=color.WHITE }
		[{ 4, 55, "s", 0 }] = { title="WIS", data="character_sheet.display_stat(A_WIS)" color=color.WHITE }
		[{ 5, 55, "s", 0 }] = { title="DEX", data="character_sheet.display_stat(A_DEX)" color=color.WHITE }
		[{ 6, 55, "s", 0 }] = { title="CON", data="character_sheet.display_stat(A_CON)" color=color.WHITE }
		[{ 7, 55, "s", 0 }] = { title="CHR", data="character_sheet.display_stat(A_CHR)" color=color.WHITE }

		-- counters
		[{ 9, 30, "s", 1 }] = { title=character_sheet.section_title("Counters", 20) }
		[{ 10,30, "d", -20 }] = { title="Gold", data="player.au" color=color.LIGHT_GREEN }
		[{ 11,30, "s", -20 }] = { title="General Speed", data="player.speed()" color=color.LIGHT_GREEN }
		[{ 12,30, "s", -20 }] = { title="Hit Points", data="player.chp()..'/'..player.mhp()" color=color.LIGHT_GREEN }
		[{ 13,30, "s", -20 }] = { title="Mana", data="player.csp()..'/'..player.msp()" color=color.LIGHT_GREEN }
		[{ 14,30, "s", -20 }] = { title="Sanity", data="player.csane()..'/'..player.msane()" color=color.LIGHT_GREEN }
		[{ 15,30, "s", -20 }] = { condition=function() return player.pgod > 0 end title="Piety", data="player.grace()" color=color.LIGHT_GREEN }

		-- combat
		[{ 9, 0, "s", 1 }] = { title=character_sheet.section_title("Combat", 25) }
		[{ 10,0, "d", -25 }] = { title="To Hit", data=function()
						if player.inventory[INVEN_MAINHAND][1] and is_known(player.inventory[INVEN_MAINHAND][1]) then	return player.dis_to_h + player.inventory[INVEN_MAINHAND][1].to_h end
						return player.dis_to_h
					end color=color.LIGHT_BLUE
		}
		[{ 11,0, "d", -25 }] = { title="To Damage", data=function()
						if player.inventory[INVEN_MAINHAND][1] and is_known(player.inventory[INVEN_MAINHAND][1]) then return player.dis_to_d + player.inventory[INVEN_MAINHAND][1].to_d end
						return player.dis_to_d
					end color=color.LIGHT_BLUE
		}
		[{ 12,0, "s", -25 }] = { title="Armor Class", data="player.ac..'+'..player.dis_to_a" color=color.LIGHT_BLUE }
		[{ 13,0, "s", -25 }] = { title="Dam. Reduc.", data=function()
						return (player.resists[dam.SLASH] or 0).."%/"..(player.resists[dam.CRUSH] or 0).."%/"..(player.resists[dam.PIERCE] or 0).."%"
					end color=color.LIGHT_BLUE
		}
		[{ 14,0, "s", -25 }] = { title="Dam. Absorb.", data=function()
						return (player.absorbs[dam.SLASH] or 0).."/"..(player.absorbs[dam.CRUSH] or 0).."/"..(player.absorbs[dam.PIERCE] or 0)
					end color=color.LIGHT_BLUE
		}

		-- combat stats
		[{ 9, 55, "s", 1 }] =    {  title=character_sheet.section_title("Combat: Main hand", 24) }
		[{ 10, 55, "d", -24 }] = { title="Attacks/Round", data="iif(player.combat_style == SKILL_ARCHERY, player.num_fire, player.num_blow)" color=color.LIGHT_GREEN }
		[{ 11, 55, "s", -24 }] = { title="Dam/Attack",    data="get_combat_dam_info(false)" color=color.LIGHT_GREEN }
		-- off hand
		[{ 12, 55, "s", 1 }] =   { condition=combat.default_weapon.is_dual_wield title=character_sheet.section_title("Combat: Off hand", 24) }
		[{ 13, 55, "d", -24 }] = { condition=combat.default_weapon.is_dual_wield title="Attacks/Round", data="iif(player.num_blow >= 2, player.num_blow / 2, player.num_blow)" color=color.LIGHT_GREEN }
		[{ 14, 55, "s", -24 }] = { condition=combat.default_weapon.is_dual_wield title="Dam/Attack",    data="get_combat_dam_info(true)" color=color.LIGHT_GREEN }

		-- generic skills
		[{ 17, 0, "s", 1 }] = { title=character_sheet.section_title("Generic Skills", 79)}
		[{ 18, 0, "s", 1 }] = { title="Fighting    :",
					data=function()
						local weapon = 0
						if player.inventory[INVEN_MAINHAND][1] then weapon = player.inventory[INVEN_MAINHAND][1].to_h end
						return character_sheet.misc_skill(player.skill_thn + (combat.default_weapon.BTH_PLUS_ADJ * (player.to_h + weapon)), 12)
					end color=color.LIGHT_GREEN }
		[{ 20, 0,"s", 1 }] = { title="Spellcasting:",
					data=function()
						local lvls = {
							get_skill(SKILL_MANA),
							get_skill(SKILL_FIRE),
							get_skill(SKILL_EARTH),
							get_skill(SKILL_WATER),
							get_skill(SKILL_AIR),
							get_skill(SKILL_CONVEYANCE),
							get_skill(SKILL_NATURE),
							get_skill(SKILL_DIVINATION),
							get_skill(SKILL_TEMPORAL),
							get_skill(SKILL_META),
							get_skill(SKILL_MIND),
							get_skill(SKILL_UDUN),
							get_skill(SKILL_GEOMANCY),
							get_skill(SKILL_DAEMON),
							get_skill(SKILL_THAUMATURGY),
							get_skill(SKILL_RUNECRAFT),
							get_skill(SKILL_NECROMANCY),
							get_skill(SKILL_SORCERY),
						}
						sort(lvls, function(a,b) return b < a end)
						local lvl = average_value(lvls[1], lvls[2], lvls[3]) + get_skill_scale(SKILL_SPELL, 20)
						return character_sheet.misc_skill(lvl, 2)
					end
					color=color.LIGHT_GREEN
				      }

		[{ 18, 30, "s", 1 }] = { title="Saving Throw:", data="character_sheet.misc_skill(player.skill_sav, 6)" color=color.LIGHT_GREEN }
		[{ 19, 30,"s", 1 }] = { title="Magic Device:", data="character_sheet.misc_skill(player.skill_dev, 6)" color=color.LIGHT_GREEN }
		[{ 20, 30,"s", 1 }] = { title="Disarming   :", data="character_sheet.misc_skill(player.skill_dis, 8)" color=color.LIGHT_GREEN }

		[{ 18, 55,"s", 1 }] = { title="Perception:", data="character_sheet.misc_skill(player.intrinsic(FLAG_SEARCH_FREQ), 2)" color=color.LIGHT_GREEN }
		[{ 19, 55,"s", 1 }] = { title="Searching :", data="character_sheet.misc_skill(player.intrinsic(FLAG_SEARCH_POWER) + ((player.intrinsic(FLAG_SEARCH_POWER_FEAT) + player.intrinsic(FLAG_SEARCH_POWER_TRAP)) / 2), 2)" color=color.LIGHT_GREEN }
		[{ 20, 55,"s", 1 }] = { title="Stealth   :", data="character_sheet.misc_skill(player.skill_stl, 1)" color=color.LIGHT_GREEN }
	}




	keys =
	{
		['c'] = {
			name = "change name"
			action = function()
				local new_name = input_box("New name?", 40, player_name())
				if new_name ~= "" then
					player_name(new_name)
				end
			end
		}
	}
}

-- All Combat stats
set_character_sheet
{
	name = "Combat Statistics Informations"
	page = 2
	display =
	{
		-- combat
		[{ 9, 0, "s", 1 }] = { title=character_sheet.section_title("Combat", 25) }
		[{ 10,0, "d", -25 }] = { title="To Hit", data=function()
						if player.inventory[INVEN_MAINHAND][1] and is_known(player.inventory[INVEN_MAINHAND][1]) then	return player.dis_to_h + player.inventory[INVEN_MAINHAND][1].to_h end
						return player.dis_to_h
					end color=color.LIGHT_BLUE
		}
		[{ 11,0, "d", -25 }] = { title="To Damage", data=function()
						if player.inventory[INVEN_MAINHAND][1] and is_known(player.inventory[INVEN_MAINHAND][1]) then return player.dis_to_d + player.inventory[INVEN_MAINHAND][1].to_d end
						return player.dis_to_d
					end color=color.LIGHT_BLUE
		}
		[{ 12,0, "s", -25 }] = { title="Armor Class", data="player.ac..'+'..player.dis_to_a" color=color.LIGHT_BLUE }
		[{ 13,0, "s", -25 }] = { title="Dam. Reduc.", data=function()
						return (player.resists[dam.SLASH] or 0).."%/"..(player.resists[dam.CRUSH] or 0).."%/"..(player.resists[dam.PIERCE] or 0).."%"
					end color=color.LIGHT_BLUE
		}
		[{ 14,0, "s", -25 }] = { title="Dam. Absorb.", data=function()
						return (player.absorbs[dam.SLASH] or 0).."/"..(player.absorbs[dam.CRUSH] or 0).."/"..(player.absorbs[dam.PIERCE] or 0)
					end color=color.LIGHT_BLUE
		}

		-- combat stats
		[{ 9, 55, "s", 1 }] =    {  title=character_sheet.section_title("Combat: Main hand", 24) }
		[{ 10, 55, "d", -24 }] = { title="Attacks/Round", data="iif(player.combat_style == SKILL_ARCHERY, player.num_fire, player.num_blow)" color=color.LIGHT_GREEN }
		[{ 11, 55, "s", -24 }] = { title="Dam/Attack",    data="get_combat_dam_info(false)" color=color.LIGHT_GREEN }
		[{ 12, 55, "s", -24 }] = { title="Dam/Round",     data="get_combat_dam_info(false, iif(player.combat_style == SKILL_ARCHERY, player.num_fire, player.num_blow))" color=color.LIGHT_GREEN }
		[{ 13, 55, "s", -24 }] = { title="Crit Chance",   data=function()
			if player.combat_style == SKILL_MASTERY then
				local i = combat.default_weapon.calc_crit_chance(player.inventory[INVEN_MAINHAND][1], nil, SKILL_MASTERY)
				return format("%d.%02d%%", (i * 100) / 5000, imod(i * 2, 100))
			else
				return "unknown"
			end
		end color=color.LIGHT_GREEN }

		-- off hand
		[{ 14, 55, "s", 1 }] =   { condition=combat.default_weapon.is_dual_wield title=character_sheet.section_title("Combat: Off hand", 24) }
		[{ 15, 55, "d", -24 }] = { condition=combat.default_weapon.is_dual_wield title="Attacks/Round", data="iif(player.num_blow >= 2, player.num_blow / 2, player.num_blow)" color=color.LIGHT_GREEN }
		[{ 16, 55, "s", -24 }] = { condition=combat.default_weapon.is_dual_wield title="Dam/Attack",    data="get_combat_dam_info(true)" color=color.LIGHT_GREEN }
		[{ 17, 55, "s", -24 }] = { condition=combat.default_weapon.is_dual_wield title="Dam/Round",     data="get_combat_dam_info(true, iif(player.num_blow >= 2, player.num_blow / 2, player.num_blow))" color=color.LIGHT_GREEN }
		[{ 18, 55, "s", -24 }] = { condition=combat.default_weapon.is_dual_wield title="Crit Chance",   data=function()
			if player.combat_style == SKILL_MASTERY then
				local i = combat.default_weapon.calc_crit_chance(player.inventory[INVEN_OFFHAND][1], nil, SKILL_MASTERY)
				return format("%d.%02d%%", (i * 100) / 5000, imod(i * 2, 100))
			else
				return "unknown"
			end
		end color=color.LIGHT_GREEN }

	}
}

--[[
set_character_sheet
{
	name = "Tactics/Exploration Settings and History"
	page = 2
	display =
	{
		-- Tactis
		[{ 4, 6, "s", 1 }] = { title=character_sheet.section_title("Tactic", 26) }
		[{ 5, 6, "s", -26 }] = { title="Name:", data="tactic_info[player.tactic + 1].name" color=color.LIGHT_BLUE }
		[{ 6, 6, "d", -26 }] = { title="To Hit:", data="tactic_info[player.tactic + 1].to_hit" color="auto" }
		[{ 7, 6, "d", -26 }] = { title="To Dam:", data="tactic_info[player.tactic + 1].to_dam" color="auto" }
		[{ 8, 6, "d", -26 }] = { title="To AC:", data="tactic_info[player.tactic + 1].to_ac" color="auto" }
		[{ 9, 6, "d", -26 }] = { title="Stealth:", data="tactic_info[player.tactic + 1].to_stealth" color="auto" }
		[{ 10,6, "d", -26 }] = { title="Disarming:", data="tactic_info[player.tactic + 1].to_disarm" color="auto" }
		[{ 11,6, "d", -26 }] = { title="Saving Throw:", data="tactic_info[player.tactic + 1].to_saving" color="auto" }

		-- Explor mode
		[{ 4, 46, "s", 1 }] = { title=character_sheet.section_title("Exploration", 26) }
		[{ 5, 46, "s", -26 }] = { title="Name:", data="move_info[player.movement + 1].name" color=color.LIGHT_BLUE }
		[{ 6, 46, "d", -26 }] = { title="Speed:", data="move_info[player.movement + 1].to_speed" color="auto" }
		[{ 7, 46, "d", -26 }] = { title="Search:", data="move_info[player.movement + 1].to_search" color="auto" }
		[{ 8, 46, "d", -26 }] = { title="Stealth:", data="move_info[player.movement + 1].to_stealth" color="auto" }
		[{ 9, 46, "d", -26 }] = { title="Perception:", data="move_info[player.movement + 1].to_percep" color="auto" }

		-- Tactis
		[{ 22, 10, "s", 1 }] = { title="['t'/'T' to change tactic, 'e'/'E' to change exploration mode]" }

		-- History
		[{ 16,10,"s", 1 }] = { title=character_sheet.section_title("History", 60)}
		[{ 17,10,"s", 0 }] = { title="", data="history[1]" color=color.WHITE }
		[{ 18,10,"s", 0 }] = { title="", data="history[2]" color=color.WHITE }
		[{ 19,10,"s", 0 }] = { title="", data="history[3]" color=color.WHITE }
		[{ 20,10,"s", 0 }] = { title="", data="history[4]" color=color.WHITE }
	}
	keys =
	{
		['t'] = { hidden=true name="previous tactic" action=function()
				player.tactic = player.tactic - 1
				if player.tactic < 0 then player.tactic = 8 end
				player.update = player.update | PU_BONUS
				update_stuff()
			end
		}
		['T'] = { hidden=true name="previous tactic" action=function()
				player.tactic = player.tactic + 1
				if player.tactic > 8 then player.tactic = 0 end
				player.update = player.update | PU_BONUS
				update_stuff()
			end
		}
		['e'] = { hidden=true name="previous explor" action=function()
				player.movement = player.movement - 1
				if player.movement < 0 then player.movement = 8 end
				player.update = player.update | PU_BONUS
				update_stuff()
			end
		}
		['E'] = { hidden=true name="previous explor" action=function()
				player.movement = player.movement + 1
				if player.movement > 8 then player.movement = 0 end
				player.update = player.update | PU_BONUS
				update_stuff()
			end
		}
	}
}
]]
function character_sheet.not_impl()
	local str  = ""
	local vals = {str = str}

	for_inventory(player, INVEN_MAINHAND, INVEN_TOTAL,
		function(obj, i, j, slot)
			%vals.str = %vals.str .. "?"
		end)

	return vals.str
end

function character_sheet.display_equip_char()
	local str = " "
	local vals = {str = str}

	for_inventory(player, INVEN_MAINHAND, INVEN_TOTAL,
		function(obj, pos, slot, item)
			local c = strchar(pos + strbyte('a') - 1)
			%vals.str = %vals.str .. c
		end)

	return vals.str
end

function character_sheet.display_equip_symb()
	local str = " "
	local vals = {str = str}

	for_inventory(player, INVEN_MAINHAND, INVEN_TOTAL,
		function(obj, pos, slot, item)
			local kind = k_info[obj.k_idx + 1]
			local symb = "#" .. strchar(conv_color[kind.x_attr + 1]) ..
				strchar(kind.x_char)

			%vals.str = %vals.str .. symb
		end)

	return vals.str
end

function character_sheet.resist(dam_type)
	local str, out_color = "", color.WHITE

	-- Collect all known inventory resistances

	local vals = {str = str}
	for_inventory(player, INVEN_MAINHAND, INVEN_TOTAL,
		function(obj, pos, slot, item)
			local val = 0

			if known_flag(obj, FLAG_RESIST) then
				local resists = obj.flags[FLAG_RESIST]

				if flag_is_known(resists, %dam_type) then
					val = resists[%dam_type]
				end
			end

			if val == 0 then
				%vals.str = %vals.str .. "#w."
			elseif val >= 100 then
				-- Immune
				%vals.str = %vals.str .. "#G*"
			elseif val < 0 then
				-- Vulnerable
				%vals.str = %vals.str .. "#rv"
			elseif val > 0 then
				-- Resistant
				%vals.str = %vals.str .. "#g+"
			end
		end) -- for_inventory_inven()

	return vals.str, out_color
end

function character_sheet.value_flag(flag1, flag2, divisor)
	local str, out_color = "", color.WHITE

	divisor = divisor or 1

	local vals = {str = str}
	for_inventory(player, INVEN_MAINHAND, INVEN_TOTAL,
		function(obj, i, j, slot)
			local val = 0

			if known_flag(obj, %flag1) then
				if not %flag2 then
					val = obj.flags[%flag1]
				else
					local flags = obj.flags[%flag1]

					if flag_is_known(flags, %flag2) then
						val = flags[%flag2]
					end
				end
			end

			local disp_val = div_round(val, %divisor)

			if val == 0 then
				-- Neutral
				%vals.str = %vals.str .. "#w"
			elseif val > 0 then
				-- Good
				%vals.str = %vals.str .. "#g"
			elseif val < 0 then
				-- Bad
				%vals.str = %vals.str .. "#r"
			end

			if disp_val == 0 then
				if val == 0 then
					%vals.str = %vals.str .. "."
				elseif val > 0 then
					%vals.str = %vals.str .. "+"
				else
					%vals.str = %vals.str .. "-"
				end
			elseif abs(disp_val) > 9 then
				%vals.str = %vals.str .. "*"
			else
				%vals.str = %vals.str .. abs(disp_val)
			end
		end) -- for_inventory_inven()

	return vals.str, out_color
end -- character_sheet.value_flag()

function character_sheet.bool_flag(flag1, flag2)
	local str, out_color = "", color.WHITE

	local vals = {str = str}
	for_inventory(player, INVEN_MAINHAND, INVEN_TOTAL,
		function(obj, i, j, slot)
			local val = nil

			if known_flag(obj, %flag1) then
				if not %flag2 then
					val = obj.flags[%flag1]
				else
					local flags = obj.flags[%flag1]

					if flag_is_known(flags, %flag2) then
						val = flags[%flag2]
					end
				end
			end

			if val then
				%vals.str = %vals.str .. "+"
			else
				%vals.str = %vals.str .. "."
			end
		end) -- for_inventory_inven()

	return vals.str, out_color
end -- character_sheet.bool_flag()

-- XXX
set_character_sheet
{
	name = "Resistances and Curses"
	page = 3
	display =
	{
		-- Resists
		[{ 1, 0, "s", 0 }] = { title="          ", data="character_sheet.display_equip_char()" color=color.LIGHT_BLUE }
		[{ 2, 0, "s", 0 }] = { title="          ", data="character_sheet.display_equip_symb()" color=color.LIGHT_WHITE }
		--[{ 1, 0, "s", 1 }] = { title=character_sheet.section_title("Resists", 26) }
		[{ 3, 0, "s", 1 }] = { title="Fire     :", data="character_sheet.resist(dam.FIRE)" color=color.WHITE }
		[{ 4, 0, "s", 1 }] = { title="Cold     :", data="character_sheet.resist(dam.COLD)" color=color.WHITE }
		[{ 5, 0, "s", 1 }] = { title="Elec     :", data="character_sheet.resist(dam.ELEC)" color=color.WHITE }
		[{ 6, 0, "s", 1 }] = { title="Acid     :", data="character_sheet.resist(dam.ACID)" color=color.WHITE }
		[{ 7, 0, "s", 1 }] = { title="Poison   :", data="character_sheet.resist(dam.POIS)" color=color.WHITE }

		[{ 8, 0, "s", 1 }] = { title="Fear     :", data="character_sheet.resist(dam.FEAR)" color=color.WHITE }
		[{ 9, 0, "s", 1 }] = { title="Lite     :", data="character_sheet.resist(dam.LITE)" color=color.WHITE }
		[{ 10, 0, "s", 1 }] = { title="Dark     :", data="character_sheet.resist(dam.DARK)" color=color.WHITE }
		--[{ 11,0, "s", 1 }] = { title="Blind    :", data="character_sheet.resist(dam.BLIND, nil)" color=color.WHITE }
		[{ 12,0, "s", 1 }] = { title="Conf     :", data="character_sheet.resist(dam.CONFUSION)" color=color.WHITE }

		[{ 13,0, "s", 1 }] = { title="Sound    :", data="character_sheet.resist(dam.SOUND)" color=color.WHITE }
		--[[
		[{ 13,0, "s", 1 }] = { title="Shards   :", data="character_sheet.resist(nil, FLAG_RES_SHARDS, nil)" color=color.WHITE }
		[{ 14,0, "s", 1 }] = { title="Nether   :", data="character_sheet.resist(nil, FLAG_RES_NETHER, FLAG_IM_NETHER)" color=color.WHITE }
		[{ 15,0, "s", 1 }] = { title="Nexus    :", data="character_sheet.resist(nil, FLAG_RES_NEXUS, nil)" color=color.WHITE }
		[{ 16,0, "s", 1 }] = { title="Chaos    :", data="character_sheet.resist(nil, FLAG_RES_CHAOS, nil)" color=color.WHITE }
		[{ 17,0, "s", 1 }] = { title="Disen    :", data="character_sheet.resist(nil, FLAG_RES_DISEN, nil)" color=color.WHITE }
		[{ 18,0, "s", 1 }] = { title="Hold Life:", data="character_sheet.resist(nil, FLAG_HOLD_LIFE, nil)" color=color.WHITE }
		[{ 19,0, "s", 1 }] = { title="Free Act :", data="character_sheet.resist(nil, FLAG_FREE_ACT, nil)" color=color.WHITE }
		[{ 20,0, "s", 1 }] = { title="Reflect  :", data="character_sheet.resist(nil, FLAG_REFLECT, nil)" color=color.WHITE }

		[{ 1, 40, "s", 0 }] = { title="          ", data="character_sheet.display_equip_char()" color=color.LIGHT_BLUE }
		[{ 2, 40, "s", 1 }] = { title="Hvy/Norn :", data="character_sheet.resist(nil, FLAG_CURSED, FLAG_HEAVY_CURSE)" color=color.WHITE }
		[{ 3, 40, "s", 1 }] = { title="Perm     :", data="character_sheet.resist(nil, FLAG_PERMA_CURSE, nil)" color=color.WHITE }
		[{ 4, 40, "s", 1 }] = { title="Auto     :", data="character_sheet.resist(nil, FLAG_AUTO_CURSE, nil)" color=color.WHITE }
		[{ 5, 40, "s", 1 }] = { title="Morgoth  :", data="character_sheet.resist(nil, FLAG_MORG_CURSE, nil)" color=color.WHITE }
		[{ 6, 40, "s", 1 }] = { title="Foul     :", data="character_sheet.resist(nil, FLAG_FOUL_CURSE, nil)" color=color.WHITE }
		[{ 7, 40, "s", 1 }] = { title="No Drop  :", data="character_sheet.resist(nil, FLAG_CURSE_NO_DROP, nil)" color=color.WHITE }
		[{ 8, 40, "s", 1 }] = { title="Bk.Breath:", data="character_sheet.resist(nil, FLAG_BLACK_BREATH, nil)" color=color.WHITE }
		[{ 9, 40, "s", 1 }] = { title="Drain XP :", data="character_sheet.resist(nil, FLAG_DRAIN_EXP, nil)" color=color.WHITE }
		[{ 10,40, "s", 1 }] = { title="Drain HP :", data="character_sheet.resist(nil, FLAG_DRAIN_LIFE, nil)" color=color.WHITE }
		[{ 11,40, "s", 1 }] = { title="Drain SP :", data="character_sheet.resist(nil, FLAG_DRAIN_MANA, nil)" color=color.WHITE }
		[{ 12,40, "s", 1 }] = { title="No Telep :", data="character_sheet.resist(nil, FLAG_NO_TELEPORT, nil)" color=color.WHITE }
		[{ 13,40, "s", 1 }] = { title="No Magic :", data="character_sheet.resist(nil, FLAG_NO_MAGIC, nil)" color=color.WHITE }
		[{ 14,40, "s", 1 }] = { title="Aggravate:", data="character_sheet.resist(nil, FLAG_AGGRAVATE, nil)" color=color.WHITE }
		[{ 15,40, "s", 1 }] = { title="Clone    :", data="character_sheet.resist(nil, FLAG_CLONE, nil)" color=color.WHITE }
		[{ 16,40, "s", 1 }] = { title="Temporary:", data="character_sheet.resist(nil, FLAG_TEMPORARY, nil)" color=color.WHITE }
		[{ 17,40, "s", 1 }] = { title="Antimagic:", data="character_sheet.resist(nil, FLAG_ANTIMAGIC, nil)" color=color.WHITE }
]]
	}
}

set_character_sheet
{
	name = "Movement, Combat"
	page = 4
	display =
	{
		-- Movement
		[{ 1, 0, "s", 0 }] = { title="          ", data="character_sheet.display_equip_char()" color=color.LIGHT_BLUE }
		[{ 2, 0, "s", 0 }] = { title="          ", data="character_sheet.display_equip_symb()" color=color.LIGHT_BLUE }
		[{ 3, 0, "s", 1 }] = { title="Fly      :", data="character_sheet.value_flag(FLAG_FLY)" color=color.WHITE }
		[{ 4, 0, "s", 1 }] = { title="Climb    :", data="character_sheet.not_impl(nil, FLAG_CLIMB, nil)" color=color.WHITE }
--		[{ 5, 0, "s", 1 }] = { title="Dig      :", data="character_sheet.value_flag(FLAG_TUNNEL)" color=color.WHITE }
		[{ 6, 0, "s", 1 }] = { title="Speed    :", data="character_sheet.not_impl(nil, FLAG_SPEED, nil)" color=color.WHITE }
		[{ 7, 0, "s", 1 }] = { title="Wraith   :", data="character_sheet.not_impl(nil, FLAG_WRAITH, nil)" color=color.WHITE }
		[{ 8, 0, "s", 1 }] = { title="Stealth  :", data="character_sheet.not_impl(nil, FLAG_STEALTH, nil)" color=color.WHITE }
		[{ 9, 0, "s", 1 }] = { title="Teleport :", data="character_sheet.not_impl(nil, FLAG_TELEPORT, nil)" color=color.WHITE }
		[{ 10, 0, "s", 1 }] = { title="Sl.Undead:", data="character_sheet.value_flag(FLAG_SLAY, SLAY_UNDEAD)" color=color.WHITE }
		[{ 11, 0, "s", 1 }] = { title="Sl.Demon :", data="character_sheet.value_flag(FLAG_SLAY, SLAY_DEMON)" color=color.WHITE }
		[{ 12, 0, "s", 1 }] = { title="Sl.Dragon:", data="character_sheet.value_flag(FLAG_SLAY, SLAY_DRAGON)" color=color.WHITE }
		[{ 13, 0, "s", 1 }] = { title="Sl.Orc   :", data="character_sheet.value_flag(FLAG_SLAY, SLAY_ORC)" color=color.WHITE }
		[{ 14, 0, "s", 1 }] = { title="Sl.Troll :", data="character_sheet.value_flag(FLAG_SLAY, SLAY_TROLL)" color=color.WHITE }
		[{ 15, 0, "s", 1 }] = { title="Sl.Giant :", data="character_sheet.value_flag(FLAG_SLAY, SLAY_GIANT)" color=color.WHITE }
		[{ 16, 0, "s", 1 }] = { title="Sl.Evil  :", data="character_sheet.value_flag(FLAG_SLAY, SLAY_EVIL)" color=color.WHITE }
		[{ 17, 0, "s", 1 }] = { title="Sl.Animal:", data="character_sheet.value_flag(FLAG_SLAY, SLAY_ANIMAL)" color=color.WHITE }

		-- Combat
		[{ 1, 40, "s", 0 }] = { title="          ", data="character_sheet.display_equip_char()" color=color.LIGHT_BLUE }
		[{ 2, 40, "s", 0 }] = { title="          ", data="character_sheet.display_equip_symb()" color=color.LIGHT_BLUE }
		[{ 3, 40, "s", 1 }] = { title="Spells   :", data="character_sheet.not_impl(nil, FLAG_SPELL, nil)" color=color.WHITE }
		[{ 4, 40, "s", 1 }] = { title="Blows    :", data="character_sheet.not_impl(nil, FLAG_BLOWS, nil)" color=color.WHITE }
		[{ 5, 40, "s", 1 }] = { title="Crits    :", data="character_sheet.not_impl(nil, FLAG_CRIT, nil)" color=color.WHITE }
		[{ 6, 40, "s", 1 }] = { title="Ammo Mult:", data="character_sheet.not_impl(nil, FLAG_XTRA_MIGHT, nil)" color=color.WHITE }
		[{ 7, 40, "s", 1 }] = { title="Ammo Shot:", data="character_sheet.not_impl(nil, FLAG_XTRA_SHOTS, nil)" color=color.WHITE }
		[{ 8, 40, "s", 1 }] = { title="Vorpal   :", data="character_sheet.not_impl(nil, FLAG_VORPAL, nil)" color=color.WHITE }
		[{ 9, 40, "s", 1 }] = { title="Quake    :", data="character_sheet.not_impl(nil, FLAG_IMPACT, nil)" color=color.WHITE }
		[{ 10, 40, "s", 1 }] = { title="Chaotic  :", data="character_sheet.not_impl(nil, FLAG_CHAOTIC, nil)" color=color.WHITE }
		[{ 11,40, "s", 1 }] = { title="Vampiric :", data="character_sheet.bool_flag(FLAG_VAMPIRIC)" color=color.WHITE }
		[{ 12,40, "s", 1 }] = { title="Poison   :", data="character_sheet.value_flag(FLAG_BRAND, dam.POIS)" color=color.WHITE }
		[{ 13,40, "s", 1 }] = { title="Burns    :", data="character_sheet.value_flag(FLAG_BRAND, dam.FIRE)" color=color.WHITE }
		[{ 14,40, "s", 1 }] = { title="Chills   :", data="character_sheet.value_flag(FLAG_BRAND, dam.COLD)" color=color.WHITE }
		[{ 15,40, "s", 1 }] = { title="Acidic   :", data="character_sheet.value_flag(FLAG_BRAND, dam.ACID)" color=color.WHITE }
		[{ 16,40, "s", 1 }] = { title="Shocks   :", data="character_sheet.value_flag(FLAG_BRAND, dam.ELEC)" color=color.WHITE }
		[{ 17,40, "s", 1 }] = { title="Wound    :", data="character_sheet.not_impl(nil, FLAG_WOUNDING, nil)" color=color.WHITE }
		[{ 18,40, "s", 1 }] = { title="NeverBlow:", data="character_sheet.not_impl(nil, FLAG_NEVER_BLOW, nil)" color=color.WHITE }

	}
}

set_character_sheet
{
	name = "Sight and Miscellaneous"
	page = 5
	display =
	{
		-- Movement
		[{ 1, 0, "s", 0 }] = { title="           ", data="character_sheet.display_equip_char()" color=color.LIGHT_BLUE }
		[{ 2, 0, "s", 0 }] = { title="           ", data="character_sheet.display_equip_symb()" color=color.LIGHT_BLUE }

		[{ 3, 0, "s", 1 }] = { title="See Invis :", data="character_sheet.not_impl(nil, FLAG_SEE_INVIS, nil)" color=color.WHITE }
		[{ 4, 0, "s", 1 }] = { title="Invis     :", data="character_sheet.not_impl(nil, FLAG_INVIS, nil)" color=color.WHITE }
		[{ 5, 0, "s", 1 }] = { title="Infra     :", data="character_sheet.not_impl(nil, FLAG_INFRA, nil)" color=color.WHITE }
		[{ 6, 0, "s", 1 }] = { title="Search    :", data="character_sheet.value_flag(FLAG_SEARCH_POWER)" color=color.WHITE }
		[{ 7, 0, "s", 1 }] = { title="Auto ID   :", data="character_sheet.not_impl(nil, FLAG_AUTO_ID, nil)" color=color.WHITE }
		[{ 8, 0, "s", 1 }] = { title="Light     :", data="character_sheet.value_flag(FLAG_LITE)" color=color.WHITE }
		[{ 9, 0, "s", 1 }] = { title="Sh.Fire   :", data="character_sheet.not_impl(FLAG_BLOW_RESPONSE, AURA_FIRE)" color=color.WHITE }
		[{10, 0, "s", 1 }] = { title="Sh.Elec   :", data="character_sheet.not_impl(FLAG_BLOW_RESPONSE, AURA_ELEC)" color=color.WHITE }
		[{11, 0, "s", 1 }] = { title="Regen     :", data="character_sheet.not_impl(nil, FLAG_REGEN, nil)" color=color.WHITE }
		--[{ 5, 0, "s", 1 }] = { title="SlowDiges:", data="character_sheet.not_impl(nil, FLAG_SLOW_DIGEST, nil)" color=color.WHITE }
		[{12, 0, "s", 1 }] = { title="Precog    :", data="character_sheet.not_impl(nil, FLAG_PRECOGNITION, nil)" color=color.WHITE }
		[{13, 0, "s", 1 }] = { title="Spell In  :", data="character_sheet.not_impl(nil, FLAG_SPELL_CONTAIN, nil)" color=color.WHITE }
		[{14, 0, "s", 1 }] = { title="Blessed   :", data="character_sheet.not_impl(nil, FLAG_BLESSED, nil)" color=color.WHITE }
		[{15, 0, "s", 1 }] = { title="Activate  :", data="character_sheet.not_impl(nil, FLAG_DEVICE_USE, nil)" color=color.WHITE }
		[{16, 0, "s", 1 }] = { title="ResMorgul :", data="character_sheet.not_impl(nil, FLAG_RES_MORGUL, nil)" color=color.WHITE }

		[{ 1, 40, "s", 0 }] = { title="            ", data="character_sheet.display_equip_char()" color=color.LIGHT_BLUE }
		[{ 2, 40, "s", 0 }] = { title="            ", data="character_sheet.display_equip_symb()" color=color.LIGHT_BLUE }
		[{ 3, 40, "s", 1 }] = { title="ESP.Full   :", data="character_sheet.value_flag(FLAG_ESP, FLAG_ESP, 10)" color=color.WHITE }
		[{ 4, 40, "s", 1 }] = { title="ESP.Orc    :", data="character_sheet.value_flag(FLAG_ESP, FLAG_ORC, 10)" color=color.WHITE }
		[{ 5, 40, "s", 1 }] = { title="ESP.Troll  :", data="character_sheet.value_flag(FLAG_ESP, FLAG_TROLL, 10)" color=color.WHITE }
		[{ 6, 40, "s", 1 }] = { title="ESP.Dragon :", data="character_sheet.value_flag(FLAG_ESP, FLAG_DRAGON, 10)" color=color.WHITE }
		[{ 7, 40, "s", 1 }] = { title="ESP.Giant  :", data="character_sheet.value_flag(FLAG_ESP, FLAG_GIANT, 10)" color=color.WHITE }
		[{ 8, 40, "s", 1 }] = { title="ESP.Demon  :", data="character_sheet.value_flag(FLAG_ESP, FLAG_DEMON, 10)" color=color.WHITE }
		[{ 9, 40, "s", 1 }] = { title="ESP.Spider :", data="character_sheet.value_flag(FLAG_ESP, FLAG_SPIDER, 10)" color=color.WHITE }
		[{10, 40, "s", 1 }] = { title="ESP.Evil   :", data="character_sheet.value_flag(FLAG_ESP, FLAG_EVIL, 10)" color=color.WHITE }
		[{11, 40, "s", 1 }] = { title="ESP.Animal :", data="character_sheet.value_flag(FLAG_ESP, FLAG_ANIMAL, 10)" color=color.WHITE }
		[{12, 40, "s", 1 }] = { title="ESP.Thundr :", data="character_sheet.value_flag(FLAG_ESP, FLAG_ANIMAL, 10)" color=color.WHITE }
		[{13, 40, "s", 1 }] = { title="ESP.Good   :", data="character_sheet.value_flag(FLAG_ESP, FLAG_GOOD, 10)" color=color.WHITE }
		[{14, 40, "s", 1 }] = { title="ESP.Nonliv :", data="character_sheet.value_flag(FLAG_ESP, FLAG_NONLIVING, 10)" color=color.WHITE }
		[{15, 40, "s", 1 }] = { title="ESP.Unique :", data="character_sheet.value_flag(FLAG_ESP, FLAG_UNIQUE, 10)" color=color.WHITE }
		[{16, 40, "s", 1 }] = { title="ESP.Hostil :", data="character_sheet.value_flag(FLAG_ESP, FLAG_ESP_HOSTILE, 10)" color=color.WHITE }
		[{17, 40, "s", 1 }] = { title="ESP.Neutr  :", data="character_sheet.value_flag(FLAG_ESP, FLAG_ESP_NEUTRAL, 10)" color=color.WHITE }
		[{18, 40, "s", 1 }] = { title="ESP.Friend :", data="character_sheet.value_flag(FLAG_ESP, FLAG_ESP_FRIENDLY, 10)" color=color.WHITE }
		[{19, 40, "s", 1 }] = { title="ESP.Sleep  :", data="character_sheet.value_flag(FLAG_ESP, FLAG_ESP_SLEEPING, 10)" color=color.WHITE }
		[{20, 40, "s", 1 }] = { title="ESP.Awake  :", data="character_sheet.value_flag(FLAG_ESP, FLAG_ESP_AWAKE, 10)" color=color.WHITE }

	}
}

function character_sheet.speed(speedid)
	local speed = player.speed(speedid)

	if speed > 0 then return '#B+'..speed
	elseif speed < 0 then return '#R'..speed
	end
	return speed
end

local generate_speed_sheet = function()
	local speeds =
	{
		SPEED_GLOBAL,
		SPEED_WALK,

		SPEED_WEAPON,
		SPEED_BAREHAND,
		SPEED_FIRE,
		SPEED_THROW,

		SPEED_CAST,
		SPEED_MIMIC,
		SPEED_MINDCRAFT,
		SPEED_NECRO,
		SPEED_POSSESSION,
		SPEED_POWER,
		SPEED_PRAY,
		SPEED_RUNECRAFT,
		SPEED_SUMMON,

		SPEED_DIG,
		SPEED_DISARM,
		SPEED_EAT,
		SPEED_INVEN,
		SPEED_QUAFF,
		SPEED_READ,
		SPEED_SEARCH,
		SPEED_SENSE,
		SPEED_STEAL,
		SPEED_USE,
	}
	local ret = {}
	local y, x = 2, 0

	for i = 1, getn(speeds) do
		local speedid = speeds[i]
		ret[{ y, x, "s", 1 }] = { title=format("%-25s:", book_capitals(get_speed_desc(speedid))), data="character_sheet.speed("..speedid..")" color=color.WHITE }

		y = y + 1
		if y > 20 then y = 2; x = x + 40 end
	end
	return ret
end

set_character_sheet
{
	name = "Speeds"
	page = 6
	display = generate_speed_sheet()
}
