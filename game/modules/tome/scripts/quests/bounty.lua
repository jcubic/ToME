-- This "add_quest()" is here just so the quest will show up in the quest
-- list, since it's not like other quests, in that you can take it over
-- and over again.
add_quest
{
	global = "QUEST_BOUNTY"
	name = "Beastmaster corpse bounty quest",
	desc = {
		"YOU SHOULDN'T SEE THIS"
	}
	level = 0
	hooks = {}
}

constant("bounty", {})

bounty.quest_r_idx = 0
add_loadsave("bounty.quest_r_idx", 0)
hook(hook.BIRTH, function() bounty.quest_r_idx = 0 end)

function bounty.get_new_monster()
	local flags_need   = flag_new()
	local flags_forbid = flag_new()

	-- Must leave a corpse
	flags_need[FLAG_DROP_CORPSE]	 = true

	-- Musn't be freindly to the player
	flags_forbid[FLAG_FRIENDLY] = true
	flags_forbid[FLAG_PET]      = true

	-- Must be able to die to leave a corpse
	flags_forbid[FLAG_NO_DEATH] = true

	-- No uniuqes
	flags_forbid[FLAG_UNIQUE] = true

	local lev = 3 + player.lev
	local prev_idx = bounty.quest_r_idx
	local r_idx

	for tries = 1, 10 do
		r_idx = get_rand_r_idx(lev, FACTION_PLAYER, -infinity, 0,
							   flags_need, flags_forbid,
							   nil, nil, nil, nil)

		if r_idx == 0 or r_idx ~= prev_idx then
			break
		end
	end

	if r_idx == 0 then
		message(color.VIOLET, "ERROR: Beastmaster couldn't find a quest " ..
				"monster!!")
		bounty.quest_r_idx = 0
		return
	end

	if r_idx == prev_idx then
		bounty.quest_r_idx = 0
		return
	end

	bounty.quest_r_idx = r_idx
end -- bounty.get_new_monster()

function bounty.increase_skill()
	local preserv_skill = skill(SKILL_PRESERVATION)
	local lore_skill = skill(SKILL_LORE)

	message("As a reward, I'll teach you a bit of monster lore.")

	if lore_skill.mod == 0 then
		lore_skill.mod = 900
	end

	lore_skill.value = lore_skill.value + lore_skill.mod

	if preserv_skill.mod == 0 then
		message("I see you don't know anything about corpse preservation. I'll teach you about that skill as well.")
		preserv_skill.mod   = 800
		preserv_skill.value = 800
		preserv_skill.dev   = true
	end
end -- bounty.increase_skill()

hook(hook.CORPSE_CREATE_POST,
function(obj, monst)
	if bounty.quest_r_idx == monst.r_idx then
		-- Don't let the automizer destroy it
		obj.flags[FLAG_NO_AUTO_DEST] = true
		flag_learn(obj.flags, FLAG_NO_AUTO_DEST, true)

		if player_can_see_bold(obj.iy, obj.ix) and
			not player.has_intrinsic(FLAG_BLIND) and
			not player.has_intrinsic(FLAG_HALLUCINATE) then
			message("#yThe object of your bounty quest has dropped a corpse!#w")
		end
	end
end)
