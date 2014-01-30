---------------------------------------------------------------------
-- Abmushes
---------------------------------------------------------------------
constant("encounters", {})
settag(encounters, TAG_NAMESPACE) -- Tag as a namespace

add_loadsave("encounters.ambush", false)
hook(hook.BIRTH, function() encounters.ambush = false end)

hook(hook.WILD_SMALL_MOVE_POST,
	function()
		if encounters.just_left_ambush then
			encounters.just_left_ambush = false
			message("You escape the ambush.")
			encounters.ambush = false
			return
		end

		local wild_feat = wf_info[wild_map(player.py, player.px).feat + 1]

		local chance = wild_feat.level - player.lev

		if rng.percent(wild_feat.level - player.lev) then
			-- Go into large wilderness view
			player.wilderness_x = player.px
			player.wilderness_y = player.py
			energy_use = get_player_energy(SPEED_WALK)
			change_wild_mode()

			player.oldpx = cur_wid / 2
			player.oldpy = cur_hgt / 2

			player.px = cur_wid / 2
			player.py = cur_hgt / 2

			player.wild_force_side_y_ok = false
			player.wild_force_side_x_ok = false

			message("You are ambushed!")

			encounters.ambush          = true
			encounters.do_ambush_setup = true
		end
	end) -- hook.WILD_SMALL_MOVE_POST

hook(hook.WILD_GEN_MON,
	function(hack_floor)
		if not encounters.ambush then
			return false
		end

		local num_mon = min(hack_floor, 60)

		return true
	end) -- hook.WILD_GEN_MON

hook(hook.WILD_LARGE_NEW_AREA_POST,
	function()
		if encounters.ambush then
			message("You escape the ambush.")
			encounters.ambush = false
		end
	end) -- WILD_LARGE_NEW_AREA_POST

hook(hook.FORBID_TRAVEL,
	function()
		if encounters.ambush then
			-- If player is already at edge, let him/her go
			if player.py == 1 or player.py == cur_hgt - 2 or
				player.px == 1 or player.px == cur_wid - 2
			then
				encounters.just_left_ambush = true
				--message("You escape the ambush.")
				--encounters.ambush = false
				return false
			end

			message("To flee the ambush you have to reach the edge " ..
					"of the map.");
			return true
		end
	end) -- hook.FORBID_TRAVEL

-- Don't use hook.WILD_GEN_MON, it doesn't necessarily get
-- called for every ambush.
function encounters.setup_ambush()
    -- Set the correct monster hook
    set_mon_num_hook()

    -- Prepare allocation table
    get_mon_num_prep()

	for i = 1, 60 do
		if not alloc_monster(0, false) then
			wiz_print("Couldn't allocate ambush monster")
			break
		end
	end

	-- Make sure all monsters a awake
	for_each_monster(
		function(m_idx, m_ptr)
			m_ptr.csleep = 0
		end)

	encounters.do_ambush_setup = false
end -- encounters.setup_ambush()

hook(hook.LEVEL_END_GEN,
	 function()
		 if dun_level ~= 0 or player.wild_mode then
			 -- Paranoia
			 return
		 end
		 if encounters.ambush and encounters.do_ambush_setup then
			 encounters.setup_ambush()
		 end
	 end)

hook(hook.NEW_LEVEL,
	 function()
		 -- Paranoia about clearing the ambush variable.  Should
		 -- take care of "reaching clvl 45 and being whisked off to
		 -- Gondor in the middle of an ambush" type situations.
		 if not encounters.do_ambush_setup then
			 encounters.ambush = false
		 end
	 end)


