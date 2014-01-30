-- The archery combat subsystem
--
-- Load it with: load_subsystem("combat_archery")
--
-- This is the "default" archery combat system as it exists for ToME(a derivate of angband one)
--
-- Parameters:
-- * base_skill_name: The name of the skill which defines this combat method
-- * on_hit: function which will be called when a blow successfully landed (default to nothing)
-- * piercing_shots: function which tells how many piercing shots can be made (defaults to 0)

-- Load the combat subsystem which defines some basic stuff for all
-- combat systems
load_subsystem("combat")

-- Make the namespace
combat.archery = {}

-- Did we hit the monster?
combat.archery.test_hit = get_subsystem_param("combat_archery", "test_hit") or
	function() return false end

-- What bonuses do we get?
combat.archery.skill_bonus = get_subsystem_param("combat_archery", "skill_bonus") or function(obj)
	player.to_h = player.to_h + get_skill_scale(SKILL_ARCHERY, 25)
	player.num_fire = player.num_fire +
		(get_skill_scale(SKILL_ARCHERY, 50) / 16)
	player.inc_intrinsic(FLAG_XTRA_MIGHT,
						 (get_skill_scale(SKILL_ARCHERY, 50) / 25))

	if SKILL_ARCHERY == obj.flags[FLAG_SKILL] then
		player.inc_intrinsic(FLAG_XTRA_MIGHT,
							 get_skill_scale(SKILL_ARCHERY, 50) / 30)
	end
end

-- By how much do we increase damage because of a critical hit?
-- Return 1, 1, 0 for no change to damage.
combat.archery.critical_hit = get_subsystem_param("combat_archery",
												  "critical_hit") or
	function() return 1, 1, 0 end

-- Extra stuff to do when a shot hits
combat.archery.on_hit =	get_subsystem_param("combat_archery", "on_hit") or
	function() return false end

-- % chance for ammo to break when it hits a target
combat.archery.breakage_chance = get_subsystem_param("combat_archery",
													 "breakage_chance") or
	function() return 0 end

-- Does the shot pierce through the monster it just hit?
combat.archery.test_pierce = get_subsystem_param("combat_archery",
												 "test_pierce") or
	function() return false end

-- Max number of times a shot can pierce through a monster
combat.archery.piercing_shots = get_subsystem_param("combat_archery",
													"piercing_shots") or
	function() return 0 end

combat.archery.INVEN_BOW = get_subsystem_param("combat_archery", "bow_slot") or 0
combat.archery.INVEN_AMMO = get_subsystem_param("combat_archery", "ammo_slot") or 0

combat.archery.BTH_PLUS_ADJ = 3

-- Resolve skill name afterwards
hook(hook.INFO_DATA_LOADED,
	function()
		combat.archery.SKILL_ARCHERY =
			find_skill(get_subsystem_param("combat_archery",
										   "base_skill_name"))
	end
)

-- Are all the ranged weapons wielded of the right type ?
function combat.archery.get_archery_skill()
 local s

	s = 0

	-- All weapons must be of the same type
	for i = 1, player.inventory_limits(combat.archery.INVEN_BOW) do
		local obj = player.inventory[combat.archery.INVEN_BOW][i]

		if obj then
			if s == 0 or s == get_flag(obj, FLAG_SKILL) then
				s = get_flag(obj, FLAG_SKILL)
			else
				s = -1
			end
		end
	end

	return s
end

-- This was split out of set_archery_combat because the code was getting
-- to be indented too far to the right.
function combat.archery.ammo_hit_monster(j_ptr, q_ptr, chance, cur_dis, dam_type, y, x)
	local k
	local special = 0
	local with_ammo = 0
	local mdeath = false

	-- No monster to hit
	if cave(y, x).m_idx <= 0 then
		return nil
	end

	if get_flag(j_ptr, FLAG_AMMO) > 0 then
		with_ammo = 1
	end

	local c_ptr = cave(y, x)

	local m_ptr = monster(c_ptr.m_idx)
	local r_ptr = race_info(m_ptr)

	local m_name = monster_desc(m_ptr, 0)
	local o_name = object_desc(q_ptr, false, 3)

	local dams, max_damage, computed_damages
	local crit_mult, crit_div, crit_bonus

	-- Did we hit it (penalize range)
	if combat.archery.test_hit(q_ptr, j_ptr, m_ptr, chance - cur_dis) then
		local fear = false

		-- Any effects upon blow ?
		if combat.archery.on_hit(q_ptr, j_ptr, m_ptr) then
			mdeath = true
			return true
		end

		-- Assume a default death
		local note_dies = " dies."

		-- Handle unseen monster
		if (m_ptr.ml == 0) then
			-- Invisible monster
			msg_format("The %s finds a mark.", o_name)

			-- Handle visible monster
		else
			-- Message
			msg_format("The %s hits %s.", o_name, m_name)

			-- Hack -- Track this monster race
			if m_ptr.ml then
				monster_race_track(m_ptr.r_idx,
						   m_ptr.ego)
			end

			-- Hack -- Track this monster
			if m_ptr.ml then
				health_track(c_ptr.m_idx)
			end
		end

		local tmul = 1

		if j_ptr.flags[FLAG_MULTIPLIER] then
			tmul = get_flag(j_ptr, FLAG_MULTIPLIER)
		end

		tmul = tmul + player.intrinsic(FLAG_XTRA_MIGHT)

		local add_damage = j_ptr.to_d + (q_ptr.to_d * with_ammo * tmul)
		local remaining_damage = min(add_damage, m_ptr.hp)

		if has_flag(q_ptr, FLAG_DAM) then
			-- Any effects upon blow ?
			combat.archery.on_hit(q_ptr, j_ptr, m_ptr)

			-- Did the hook kill the or make it disappear?
			if c_ptr.m_idx == 0 then
				return true
			end

			dams = q_ptr.flags[FLAG_DAM]
			max_damage = 0
			computed_damages = {}

			computed_damages, max_damage =
				combat.compute_damage(q_ptr, m_ptr)

			crit_mult, crit_div, crit_bonus =
				combat.archery.critical_hit(q_ptr, j_ptr, m_ptr)

			max_damage = max_damage + (q_ptr.to_d * with_ammo)
			max_damage = max_damage * tmul * crit_mult / crit_div
			max_damage = max_damage + j_ptr.to_d + crit_bonus

			if max_damage == 0 then
				return false
			end

			-- Now apply the damage
			for i = 1, getn(computed_damages) do
				if mdeath then break end

				local dam_type = computed_damages[i].type
				k = computed_damages[i].amnt * tmul * crit_mult / crit_div

				-- Add a percent of the additional damage
				local add_k = (add_damage * k) / max_damage
				k = k + add_k
				remaining_damage = remaining_damage - add_k

				-- No negative damage
				if k < 0 then k = 0 end

				-- Damage, check for fear and death
				-- All damage doing uses project
				local proj_flags = (PROJECT_JUMP | PROJECT_HIDE |
									PROJECT_HIDE_BLAST | PROJECT_STOP |
									PROJECT_KILL | PROJECT_NO_REFLECT)

				-- Add silent flag forall but the last
				if i < getn(computed_damages) then
					proj_flags = proj_flags | PROJECT_SILENT
				end
				project(0, 0, y, x, k, dam_type, proj_flags)

				-- Complex message
				if (wizard) then
					message("You do "..k.." damage (" ..
							get_dam_type_info(dam_type, "desc") .. ")")
				end

				if c_ptr.m_idx == 0 then
					mdeath = true
					break
				else
					fear = m_ptr.monfear
				end
			end
		end

		if not mdeath and remaining_damage > 0 then
			-- If there is any bonus damage that didn't get applied,
			-- apply it as pure damage
			if (wizard) then
				message("You do ".. remaining_damage.." damage (" ..
						"bonuses)")
			end

			project(0, 0, y, x, remaining_damage, dam.DEFAULT,
					PROJECT_JUMP | PROJECT_HIDE | PROJECT_HIDE_BLAST |
					PROJECT_STOP | PROJECT_KILL | PROJECT_NO_REFLECT)
			if c_ptr.m_idx == 0 then
				mdeath = true
			else
				fear = m_ptr.monfear
			end
		end

		-- Blow responses (passive attacks)
		if c_ptr.m_idx ~= 0 and has_flag(m_ptr, FLAG_BLOW_RESPONSE) then
			local responses = m_ptr.flags[FLAG_BLOW_RESPONSE]

			for i = 1, flag_max_key(responses) do
				if responses[i] then
					local blow_response = get_blow_response(i)

					blow_response.weapon_blow(WHO_PLAYER, c_ptr.m_idx,
											   q_ptr, j_ptr, dams,
 											   computed_damages,
											   crit_mult, crit_div,
											   max_damage,
											   responses[i],
											   flag_get2(responses, i))
				end
			end
		end -- Blow responses (passive attacks)

		if c_ptr.m_idx ~= 0 and m_ptr.r_idx > 0 then
			hook.process(hook.PLAYER_HIT_MON_POST, c_ptr.m_idx, m_ptr,
						 combat.archery.SKILL_ARCHERY, q_ptr, j_ptr)
		end

		-- No death
		if c_ptr.m_idx ~= 0 and m_ptr.r_idx > 0 then
			-- Take note
			if (m_ptr.monfear > 0) and (m_ptr.ml) then
				-- Message
				msg_format("%s flees in terror!", monster_desc(m_ptr, 0))
			end
		end
	end

	return true
end -- ammo_hit_monster()


combat.archery.COMBAT_ARCHERY = add_combat_system
{
	name	= "Archery"
	desc	= "Uses various missile launchers to inflict pain and suffering."
	skill	= get_subsystem_param("combat_archery", "base_skill_name")
	energy	= function() return nil end
	available = function()
		if player.inventory_limits(combat.archery.INVEN_BOW) == 0 then return false end
		return true
	end,
	info = function()
		-- Get the "bow" (if any)
		local j_ptr = player.inventory[combat.archery.INVEN_BOW][1]
		local o_ptr

		-- Require a launcher
		if not j_ptr or not j_ptr.flags[FLAG_AMMO] then
			return "nil"
		end

		-- Only if needed
		local item
		if get_flag(j_ptr, FLAG_AMMO) > 0 then
			item = compute_slot(combat.archery.INVEN_AMMO, 0)
			o_ptr = get_object(item)

			-- If nothing correct try to choose from the backpack
			if ((not o_ptr) or (get_flag(j_ptr, FLAG_AMMO) ~= o_ptr.tval)) then
				return "no ammo"
			end

			-- Get the "ammo"
			o_ptr = get_object(item)
		else
			item = compute_slot(combat.archery.INVEN_BOW, 0)
			o_ptr = get_object(item)
		end

		-- Base damage from thrown object plus launcher bonus
		local tdam, tdd, tds = 0, 0, 0

		if get_flag(j_ptr, FLAG_AMMO) == -1 then
			tdd, tds = o_ptr.dd, o_ptr.ds
			if is_known(o_ptr) then tdam = o_ptr.to_d end
		else
			tdd, tds = o_ptr.dd, o_ptr.ds
			if is_known(o_ptr) then tdam = tdam + o_ptr.to_d end
			if is_known(j_ptr) then tdam = tdam + j_ptr.to_d end
		end

		local tmul = 1
		if has_flag(j_ptr, FLAG_MULTIPLIER) then tmul = get_flag(j_ptr, FLAG_MULTIPLIER) end

		-- Get extra "power" from "extra might"
		tmul = tmul + player.intrinsic(FLAG_XTRA_MIGHT)

		-- Boost the damage
		tdam = tdam * tmul
		tdd = tdd * tmul

		-- Add in the player damage
		tdam = tdam

		if tdam > 0 then
			return tdd.."d"..tds.."+"..tdam
		else
			return tdd.."d"..tds
		end
	end

	attack = function(y, x, max, dir)
		combat.archery.attack(dir)
	end,

	hooks       =
	{
		[hook.CALC_BONUS] = function()
			if player.combat_style == combat.archery.SKILL_ARCHERY then
				local obj = player.inventory[combat.archery.INVEN_BOW][1]

				-- Compute "extra shots" if needed
				if (obj) and (player.heavy_shoot == nil) then
					local archery = combat.archery.get_archery_skill()

					if archery ~= -1 then
						combat.archery.skill_bonus(obj)

						if archery == obj.flags[FLAG_SKILL] then
							player.num_fire = player.num_fire + 1
						end
					end

					-- Add in the "bonus shots"
					player.num_fire = player.num_fire + player.extra_shots

					-- Require at least one shot
					if (player.num_fire < 1) then player.num_fire = 1 end
				end
			end
		end,

		[hook.GET_POST] = function(obj, slot)
			if player.combat_style == combat.archery.SKILL_ARCHERY then
				local item = compute_slot(combat.archery.INVEN_AMMO, 1)
				local ammo = get_object(item)

				if not ammo then
					return
				end

				if object_similar(ammo, obj) and
					(ammo.number + obj.number) < 99 then
					message("Ammo merged into quiver.")

					object_absorb(ammo, obj)

					item_increase(slot, -obj.number)
					item_optimize(slot)

					return true
				end
			end

		end
		[hook.WIELD_INFO] = function(obj, item, slot)
			if item_slot_to_inven(slot) == INVEN_AMMO then
				return true, obj.number, slot
			end
		end
		[hook.WIELD_POST] = function(obj, item, slot)
			if item_slot_to_inven(slot) == combat.archery.INVEN_BOW and obj and obj.flags[FLAG_AMMO] and get_combat_availability(combat.archery.SKILL_ARCHERY) and player.combat_style ~= combat.archery.SKILL_ARCHERY then
				player.combat_style = combat.archery.SKILL_ARCHERY
				message("Autoswitch to archery combat mode.")
			end
		end
	}
}

function combat.archery.attack(force_dir)
	local msec = delay_factor * delay_factor * delay_factor
	local ret, item
	local hit_body = false
	local special
	local breakage = 0

	if player.combat_style ~= combat.archery.SKILL_ARCHERY then
		message("You are not in archery combat mode.")
		return
	end

	-- Get the "bow" (if any)
	local j_ptr = get_object(compute_slot(combat.archery.INVEN_BOW, 1))
	local o_ptr

	-- Require a launcher
	if not j_ptr or not j_ptr.flags[FLAG_AMMO] then
		message("You have nothing to fire with.")
		return
	end

	local with_ammo

	if j_ptr.flags[FLAG_AMMO] == j_ptr.tval then
		-- We're throwing what's in the bow slot, like a boomerang
		-- or throwing knife
		with_ammo = 0
		o_ptr     = j_ptr
	else
		-- We're using the item in the bow slot to launch something else
		with_ammo = 1
		item      = compute_slot(combat.archery.INVEN_AMMO, 1)
		o_ptr     = get_object(item)

		-- If nothing correct try to choose from the backpack or floor
		if not o_ptr or j_ptr.flags[FLAG_AMMO] ~= o_ptr.tval or
		    o_ptr.k_idx == 0 then
			-- Get an item
			ret, item = get_item("Your quiver is empty.  Fire which item? ",
								 "You have nothing to fire.",
								 USE_INVEN | USE_FLOOR,
								 get_flag(j_ptr, FLAG_AMMO))
			if (not ret) then return end
		end

		-- Get the "ammo"
		o_ptr = get_object(item)
	end

	if not o_ptr then
		message(color.VIOLET, "*ERROR* o_ptr in nil!!!")
		return
	end

	-- Get a direction (or cancel)
	local dir
	if force_dir then dir = force_dir
	else
		ret, dir = get_aim_dir{path_color_monster_block=true}
		if not ret then return end
	end

	-- Get local object
	local q_ptr = new_object()

	-- Obtain a local object
	object_copy(q_ptr, o_ptr)

	if o_ptr == j_ptr then
		j_ptr = q_ptr
	end

	-- Single object
	q_ptr.number = 1

	-- Not returning
	if has_flag(j_ptr, FLAG_RETURN) == nil then
		-- Reduce and describe player.inventory
		item_increase(item, -1)
		item_describe(item)
		item_optimize(item)
	end

	-- Describe the object
	local o_name = object_desc(q_ptr, false, 3)

	-- Find the color and symbol for the object for throwing
	local missile_attr = get_kind(q_ptr).d_attr
	local missile_char = get_kind(q_ptr).d_char

	-- Use a base distance
	local tdis = 10

	-- Actually "fire" the object
	local bonus = player.to_h + j_ptr.to_h + (q_ptr.to_h * with_ammo)

	-- Chance to hit
	local chance = player.skill_thb + (bonus * combat.archery.BTH_PLUS_ADJ)
	if chance < 5 then chance = 5 end

	-- Get extra "power" from "extra might"
	local xtra_might = player.intrinsic(FLAG_XTRA_MIGHT)

	-- Base range
	tdis = get_flag(j_ptr, FLAG_BASE_RANGE) + 5 * xtra_might

	-- Take a (partial) turn
	energy_use = get_player_energy(SPEED_FIRE) / player.num_fire

	-- piercing shots ?
	local num_pierce = combat.archery.piercing_shots()

	-- Start at the player
	local by = player.py
	local bx = player.px
	local y = player.py
	local x = player.px
	local ny, nx

	-- Predict the "target" location
	local dy, dx = explode_dir(dir)
	local tx = player.px + 99 * dx
	local ty = player.py + 99 * dy

	-- Check for "target request"
	if (dir == 5) then
		tx = target_col
		ty = target_row
	end

	-- Get damage type
	local dam_type = dam.PIERCE
	if has_flag(q_ptr, FLAG_DAM) then
		dam_type = get_flag(q_ptr, FLAG_DAM)
	end

	-- For use in the MONSTER_DEATH hook
	combat.curr          = {}
	combat.curr.weap     = q_ptr
	combat.curr.launcher = j_ptr

	while not nil do
		-- Travel until stopped
		for cur_dis = 0, tdis do
			-- Hack -- Stop at the target
			if ((y == ty) and (x == tx)) then break end

			-- Calculate the new location (see "project()")
			ny = y
			nx = x
			ny, nx = mmove2(ny, nx, by, bx, ty, tx)

			-- Stopped by walls/doors
			if (cave_floor_bold(ny, nx) == nil) then break end

			-- Save the new location
			x = nx
			y = ny

			local c_ptr = cave(y, x)
			local m_ptr
			if c_ptr.m_idx > 0 then
				m_ptr             = monster(c_ptr.m_idx)
				combat.curr.m_idx = c_ptr.m_idx
			else
				m_ptr             = nil
				combat.curr.m_idx = 0
			end

			-- The player can see the (on screen) missile
			if (panel_contains(y, x) and
			    ((cave(y, x).info & CAVE_SEEN) ~= 0)) then
				-- Draw, Hilite, Fresh, Pause, Erase
				sdl_utl_fast_draw(true)
				print_rel(missile_char, missile_attr, y, x)
				term.move_cursor_relative(y, x)
				term.fresh()
				term.xtra(TERM_XTRA_EFFECTS_DELAY, msec)
				lite_spot(y, x)
				term.fresh()
				sdl_utl_fast_draw(false)
			-- The player cannot see the missile
			else
				-- Pause anyway, for consistancy
				term.xtra(TERM_XTRA_EFFECTS_DELAY, msec)
			end

			-- Monster hitting code split off into auxilliary
			-- function
			local prev_hp
			local r_idx
			local re_idx

			if m_ptr then
				prev_hp = m_ptr.hp
				-- Save this for WEAP_DAM_DEALT, in case the
				-- monster dies and m_ptr is wiped
				r_idx  = m_ptr.r_idx
				re_idx = m_ptr.ego
			else
				prev_hp = 0
			end
			hit_body = combat.archery.ammo_hit_monster(j_ptr, q_ptr, chance, cur_dis, dam_type, y, x)

			local dam_dealt = 0

			if m_ptr then
				if m_ptr.hp < 0 or m_ptr.r_idx == 0 then
					dam_dealt = prev_hp
				else
					dam_dealt = prev_hp - m_ptr.hp
				end
			end

			if m_ptr or hit_body then
				hook.process(hook.WEAP_DAM_DEALT, q_ptr, j_ptr, m_ptr,
							 r_idx, re_idx, dam_dealt)
				item_hooks.process_all(q_ptr, FLAG_WEAP_DAM_DEALT,
										j_ptr, m_ptr, r_idx, re_idx,
									   dam_dealt)
				item_hooks.process_all(j_ptr, FLAG_WEAP_DAM_DEALT,
										q_ptr, m_ptr, r_idx, re_idx,
									   dam_dealt)
			end

			combat.anger_friend(c_ptr, m_ptr)

			if hit_body then
				break
			end
		end

		combat.curr = nil

		-- Chance of breakage (during attacks)
		local j = 0
		if hit_body == true then
			j = combat.archery.breakage_chance(q_ptr)
		end

		-- If the ammo doesn't break, it can pierce through
		if (num_pierce > 0 and hit_body == true and
			combat.archery.test_pierce(q_ptr, j_ptr, m_ptr)) then

			num_pierce = num_pierce - 1
			hit_body = false

			-- If target isn't reached, continue moving to target
			if (not ((tx < x and x < bx) or (bx < x and x < tx)) and
			                not ((ty < y and y < by) or (by < y and y < ty))) then
				-- Continue moving in same direction if we reached the target
				local dx = tx - bx
				local dy = ty - by
				tx = x + 99 * dx
				ty = y + 99 * dy

				-- New base location
				by = y
				bx = x
			end

			msg_format("The %s pierces through!", o_name)
		else
			break
		end
	end

	if not has_flag(j_ptr, FLAG_RETURN) then
		-- Drop (or break) near that location
		drop_near(q_ptr, breakage, y, x)
	else
		-- Travel back to the player
		for cur_dis = 0, tdis do
			-- Stop at the target
			if ((y == player.py) and (x == player.px)) then break end

			-- Calculate the new location (see "project()")
			ny = y
			nx = x
			ny, nx = mmove2(ny, nx, ty, tx, player.py, player.px)

			-- Save the new location
			x = nx
			y = ny

			-- The player can see the (on screen) missile
			if (panel_contains(y, x) and ((cave(y, x).info & CAVE_SEEN) ~= 0)) then
				-- Draw, Hilite, Fresh, Pause, Erase
				print_rel(missile_char, missile_attr, y, x)
				term.move_cursor_relative(y, x)
				term.fresh()
				term.xtra(TERM_XTRA_DELAY, msec)
				lite_spot(y, x)
				term.fresh()
			-- The player cannot see the missile
			else
				-- Pause anyway, for consistancy
				term.xtra(TERM_XTRA_DELAY, msec)
			end
		end
		delete_object(q_ptr)
	end
end

if not get_subsystem_param("combat_archery", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('f') then
			if player.wild_mode or player.control > 0 then return end
			combat.archery.attack()
			return true
		end
	end)
end
