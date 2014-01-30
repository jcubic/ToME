------------------------------------------------------------------------
-- Current knowledge menu
------------------------------------------------------------------------

load_subsystem("curr_know",
{
	unique_ignore =
		function(monst, r_dix)
			local faction = monst.flags[FLAG_FACTION]

			if faction then
				-- Don't list uniques which are neutral or friendly to
				-- player
				if factions.get_friendliness(faction,
											 FACTION_PLAYER) >= 0
				then
					return true
				end
			end

			return false
		end,

	sort_uniques_by_level =
		function(list)
			-- Sort list by monster level (except Morgoth always goes last),
			-- or by name if the levels are the same
			for i = 1, getn(list) do
				local level = list[i].monst.level

				if strfind(list[i].monst.name, "morgoth") then
					level = 20000
				end

				list[i].pos = level
			end

			sort(list,
				 function(a, b)
					 if a.pos == b.pos then
						 return strlower(a.monst.name) <
							 strlower(b.monst.name)
					 else
						 return a.pos < b.pos
					 end
				 end)

			return list
		end, -- sort_uniques_by_level()

	symbol_to_kill_type =
		function(symbol)
			local table = {
				["!"] = "ERROR (this is a bug)",
				["@"] = "Unusual humanoid",
				["#"] = "Ent/Tree/Column",
				["$"] = "Creeping coin",
				["+"] = "Door mimic/Jumpgate mimic",
				["*"] = "Floating ball",
				["("] = "ERROR (this is a bug)",
				[")"] = "ERROR (this is a bug)",
				["_"] = "Fountain mimic",
				["."] = "Trapper",
				[","] = "Mushroom patch/Fungus",
				[":"] = "Rubble mimic",
				["<"] = "Stair mimic",
				[">"] = "Stair mimic",
				["="] = "ERROR (this is a bug)",
				["["] = "ERROR (this is a bug)",
				["\\"] = "ERROR (this is a bug)",
				["]"] = "ERROR (this is a bug)",
				["|"] = "Animate blade",
				["~"] = "Fish/Whale/Squid/etc",
			}

			return table[symbol] or get_symbol_info(symbol)
		end -- symbol_to_kill_type()
})

add_regexp_plural {
	["mouse$"]  = "mice",
	[" louse$"] = " lice",
	["ungus$"]  = "ungi",

	-- For "vortex", "cortex" and "vertex"
	["rtex$"] = "rtices",

	-- Plurals that are the same as the signular
	["fish$"] = "fish",
}

-- Special pluralization for creeping coins:
--  "creeping foo coins" -> "piles of creeping foo coins"
add_regexp_plural("^(creeping .* coins)$",
				  "piles of %1")

--
-- Set up menu
--
curr_know.add_menu_items(
{
	curr_know.default_menu_items.artifacts,
	curr_know.default_menu_items.uniques,
	curr_know.default_menu_items.objects,
	curr_know.default_menu_items.kill_count,
	curr_know.default_menu_items.recall_depths,

	-- Display corruptions (Corruptions)
	-- Display current pets (Current Pets)
	curr_know.default_menu_items.quests,
	fates.curr_know_menu_item,

	curr_know.default_menu_items.traps,
	curr_know.default_menu_items.dungeon_towns,
	-- Display notes
})						 

