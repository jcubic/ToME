-- The "no wilderness continuity" subsystem
--
-- Load it with: load_subsystem("no_wilderness_continuity")
--
-- This makes wilderness maps borders send the player to the wilderness map
-- instead of the next wilderness area

hook
{
	[hook.LEVEL_END_GEN] = function()
		if dun_level == 0 and not player.wild_mode then
			local c
			for y = 0, cur_hgt - 1 do
				c = cave(y, 0)
				c.feat, c.mimic = FEAT_WILDERNESS_TO_MAP, (c.mimic > 0) and c.mimic or c.feat
				c = cave(y, cur_wid - 1)
				c.feat, c.mimic = FEAT_WILDERNESS_TO_MAP, (c.mimic > 0) and c.mimic or c.feat
			end
			for x = 0, cur_wid - 1 do
				c = cave(0, x)
				c.feat, c.mimic = FEAT_WILDERNESS_TO_MAP, (c.mimic > 0) and c.mimic or c.feat
				c = cave(cur_hgt - 1, x)
				c.feat, c.mimic = FEAT_WILDERNESS_TO_MAP, (c.mimic > 0) and c.mimic or c.feat
			end
		end
	end

	[hook.INFO_DATA_LOADED] = function()
		new_feature_type
		{
			define_as = "FEAT_WILDERNESS_TO_MAP"
			name = "you should not see this(wild to map)"
			display = '.' color = color.WHITE
			flags =
			{
				WALL=true NO_WALK=true NO_VISION=true PERMANENT=true SUPPORT_LIGHT=true
				DONT_NOTICE_RUNNING=true
				MOVE_PRE_DEST = function()
					term.disturb(0, 0)
					if hook.process(hook.FORBID_TRAVEL) then
						return true
					end
					if get_check("Leave this area?") then
						change_wild_mode()
						return true
					end
					return true
				end
			}
		}
	end
}

-- No wilderness border see through, it's useless
wild_see_through = false
