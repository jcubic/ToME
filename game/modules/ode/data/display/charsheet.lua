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

		-- Midscreen info
		[{ 1, 30, "s", 1 }] = { title=character_sheet.section_title("Progression", 20) }
		[{ 2, 30, "d", -20 }] = { title="Level", data="player.lev" color=color.LIGHT_GREEN }
		[{ 3, 30, "d", -20 }] = { title="Exp", data="player.exp" color=color.LIGHT_GREEN }
		[{ 4, 30, "s", -20 }] = { title="Exp to Adv", data=function() if player.lev < 5 then return (player_exp[player.lev] * player.expfact) / 100 else return "****" end end color=color.LIGHT_GREEN }

		-- stats
		[{ 1, 55, "s", 1 }] = { title=character_sheet.section_title("Stats", 24) }
		[{ 2, 55, "s", 0 }] = { title="STR", data="character_sheet.display_stat(A_STR)" color=color.WHITE }
		[{ 2, 55, "s", 0 }] = { title="CON", data="character_sheet.display_stat(A_CON)" color=color.WHITE }

		-- counters
		[{ 9, 30, "s", 1 }] = { title=character_sheet.section_title("Counters", 20) }
		[{ 10,30, "d", -20 }] = { title="Gold", data="player.au" color=color.LIGHT_GREEN }
		[{ 11,30, "s", -20 }] = { title="General Speed", data="player.speed()" color=color.LIGHT_GREEN }
		[{ 12,30, "s", -20 }] = { title="Hit Points", data="player.chp()..'/'..player.mhp()" color=color.LIGHT_GREEN }

		-- combat
		[{ 9, 0, "s", 1 }] = { title=character_sheet.section_title("Combat", 25) }
		[{ 10,0, "d", -25 }] = { title="Melee : To Hit", data=function()
						if player.inventory[INVEN_MAINHAND][1] and is_known(player.inventory[INVEN_MAINHAND][1]) then	return player.dis_to_h + player.inventory[INVEN_MAINHAND][1].to_h end
						return player.dis_to_h
					end color=color.LIGHT_BLUE
		}
		[{ 11,0, "d", -25 }] = { title="Melee : To Damage", data=function()
						if player.inventory[INVEN_MAINHAND][1] and is_known(player.inventory[INVEN_MAINHAND][1]) then return player.dis_to_d + player.inventory[INVEN_MAINHAND][1].to_d end
						return player.dis_to_d
					end color=color.LIGHT_BLUE
		}
		[{ 14,0, "s", -25 }] = { title="Armor Class", data="player.ac..'+'..player.dis_to_a" color=color.LIGHT_BLUE }

		-- combat stats
		[{ 9, 55, "s", 1 }] = { title=character_sheet.section_title("Combat Stats", 24) }
		[{ 10, 55, "d", -24 }] = { title="Blows/Round", data="player.num_blow" color=color.LIGHT_GREEN }
		[{ 11, 55, "s", -24 }] = { title="Melee.Dam/Round", data="get_combat_dam_info()" color=color.LIGHT_GREEN }

		-- generic skills
		[{ 17, 0, "s", 1 }] = { title=character_sheet.section_title("Generic Skills", 79)}
		[{ 18, 0, "s", 1 }] = { title="Fighting    :",
					data=function()
						return "???"
					end color=color.LIGHT_GREEN }
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

-- To format your own character sheet written to file, do this:

--[[
character_sheet.dump_to_file = function(file)
	fs.write(file, current_module.name .. ' ' .. current_module.version[1] ..
	         '.' .. current_module.version[2] .. '.' ..
	         current_module.version[3] .. ' Character Sheet')

	-- etc.
end
]]

-- Useful examples:

--[[
	fs.write(file, "\nEquipment")
	fs.write(file, "---------------")
	for_inventory(player, INVEN_PACK, INVEN_TOTAL, function(obj, i, j, slot)
		fs.write(%file, format("%c) %s", index_to_label(i),
		                                object_desc(obj, true, 3)))
	end)

	fs.write(file, "\nInventory")
	fs.write(file, "---------------")
	for_inventory(player, 0, INVEN_PACK, function(obj, i, j, slot)
		fs.write(%file, format("%c) %s", index_to_label(slot),
		                                object_desc(obj, true, 3)))
	end)

	fs.write(file, "\nKills")
	fs.write(file, "---------------")
	character_sheet.print_kill_count(file)
]]
