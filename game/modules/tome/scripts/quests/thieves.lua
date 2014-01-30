-- Investigate the abandoned house

add_quest
{
	global = "QUEST_THIEVES"
	name = "Oh no! More thieves!"
	desc =
	{
		"There is an old abandoned house in town.",
		"Usually, nobody goes there. Lately, however there seems",
		"to be some activity - masked people going in or out",
		"at night. Please investigate further."
	}
	level = 12
	hooks =
	{
		init = function()
			cave_set_feat(37, 101, FEAT_MORE)
			cave(37, 101).flags[FLAG_DUNGEON] = DUNGEON_THIEVES_LAIR
		end

		ring_alarm = function()
			local status = {}
			for_each_monster(function(m_idx, monst)
				if monst.csleep > 0 then
					monst.csleep = 0
					%status.wakeup = true
				end
			end)
			if status.wakeup then
				message("You hear people shouting from the nearby rooms!")
			end

			cave_set_feat(11, 18, FEAT_OPEN)
			cave_set_feat(11, 14, FEAT_OPEN)
			cave_set_feat(11, 10, FEAT_OPEN)
			cave_set_feat(11, 6, FEAT_OPEN)
			cave_set_feat(17, 18, FEAT_OPEN)
			cave_set_feat(17, 14, FEAT_OPEN)
			cave_set_feat(17, 10, FEAT_OPEN)
			cave_set_feat(17, 6, FEAT_OPEN)
			message("The door explodes.")
			cave_set_feat(14, 20, FEAT_FLOOR)

                        -- Remove the hook that called us, so that
                        -- we only get called once
                        local c_ptr = cave(player.py, player.px)
                        c_ptr.flags[FLAG_MOVE_POST_DEST] = nil
		end
	}
}

declare_global_constants {
	"__thieves_ring_alarm"
}

function __thieves_ring_alarm()
    trigger_quest_hook(QUEST_THIEVES, "ring_alarm")
end

