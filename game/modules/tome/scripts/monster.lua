-- Monsters handling

constant("get_attacking_monst",
function(m_idx)
	if m_idx == 0 then
		if __monst_helper.helper then
			return __monst_helper.helper
		elseif player.body_monster.r_idx ~= 0 then
			return player.body_monster
		end
	else
		return monster(m_idx)
	end
end)

--------------------------------------------------------------------------
---------------------------- Melee attacks -------------------------------
--------------------------------------------------------------------------
load_subsystem("combat_monster_default", {
	on_hit = function(y, x, c_ptr, m_idx, m_ptr, t_idx, t_ptr, t_flags, blow, params, special)
		local t_name
		if t_ptr then
			if tag(t_ptr) == TAG_MONSTER then
				t_name = monster_desc(t_ptr, 0)
			else
				t_name = "you"
			end
		end

		-- only one of cut or stun
		if special.cut and special.stun then
			-- Cancel cut
			if (rng.number(100) < 50) then
				special.cut = 0
			-- Cancel stun
			else
				special.stun = 0
			end
		end

		-- Handle cut
		if (special.cut) then
			local k =0

			-- Critical hit (zero if non-critical)
			local tmp = combat_monster.default.critical_hit(blow.d_dice, blow.d_side, params.dam)

			-- Roll for damage
			local t =
			{
				[0] = 0,
				[1] = rng(5),
				[2] = rng(5) + 5,
				[3] = rng(20) + 20,
				[4] = rng(50) + 50,
				[5] = rng(100) + 100,
				[6] = 300,
			}
			if t[tmp] then k = t[tmp]
			else k = 500 end

			-- Apply the cut
			if (k > 0) then
				if t_ptr == player then
					timed_effect(timed_effect.CUT, k, 1)
				else
					monster_msg(strcap(t_name).." is bleeding.", t_ptr.ml)
					t_ptr.bleeding = t_ptr.bleeding + k
				end
			end
		end

		-- Handle stun
		if (special.stun) then
			local k = 0

			-- Critical hit (zero if non-critical)
			local tmp = combat_monster.default.critical_hit(blow.d_dice, blow.d_side, params.dam)

			-- Roll for damage
			local t =
			{
				[0] = 0,
				[1] = rng(5),
				[2] = rng(10) + 10,
				[3] = rng(20) + 20,
				[4] = rng(30) + 30,
				[5] = rng(40) + 40,
				[6] = 100,
			}
			if t[tmp] then k = t[tmp]
			else k = 200 end

			-- Apply the stun
			if (k > 0) then
				if t_ptr == player then
					timed_effect(timed_effect.STUN, k, 1)
				end
			end
		end
	end
})

-- All the monster attack methods
add_monster_attack_method
{
	["name"]	= "HIT",
	["desc"]	= "hit",
	["action"]	= "hits @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun = true
		special.cut = true
	end,
	["on_eat"]  = true,
}
-- The same as HIT, except that it indicates to the syste that the
-- monster is simulating using a weapon.
add_monster_attack_method
{
	["name"]	= "WEAPON",
	["desc"]	= "hit",
	["weapon"]  = true,
	["slot"]    = INVEN_MAINHAND,
	["action"]	= "hits @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun    = true
		special.cut     = true
		special.weapon  = true
	end,
	["on_eat"]  = true,
}
add_monster_attack_method
{
	["name"]	= "WEAPON_OFFHAND",
	["desc"]	= "hit",
	["weapon"]  = true,
	["slot"]    = INVEN_OFFHAND,
	["action"]	= "hits @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun    = true
		special.cut     = true
		special.weapon  = true
	end,
	["on_eat"] = true,
}
add_monster_attack_method
{
	["name"]	= "BARE_MAINHAND",
	["desc"]	= "hit",
	["action"]	= "hits @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun    = true
	end,
	["on_eat"]     = true,
	["barehand"]   = true,
	["hand1_slot"] = INVEN_MAINHAND,
}
add_monster_attack_method
{
	["name"]	= "BARE_OFFHAND",
	["desc"]	= "hit",
	["action"]	= "hits @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun    = true
	end,
	["on_eat"]     = true,
	["barehand"]   = true,
	["hand1_slot"] = INVEN_OFFHAND,
}
add_monster_attack_method
{
	["name"]	= "TOUCH",
	["desc"]	= "touch",
	["action"]	= "touches @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
	end,
	["on_eat"]  = true,
}
add_monster_attack_method
{
	["name"]	= "TOUCH_BAREHAND",
	["desc"]	= "touch",
	["action"]	= "touches @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
	end,
	["on_eat"]     = true,
	["barehand"]   = true,
	["hand1_slot"] = INVEN_MAINHAND,
}
add_monster_attack_method
{
	["name"]	= "TOUCH_OFFHAND",
	["desc"]	= "touch",
	["action"]	= "touches @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
	end,
	["on_eat"]     = true,
	["barehand"]   = true,
	["hand1_slot"] = INVEN_OFFHAND,
}
add_monster_attack_method
{
	["name"]	= "PUNCH",
	["desc"]	= "punch",
	["action"]	= "punches @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun = true
	end,
	["on_eat"]     = true,
	["barehand"]   = true,
	["hand1_slot"] = INVEN_MAINHAND,
}
add_monster_attack_method
{
	["name"]	= "PUNCH_OFFHAND",
	["desc"]	= "punch",
	["action"]	= "punches @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun = true
	end,
	["on_eat"]     = true,
	["barehand"]   = true,
	["hand1_slot"] = INVEN_OFFHAND,
}
add_monster_attack_method
{
	["name"]	= "KICK",
	["desc"]	= "kick",
	["action"]	= "kicks @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun = true
	end,
}
add_monster_attack_method
{
	["name"]	= "CLAW",
	["desc"]	= "claw",
	["action"]	= "claws @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.cut = true
	end,
	["on_eat"]  = true,
}
add_monster_attack_method
{
	["name"]	= "CLAW_MAINHAND",
	["desc"]	= "claw",
	["action"]	= "claws @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.cut = true
	end,
	["on_eat"]     = true,
	["barehand"]   = true,
	["hand1_slot"] = INVEN_MAINHAND,
}
add_monster_attack_method
{
	["name"]	= "CLAW_OFFHAND",
	["desc"]	= "claw",
	["action"]	= "claws @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.cut = true
	end,
	["on_eat"]     = true,
	["barehand"]   = true,
	["hand1_slot"] = INVEN_OFFHAND,
}
add_monster_attack_method
{
	["name"]	= "BITE",
	["desc"]	= "bite",
	["action"]	= "bites @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.cut = true

		local monst = get_attacking_monst(m_idx)
--		if (rng.percent(5)) and not strfind(strlower(monster_race_desc(m.r_idx, m.ego)), "vampire") then
--			special.vampire = true
--		end
	end,
	["on_eat"]  = true,
}
add_monster_attack_method
{
	["name"]	= "STING",
	["desc"]	= "sting",
	["action"]	= "stings @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
	end,
	["on_eat"]  = true,
}
add_monster_attack_method
{
	["name"]	= "BUTT",
	["desc"]	= "butt",
	["action"]	= "butts @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun = true
	end,
	["on_eat"]  = true,
}
add_monster_attack_method
{
	["name"]	= "CRUSH",
	["desc"]	= "crush",
	["action"]	= "crushes @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun = true
	end,
	["on_eat"]  = true,
}
add_monster_attack_method
{
	["name"]	= "BEAR-HUG",
	["desc"]	= "crush",
	["action"]	= "crushes @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
		special.stun = true
	end,
	["on_eat"]     = true,
	["barehand"]   = true,
	["hand1_slot"] = INVEN_MAINHAND,
	["hand2_slot"] = INVEN_OFFHAND,
}
add_monster_attack_method
{
	["name"]	= "ENGULF",
	["desc"]	= "engulf",
	["action"]	= "engulfs @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
	end,
	["on_eat"]  = true,
}
add_monster_attack_method
{
	["name"]	    = "CHARGE",
	["desc"]	    = "charge",
	["action"]	    = "charges @target@",
	["body_action"] = "charge @target@",
	["fct"]		    = function(special, type, m_idx)
		special.touched = true
	end,
}
-- The same as CHARGE, but indicates to the system that the
-- monster is simulating using a weapon.
add_monster_attack_method
{
	["name"]	    = "WEAPON_CHARGE",
	["desc"]	    = "charge",
	["weapon"]      = true,
	["slot"]        = INVEN_MAINHAND,
	["weapon"]      = true,
	["action"]	    = "charges @target@",
	["body_action"] = "charge @target@",
	["fct"]		    = function(special, type, m_idx)
		special.touched = true
		special.weapon  = true
	end,
}
add_monster_attack_method
{
	["name"]	= "CRAWL",
	["desc"]	= "crawl",
	["action"]	= "crawls on @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
	end,
	["on_eat"]  = true,
}
add_monster_attack_method
{
	["name"]	= "DROOL",
	["desc"]	= "drool",
	["action"]	= "drools on @target@",
	["on_eat"]  = true,
	["body"]    = function(special, type, m_idx)
	end,
}
add_monster_attack_method
{
	["name"]	= "SPIT",
	["desc"]	= "spit",
	["action"]	= "spits at @target@",
	["on_eat"]  = true,
	["body"]    = function(special, type, m_idx)
	end,
}
add_monster_attack_method
{
	["name"]	= "EXPLODE",
	["desc"]	= "explode",
	["action"]	= "explodes",
	["explode"] = true,
	["fct"]		= function(special, type, m_idx)
		special.explode = true
	end,
}
add_monster_attack_method
{
	["name"]	= "GAZE",
	["desc"]	= "gaze",
	["action"]	= "gazes @target@",
}
add_monster_attack_method
{
	["name"]	= "WAIL",
	["desc"]	= "wail",
	["action"]	= "wails at @target@",
}
add_monster_attack_method
{
	["name"]	= "SPORE",
	["desc"]	= "release spores",
	["action"]	= "releases spores at @target@",
	["on_eat"]  = true,
	["body"]    = function(special, type, m_idx)
	end,
}
add_monster_attack_method
{
	["name"]	= "BEG",
	["desc"]	= "beg",
	["action"]	= "begs @target@ for money",
}
add_monster_attack_method
{
	["name"]	= "INSULT",
	["desc"]	= "insult",
	["fct"]         = function(special, type, m_idx)
		local desc_insult =
		{
			"insults @target@!",
			"insults @target_possesive@ mother!",
			"gives @target@ the finger!",
			"humiliates @target@!",
			"defiles @target@!",
			"dances around @target@!",
			"makes obscene gestures!",
			"moons @target@!!!"
		}
		special.action = desc_insult[rng(getn(desc_insult))]
	end,
}
add_monster_attack_method
{
	["name"]	= "MOAN",
	["desc"]	= "moan",
	["fct"]         = function(special, type, m_idx)
		local moan =
		{
			"seems sad about something.",
			"asks if @target@ have seen his dogs.",
			"tells @target@ to get off his land.",
			"mumbles something about mushrooms.",
		}
		local mathilde =
		{
			-- Mathilde's sentence
			"giggles at @target@.",
			"asks @target@ if @target@ want to giggle with her.",
			"says she is always happy."
		}
		local monst = get_attacking_monst(m_idx)
		if monst.r_idx == test_monster_name("Mathilde, the Science Student")
		then
			special.action = mathilde[rng(getn(mathilde))]
		else
			special.action = moan[rng(getn(moan))]
		end
	end,
}
add_monster_attack_method
{
	["name"]	= "SING",
	["desc"]	= "sing",
	["fct"]         = function(special, type, m_idx)
		local sing =
		{
			"sings 'We are a happy family.'",
			"sings 'I love you, you love me.'",
		}
		special.action = sing[rng(getn(sing))]
	end,
}
add_monster_attack_method
{
	["name"]	= "BURN",
	["desc"]	= "burn",
	["action"]	= "burns @target@",
	["fct"]		= function(special, type, m_idx)
		special.touched = true
	end,
	["on_eat"]  = true,
}

-- All monster attack types
add_monster_attack_type
{
	["name"]	= "HURT",
	["desc"]	= "hurt",
	["power"]	= 60,
	["type"]	= dam.PURE,
	["obvious"]	= true,
}
add_monster_attack_type
{
	["name"]	= "SLASH",
	["desc"]	= "slash",
	["power"]	= 60,
	["type"]	= dam.SLASH,
	["obvious"]	= true,
}
add_monster_attack_type
{
	["name"]	= "CRUSH",
	["desc"]	= "crush",
	["power"]	= 60,
	["type"]	= dam.CRUSH,
	["obvious"]	= true,
}
add_monster_attack_type
{
	["name"]	= "PIERCE",
	["desc"]	= "pierce",
	["power"]	= 60,
	["type"]	= dam.PIERCE,
	["obvious"]	= true,
}
add_monster_attack_type
{
	["name"]	= "INSANITY",
	["desc"]	= "drain sanity",
	["power"]	= 60,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		local monst = get_attacking_monst(m_idx)
		take_sanity_hit(params.dam, monster_desc(monster(m_idx), 136))
		params.dam = 0
	end,
	["on_eat"]  = function(corpse, monst, blow, result, info)
		local amnt = rng.roll(blow.d_dice, blow.d_sides) * blow.power / 100
		message("You feel your sanity slipping away!")
		take_sanity_hit(amnt, "eating an insane monster")
		result.harmful = true
	end,
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "POISON",
	["desc"]	= "poison",
	["power"]	= 5,
	["type"]	= dam.POIS,
	["obvious"]	= true,
	["on_eat"]  = {death_str = "poisonous food", use_project=true}
}
add_monster_attack_type
{
	["name"]	= "FEAR",
	["desc"]	= "scare",
	["power"]	= 5,
	["type"]	= dam.FEAR,
	["obvious"]	= true,
}
add_monster_attack_type
{
	["name"]	= "EAT_GOLD",
	["desc"]	= "steal gold",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		-- Saving throw (unless paralyzed) based on dex and level
		if (not player.has_intrinsic(FLAG_PARALYZED)) and (rng.number(100) < (player.stat(A_DEX) + player.lev / 2)) then
			-- Saving throw message
			message("You quickly protect your money pouch!")

			-- Occasional blink anyway
			if (rng.number(3)) then special.blinked = true end
		-- Eat gold
		else
			local gold = (player.au / 10) + rng(25)
			if (gold < 2) then gold = 2 end
			if (gold > 5000) then gold = (player.au / 20) + rng(3000) end
			if (gold > player.au) then gold = player.au end
			player.au = player.au - gold

			if (gold <= 0) then
				message("Nothing was stolen.")
			elseif (player.au > 0) then
				message("Your purse feels lighter.")
				msg_format("%d coins were stolen!", gold)
			else
				message("Your purse feels lighter.")
				message("All of your coins were stolen!")
			end

			local monst = get_attacking_monst(m_idx)
			while (gold > 0) do
				local j_ptr = new_object()

				-- Prepare a gold object
				object_prep(j_ptr, lookup_kind(TV_GOLD, 9))

				-- Determine how much the treasure is "worth"
				if gold < 15000 then
					set_flag(j_ptr, FLAG_GOLD_VALUE, gold)
				else
					set_flag(j_ptr, FLAG_GOLD_VALUE, 15000)
				end

				monster_carry(monst, m_idx, j_ptr)

				gold = gold - 15000
			end

			-- Redraw gold
			flag_set(player.redraw, FLAG_PR_GOLD,1)

			-- Window stuff
			player.window[FLAG_PW_PLAYER] = true

			-- Blink away
			special.blinked = true
		end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "EAT_ITEM",
	["desc"]	= "steal item",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		-- Saving throw (unless paralyzed) based on dex and level
		if (not player.has_intrinsic(FLAG_PARALYZED)) and (rng.number(100) < (player.stat(A_DEX) + player.lev / 2)) then
			-- Saving throw message
			message("You grab hold of your backpack!");

			-- Occasional "blink" anyway
			special.blinked = true;

			-- Obvious
			special.obvious = true;
		else
			local monst = get_attacking_monst(m_idx)

			-- Find an item
			for k = 0, 9 do
				-- Pick an item
				local i = rng(player.inventory_limits(INVEN_INVEN))

				-- Obtain the item
				local o_ptr = player.inventory[INVEN_INVEN][i]

				-- Skip non-objects
				if ((o_ptr) and (is_artifact(o_ptr) == nil)) then
					-- Get a description
					local o_name = object_desc(o_ptr, false, 3)

					-- Message
					local target = "Your"
					if o_ptr.number > 1 then target = "One of your" end
					msg_format("%s %s was stolen!", target, o_name)

					local j_ptr = object_dup(o_ptr)

					-- Modify number
					j_ptr.number = 1;

					-- Forget mark
					j_ptr.marked = false

					monster_carry(monst, m_idx, j_ptr)

					-- Steal the items
					item_increase(compute_slot(INVEN_INVEN, i), -1)
					item_optimize(compute_slot(INVEN_INVEN, i))

					-- Obvious
					special.obvious = true

					-- Blink away
					special.blinked = true

					-- Done
					break
				end
			end
		end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "EAT_FOOD",
	["desc"]	= "eat food",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		-- Steal some food
		special.obvious = false
		for k = 0, 9 do
			-- Pick an item from the pack
			local i = rng(player.inventory_limits(INVEN_INVEN))

			-- Get the item
			local o_ptr = player.inventory[INVEN_INVEN][i]

			-- Skip non-objects
			if (o_ptr) and (o_ptr.tval == TV_FOOD) then
				-- Get a description
				local o_name = object_desc(o_ptr, false, 0)

				-- Message
				local target = "Your"
				if o_ptr.number > 1 then target = "One of your" end
				msg_format("%s %s was eaten!", target, o_name)

				-- Steal the items
				item_increase(i, -1)
				item_optimize(i)

				-- Obvious
				special.obvious = true

				-- Done
				break
			end
		end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "EAT_LITE",
	["desc"]	= "consume fuel",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		-- Access the lite
		local o_ptr = player.inventory[INVEN_LITE][1]

		special.obvious = false

		-- Drain fuel
		if o_ptr and ((get_flag(o_ptr, FLAG_FUEL) > 0) and (is_artifact(o_ptr) == nil)) then
			-- Reduce fuel
			local fuel = get_flag(o_ptr, FLAG_FUEL) - (250 + rng(250))
			if (fuel < 1) then fuel = 1 end
			set_flag(o_ptr, FLAG_FUEL, fuel)

			-- Notice
			if not player.has_intrinsic(FLAG_BLIND) then
				message("Your light dims.")
				obvious = true
			end

			-- Window stuff
			player.window[FLAG_PW_EQUIP] = true
		end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "ACID",
	["desc"]	= "shoot acid",
	["power"]	= 0,
	["type"]	= dam.ACID,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		message("You are covered in acid!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = {
		death_str = "acidic food",
		dam_msg   = "You get a bad case of indigestion!"
	}
}
add_monster_attack_type
{
	["name"]	= "ELEC",
	["desc"]	= "electrocute",
	["power"]	= 10,
	["type"]	= dam.ELEC,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		message("You are struck by electricity!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
}
add_monster_attack_type
{
	["name"]	= "FIRE",
	["desc"]	= "burn",
	["power"]	= 10,
	["type"]	= dam.FIRE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		message("You are enveloped in flames!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = {
		death_str = "a fiery meal",
		dam_msg   = "That meat really burns on its way down your throat!"
	}
}
add_monster_attack_type
{
	["name"]	= "DARK_FIRE",
	["desc"]	= "burn with dark fire",
	["power"]	= 15,
	["type"]	= dam.DARK_FIRE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		message("You are enveloped in dark flames!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = {
		death_str = "a fiery meal",
		dam_msg   = "That meat really burns on its way down your throat!"
	}
}
add_monster_attack_type
{
	["name"]	= "VALINOREAN_FIRE",
	["desc"]	= "burn with divine fire",
	["power"]	= 15,
	["type"]	= dam.VALINOREAN_FIRE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		message("You are enveloped in divine flames!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = {
		death_str = "a fiery meal",
		dam_msg   = "That meat really burns on its way down your throat!"
	}
}
add_monster_attack_type
{
	["name"]	= "COLD",
	["desc"]	= "freeze",
	["power"]	= 10,
	["type"]	= dam.COLD,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		message("You are covered with frost!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
}
add_monster_attack_type
{
	["name"]	= "TIME",
	["desc"]	= "time",
	["power"]	= 10,
	["type"]	= dam.TIME,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		message("You feel that your life has been clocked back.")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
}
add_monster_attack_type
{
	["name"]	= "BLIND",
	["desc"]	= "blind",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = false

		local monst = get_attacking_monst(m_idx)
		-- Increase "blind"
		if rng.percent(100 - player.resist(dam.BLIND)) then
			timed_effect.inc(timed_effect.BLIND, 10 + rng(monst.level))
			special.obvious = true
		end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = {type=dam.BLIND use_project=true}
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "HALLU",
	["desc"]	= "hallucinate",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = true

		local monst = get_attacking_monst(m_idx)
		timed_effect.inc(timed_effect.HALLUCINATE, 10 + rng(monst.level))
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(obj, monst, blow, special, info)
		timed_effect.inc(timed_effect.HALLUCINATE,
						 10 + rng(monst.level))
		special.harmful = true
	end,
}
add_monster_attack_type
{
	["name"]	= "PARALYZE",
	["desc"]	= "paralyze",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		if (player.has_intrinsic(FLAG_FREE_ACT)) then
			message("You are unaffected!")
		elseif rng.percent(player.skill_sav) then
			message("You resist the effects!")
		else
			-- Keep player from being permanently paralyzed because
			-- the paralyzing monster keeps hiting and re-paralyzing
			-- while the player can't move.
			local monst = get_attacking_monst(m_idx)
			if not player.has_intrinsic(FLAG_PARALYZED) then
				local amnt = 3 + rng(monst.level)
				timed_effect.inc(timed_effect.PARALYZED, amnt)
			end
		end
		special.obvious = true
	end,
	["monster"]	= function(params, special, method, m_idx)
		local mon    = params.t_ptr
		local flags  = params.t_flags
		local name   = params.t_name
		local source = get_attacking_monst(m_idx)

		if flags[FLAG_FREE_ACT] or flags[FLAG_UNIQUE] then
			monster_msg(strcap(name) .. " is unaffected!", mon.ml)
		elseif monster_saving_throw(source, mon) then
			monster_msg(strcap(name) .. " resists!", mon.ml)
		else
			if not monster_effect.get(mon, monster_effect.PARALYZE) then
				local monst = get_attacking_monst(m_idx)
				local amnt = 3 + rng(monst.level)
				monster_effect.inc(m_idx, mon, monster_effect.PARALYZE, amnt)
				if mon.ml and source.ml then
					special.obvious = true
				end
			end
		end
	end,
	["on_eat"]  = function(obj, monst, blow, result, info)
		if not player.has_intrinsic(FLAG_FREE_ACT) then
			local amnt = rng.roll(blow.d_dice, blow.d_side)
			amnt = amnt * blow.power / 100
			timed_effect.inc(timed_effect.PARALYZED, amnt)

			result.msg     = "Eating the @corpse@ paralyzes you!"
			result.harmful = true
		else
			result.msg = "You stiffen briefly."
		end
	end,
}
add_monster_attack_type
{
	["name"]	= "CONFUSE",
	["desc"]	= "confuse",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		project(m_idx, 0, player.py, player.px, params.dam, dam.CONFUSE,
				PROJECT_JUMP | PROJECT_STOP | PROJECT_KILL | PROJECT_HIDE |
				PROJECT_HIDE_BLAST | PROJECT_NO_REFLECT)
	end,
	["monster"]	= function(params, special, method, m_idx)
		local mon = params.t_ptr
		project(m_idx, 0, mon.fy, mon.fx, params.dam, dam.CONFUSE,
				PROJECT_JUMP | PROJECT_STOP | PROJECT_KILL | PROJECT_HIDE |
				PROJECT_HIDE_BLAST | PROJECT_NO_REFLECT)
	end,
	["on_eat"]  = {type=dam.CONFUSE use_project=true}
}
add_monster_attack_type
{
	["name"]	= "TERRIFY",
	["desc"]	= "scare",
	["power"]	= 5,
	["type"]	= dam.FEAR,
	["player"]	= function(params, special, method, m_idx)
		project(m_idx, 0, player.py, player.px, params.dam, dam.FEAR,
				PROJECT_JUMP | PROJECT_STOP | PROJECT_KILL | PROJECT_HIDE |
				PROJECT_HIDE_BLAST | PROJECT_NO_REFLECT)
	end,
	["monster"]	= function(params, special, method, m_idx)
		local mon = params.t_ptr
		project(m_idx, 0, mon.fy, mon.fx, params.dam, dam.FEAR,
				PROJECT_JUMP | PROJECT_STOP | PROJECT_KILL | PROJECT_HIDE |
				PROJECT_HIDE_BLAST | PROJECT_NO_REFLECT)
	end,
	["on_eat"]  = {use_project=true}
}
add_monster_attack_type
{
	["name"]	= "LOSE_STR",
	["desc"]	= "reduce strength",
	["power"]	= 0,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = false
		if (do_dec_stat(A_STR, STAT_DEC_NORMAL)) then special.obvious = true end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(obj, monst, blow, result, info)
		result.harmful = food.dec_stat(A_STR)
	end
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "LOSE_INT",
	["desc"]	= "reduce intelligence",
	["power"]	= 0,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = false
		if (do_dec_stat(A_INT, STAT_DEC_NORMAL)) then special.obvious = true end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(obj, monst, blow, result, info)
		result.harmful = food.dec_stat(A_INT)
	end
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "LOSE_WIS",
	["desc"]	= "reduce wisdom",
	["power"]	= 0,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = false
		if (do_dec_stat(A_WIS, STAT_DEC_NORMAL)) then special.obvious = true end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(obj, monst, blow, result, info)
		result.harmful = food.dec_stat(A_WIS)
	end
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "LOSE_DEX",
	["desc"]	= "reduce dexterity",
	["power"]	= 0,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = false
		if (do_dec_stat(A_DEX, STAT_DEC_NORMAL)) then special.obvious = true end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(obj, monst, blow, result, info)
		result.harmful = food.dec_stat(A_DEX)
	end
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "LOSE_CON",
	["desc"]	= "reduce constitution",
	["power"]	= 0,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = false
		if (do_dec_stat(A_CON, STAT_DEC_NORMAL)) then special.obvious = true end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(obj, monst, blow, result, info)
		result.harmful = food.dec_stat(A_CON)
	end
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "LOSE_CHR",
	["desc"]	= "reduce charisma",
	["power"]	= 0,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = false
		if (do_dec_stat(A_CHR, STAT_DEC_NORMAL)) then special.obvious = true end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(obj, monst, blow, result, info)
		result.harmful = food.dec_stat(A_CON)
	end
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "LOSE_ALL",
	["desc"]	= "ruin",
	["power"]	= 2,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = false
		if (do_dec_stat(A_STR, STAT_DEC_NORMAL)) then special.obvious = true end
		if (do_dec_stat(A_CON, STAT_DEC_NORMAL)) then special.obvious = true end
		if (do_dec_stat(A_DEX, STAT_DEC_NORMAL)) then special.obvious = true end
		if (do_dec_stat(A_INT, STAT_DEC_NORMAL)) then special.obvious = true end
		if (do_dec_stat(A_WIS, STAT_DEC_NORMAL)) then special.obvious = true end
		if (do_dec_stat(A_CHR, STAT_DEC_NORMAL)) then special.obvious = true end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(obj, monst, blow, result, info)
		result.harmful = food.dec_stat(A_STR)
		result.harmful = food.dec_stat(A_CON) or result.harmful
		result.harmful = food.dec_stat(A_DEX) or result.harmful
		result.harmful = food.dec_stat(A_INT) or result.harmful
		result.harmful = food.dec_stat(A_WIS) or result.harmful
		result.harmful = food.dec_stat(A_CHR) or result.harmful
	end
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "DISEASE",
	["desc"]	= "disease",
	["power"]	= 5,
	["type"]	= dam.POIS,
	["player"]	= function(params, special, method, m_idx)
		special.obvious = FALSE

		-- Damage CON (10% chance)
		if (rng.percent(10)) then
			-- 1% chance for perm. damage
			local perm = STAT_DEC_NORMAL
			if rng.percent(1) then perm = STAT_DEC_PERMANENT end
			if (dec_stat(A_CON, rng(10), perm)) then special.obvious = true end
		end
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(obj, monst, blow, result, info)
		result.dam = {use_project=true}

		-- Damage CON (10% chance)
		if (rng.percent(10)) then
			-- 1% chance for perm. damage
			local prev = player.stat(stat)
			local perm = STAT_DEC_NORMAL
			if rng.percent(1) then perm = STAT_DEC_PERMANENT end
			dec_stat(A_CON, rng(10), perm)
			result.harmful = (player.stat(stat) < prev)
		end
	end
}

function monsters.suck_life(dice, sides)
	local amnt = 0
	if player.has_intrinsic(FLAG_HOLD_LIFE) and
		rng.percent(70 + (player.intrinsic(FLAG_HOLD_LIFE) * 5))
	then
		message("You keep hold of your life force!")
	else
		local amnt = rng.roll(dice, sides) + (player.exp / 100) * 2
		if player.has_intrinsic(FLAG_HOLD_LIFE) then
			amnt = amnt / (5 + player.intrinsic(FLAG_HOLD_LIFE))
			message("You feel your life slipping away!")
		else
			message("You feel your life draining away!")
		end
		lose_exp(amnt)
	end
	return amnt
end
add_monster_attack_type
{
	["name"]	= "EXP_10",
	["desc"]	= "suck life",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		monsters.suck_life(10, 6)
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(corpse, monst, blow, result, info)
		result.harmful = food.suck_life(10, 6)
	end,
}
add_monster_attack_type
{
	["name"]	= "EXP_20",
	["desc"]	= "suck life",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		monsters.suck_life(20, 6)
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(corpse, monst, blow, result, info)
		result.harmful = food.suck_life(20, 6)
	end,
}
add_monster_attack_type
{
	["name"]	= "EXP_40",
	["desc"]	= "suck life",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		monsters.suck_life(40, 6)
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(corpse, monst, blow, result, info)
		result.harmful = food.suck_life(40, 6)
	end,
}
add_monster_attack_type
{
	["name"]	= "EXP_80",
	["desc"]	= "suck life",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["obvious"]	= true,
	["player"]	= function(params, special, method, m_idx)
		monsters.suck_life(80, 6)
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["on_eat"]  = function(corpse, monst, blow, result, info)
		result.harmful = food.suck_life(80, 6)
	end,
}
add_monster_attack_type
{
	["name"]	= "DISENCHANT",
	["desc"]	= "disenchant",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		project(m_idx, 0, player.py, player.px, params.dam, dam.UN_BONUS,
				PROJECT_JUMP | PROJECT_STOP | PROJECT_KILL | PROJECT_HIDE |
				PROJECT_HIDE_BLAST | PROJECT_NO_REFLECT)
	end,
	["monster"]	= function(params, special, method, m_idx)
		mon = params.t_ptr
		project(m_idx, 0, mon.fy, mon.fx, params.dam, dam.UN_BONUS,
				PROJECT_JUMP | PROJECT_STOP | PROJECT_KILL | PROJECT_HIDE |
				PROJECT_HIDE_BLAST | PROJECT_NO_REFLECT)
	end,
}
add_monster_attack_type
{
	["name"]	= "DRAIN_CHARGE",
	["desc"]	= "drain charges",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		message("IMPLEMENT DRAIN_CHARGE!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "ABOMINATION",
	["desc"]	= "abomination",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		message("IMPLEMENT ABOMINATION!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["no_body"] = true,
}
add_monster_attack_type
{
	["name"]	= "SHATTER",
	["desc"]	= "shatter",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		message("IMPLEMENT SHATTER!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
}
add_monster_attack_type
{
	["name"]	= "PARASITE",
	["desc"]	= "parasitize",
	["power"]	= 5,
	["type"]	= dam.PURE,
	["player"]	= function(params, special, method, m_idx)
		message("IMPLEMENT PARASITE!")
	end,
	["monster"]	= function(params, special, method, m_idx)
	end,
	["no_body"] = true,
}

--------------------------------------------------------------------------
--------------------------- Monster regen --------------------------------
--------------------------------------------------------------------------

--- @fn
-- @brief Regenerate a monster every 10 game turns
function monster_regen(m_idx, monst)
	-- Regen life
	if monst.hp < monst.maxhp then
		local regen = flag_get(monst.flags, FLAG_REGENERATE)
		if regen then
			local nb = abs(regen)
			local amt = nb / 1000
			local im = imod(nb, 1000)
			if im ~= 0 then
				if imod(turn / 10, 1000 / im) == 0 then
					amt = amt + 1
				end
			end

			if amt ~= 0 then
				if regen < 0 then amt = -amt end
				monst.hp = monst.hp + amt
				if monst.hp > monst.maxhp then monst.hp = monst.maxhp end
			end
		end
	end

	-- Regen mana
	local mana, max_mana = flag_get(monst.flags, FLAG_MANA_CURR), flag_get(monst.flags, FLAG_MANA_MAX)
	if mana < max_mana then
		local regen = flag_get(monst.flags, FLAG_MANA_REGENERATE)
		if regen then
			local nb = abs(regen)
			local amt = nb / 1000
			local im = imod(nb, 1000)
			if im ~= 0 then
				if imod(turn / 10, 1000 / im) == 0 then
					amt = amt + 1
				end
			end

			if amt ~= 0 then
				if regen < 0 then amt = -amt end
				if amt + mana > max_mana then amt = max_mana - mana end
				flag_inc(monst.flags, FLAG_MANA_CURR, amt)
			end
		end
	end

	-- Get rid of whatever effecst it might have
	if m_idx ~= 0 then
		monster_effect.timeout(m_idx, monst)
	end
end

--------------------------------------------------------------------------
-------------------------------- Pets ------------------------------------
--------------------------------------------------------------------------
load_subsystem("pets", {
	pet_types = {
		ally      = true,
		companion = true,
		partial   = true,
		pet       = true,
	}

	get_pet_type =
		function(monst)
			if monst.flags[FLAG_PET_TYPE] then
				return monst.flags[FLAG_PET_TYPE]
			end

			if monst.flags[FLAG_PERMANENT] then
				return "companion"
			elseif monst.flags[FLAG_PARTIAL_SUMMON] then
				return "partial"
			elseif monst.flags[FLAG_PET] then
				return "pet"
			else
				return "ally"
			end
		end -- get_pet_type

	set_pet_ai =
		function(monst)
			pets.default_set_pet_ai(monst)
			if not monst.flags[FLAG_PARTIAL_SUMMON] and
				(monst.flags[FLAG_EMPTY_MIND] or
				 (monst.flags[FLAG_ANIMAL] and monst.flags[FLAG_STUPID]))
			then
				monst.flags[FLAG_PET] = true
			end
		end

	remove_pet_ai =
		function(monst)
			monst.flags[FLAG_PET] = nil
			pets.default_remove_pet_ai(monst)
		end
}) -- pets

function pets.give_cmd_to_allies(menu_item, pets_list)
	return pets.give_cmd_to_all_type(pets_list, "ally")
end

function pets.give_cmd_to_companions(menu_item, pets_list)
	return pets.give_cmd_to_all_type(pets_list, "companion")
end

function pets.give_cmd_to_partials(menu_item, pets_list)
	return pets.give_cmd_to_all_type(pets_list, "partial")
end

function pets.give_cmd_to_pets(menu_item, pets_list)
	return pets.give_cmd_to_all_type(pets_list, "pet")
end

pets.add_menu_items {
	{
		do_menu_item = pets.give_cmd_to_allies,
		desc         = "Give command to allies",
	},
	{
		do_menu_item = pets.give_cmd_to_companions,
		desc         = "Give command to companions",
	},
	{
		do_menu_item = pets.give_cmd_to_partials,
		desc         = "Give command to partials",
	},
	{
		do_menu_item = pets.give_cmd_to_pets,
		desc         = "Give command to pets",
	},
	pets.default_menu_items.command_name,
	pets.default_menu_items.command_not_name,
}

--------------------------------------------------------------------------
-------------------------------- Misc ------------------------------------
--------------------------------------------------------------------------
constant("compare_monsters",
function(monst_a, monst_b, what, args)
	local mhp_a, lev_a
	if monst_a.r_idx == RACE_PLAYER then
		mhp_a = player.mhp()
		lev_a = player.lev
	else
		mhp_a = monst_a.maxhp
		lev_a = monst_a.level
	end

	local mhp_b, lev_b
	if monst_b.r_idx == RACE_PLAYER then
		mhp_b = player.mhp()
		lev_b = player.lev
	else
		mhp_b = monst_b.maxhp
		lev_b = monst_b.level
	end

	local pct
	if what == "level" then
		if lev_a == 0 then
			pct = infinity
		else
			pct = lev_b * 100 / lev_a
		end
	elseif what == "maxhp" then
		pct = mhp_b * 100 / mhp_a
	else
		return false
	end

	if pct >= args.min and pct <= args.max then
		return true
	end
	return false
end)
