-- Find the one ring and destroy it

add_quest
{
	global = "QUEST_ONE_RING"
	name = "The One Ring"
	desc =
	{
		"Find the One Ring, then bring it to Mount Doom, in Mordor, to cast",
		"it in the Great Fire where it was once forged.",
		"But beware: *NEVER* use it, or you will be forever corrupted.",
		"Once it is destroyed you will be able to permanently defeat Sauron.",
		"The ring must be cast back into the fires of Mount Doom!",
	}
	level = 99
	hooks =
	{
		[hook.INIT_QUEST] = function()
		end

		-- Forbid death when using the One Ring, but this comes at a cost.
		-- A sharp one
		[hook.DIE_PRE] = function()
			for i = 1, player.inventory_limits(INVEN_RING) do
				local obj = player.inventory[INVEN_RING][i]
				if obj and obj.artifact_id == ART_ONE_RING then
					obj.flags[FLAG_LIFE] = obj.flags[FLAG_LIFE] - 20
					if obj.flags[FLAG_LIFE] < -200 then obj.flags[FLAG_LIFE] = -200 end
					flag_learn(obj.flags, FLAG_LIFE, true)
					message(color.LIGHT_RED, "You feel the One Ring hold over your soul increases.")
					message(color.LIGHT_RED, "The ring reconstruct your mortal body around your lamenting soul.")
					return true, true
				end
			end
		end

		-- Drop the one ring
		[hook.MONSTER_DEATH] = function(m_idx)
			local monst = monster(m_idx)

			if quest(QUEST_ONE_RING).status ~= QUEST_STATUS_TAKEN then return end

			if (monst.r_idx == RACE_SAURON and rng.percent(30)) then
				if a_info(ART_ONE_RING).cur_num == 0 then
					local obj = create_artifact(ART_ONE_RING)
					local destroy = true

					for i = 1, player.inventory_limits(INVEN_INVEN) - 1 do
						if not player.inventory[INVEN_INVEN][i] or player.inventory[INVEN_INVEN][i].k_idx == 0 then destroy = false break end
					end
					if destroy then
						message(color.VIOLET, "You feel the urge to drop your "..object_desc(player.inventory[INVEN_INVEN][player.inventory_limits(INVEN_INVEN) - 1], false, 0).." to make room in your inventory.");
						inven_drop(player.inventory_limits(INVEN_INVEN) - 1, 100, player.py, player.px, false)
					end
					message(color.VIOLET, "You feel the urge to pick up that plain gold ring you see.")
					inven_carry(obj, false)
				end
			end
		end
	}
}
