-- Take care of displaying the screen

declare_global_constants {
	"SCREEN_MODE_CINEMATIC",
	"SCREEN_MODE_BATTLE",
	"SCREEN_MODE_NORMAL",
}

SCREEN_MODE_NORMAL = record_screen_mode
{
	["name"] = "Normal Screen Mode";
	{
		["name"] = "Wipe",
		["flag"] = FLAG_PR_WIPE,
		["x"] = 0, ["y"] = 0,
		["width"] = function(h, w) return w - 1 end, ["height"] = function(h, w) return h - 1 end,
		["clean"] = FALSE,
		["display"] = function(y, x)
			message()
			term.clear()
		end,
	},
	{
		["name"] = "Map",
		["flag"] = FLAG_PR_MAP,
		["x"] = 13, ["y"] = 1,
		["width"] = function(h, w) return w - 1 - 13 end, ["height"] = function(h, w) return h - 1 - 1 end,
		["clean"] = FALSE,
		["display"] = function(y, x, h, w)
			SCREEN_HGT = 22
			SCREEN_WID = 66
			ROW_MAP = y
			COL_MAP = x
			ROW_MAP_HGT = y + h + 1
			COL_MAP_WID = x + w + 1
			panel_bounds()
			prt_map()
		end,
	},
	{
		-- This doesnt redraw message, just sets up the message zone
		["name"] = "Message",
		["flag"] = FLAG_PR_WIPE,
		["x"] = 0, ["y"] = 0,
		["width"] = function(h, w) return w - 1 end, ["height"] = 1,
		["clean"] = FALSE,
		["display"] = function(y, x, h, w)
			message_row = y
			message_col = x
			message_col_max = w
		end,
	},
	{
		["name"] = "Race",
		["flag"] = { FLAG_PR_MISC, FLAG_PR_BASIC, FLAG_PR_TITLE, FLAG_PR_MIMIC },
		["x"] = 0, ["y"] = 1, ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			local race_name = player.get_race().title
			if player.mimic_form ~= 0 then
				race_name = mimic.get_info(player.mimic_form, "show_name")
			end
			if wizard then
				race_name = "[=-WIZARD-=]"
			elseif winner_state == WINNER_NORMAL then
				race_name = "***WINNER***"
			elseif winner_state == WINNER_ULTRA then
				race_name = "***GOD***"
			end
			term.print(color.LIGHT_BLUE, race_name, y, x)
		end,
	},
	{
		["name"] = "Gold",
		["flag"] = { FLAG_PR_GOLD, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 4, ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			term.print("Gold", y, x)
			term.print(color.LIGHT_GREEN, format("%8d", player.au), y, x + 4)
		end,
	},
	{
		["name"] = "Level",
		["flag"] = { FLAG_PR_LEV, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 2, ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			if player.lev >= player.max_plv then
				term.print("LEVEL", y, x)
				term.print(color.LIGHT_GREEN, format("%6d", player.lev), y, x + 6)
			else
	       			term.print("Level", y, x)
				term.print(color.YELLOW, format("%6d", player.lev), y, x + 6)
			end
		end,
	},
	{
		["name"] = "Exp",
		["flag"] = { FLAG_PR_EXP, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 3, ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			local val
			if exp_need == nil then
				val = format("%8d", player.exp)
			else
				if player.lev >= current_module.max_plev then
					val = "********"
				else
					val = format("%8d", (player_exp[player.lev] * player.expfact / 100) - player.exp)
				end
			end

			if player.exp >= player.max_exp then
				term.print("EXP ", y, x)
				term.print(color.LIGHT_GREEN, val, y, x + 4)
			else
				term.print("Exp ", y, x)
				term.print(color.YELLOW, val, y, x + 4)
			end
		end,
	},
	{
		["name"] = "Stats",
		["flag"] = { FLAG_PR_STATS, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 5, ["width"] = 13, ["height"] = 6,
		["display"] = function(y, x)
			for stat = 0, stats.MAX do
				local tmp = cnv_stat(player.stat_ind[stat])

				-- Display "injured" stat
				if (player.stat_ind[stat] < player.stat_top[stat]) then
					local colour

					if (player.stat_cnt[stat] ~= 0) then
						colour = color.ORANGE
					else
						colour = color.YELLOW
					end

					term.print(format("%s: ", player.stats.def[stat].reduced), y + stat, x)
					term.print(colour, tmp, y + stat, x + 6)
				-- Display "healthy" stat
				else
					term.print(format("%s: ", player.stats.def[stat].short), y + stat, x)
					term.print(color.LIGHT_GREEN, tmp, y + stat, x + 6)
				end

				-- Display "boosted" stat
				if (player.stat_ind[stat] > player.stat_top[stat]) then
					term.print(format("%s: ", player.stats.def[stat].short), y + stat, x)
					term.print(color.VIOLET, tmp, y + stat, x + 6)
				end

				-- Indicate natural maximum
				if (player.stat_top[stat] == stats.MAX_NATURAL) then
					term.print("!", y + stat, x + 3)
				end
			end
		end,
	},
	{
		["name"] = "Armour",
		["flag"] = { FLAG_PR_ARMOR, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 12, ["width"] = 13, ["height"] = 3
		["display"] = function(y, x)
			term.print(color.AQUAMARINE, "AC", y, x)
			term.print(color.LIGHT_GREEN, format("%5d", player.dis_ac + player.dis_to_a), y, x + 7)

			term.print(color.AQUAMARINE, "DR", y + 1, x)
			term.print(color.LIGHT_GREEN, format("%9s", (player.resists[dam.SLASH] or 0).."/"..(player.resists[dam.CRUSH] or 0).."/"..(player.resists[dam.PIERCE] or 0)), y + 1, x + 3)

			term.print(color.AQUAMARINE, "DA", y + 2, x)
			term.print(color.LIGHT_GREEN, format("%9s", (player.absorbs[dam.SLASH] or 0).."/"..(player.absorbs[dam.CRUSH] or 0).."/"..(player.absorbs[dam.PIERCE] or 0)), y + 2, x + 3)
		end,
	},
	{
		["name"] = "HP",
		["flag"] = { FLAG_PR_HP, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 15 ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			local c

			if (player_char_health) then lite_spot(player.py, player.px) end

			if player.undead_form.active then
				term.print(color.LIGHT_DARK, "DP ", y, x)
			else
				term.print(color.RED, "HP ", y, x)
			end

			local tmp = format("%4d/%4d", player.chp(), player.mhp())

			if (player.chp() >= player.mhp()) then
				c = color.LIGHT_GREEN
			elseif (player.chp() > (player.mhp() * hitpoint_warn) / 10) then
				c = color.YELLOW
			else
				c = color.RED
			end
			term.print(c, tmp, y, x + 3)
		end,
	},
	{
		["name"] = "Sanity",
		["flag"] = { FLAG_PR_SANITY, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 16 ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			local c
			local perc

			if (player.msane() <= 0) then
				perc = 100
			else
				perc = (100 * player.csane()) / player.msane()
			end

			term.print(color.ORANGE, "SN ", y, x)

			local tmp = format("%4d/%4d", player.csane(), player.msane())

			if (perc >= 100) then
				c = color.LIGHT_GREEN
			elseif (perc > (10 * hitpoint_warn)) then
				c = color.YELLOW
			else
				c = color.RED
			end

			term.print(c, tmp, y, x + 3)
		end
	},
	{
		["name"] = "SP",
		["flag"] = { FLAG_PR_MANA, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 17 ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			local c, tmp

			-- Normal mana display
			if player.antimagic == 0 then
				term.print(color.LIGHT_GREEN, "SP ", y, x)

				tmp = format("%4d/%4d", player.csp(), player.msp())

				if (player.csp() >= player.msp()) then
					c = color.LIGHT_GREEN
				elseif (player.csp() > (player.msp() * hitpoint_warn) / 10) then
					c = color.YELLOW
				else
					c = color.RED
				end

				term.print(c, tmp, y, x + 3)
			-- Antimagic
			elseif player.inventory[INVEN_MAINHAND][1] and player.inventory[INVEN_MAINHAND][1].flags[FLAG_ANTIMAGIC] then
				local max_am = player.max_antimagic_levels()
				local cur_am = player.cur_antimagic_levels()

				term.print(color.LIGHT_BLUE, "AM ", y, x)

				tmp = format("%4d/%4d", cur_am,max_am)

				if (cur_am == 0) then
					c = color.LIGHT_GREEN
				elseif (max_am - cur_am > (max_am * hitpoint_warn) / 10) then
					c = color.YELLOW
				else
					c = color.RED
				end

				term.print(c, tmp, y, x + 3)
			end
		end
	},
	{
		["name"] = "Piety",
		["flag"] = { FLAG_PR_PIETY, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 18 ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			-- Do not show piety unless it matters
			if (player.pgod == 0) then return end

			term.print(color.LIGHT_WHITE, "Pt ", y, x)

			local tmp = format("%9d", player.grace())

			if player.praying then
				term.print(color.LIGHT_GREEN, tmp, y, x + 3)
			else
				term.print(color.GREEN, tmp, y, x + 3)
			end
		end
	},
	{
		["name"] = "Monster Hitpoints",
		["flag"] = { FLAG_PR_MH, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 19 ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			local symb = player.inventory[INVEN_SYMBIOTE][1]
			if not symb then
				return
			end

			term.print(color.WHITE, "MH ", y, x)

			local monst = symb.flags[FLAG_MONSTER_OBJ]
			if not monst then
				term.print(color.LIGHT_RED, "????/????")
				return
			end

			local tmp = format("%4d/%4d", monst.hp, monst.maxhp)
			if monst.hp == monst.maxhp then
				term.print(color.LIGHT_GREEN, tmp, y, x + 3)
			else
				term.print(color.YELLOW, tmp, y, x + 3)
			end
		end
	},
--[[
	{
		["name"] = "Monster HP",
		["flag"] = { FLAG_PR_MH, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = 19, ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			local c
			local max

			-- Get the carried monster
			local o_ptr = player.inventory[INVEN_CARRY][1]

			if not o_ptr or (get_flag(o_ptr, FLAG_MONSTER_IDX) == 0) then return	end

			local r_ptr = race_info_idx(get_flag(o_ptr, FLAG_MONSTER_IDX), 0)
			max = rng.maxroll(r_ptr.hdice, r_ptr.hside)

			term.print("MH ", y, x)

			local tmp = format("%4d/%4d", get_flag(o_ptr, FLAG_MONSTER_CHP), max)

			if (get_flag(o_ptr, FLAG_MONSTER_CHP) >= max) then
				c = color.LIGHT_GREEN
			elseif (get_flag(o_ptr, FLAG_MONSTER_CHP) > (max * hitpoint_warn) / 10) then
				c = color.YELLOW
			else
				c = color.RED
			end
			term.print(c, tmp, y, x + 3)
		end
	},
]]
	{
		["name"] = "Tracked Health",
		["flag"] = { FLAG_PR_HEALTH, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = function(h, w) return h - 4 end, ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			-- Not tracking
			if (health_who == 0) then
				return

			-- Tracking an unseen monster
			elseif (monster(health_who).ml == nil) then
				-- Indicate that the monster health is "unknown"
				term.putstr(x, y, 12, color.WHITE, "[----------]");

			-- Tracking a hallucinatory monster
			elseif player.has_intrinsic(FLAG_HALLUCINATE) then
				-- Indicate that the monster health is "unknown"
				term.putstr(x, y, 12, color.WHITE, "[----------]");

			-- Tracking a dead monster (???)
			elseif (monster(health_who).hp < 0) then
				-- Indicate that the monster health is "unknown"
				term.putstr(x, y, 12, color.WHITE, "[----------]");

			-- Tracking a visible monster
			else
				local pct, len

				local m_ptr = monster(health_who)

				-- Default to almost dead
				local attr = color.RED

				-- Extract the "percent" of health
				if m_ptr.maxhp <= 0 then
					print(monster_desc(m_ptr, 128) .. ": maxhp = " ..
						  m_ptr.maxhp .. ", health_who = " .. health_who)
					pct = 100
				else
					pct = 100 * m_ptr.hp / m_ptr.maxhp
				end

				-- Badly wounded
				if (pct >= 10) then attr = color.LIGHT_RED end

				-- Wounded
				if (pct >= 25) then attr = color.ORANGE end

				-- Somewhat Wounded
				if (pct >= 60) then attr = color.YELLOW end

				-- Healthy
				if (pct >= 100) then attr = color.LIGHT_GREEN end

				-- Afraid
				if (m_ptr.monfear > 0) then attr = color.VIOLET end

				-- Asleep
				if (m_ptr.csleep > 0) then attr = color.BLUE end

				-- Poisoned
				if (m_ptr.poisoned > 0) then attr = color.GREEN end

				-- Bleeding
				if (m_ptr.bleeding > 0) then attr = color.RED end

				-- Convert percent into "health"
				if pct < 10 then len = 1
				elseif pct < 90 then len = (pct / 10) + 1
				else len = 10
				end

				-- Default to "unknown"
				term.putstr(x, y, 12, color.WHITE, "[----------]")

				-- Dump the current "health" (use '*' symbols)
				term.putstr(x + 1, y, len, attr, "**********")
			end
		end
	},
	{
		["name"] = "Depth",
		["flag"] = { FLAG_PR_DEPTH, FLAG_PR_BASIC, },
		["x"] = function(h, w) return w - 14 end, ["y"] = function(h, w) return h - 1 end,
		["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			local depths
			local d_ptr = dungeon(current_dungeon_idx)
			local level_name = get_dungeon_name()

			if (player.wild_mode) then
				return
			elseif (level_name ~= "") and (has_flag(level_flags, FLAG_SAVE_LEVEL) or (has_flag(level_flags, FLAG_SPECIAL) and generate_special_feeling)) then
				depths = level_name
			elseif (has_flag(level_flags, FLAG_SPECIAL) and generate_special_feeling) then
				depths = "Special"
			elseif (player.inside_quest ~= 0) then
				depths = "Quest"
			elseif (dun_level == 0) then
				if wf_info[1 + wild_map(player.wilderness_y, player.wilderness_x).feat].name ~= "" then
					depths = wf_info[1 + wild_map(player.wilderness_y, player.wilderness_x).feat].name
				else
					depths = "Town/Wild"
				end
			else
				if (has_flag(level_flags, FLAG_TOWER)) then
					depths = format("%c%c%c -%d", d_ptr.short_name[1],
					              d_ptr.short_name[2],
					              d_ptr.short_name[3],
					              dun_level - d_ptr.mindepth + 1)
				else
					depths = format("%c%c%c %d",
					              d_ptr.short_name[1],
					              d_ptr.short_name[2],
					              d_ptr.short_name[3],
					              dun_level - d_ptr.mindepth + 1)
				end
			end

			-- Right-Adjust the "depth", and clear old values
			if (player.word_recall > 0) then
				term.blank_print(color.ORANGE, format("%13s", depths), y, x)
			else
				term.blank_print(format("%13s", depths), y, x)
			end
		end
	},
	{
		["name"] = "Cut",
		["flag"] = { FLAG_PR_CUT, FLAG_PR_EXTRA, },
		["x"] = 0, ["y"] = function(h, w) return h - 3 end, ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			if not timed_effect.get(timed_effect.CUT) then return end
			local c = min(40, max(1, timed_effect.get(timed_effect.CUT) / 10)) * timed_effect.get(timed_effect.CUT, "power")

			if (c > 500) then
				term.print(color.LIGHT_RED, "Mortal wound", y, x)
			elseif (c > 200) then
				term.print(color.RED, "Deep gash", y, x)
			elseif (c > 100) then
				term.print(color.RED, "Severe cut", y, x)
			elseif (c > 50) then
				term.print(color.ORANGE, "Nasty cut", y, x)
			elseif (c > 25) then
				term.print(color.ORANGE, "Bad cut", y, x)
			elseif (c > 10) then
				term.print(color.YELLOW, "Light cut", y, x)
			elseif (c > 0) then
				term.print(color.YELLOW, "Graze", y, x)
			end
		end
	},
	{
		["name"] = "Stun",
		["flag"] = { FLAG_PR_STUN, FLAG_PR_EXTRA, },
		["x"] = 0, ["y"] = function(h, w) return h - 2 end, ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			if not timed_effect.get(timed_effect.STUN) then return end
			local s = min(20, max(1, timed_effect.get(timed_effect.STUN) / 10)) * timed_effect.get(timed_effect.STUN, "power")

			if (s > 20) then
				term.print(color.RED, "Knocked out", y, x)
			elseif (s > 10) then
				term.print(color.ORANGE, "Heavy stun", y, x)
			elseif (s > 0) then
				term.print(color.ORANGE, "Stun", y, x)
			end
		end
	},
	{
		["name"] = "Hunger",
		["flag"] = { FLAG_PR_HUNGER, FLAG_PR_EXTRA, },
		["x"] = 0, ["y"] = function(h, w) return h - 1 end, ["width"] = 7, ["height"] = 1,
		["display"] = function(y, x)
			local food_lvl = timed_effect.get(timed_effect.FOOD) or 0
			-- Fainting / Starving
			if (food_lvl < food.FOOD_FAINT) then
				term.print(color.RED, "Weak!", y, x)

			-- Weak
			elseif (food_lvl < food.FOOD_WEAK) then
				term.print(color.ORANGE, "Weak", y, x)

			-- Hungry
			elseif (food_lvl < food.FOOD_ALERT) then
				term.print(color.YELLOW, "Hungry", y, x)

			-- Normal
			elseif (food_lvl < food.FOOD_FULL) then
				return

			-- Full
			elseif (food_lvl < food.FOOD_MAX) then
				term.print(color.LIGHT_GREEN, "Full", y, x)

			-- Gorged
			else
				term.print(color.GREEN, "Gorged", y, x)
			end
		end
	},
	{
		["name"] = "Blind",
		["flag"] = { FLAG_PR_BLIND, FLAG_PR_EXTRA, },
		["x"] = 7, ["y"] = function(h, w) return h - 1 end,
		["width"] = 6, ["height"] = 1,
		["display"] = function(y, x)
			if player.has_intrinsic(FLAG_BLIND) then
				term.print(color.ORANGE, "Blind", y, x)
			end
		end
	},
	{
		["name"] = "Confused",
		["flag"] = { FLAG_PR_CONFUSED, FLAG_PR_EXTRA, },
		["x"] = 13, ["y"] = function(h, w) return h - 1 end,
		["width"] = 5, ["height"] = 1,
		["display"] = function(y, x)
			if player.has_intrinsic(FLAG_CONFUSED) then
				term.print(color.ORANGE, "Conf", y, x)
			end
		end
	},
	{
		["name"] = "Afraid",
		["flag"] = { FLAG_PR_AFRAID, FLAG_PR_EXTRA, },
		["x"] = 18, ["y"] = function(h, w) return h - 1 end,
		["width"] = 7, ["height"] = 1,
		["display"] = function(y, x)
			if player.has_intrinsic(FLAG_FEAR) then
				term.print(color.ORANGE, "Afraid", y, x)
			end
		end
	},
	{
		["name"] = "Poison",
		["flag"] = { FLAG_PR_POISONED, FLAG_PR_EXTRA, },
		["x"] = 25, ["y"] = function(h, w) return h - 1 end,
		["width"] = 7, ["height"] = 1,
		["display"] = function(y, x)
			if timed_effect.get(timed_effect.POISON) then
				term.print(color.ORANGE, "Poison", y, x)
			end
		end
	},
	{
		["name"] = "Detect Trap",
		["flag"] = { FLAG_PR_DTRAP, FLAG_PR_EXTRA, },
		["x"] = 32, ["y"] = function(h, w) return h - 1 end,
		["width"] = 6, ["height"] = 1,
		["display"] = function(y, x)
			if (cave(player.py, player.px).info & CAVE_DETECT) ~= 0 then
				term.print(color.LIGHT_GREEN, "DTrap", y, x)
			end
		end
	},
	{
		["name"] = "State",
		["flag"] = { FLAG_PR_STATE, FLAG_PR_EXTRA, },
		["x"] = 38, ["y"] = function(h, w) return h - 1 end,
		["width"] = 11, ["height"] = 1,
		["display"] = function(y, x)
			local attr = color.WHITE
			local text = ""

			-- Resting(comes before paralysis, because resting uses paralysis)
			if timed_effect.get(timed_effect.RESTING) then
				local i = timed_effect.get(timed_effect.RESTING)

				-- Start with "Rest"
				text = "Rest "

				-- Rest until done
				local check_type = 
					timed_effect.get(timed_effect.RESTING, "check")
				if check_type == rest.CHECK_ALL then
					text = text.."&&&&&"
				elseif check_type == rest.CHECK_NONE then
					text = text..format("%5d", i)
				else
					text = text.."BUG!!"
				end

			-- Paralysis
			elseif player.has_intrinsic(FLAG_PARALYZED) then
				attr = color.RED
				text = "Paralyzed!"

			-- Repeating
			elseif (command_rep > 0) then
				if (command_rep > 999) then
					text = format("Rep. %3d00", command_rep / 100)
				else
					text = format("Repeat %3d", command_rep)
				end

			-- Searching
			elseif player.movement_mode > 1 then
				text = mov_mode.get_info(player.movement_mode, "name")
			end

			-- Display the info (or blanks)
			term.print(attr, text, y, x)
		end
	},
	{
		["name"] = "Speed",
		["flag"] = { FLAG_PR_SPEED, FLAG_PR_EXTRA, },
		["x"] = 49, ["y"] = function(h, w) return h - 1 end,
		["width"] = 11, ["height"] = 1,
		["display"] = function(y, x)
			local i = player.pspeed
			local attr = color.WHITE
			local buf = ""

			-- Visually "undo" the Search Mode Slowdown
			if mov_mode.current() == "Searching" then i = i + 10 end

			-- Fast
			if (i > 110) then
				attr = color.LIGHT_GREEN
				buf = format("Fast (+%d)", (i - 110))

			-- Slow
			elseif (i < 110) then
				attr = color.LIGHT_UMBER
				buf = format("Slow (-%d)", (110 - i))
			end

			-- Display the speed
			term.print(attr, buf, y, x)
		end
	},
	{
		["name"] = "Skill",
		["flag"] = { FLAG_PR_STUDY, FLAG_PR_EXTRA, },
		["x"] = 60, ["y"] = function(h, w) return h - 1 end,
		["width"] = 6, ["height"] = 1,
		["display"] = function(y, x)
			if (player.skill_points > 0) then
				term.print("Skill", y, x)
			end
		end
	},
}

SCREEN_MODE_BATTLE = overlay_screen_mode(SCREEN_MODE_NORMAL,
{
	{
		["name"] = "Map",
		["flag"] = FLAG_PR_MAP,
		["x"] = 13, ["y"] = 1,
		["width"] = function(h, w) return w - 1 - 13 - 20 end, ["height"] = function(h, w) return h - 1 - 1 end,
		["clean"] = FALSE,
		["display"] = function(y, x, h, w)
			SCREEN_HGT = 22
			SCREEN_WID = 66
			ROW_MAP = y
			COL_MAP = x
			ROW_MAP_HGT = y + h + 1
			COL_MAP_WID = x + w + 1
			panel_bounds()
			prt_map()
		end,
	},
	{
		["name"] = "Battlemode",
		["flag"] = { FLAG_PR_MISC, FLAG_PR_BASIC, FLAG_PR_TITLE },
		["x"] = function(h, w) return w - 21 end, ["y"] = 1, ["width"] = 20, ["height"] = function(h, w) return h - 1 - 2 end,
		["display"] = function(y, x, h, w)
			draw_box(y, x, h, w)
		end,
	},
})

SCREEN_MODE_CINEMATIC = record_screen_mode
{
	["name"] = "Cinematic Screen Mode";
	{
		["name"] = "Wipe",
		["flag"] = FLAG_PR_WIPE,
		["x"] = 0, ["y"] = 0,
		["width"] = function(h, w) return w - 1 end, ["height"] = function(h, w) return h - 1 end,
		["clean"] = FALSE,
		["display"] = function(y, x)
			message()
			term.clear()
		end,
	},
	{
		["name"] = "Map",
		["flag"] = FLAG_PR_MAP,
		["x"] = 0, ["y"] = 3,
		["width"] = function(h, w) return w - 1 end, ["height"] = function(h, w) return h - 1 - 6 end,
		["clean"] = FALSE,
		["display"] = function(y, x, h, w)
			ROW_MAP = y
			COL_MAP = x
			ROW_MAP_HGT = y + h + 1
			COL_MAP_WID = x + w + 1
			panel_bounds()
			prt_map()
		end,
	},
	{
		-- This doesnt redraw message, just sets up the message zone
		["name"] = "Message",
		["flag"] = FLAG_PR_WIPE,
		["x"] = 5, ["y"] = function(h, w) return h - 1 - 2 end,
		["width"] = function(h, w) return w - 1 - 10 end, ["height"] = 1,
		["clean"] = FALSE,
		["display"] = function(y, x, h, w)
			message_row = y
			message_col = x
			message_col_max = w
		end,
	},
}

-- Setup the default one
set_screen_mode(SCREEN_MODE_NORMAL, nil)
