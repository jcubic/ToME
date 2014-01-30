-- necro.lua
-- Defines the Necromancy school of magic, with it's spells, artifacts, objects, and monster egos.
--
--
-- This first section is outdated. It's my initial design, but was subject to (a lot of) change.
--
-- This section is the 'dream' section -- the overall plan for where I want necromancy to go.
-- What do I want to do?
-- Select from a list (exactly the same as a spellbook)
-- With the following spells
-- animate - animates a corpse. - level 1 spell
--              level 1. 150% hp, -10 speed, immune to piercing, little resistant to slash, little resistant to bash, resist cold
--                      +10 level
--              level 5. 150% hp, -5 speed, ditto
--              level 10. normal speed, immune cold
--              level 20. double speed (smooth transition here, please...)
--              level 30. add resists
--              level 40. add hp, and more resists.
--              level 50. immune to all physical damage, +40 level
-- animate skeleton - animates a skeleton
--              level 10 spell, works only on skeletons (include junk skeletons!!!)
--              Note that though this is effectively transitions into a lich, they NEVER gain
--              summon spells.
--              level 1. 100% hp, normal speed, immune to slash & pierce.
--              level 10. 150% hp, +5 speed
--              level 20. 200% hp, +10 speed, gains spell attacks
--              level 30. 200% hp, +10 speed, gains more spell attacks, casts more often
--              level 40. gains teleport to, heal other, heal self.
--              level 50. 50% hp, gains wraithform? no, I think not. That's what specters are for...
-- call spirit - calls the spirit of a corpse or skeleton. - might work on a hunk of meat?
--              level 20 spell, summons a specter of the appropriate sort.
--              level 1 - 50% hp, normal speed, wraithform
--              level 10 - 100% hp, +5 speed, gain confusion attack
--              level 20 - 200% hp, +10 speed, gain drain opponent to heal self
--              level 40 - 300% hp, +20 speed, double physical attacks
-- horrify - scare, and eventually confuse, and then stun, a monster
--              Starts with a fairly high damage, but doesn't increase it's damage much.
--              BUT! you can cast it through walls, on monsters withought LOS, if they're in range.
--              no good at all on non-living or natural creatures, effects only enemies.
--              eventually auto-cast at a lower level? -- i.e. no stun.
-- vampiric bite (like vamiric racial ability)
--              gained as an intrinsic ability at level 15 or so.
-- conjure vampiric teeth (exactly like tome 2.3 spell)
-- word of death
--              if the monster makes his saving throw, then he's still damaged and *irresitably* stuned.
-- Charm Undead
--              level 17 spell
--              *powerful* charm spell - a high-level Necromancer can easily charm almost any undead.
--              At first, touch range, then bolt, then beam, then ball, then view, then area, finally global.
--              eventually, auto-cast as part of a necromancer's aura.
--
-- The Necronomicon is complete
-- Necronomicon spells:
-- genocide - just like the scroll
-- mass genocide - just like the scroll
-- draining touch - like vamiric bite, but doesn't feed you, heals mana instead of hp, and works on undead/demons too.
-- Sacrifice - kill a (living) pet to regenerate your mana. Mana gain depends on the creature.
--              See the demonology spell, only works on living pets.
--
--
-- TODO: These spells aren't implemented yet...
-- aura turning - attempt to damage a monster via it's own aura.
--              works only on magical creatures of lesser ability than the caster.
--              level 15 spell, irrestisable damage, fairly high damage (though not as high as manathrust...)
-- Repair Undead
--              level 15 spell, ball attack, heals only friendly undead.
-- detect undead.
--              level 2 spell, 1 mana cost (constant)
--              necromancers gain intrinsic esp-undead at about level 15.
-- draining touch
--              temporarily makes a weapon vampiric - *even* if it's an artifact/ego weapon.
--              works on more than one weapon at a time, and doesn't change the value of the weapon.
-- decaying touch?
-- absorb soul - ball spell, lasting effect - living monsters killed in area regen your hp. (mana?)
--              Unlike the tome-233 spell, this should either be only in the area effected by your
--              ball, or only visible monsters, or something like that.
-- bind servant - turns a pet into a companion, shares life.
--              level 30 zero-cost spell, opens a 'new spellbook'
--              1. select a pet to turn into a servant (if you don't already have one)
--              2. drain servant - his mana/hp may be used (ineffecently) as mana.
--              3. damage servant - his mana/hp may be used to heal you
--              4. summon servant - teleport your servant to yourself.
--              5. You may cast his spells, mana cost to be determined.
--              advantages:
--              servant and you gain (large) regeneration bonus.
--              you gain access to servant's spells, and can cast them as your own, draining
--              HIS mana (or hp) to do so. servant can also cast your spells?
--              disadvantages:
--                      Damage to the servant IS damage to you. At the rate of .5% hp per percent of HIS hp damaged.
--                      This, of course, can kill you.


declare_global_constants{
        "SCHOOL_NECRO",
        "HORRIFY",
        "ANIMATE",
        "scare_monster",
        "confuse_monster",
        "stun_monster",
        "get_horrify_dam",
        "next_to_player",
        "SV_SWORD_TEETH",
        "SUMMON_TEETH",
        "VAMPIRIC_BITE",
        "read_the_necronomicon",
        "NECRO_SACRIFICE",
        "NECRO_DRAIN",
        "NECRO_GENOCIDE",
        "NECRO_MASS_GENOCIDE",
        "get_close_m_idx",
        "WORD_OF_DEATH",
        "CHARM_UNDEAD",
        "ASSEMBLE",
        "CONJURE",
	"ABSORB_SOUL",
}
declare_globals {
}

SV_SWORD_TEETH = 34 -- TODO There's got to be a better (more compatible) way to get these values...

-- Create the schools
SCHOOL_NECRO = add_school
{
        ["name"]  = "Necromancy",
        ["skill"] = "SKILL_NECROMANCY",
        ["hooks"] =
        {
                [hook.CALC_BONUS] = function()
                        local level = get_skill_scale(SKILL_NECROMANCY, 50)
                        --FIXME Dagnabit! There's got to be a pretty way to do this!
                        --one that doesn't make me want to define a helper function!
                        --[[
                        This doesn't work at ALL. I have no idea how to do this...
function increase_resistance_to(flag,percent)
        local cur=flag_get(player.resists,flag)
        if cur < percent then
                flag.set(player.resists,flag,percent)
        end
end
                        if level >= 50 then
                        increase_resistance_to(FLAG_RES_NETHER,50)
--                              flag_get(player.resists,FLAG_RES_NETHER))
                        elseif level >= 40 then
                        increase_resistance_to(FLAG_RES_NETHER,30)
--                              flag_add_higher(player.resists, 75)
                        elseif level >= 30 then
                        increase_resistance_to(FLAG_RES_NETHER,20)
--                              flag_add_higher(player.resists, 50)
                        elseif level >= 20 then
                        increase_resistance_to(FLAG_RES_NETHER,10)
--                              flag_add_higher(player.resists, 25)
                        end
                        ]]
                end,
        }
        -- TODO:
        -- gain "vampiric bite" ability
        -- auto-cast some centered-on the player ball spells, as the
        -- 'aura' of the necromancer. Charm, and NECRO_STUN
        -- When a high level necromancer walks into the room, lesser
        -- undead should fall on their knees and worship. But greater
        -- undead just get ticked off.
        --

}



function get_close_m_idx(pet,dist)
                local ret, dir
                local y, x
                if not dist then dist=1 end

                ret, dir = get_aim_dir()
                if not ret then return end

                -- Handle a direction
                local dy, dx = explode_dir(dir)
                y = player.py + dy
                x = player.px + dx

                -- Handle a actual target.
                if dir == 5 then
                        y = target_row
                        x = target_col
                end

                local c_ptr = cave(y, x)

                -- Check max allowed distance
                if distance(player.py,player.px,y,x) > dist then
                        return
                end

                -- ok that is a monster
                if c_ptr.m_idx > 0 then
                        return true, c_ptr.m_idx
                end

                return
end

function next_to_player()
        -- TODO this should probably use the aim_dir stuff?
        -- maybe even get the direction from the player...
        local grids =
        {{x=-1,y=-1},{x=-1,y=0},{x=-1,y=1},
         {x=0,y=-1},        {x=0,y=1},
         {x=1,y=-1}, {x=1,y=0}, {x=1,y=1},}
        for i, s in grids do
                -- i is the index (and useless) s is the value (a table of y,x offsets)
                -- Extract these in combination with the player's location.
                local y = player.py + s.y
                local x = player.px + s.x

                -- Check for a non-wall, non-door grid
                if cave_floor_bold(y,x) then
                        --check for no monsters
                        if cave(y,x).m_idx == 0 then
                                return y,x
                        end
                end
        end
end



function confuse_monster(mon,time,who)
        assert(mon, "No monster passed to confuse_monster")
        assert(time, "No time passed to confuse_monster")
        if (wizard) then message("confuse_monster, " .. time) end


        if not who then who=0 end
        local r_ptr=race_info(mon)
        local m_name=monster_desc(mon,0)
        if (has_flag(r_ptr, FLAG_NO_CONF)) then
                if (mon.ml) then
                        flag_set(r_ptr.r_flags, FLAG_NO_CONF, true)
                end
                message(strcap(m_name).." is unaffected.")
        elseif monster_saving_throw(who, mon) then
                message(strcap(m_name).." is unaffected.")
        else
                message(strcap(m_name).." appears confused.")
                mon.confused = mon.confused + time
        end
end
function stun_monster(mon,time,who)
        assert(mon, "No monster passed to stun_monster")
        assert(time, "No time passed to stun_monster")
        if (wizard) then message("stun_monster, "..time) end

        if not who then who=0 end
        local r_ptr=race_info(mon)
        local m_name=monster_desc(mon,0)
        if has_flag(r_ptr, FLAG_NO_STUN) then
                if (mon.ml) then
                        flag_set(r_ptr.r_flags, FLAG_NO_STUN, true)
                end
                monster_msg(strcap(m_name).." is unaffected!", mon.ml)
        elseif monster_saving_throw(who, mon) then
                monster_msg(strcap(m_name).." is unaffected!", mon.ml)
        else
                monster_msg(strcap(m_name).." is stunned!", mon.ml)
                mon.stunned = mon.stunned + time
        end
end
function scare_monster(mon,time, who)
        assert(mon, "No monster passed to scare_monster")
        assert(time, "No time passed to scare_monster")
        if (wizard) then message("scare_monster, "..time) end

        local r_ptr=race_info(mon)
        if not who then who=0 end
        local m_name=monster_desc(mon,0)

        if has_flag(r_ptr, FLAG_NO_FEAR) then
        --TODO: add to monster memory.
                if (mon.ml) then
                        flag_set(r_ptr.r_flags, FLAG_NO_FEAR, true)
                end
                monster_msg(strcap(m_name).." is unaffected!", mon.ml)
        elseif monster_saving_throw(who, mon) then
                monster_msg(strcap(m_name).." is unaffected!", mon.ml)
        else
                monster_msg(strcap(m_name).." is afraid!", mon.ml)
                mon.monfear = mon.monfear + time
        end
end

-- A user interface to cast the spells.
-- We add an MKEY for them (note that MKEYS have to be enabled in
-- s_info.txt or somewhere similar to show up for the user)

add_mkey
{
        mkey	= "Necromancy",
	type	= "skill",
        fct	= function()
                -- create a necromancy book (has SPECIAL_GENE)
                local necro_book = make_book
		(
		        HORRIFY,
		        ANIMATE,
		        CHARM_UNDEAD,
		        SUMMON_TEETH,
		        VAMPIRIC_BITE,
		        NECRO_DRAIN,
		        ASSEMBLE,
		        NECRO_SACRIFICE,
		        ABSORB_SOUL,
		        CONJURE,
		        NECRO_GENOCIDE,
		        WORD_OF_DEATH,
		        NECRO_MASS_GENOCIDE
		)

		local spl = nil
		local ret, rep = repeat_keys.pull()
		if ret then
			if is_ok_spell(rep, necro_book) then
				spl = rep
			end
		end

                -- select a spell from that book
                spl = spl or get_school_spell("cast", "is_ok_spell", necro_book)

                -- and delete it.
                delete_object(necro_book)

                if spl == -1 then return end

                -- Now to actually cast the spell...
                cast_school_spell(spl, spell(spl))

        end,
}

dam.CHARM = dam.add
{
        color   = function() return iif(rng(5)<3, color.YELLOW, color.LIGHT_GREEN) end,
        gfx     = 4,
        desc    = "charm",
        monster = function(state)
                if state.m_ptr.faction == FACTION_PLAYER then return { dam=0 } end

                -- TODO: adjust for Charisma?  dam += (adj_con_fix[p_ptr->stat_ind[A_CHR]] - 1);
                if has_flag(state.m_ptr,FLAG_QUEST) or has_flag(state.m_ptr,FLAG_UNIQUE)
                or (has_flag(state.m_ptr,FLAG_NO_CONF) and not state.charmconf)
                or state.m_ptr.level > rng.number(dam) then
                        -- TODO: memorize NO_CONF flag...
                        return { msg=" is unaffected!", dam=0, }
                end
                -- TODO: Aggravate flag on player totally prevents charmming
                if FALSE then
                        return { msg=" hates you too much!", dam=0, }
                end
                -- TODO: Inc piety for GOD_YAVANNA, or anything else...
                -- if ((r_ptr->flags3 & RF3_ANIMAL) && (!(r_ptr->flags3 & RF3_EVIL)))
                -- inc_piety(GOD_YAVANNA, m_ptr->level * 2);

                -- Charm the monster... and get out of here
				if factions.change_faction(state.m_ptr.faction,
										   FACTION_PLAYER)
				then
					state.dam = 0
					state.msg=" suddenly seems friendly!"
				else
					return {msg=" is unaffected!", dam=0}
				end
                if state.charmmsg then state.msg=state.charmmsg end
        end
}
dam.CHARM_UNDEAD = dam.derive
{
        source  = dam.CHARM
        desc    = "control undead",
        modifier= function(target,state)
                if target == "monster" and not has_flag(state.m_ptr,FLAG_UNDEAD) then
                        state.dam=0
                end
                -- Charm even undead who resist confusion
                state.charmconf=true
                -- give a more 'forcefull' success message.
                state.charmmsg=" is in your thrall!"
                -- TODO: Change the messages: " is in your thrall!"
                -- There seems to be no (easy) way to do that in this framework...
        end
}


-- TODO: shouldn't this be CHARM_ANIMAL, just to be consistent with the others?
-- The other referance is in tome/data/magic/yavanna.lua
dam.CONTROL_ANIMAL = dam.derive
{
        source  = dam.CHARM
        desc    = "control animal",
        modifier= function(target,state)
                if target == "monster" and not has_flag(state.m_ptr,FLAG_ANIMAL) then
                        state.dam=0
                end
        end
}

dam.NECRO_STUN = dam.add
{
        color   = function() return iif(rng(5)<3, color.YELLOW, color.LIGHT_GREEN) end,
        gfx     = 4,
        desc    = "unholy aura",
        monster = function(state)
                -- here, test for 'pet' status, and if a pet, cure fear and conf + heal?
                if state.m_ptr.faction ~= FACTION_PLAYER then
                        -- these effects are in addition to the actual damage...
                        scare_monster(state.m_ptr,20+min(state.dam/10,20))
                        if not state.necro_no_conf then confuse_monster(state.m_ptr,10+min(state.dam/20,10)) end
                        if not state.necro_no_stun then stun_monster(state.m_ptr,1 +min(state.dam/100,2)) end
                else
                        if has_flag(state.m_ptr,FLAG_UNDEAD) then
                                state.dam = 0 - ( 3 + state.dam/10 ) -- heal 2 hp per 100 damage.
                        end
                        local desc, pronoun = monster_desc(state.m_ptr,0),monster_desc(state.m_ptr,34)
                        -- Note: always cures these, even at low levels
                        if state.m_ptr.monfear then
                                state.m_ptr.monfear = 0
                                message(desc .." regains " .. pronoun .. " courage")
                        end
                        if state.m_ptr.stunned then
                                state.m_ptr.stunned = 0
                                message(desc .. " is no longer stunned")
                        end
                        if state.m_ptr.confused then
                                state.m_ptr.confused= 0
                                message(desc .. " reaches a decision")
                        end
                end
        end,
}
dam.NECRO_CONF = dam.derive
{
        source  = dam.NECRO_STUN
        desc    = "control undead",
        modifier= function(target,state)
                if target == "monster" then
                        state.necro_no_stun=true
                end
        end
}
dam.NECRO_FEAR = dam.derive
{
        source  = dam.NECRO_STUN
        desc    = "control undead",
        modifier= function(target,state)
                if target == "monster" then
                        state.necro_no_stun=true
                        state.necro_no_conf=true
                end
        end
}

function get_horrify_dam()
        return 2 + get_level(HORRIFY, 10), 4 + get_level(HORRIFY, 10)
end
HORRIFY = add_spell
{
        name     = "Horrify",
        school   = SCHOOL_NECRO,
        level    = 1,
        mana     = 1,
        mana_max =  25,
        fail     = 10,
	-- Unaffected by blindness
	["blind"] =     FALSE,
        spell    = function()
                        local ret, dir

                        ret, dir = get_aim_dir()
                        if not ret then return SPELL_NOTHING end
                        if get_cast_level(50) < 10 then
                                return fire_bolt(dam.NECRO_FEAR, dir, rng.roll(get_horrify_dam()))
                        elseif get_cast_level(50) < 30 then
                                return fire_beam(dam.NECRO_CONF, dir, rng.roll(get_horrify_dam()))
                        else
                                return fire_ball(dam.NECRO_STUN, dir, rng.roll(get_horrify_dam()), get_cast_level(4))
                        end
        end,
        info     = function()
                        return "dam "..rng.desc_roll(get_horrify_dam())
        end,
        desc     ={
                        "Use your aura to directly attack a monster.",
                        "At level 1, it scares them",
                        "At level 10, it may confuse them",
                        "At level 30, it may also stun them for 1d2 turns",
                }
}

ANIMATE = add_spell
{
        name     = "Animate",
        school   = SCHOOL_NECRO,
        level    = 1,
        mana     = 6,
        mana_max = 75,
        fail     = 30,
        spell    = function()
                -- TODO: what about unique corpses?
                local ret, item, m_ptr, o_ptr, r_idx, ego
                local level=get_cast_level(50)

                ret, item = get_item("Animate which corpse?",
                 "You don't have a corpse.",
                 USE_INVEN | USE_FLOOR,
                 function (obj) return (obj.tval == TV_CORPSE) and (obj.sval == SV_CORPSE_CORPSE) end
                )
                if not ret then return end
                o_ptr = get_object(item)
				m_ptr = o_ptr.flags[FLAG_MONSTER_OBJ]
                r_idx = m_ptr.r_idx
                ego   = RACE_EGO_ZOMBIE1
                if level >= 10 then ego = RACE_EGO_ZOMBIE2 end
                if level >= 25 then ego = RACE_EGO_ZOMBIE3 end
                if level >= 35 then ego = RACE_EGO_ZOMBIE4 end
                if level >= 50 then ego = RACE_EGO_ZOMBIE5 end
                --TODO: add more egos, with zombie5 (and maybe 4) having access to low level spells,
                --if the original monster had them (we'd have to test that here...)

                -- Actually create the monster
                local x,y;
                y,x = next_to_player();
		place_monster_one_no_drop = true
                ret = place_monster_one(y,x,r_idx,ego,0,FACTION_PLAYER)
		place_monster_one_no_drop = false
                if not ret then
                        msg_format("The corpse twitches, but can't stand up.")
                        return
                end
                -- Great! Now delete the corpse...
                item_increase(item,-1)
                item_describe(item)
                item_optimize(item)

                -- TODO: This is where we should check for failure. Especially if we're low on mana,
                -- the zombie may break free of the our control in the first moments after rising.

                -- Describe what happened
                local mon = monster(ret)
                msg_format("The corpse rises, and %s stands on its own.",monster_desc(mon,0))
        end,
        info     = function()
                local level=get_cast_level(50)
                local ego

                -- "150%hp,-10 speed"
                ego   = RACE_EGO_ZOMBIE1
                if level >= 10 then ego = RACE_EGO_ZOMBIE2 end
                if level >= 25 then ego = RACE_EGO_ZOMBIE3 end
                if level >= 35 then ego = RACE_EGO_ZOMBIE4 end
                if level >= 50 then ego = RACE_EGO_ZOMBIE5 end
                if ego == RACE_EGO_ZOMBIE1 then return "-10 speed, +50% hp" end
                if ego == RACE_EGO_ZOMBIE2 then return "-5 speed, +100% hp" end
                if ego == RACE_EGO_ZOMBIE3 then return "normal speed, +200% hp" end
                if ego == RACE_EGO_ZOMBIE4 then return "+5 speed, +200% hp" end
                if ego == RACE_EGO_ZOMBIE5 then return "+10 speed, +300% hp" end
                return "BUG! unkown ego?"
        end,
        desc     ={
                        "Animate a corpse, giving movement and a sembalence of life to dead flesh,",
                        "and purpose to a dead brain. At first, your zombies will be slow and weak.",
                        "But with practise, you'll learn to make fast and tough zombies that will",
                        "be a terror to your enemies.",
                }
}

ASSEMBLE = add_spell
{
        name     = "Assemble",
        school   = SCHOOL_NECRO,
        level    = 17,
        mana     = 30,
        mana_max = 100,
        fail     = 30,
        spell    = function()
                -- TODO: what about unique corpses?
                local ret, item, o_ptr, m_ptr, r_idx, ego
                local level=get_cast_level(50)

                -- Skeletons may be assembled from skeletons or from corpses.
                -- BUT! The corpse MUST have the DROP_SKELETON flag, as there may be
                -- races which drop a corpse, but don't have bones...
                ret, item = get_item("Assemble which skeleton?",
                "You don't have a skeleton, not even one clothed in flesh.",
                USE_INVEN | USE_FLOOR,
                        function (obj)
                                if (obj.tval ~= TV_CORPSE) then return 0 end
                                if obj.sval == SV_CORPSE_SKELETON then return true end
                                if obj.sval ~= SV_CORPSE_CORPSE then return 0 end
								local m_ptr = obj.flags[FLAG_MONSTER_OBJ]
                                return has_flag(m_ptr, FLAG_DROP_SKELETON)
                        end
                )
                if not ret then return end
                o_ptr = get_object(item)
				m_ptr = o_ptr.flags[FLAG_MONSTER_OBJ]
                r_idx = m_ptr.r_idx
                ego   = RACE_EGO_SKELETON1
                if level >= 10 then ego = RACE_EGO_SKELETON2 end
                if level >= 25 then ego = RACE_EGO_SKELETON3 end
                -- we've got our dead thing. Now to summon a monster, and modify it.
                -- Don't permit the new monster to have inventory (no summon and rekill for items!)

                -- Actually create the monster
                local x,y;
                y,x = next_to_player();
		place_monster_one_no_drop = true
                ret = place_monster_one(y,x,r_idx,ego,0,FACTION_PLAYER)
		place_monster_one_no_drop = false
                if not ret then
                        msg_format("The bones twitch, but can't stand up.")
                        return
                end
                -- Great! Now delete the corpse/skeleton
                item_increase(item,-1)
                item_describe(item)
                item_optimize(item)

                -- TODO: This is where we should check for failure. Especially if we're low on mana,
                -- the zombie may break free of the our control in the first moments after rising.

                -- Describe what happened
                msg_format("The bones dance!")
        end,
        info     = function()
                return ""
        end,
        desc     ={
                }
}

CONJURE = add_spell
{
        name     = "Conjure",
        school   = SCHOOL_NECRO,
        level    = 33,
        mana     = 100,
        mana_max = 200,
        fail     = 30,
        spell    = function()
                local ret, item, o_ptr, m_ptr, r_idx, ego
                local level=get_cast_level(50)

                -- Specters, unlike zombies or skeletons, don't have physical substance,
                -- and it's possible to create many from one, requiring only a piece of
                -- a monster as a focus.
                ret, item = get_item("Use what focus?",
                "You have no dead flesh, nor any bones, with which to conjure.",
                USE_INVEN | USE_FLOOR,
                        function (obj)
							if (obj.tval ~= TV_CORPSE) then return 0 end
							if not obj.flags[FLAG_MONSTER_OBJ] then
								return 0
							end
							return true
                        end
                )
                if not ret then return end
                o_ptr = get_object(item)
				m_ptr = o_ptr.flags[FLAG_MONSTER_OBJ]
                r_idx = m_ptr.r_idx
                ego   = RACE_EGO_SPECTER1
                if level >= 10 then ego = RACE_EGO_SPECTER2 end
                -- we've got our dead thing. Now to summon a monster, and modify it.
                -- Don't permit the new monster to have inventory (no summon and rekill for items!)

                -- Actually create the monster
                local x,y;
                y,x = next_to_player();
		place_monster_one_no_drop = true
                ret = place_monster_one(y,x,r_idx,ego,0,FACTION_PLAYER)
		place_monster_one_no_drop = false
                if not ret then
                        msg_format("The bones twitch, but can't stand up.")
                        return
                end
                -- Note: Don't destroy the focus object...

                -- TODO: *THIS* is where we should check for failure. Especially if we're low on mana,
                -- the zombie may break free of the our control in the first moments after rising.

                -- Describe what happened
                msg_format("A spectral apparition appears!")
        end,
        info     = function()
                return ""
        end,
        desc     ={
                }
}

SUMMON_TEETH=add_spell
{
        name     = "Necromatic Teeth",
        school   = SCHOOL_NECRO,
        level    = 7,
        mana     = 20,
        mana_max = 40,
        fail     = 20,
	-- Unaffected by blindness
	["blind"] =     FALSE,
        spell    = function()
                local obj=create_object(TV_SWORD,SV_SWORD_TEETH)
                local level=get_cast_level(50)
                -- timeout formula is from tome 2.33
                obj.timeout = rng.number(100 + level * 4) + 200 + level * 3
                obj.number = 1
                -- TODO: add 1/2 dungeon level?
                -- TODO: apply_magic doesn't seem to do anything? It should
                -- make our teeth into an ego or double ego weapon, but it doesn't
                apply_magic(obj,level*2,true,true,true)

                -- one in blah player-created artifacts becomes permenent (at level 50)
                if level < 50 or not artifact_p(obj) or rng.number(30) then
                        set_flag(obj,FLAG_TEMPORARY,obj.timeout)
                end
                set_aware(obj)
                set_known(obj)
                obj.ident = obj.ident | IDENT_MENTAL
                inven_carry_inven(player.inventory[INVEN_INVEN],obj,FALSE)
                return true
        end,
        info     = function()
                local level=get_cast_level(50)
                return (200 + level * 3 ) .. " to " .. (300 + level * 7) .. " turns"
        end,
        desc     ={
                "Creates a vampiric weapon, that will let you feast on the blood",
                "of your foes. For a time, anyway. Necromancers of legend created",
                "unstopable weapons. Maybe this is how?",
                }
}

-- vampiric bite (like vampiric racial ability)
--              gained as an intrinsic ability at level 15 or so.
--              Works only on the living (not undead, demon, or nonliving)
VAMPIRIC_BITE=add_spell
{
        name     = "Vampiric Bite",
        school   = SCHOOL_NECRO,
        level    = 7,
        mana     = 20,
        mana_max = 40,
        fail     = 20,
        spell    = function()
                local ret, m_idx

                ret, m_idx = get_close_m_idx()
                -- print("ret="..ret.." midx="..m_idx)
                if not ret then return SPELL_NOTHING end
                local mon = monster(m_idx)
                local dam,fear,heal
                local level=get_cast_level(50)
                fear    = 0
                dam     = rng.roll(get_cast_level(1,6),10) -- should be 1d10 - 6d10
                heal    = min(mon.hp,dam)
                -- damage the monster


                print("mon= idx=" .. m_idx .. " f_undead=" ..  monster_desc(mon,136))

                -- Heal the player
                -- Right now, it transfers the hp directly 2:1 to the player...
                --  Note that how much damage is done depends on which kind of creature
                --  you bit.
                if has_flag(mon,FLAG_UNDEAD) then
                        -- lots hurt player, starve player
                        message("That tastes horrible!")
                        take_hit(heal/2," drinking dead blood")

                        -- note: this is the same effect as a potion of salt water.
                        -- Maybe I could call its effect?
                        message("You vomit")
                        --set_food(PY_FOOD_STARVE - 1)
                        timed_effect.set(timed_effect.FOOD, food.FOOD_FAINT + 1)
                        timed_effect.set(timed_effect.POISON, 10, 5)
                        timed_effect.inc(timed_effect.PARALYZED, 4)
                elseif has_flag(mon,FLAG_DEMON) then
                        -- some hurt player, starve player
                        message("That tastes bad.")
                        take_hit(heal/4," trying to drink a demon")
                        timed_effect.inc(timed_effect.FOOD, -100)
                elseif has_flag(mon,FLAG_NONLIVING) then
                        -- little hurt player, starve player
                        message("That was not satisfying.")
                        take_hit(heal/8," chipped fangs")
                        timed_effect.inc(timed_effect.FOOD, -100)
                else
                        -- heal player, feed player
                        message("You slake your thirst upon the blood of ".. monster_desc(mon,136))
                        hp_player(heal/2)
			timed_effect.inc(timed_effect.FOOD, 500)
                end
                -- Note: This take_hit call MUST be after the above, otherwise the monster
                -- might be dead, making it hard to read the flag values & etc. :)
                mon_take_hit(m_idx,dam,fear," collapses.")
        end,
        info     = function()
                return rng.desc_roll(get_cast_level(1,6),10) .. " dam"
        end,
        desc     ={
                "Grow fangs and drink the blood of your enemies. Your friends,",
                "too of course. But only if you're feeling hungry...",
                }
}

-- Word of Death
--              if the monster makes his saving throw, then he's still damaged and *irresitably* stuned.
WORD_OF_DEATH=add_spell
{
        name     = "Power Word DIE!!",
        school   = SCHOOL_NECRO,
        level    = 45,
        mana     = 300,
        mana_max = 300,
        fail     = 30,
        spell    = function()
                local ret, m_idx

                ret, m_idx = get_close_m_idx()
                if not ret then return SPELL_NOTHING end
                local mon = monster(m_idx)
                local dam,fear
                local level=get_cast_level(50)
                fear    = 0
                dam     = 800
                -- Describe the spell
                message("You touch " ..  monster_desc(mon,0) .. " and scream DIE!!")
                -- Actually apply the damage
                if not has_flag(mon,FLAG_UNIQUE) or not monster_saving_throw(0, mon) then
                        if(not mon_take_hit(m_idx,dam,fear," dies.")) then
                                message("%s partially resists",monster_desc(mon,0));
                        end
                else
                        mon_take_hit(m_idx,mon.hp+1,fear," dies.")
                end
        end,
        info     = function()
                if(get_cast_level(50)<0) then return "" end
                return "800 damage"
        end,
        desc     ={
                "Touch another being, and by an act of will and a word, sever their life force",
                "from their body. Works (eventually) on any creature.",
                }
}
-- Charm Undead
--              level 17 spell
--              *powerful* charm spell - a high-level Necromancer can easily charm almost any undead.
--              At first, touch range, then bolt, then beam, then ball, then view, then area, finally global.
--              eventually, auto-cast as part of a necromancer's aura.
CHARM_UNDEAD=add_spell
{
        name     = "Dominate the Dead",
        school   = SCHOOL_NECRO,
        level    = 5,
        mana     = 8,
        mana_max = 30,
        fail     = 20,
	-- Unaffected by blindness
	["blind"] =     FALSE,
        spell    = function()
                local ret, dir, dist
                dist = 99
                if get_cast_level(5) < 2 then dist = 1 end

                ret, dir = get_aim_dir(dist)
                if not ret then return SPELL_NOTHING end

                -- Calculage the power and cast the spell
                local damage = 10 + get_cast_level(170)
                local level  = get_cast_level(5)
                if level < 3 then
                        fire_bolt(dam.CHARM_UNDEAD,dir,dam)
                elseif level < 4 then
                        fire_beam(dam.CHARM_UNDEAD,dir,dam)
                elseif level < 5 then
                        -- radius should start at 2 and go to 5...
                        fire_ball(dam.CHARM_UNDEAD,dir,dam,get_cast_level(15)-10)
                else
                        local proj_flags = PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID | PROJECT_VIEWABLE
                        project_los(dam.CHARM_UNDEAD,damage)
                end
        end,
        info     = function()
                local damage = 10 + get_cast_level(170)
                local level  = get_cast_level(5)
                if level < 3 then
                        return damage .. " power bolt"
                elseif level < 4 then
                        return damage .. " power beam"
                elseif level < 5 then
                        return damage .. " power, " .. get_cast_level(15)-10 .. " radius"
                else
                        return damage .. " power"
                end
        end,
        desc     ={
                "A good necromancer commands obedience by his very presence. This spell attempts to",
                "expand and direct your aura towards taking over control of an undead that",
                "rose from the grave by other means, or is otherwise reluctant to acknowledge you as",
                "supreme master of all.",
                }
}


-- The Necronomicon Spells
--      These Spells are different. They have a very low casting cost, and they're all destructive.
--      Sometimes massively destructive. They also drive you insane, without telling you so. :)
-- If we ever implement a 'gradual corruption' system (1 point per 1000 turns wearing a ring of power?)
-- then this should probably play into it as well.
--
--      Note that insanity is less likely, but more massive, with more powerfull spells.
function read_the_necronomicon(pwr)
        -- TODO balance this function...

        local power=pwr
        if not power then power = 2 end
        -- for testing, always go insane :)
        if rng.number(power) ~= 1 then return end
        power = rng.number(power * 20)
        take_sanity_hit(power, "dwelving too deeply in the dark arts")
        if wizard then print("took sanity hit for " .. power) end
end



-- draining touch - like vamiric bite, but doesn't feed you,
-- heals mana instead of hp, and works on undead/demons too.
-- Won't offend pets, even in the process of killing them
-- More efficient, but slower, than Sacrifice. Otherwise quite similar.
-- Also, only right next to the player.
NECRO_DRAIN=add_spell
{
        name     = "Draining Touch",
        school   = SCHOOL_NECRO,
        level    = 10,
        mana     = 1,
        mana_max = 1,
        fail     = 20,
        spell    = function()
                local ret, m_idx

                ret, m_idx = get_close_m_idx()
                if not ret then return SPELL_NOTHING end
                local mon = monster(m_idx)
                local dam,fear,oldfear,heal
                fear    = 0
                dam     = rng.roll(get_horrify_dam())
                oldfear = mon.monfear
                heal    = min(mon.hp,dam)
                -- damage the monster
                mon_take_hit(m_idx,dam,fear," collapses.")

                -- This spell doesn't scare monsters :)
                mon.monfear = oldfear

                -- Heal the player
                -- TODO: the algorythm in sacrifice is based on percentage.
                -- maybe this should be too?
                -- Right now, it transfers the hp directly 1:1 to the player...
                hp_player(heal)
                read_the_necronomicon(1)
        end,
        info     = function()
                return "dam "..rng.desc_roll(get_horrify_dam())
        end,
        desc     ={
                "Suck the vital essence from a monster.",
                }
}


-- Sacrifice - kill a (living) pet to regenerate your mana. Mana gain depends on the creature.
--              See the demonology spell, only works on living pets.
NECRO_SACRIFICE=add_spell
{
        name     = "Sacrifice",
        school   = SCHOOL_NECRO,
        level    = 20,
        mana     = 2,
        mana_max = 2,
        fail     = 20,
	-- Unaffected by blindness
	["blind"] =     FALSE,
        spell    = function()
                local ret, x, y, c_ptr

                -- Note: This spell requires a pet, and, in theory (though not yet in practise) the
                -- get_aim_dir function will not target a pet. So we use tgt_pt instead.
                ret, x, y = tgt_pt()
                if not ret then return SPELL_NOTHING end
                c_ptr = cave(y, x)

                -- ok that is a monster
                if c_ptr.m_idx > 0 then
                        local mon = monster(c_ptr.m_idx)

                        -- must be pet
                        if mon.faction ~= FACTION_PLAYER then
                                message("This spell requires the life of a *willing* sacrifice.")
                                return
                        end

                        -- must be alive (not undead, demon, or nonliving)
                        if has_flag(mon,FLAG_DEMON) or has_flag(mon,FLAG_NONLIVING) or has_flag(mon,FLAG_UNDEAD) then
                                message("This spell requires the *life* of a willing sacrifice.")
                                return
                        end

                        -- note: no saving throw, this is a *willing* sacrifice!
                        local m_name = monster_desc(mon, 0)
                        message("You spill the living blood of "..m_name.." and are wreathed in power.")

                        local heal = (mon.hp * 100) / mon.maxhp
                        heal = ((30 + get_cast_level(50, 0)) * heal) / 100

                        -- What I'd really like to do here is incease mana,
                        -- perhaps beyond the normal max,
                        -- and have it gradually diminish if unused... maybe. Naw.
                        increase_mana(heal)

                        delete_monster_idx(c_ptr.m_idx)

                        read_the_necronomicon(2)
                        return true
                end
                read_the_necronomicon(2)
        end,
        info     = function()
                return ""
        end,
        desc     ={
                "A willing sacrifice of living blood calls forth power.",
                }
}

-- Genocide - just like the scroll
NECRO_GENOCIDE=add_spell
{
        name     = "Genocide ",
        school   = SCHOOL_NECRO,
        level    = 35,
        mana     = 3,
        mana_max = 3,
        fail     = 20,
	-- Unaffected by blindness
	["blind"] =     FALSE,
        spell    = function()
                genocide(true)
                read_the_necronomicon(3)
        end,
        info     = function()
                return ""
        end,
        desc     ={
                "Destroys all monsters of a particular type in the area",
                }
}


-- Mass Genocide - just like the scroll
NECRO_MASS_GENOCIDE=add_spell
{
        name     = "Mass Genocide ",
        school   = SCHOOL_NECRO,
        level    = 45,
        mana     = 4,
        mana_max = 4,
        fail     = 20,
	-- Unaffected by blindness
	["blind"] =     FALSE,
        spell    = function()
                mass_genocide(true)
                read_the_necronomicon(4)
        end,
        info     = function()
                return ""
        end,
        desc     ={
                "Destroys all monsters close to you.",
                }
}

ABSORB_SOUL = add_spell
{
	name =      "Absorb Soul",
	school =    SCHOOL_NECRO,
	level =     30,
	mana =      50,
	mana_max =  100,
	fail =      50,
	-- Unaffected by blindness
	["blind"] =     FALSE,
	spell =     function()
		return timed_effect(timed_effect.ABSORB_SOUL, rng(30) + 20 + get_cast_level(50), 40 + get_cast_level(60))
	end,
	info =      function()
		return "dur "..(20 + get_cast_level(50)).."+d30 chance "..(40 + get_cast_level(60)).."%"
	end,
	desc =      {
		"Gives a chance to absorb the soul of dying monsters and heal you with them.",
	}
}

hook(hook.MONSTER_DEATH, function(m_idx)
	if timed_effect.get(timed_effect.ABSORB_SOUL) and rng.percent(timed_effect.get(timed_effect.ABSORB_SOUL, "chance")) then
		local monst = monster(m_idx)
		if not monst.flags[FLAG_UNDEAD] and not monst.flags[FLAG_NONLIVING] then
			if player.undead_form.active then
				message(color.LIGHT_DARK, "You absorb the soul to restore a part of yours.")
				player.undead_form.number_souls = player.undead_form.number_souls - 1
				if player.undead_form.number_souls <= 0 then
					player.undead_form.active = false
					player.update = player.update | PU_BONUS | PU_HP
					player.redraw[FLAG_PR_HP] = true
					message(color.LIGHT_DARK, "You have absorbed enough souls to be reborn to life!")
				end
			else
				message(color.LIGHT_DARK, "You absorb the life of the dying soul.");
				hp_player(1 + (monst.level / 2) + get_level(ABSORB_SOUL, 25))
			end
		end
	end
end)


-- These are the *.info items. This function is where we create the egos, items, artifacts,
-- and whatever else. They have to be created inside this hook because the normal loading
-- code clears things out, which would wipe out our additions. Using this hook, we load our
-- stuff afterwards, and it sticks.


hook(hook.INFO_DATA_LOADED,function()
        --INFO is now loaded, we can use new_item_kind, and other things...
	getter.auto_share_flags(true)
        new_item_kind(
                {
                        tval = TV_SWORD
                        sval = SV_SWORD_TEETH
                        name = "& necromantic teeth~"
                        display = '|' color = color.LIGHT_DARK
                        desc =
                        {
                                "It's a bladed weapon.",
                                "This looks like some animal's teeth or at least you think",
                                "it comes from an animal..."
                        }
                        level = 0 weight = 7 cost = 0
                        allocation = { {0,1}, {5,1}, {10,1}, {20,1} }
                        flags = {
                                WEAPON = true
                                SPECIAL_GENE = true
                                VAMPIRIC = obvious(true)
                                DAM = getter.damages{PIERCE = {2,4} DARK_FIRE = {2,8}}
                                SKILL = getter.skill("Sword-mastery")
                        }
                }
        )

        -- The specters, very low hp, but fast and not very vulnerable to light.
        local specter=
        {
                define_as = "RACE_EGO_SPECTER1"
                race = "G"
                name = "specter" place = "after"
                level = mods.add(5) rarity = 25
                speed = mods.add(10) life = {mods.percent(12),mods.add(0)} ac = mods.add(10)
                aaf = mods.add(5) sleep = mods.add(0)
                exp = mods.percent(120)
                weight = mods.add(0)
		need_flags = { NEVER_RANDOM_EGO=true }
                added_flags =
                {
                        UNDEAD=true, NO_CUT=true, NO_SLEEP=true,
                        COLD_BLOOD=true, DROP_NUMBER=0, EVIL=true,
                        RESIST=getter.resists{FEAR=100, POIS=100, LITE=-50},
                        CAN_FLY=2, PASS_WALL=getter.flags{ PASS_INCORP=1 }

                }
                removed_flags =
                {
                        MORTAL=true, DROP_NUMBER=true, GOOD=true, SMART=true, HAS_EGG=true,
                        MULTIPLY=true, DROP_CORPSE=true, DROP_SKELETON=true,
                        UNIQUE=true,
                }
        }
        new_monster_ego(table_copy(specter))

        specter.define_as = "RACE_EGO_SPECTER2"
        specter.level = mods.add(20)
        specter.speed = mods.add(10)
        specter.life  = {mods.percent(25),mods.add(0)}
        specter.exp   = mods.percent(200)
        specter.added_flags.RESIST=getter.resists{FIRE=50, COLD=50, ELEC=50, ACID=50, FEAR=100, POIS=100, LITE=-25}
        new_monster_ego(table_copy(specter))

        -- The skeletons. Normal speed, vulnerable to fire and light, fair hp.
        local skeleton=
        {
                define_as = "RACE_EGO_SKELETON1"
                race = "s"
                name = "skeleton" place = "after"
                level = mods.add(5) rarity = 25
                speed = mods.add(0) life = {mods.percent(100),mods.add(0)} ac = mods.add(5)
                aaf = mods.add(5) sleep = mods.add(0)
                exp = mods.percent(120)
                weight = mods.add(0)
		need_flags = { NEVER_RANDOM_EGO=true }
                added_flags =
                {
                        UNDEAD=true, EMPTY_MIND=true, NO_CUT=true, NO_SLEEP=true,
                        COLD_BLOOD=true, DROP_NUMBER=0, EVIL=true, STUPID=true,
                        RESIST=getter.resists{FIRE=-50, FEAR=100, POIS=100, LITE=-300}
                }
                removed_flags =
                {
                        --FIXME: How do I remove - and add - *spells* ?
                        MORTAL=true, DROP_NUMBER=true, GOOD=true, SMART=true, HAS_EGG=true,
                        MULTIPLY=true, DROP_CORPSE=true,
                }
        }
        new_monster_ego(table_copy(skeleton))

        skeleton.define_as = "RACE_EGO_SKELETON2"
        skeleton.level = mods.add(20)
        skeleton.speed = mods.add(0)
        skeleton.life  = {mods.percent(150),mods.add(0)}
        skeleton.exp   = mods.percent(200)
        skeleton.added_flags.RESIST=getter.resists{COLD=100, FEAR=100, CONFUSION=50, POIS=100, LITE=-200}
        new_monster_ego(table_copy(skeleton))

        skeleton.define_as = "RACE_EGO_SKELETON3"
        skeleton.level = mods.add(40)
        skeleton.speed = mods.add(5)
        skeleton.life  = {mods.percent(200),mods.add(0)}
        skeleton.exp   = mods.percent(400)
        skeleton.added_flags.RESIST=getter.resists{COLD=100, FIRE=75, FEAR=100, CONFUSION=50, ELEC= 50, POIS=100, LITE=-100}
        new_monster_ego(table_copy(skeleton))

--      The zombies. Slow, stupid, very vulnerable to fire and light, but LOTS of hp!
        local zombie=
        {
                define_as = "RACE_EGO_ZOMBIE1"
                race = "z"
                name = "zombie" place = "after"
                level = mods.add(5) rarity = 25
                speed = mods.sub(10) life = {mods.percent(150),mods.add(0)} ac = mods.add(5)
                aaf = mods.add(5) sleep = mods.add(0)
                exp = mods.percent(120)
                weight = mods.add(0)
		need_flags = { NEVER_RANDOM_EGO=true }
                added_flags =
                {
                        UNDEAD=true, EMPTY_MIND=true, NO_CUT=true, NO_SLEEP=true,
                        COLD_BLOOD=true, DROP_NUMBER=0, EVIL=true, STUPID=true,
                        RESIST=getter.resists{FIRE=-300, FEAR=100, POIS=100, LITE=-300}
                }
                removed_flags =
                {
                        --FIXME: How do I remove *spells* ?
                        MORTAL=true, DROP_NUMBER=true, GOOD=true, SMART=true, HAS_EGG=true,
                        MULTIPLY=true,
                }
        }
        new_monster_ego(table_copy(zombie))

        zombie.define_as = "RACE_EGO_ZOMBIE2"
        zombie.level = mods.add(15)
        zombie.speed = mods.sub(5)
        zombie.life  = {mods.percent(200),mods.add(0)}
        zombie.exp   = mods.percent(170)
        zombie.added_flags.RESIST=getter.resists{FIRE=-200, COLD=50, ELEC=50, CONFUSION=100, FEAR=100, POIS=100, LITE=-300}
        new_monster_ego(table_copy(zombie))

        zombie.define_as = "RACE_EGO_ZOMBIE3"
        zombie.level = mods.add(20)
        zombie.speed = mods.add(2)
        zombie.life  = {mods.percent(250),mods.add(0)}
        zombie.exp   = mods.percent(200)
        zombie.added_flags.RESIST=getter.resists{FIRE=-150, COLD=100, ELEC=50, CONFUSION=100, FEAR=100, POIS=100, LITE=-200}
        new_monster_ego(table_copy(zombie))

        zombie.define_as = "RACE_EGO_ZOMBIE4"
        zombie.level = mods.add(25)
        zombie.speed = mods.add(3)
        zombie.life  = {mods.percent(300),mods.add(0)}
        zombie.exp   = mods.percent(300)
        zombie.added_flags.RESIST=getter.resists{FIRE=-100, COLD=100, ELEC=50, CONFUSION=100, FEAR=100, POIS=100, LITE=-200}
        new_monster_ego(table_copy(zombie))

        zombie.define_as = "RACE_EGO_ZOMBIE5"
        zombie.level = mods.add(40)
        zombie.speed = mods.add(5)
        zombie.life  = {mods.percent(400),mods.add(0)}
        zombie.exp   = mods.percent(400)
        zombie.added_flags.RESIST=getter.resists{FIRE=-50, COLD=100, ELEC=50, CONFUSION=100, FEAR=100, POIS=100, LITE=-100}
        new_monster_ego(table_copy(zombie))
	getter.auto_share_flags(false)
end)


------------------------- Undead Form --------------------------
add_loadsave("player.undead_form", {})
hook(hook.BIRTH, function() player.undead_form = {} end)
player.undead_form = {}
hook(hook.DIE_PRE, function()
	if has_ability(AB_UNDEAD_FORM) and not player.undead_form.active then
		player.undead_form.active = true
		player.undead_form.number_souls = player.lev + (rng(player.lev / 2) - (player.lev / 4))
		if player.undead_form.number_souls < 1 then player.undead_form.number_souls = 1 end
		message(color.LIGHT_DARK, format("You have to absorb the soul of %d monster%s to be brought back to life.", player.undead_form.number_souls, iif(player.undead_form.number_souls == 1, "", "s")));
		return true
	end
end)
