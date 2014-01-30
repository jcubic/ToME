-- Take care of displaying the screen

declare_global_constants {
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
		["clean"] = false,
		["display"] = function(y, x)
			message()
			term.clear()
		end,
	},
	{
		["name"] = "Map",
		["flag"] = FLAG_PR_MAP,
		["x"] = 0, ["y"] = 1,
		["width"] = function(h, w) return w - 1 end, ["height"] = function(h, w) return h - 1 - 1 end,
		["clean"] = false,
		["display"] = function(y, x, h, w)
			SCREEN_HGT = 22
			SCREEN_WID = 79
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
		["clean"] = false,
		["display"] = function(y, x, h, w)
			message_row = y
			message_col = x
			message_col_max = w
		end,
	},
	{
		["name"] = "Gold",
		["flag"] = { FLAG_PR_GOLD, FLAG_PR_BASIC, },
		["x"] = 10, ["y"] = function(h, w) return h - 1 end, ["width"] = 13, ["height"] = 1,
		["display"] = function(y, x)
			term.print("Gold: "..player.au, y, x)
		end,
	},
	{
		["name"] = "Level",
		["flag"] = { FLAG_PR_DEPTH, FLAG_PR_BASIC, },
		["x"] = 0, ["y"] = function(h, w) return h - 1 end, ["width"] = 9, ["height"] = 1,
		["display"] = function(y, x)
			term.print("Level: "..tostring(dun_level), y, x)
		end
	},
	{
		["name"] = "Exp",
		["flag"] = { FLAG_PR_EXP, FLAG_PR_BASIC, },
		["x"] = 64, ["y"] = function(h, w) return h - 1 end, ["width"] = 20, ["height"] = 1,
		["display"] = function(y, x)
			term.print("Exp: "..player.lev.."/"..player.exp, y, x)
		end,
	},
	{
		["name"] = "Armour",
		["flag"] = { FLAG_PR_ARMOR, FLAG_PR_BASIC, },
		["x"] = 55, ["y"] = function(h, w) return h - 1 end, ["width"] = 8, ["height"] = 1,
		["display"] = function(y, x)
			term.print("Ac: "..(player.to_a + player.ac), y, x)
		end,
	},
	{
		["name"] = "HP",
		["flag"] = { FLAG_PR_HP, FLAG_PR_BASIC, },
		["x"] = 24, ["y"] = function(h, w) return h - 1 end, ["width"] = 12, ["height"] = 1,
		["display"] = function(y, x)
			term.print("Hp: "..player.chp().."("..player.mhp()..")", y, x)
		end,
	},
	{
		["name"] = "Str",
		["flag"] = { FLAG_PR_STATS, FLAG_PR_BASIC, },
		["x"] = 36, ["y"] = function(h, w) return h - 1 end, ["width"] = 9, ["height"] = 1,
		["display"] = function(y, x)
			term.print("Str: "..player.stat(A_STR), y, x)
		end,
	},
	{
		["name"] = "Con",
		["flag"] = { FLAG_PR_STATS, FLAG_PR_BASIC, },
		["x"] = 45, ["y"] = function(h, w) return h - 1 end, ["width"] = 9, ["height"] = 1,
		["display"] = function(y, x)
			term.print("Con: "..player.stat(A_CON), y, x)
		end,
	},
--[[
	{
		["name"] = "Hunger",
		["flag"] = { FLAG_PR_HUNGER, FLAG_PR_EXTRA, },
		["x"] = 0, ["y"] = function(h, w) return h - 1 end, ["width"] = 7, ["height"] = 1,
		["display"] = function(y, x)
			-- Fainting / Starving
			if (player.food < PY_FOOD_FAINT) then
				term.print(color.RED, "Weak", y, x)

			-- Weak
			elseif (player.food < PY_FOOD_WEAK) then
				term.print(color.ORANGE, "Weak", y, x)

			-- Hungry
			elseif (player.food < PY_FOOD_ALERT) then
				term.print(color.YELLOW, "Hungry", y, x)

			-- Normal
			elseif (player.food < PY_FOOD_FULL) then
				return

			-- Full
			elseif (player.food < PY_FOOD_MAX) then
				term.print(color.LIGHT_GREEN, "Full", y, x)

			-- Gorged
			else
				term.print(color.GREEN, "Gorged", y, x)
			end
		end
	},
]]
}

-- Setup the default one
set_screen_mode(SCREEN_MODE_NORMAL, nil)
