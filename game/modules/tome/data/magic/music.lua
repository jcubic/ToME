-- handle the music school
-- *ALL* lasting spell must return the mana cost in the lasting function

new_flag("MUSIC_LEVEL")

-- Flag desc
hook
{
	[hook.OBJECT_DESC] = function(obj, to_file, trim_down)
		if is_known(obj) then
			if has_flag(obj, FLAG_MUSIC_LEVEL) then
				local level = get_flag(obj, FLAG_MUSIC_LEVEL)
				term.text_out("It can uses musics up to level ")
				term.text_out(color.LIGHT_BLUE, level)
				term.text_out(".")
			end
		end
	end,
	[hook.ITEM_NAME_INSERT] = function(obj)
		if obj.tval == TV_INSTRUMENT  then
			if is_known(obj) then
				return true, "<"..get_flag(obj, FLAG_MUSIC_LEVEL)..">"
			else
				return true, "<?>"
			end
		end
	end,
}


declare_global_constants {
	"MUSIC_AMBARKANTA",
	"MUSIC_BLOW",
	"MUSIC_CONF",
	"MUSIC_HEAL",
	"MUSIC_HERO",
	"MUSIC_HOLD",
	"MUSIC_LITE",
	"MUSIC_MIND",
	"MUSIC_STOP",
	"MUSIC_STUN",
	"MUSIC_TIME",
	"MUSIC_WIND",
	"MUSIC_YLMIR",
}

MUSIC_STOP = add_spell
{
	["name"] =      "Stop singing(I)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     1,
	["mana"] =      0,
	["mana_max"] =  0,
	["fail"] =      -400,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 1 },
	["blind"] =     FALSE,
	["spell"] =     function()
			player.start_lasting_spell(0)
			return true
	end,
	["info"] =      function()
			return ""
	end,
	["desc"] =      {
			"Stops the current song, if any."
	}
}

--- Drums
MUSIC_HOLD = add_spell
{
	["name"] =      "Holding Pattern(I)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     1,
	["mana"] =      1,
	["mana_max"] =  10,
	["fail"] =      20,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 1 },
	["blind"] =     FALSE,
	["lasting"] =   function()
			project_los(dam.OLD_SLOW, 10 + get_cast_level(100))
			return get_mana(MUSIC_HOLD)
	end,
	["spell"] =     function()
			player.start_lasting_spell(MUSIC_HOLD)
			return true
	end,
	["info"] =      function()
			return "power "..(10 + get_cast_level(100))
	end,
	["desc"] =      {
			"Slows down all monsters listening the song.",
			"Consumes the amount of mana each turn.",
	}
}

MUSIC_CONF = add_spell
{
	["name"] =      "Illusion Pattern(II)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     5,
	["mana"] =      2,
	["mana_max"] =  15,
	["fail"] =      30,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 2 },
	["blind"] =     FALSE,
	["lasting"] =   function()
			project_los(dam.OLD_CONF, 10 + get_cast_level(100))
			return get_mana(MUSIC_CONF)
	end,
	["spell"] =     function()
			player.start_lasting_spell(MUSIC_CONF)
			return true
	end,
	["info"] =      function()
			return "power "..(10 + get_cast_level(100))
	end,
	["desc"] =      {
			"Tries to confuse all monsters listening the song.",
			"Consumes the amount of mana each turn.",
	}
}

MUSIC_STUN = add_spell
{
	["name"] =      "Stun Pattern(IV)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     19,
	["mana"] =      3,
	["mana_max"] =  25,
	["fail"] =      45,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 4 },
	["blind"] =     FALSE,
	["lasting"] =   function()
			project_los(dam.STUN, 10 + get_cast_level(90))
			return get_mana(MUSIC_STUN)
	end,
	["spell"] =     function()
			player.start_lasting_spell(MUSIC_STUN)
			return true
	end,
	["info"] =      function()
			return "power "..(10 + get_cast_level(90))
	end,
	["desc"] =      {
			"Stuns all monsters listening the song.",
			"Consumes the amount of mana each turn.",
	}
}

--- Harps
MUSIC_LITE = add_spell
{
	["name"] =      "Song of the Sun(I)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     1,
	["mana"] =      1,
	["mana_max"] =  1,
	["fail"] =      20,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["blind"] =     FALSE,
	["flag"] =      { FLAG_MUSIC_LEVEL, 1 },
	["lasting"] =   function()
			set_lite(5)
			return 1
	end,
	["spell"] =     function()
			player.start_lasting_spell(MUSIC_LITE)
			return true
	end,
	["info"] =      function()
			return ""
	end,
	["desc"] =      {
			"Provides light as long as you sing.",
			"Consumes the amount of mana each turn.",
	}
}

MUSIC_HEAL = add_spell
{
	["name"] =      "Flow of Life(II)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     13,
	["mana"] =      5,
	["mana_max"] =  30,
	["fail"] =      35,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 2 },
	["lasting"] =   function()
			hp_player(7 + get_cast_level(100))
			return get_mana(MUSIC_HEAL)
	end,
	["spell"] =     function()
			player.start_lasting_spell(MUSIC_HEAL)
			return true
	end,
	["info"] =      function()
			return "heal "..(7 + get_cast_level(100)).."/turn"
	end,
	["desc"] =      {
			"Heals you as long as you sing.",
			"Consumes the amount of mana each turn.",
	}
}

MUSIC_HERO = add_spell
{
	["name"] =      "Heroic Ballad(II)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     19,
	["mana"] =      4,
	["mana_max"] =  14,
	["fail"] =      45,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 2 },
	["lasting"] =   function()
			set_hero(5)
			if get_cast_level() >= 20 then
				set_shero(5)
			end
			if get_cast_level() >= 40 then
				set_strike(5)
			end
			if get_cast_level() >= 50 then
				timed_effect(timed_effect.RES_DARKFIRE, 5, 40)
			end
			return get_mana(MUSIC_HERO)
	end,
	["spell"] =     function()
			player.start_lasting_spell(MUSIC_HERO)
			return true
	end,
	["info"] =      function()
			return ""
	end,
	["desc"] =      {
			"Increases melee accuracy",
			"At level 20 it increases it even more and reduces armour a bit",
			"At level 40 it increases it again",
			"At level 50 it grants protection against dark fire",
			"Consumes the amount of mana each turn.",
	}
}

MUSIC_TIME = add_spell
{
	["name"] =      "Hobbit Melodies(III)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     39,
	["mana"] =      10,
	["mana_max"] =  30,
	["fail"] =      70,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 3 },
	["lasting"] =   function()
			set_shield(5, 10 + get_cast_level(50), 0, 0, 0)
			if get_cast_level() >= 30 then
				set_fast(5, 7 + get_cast_level(10))
			end
			return get_mana(MUSIC_TIME)
	end,
	["spell"] =     function()
			player.start_lasting_spell(MUSIC_TIME)
			return true
	end,
	["info"] =      function()
			if get_cast_level() >= 30 then
				return "AC "..(10 + get_cast_level(50)).." speed "..(7 + get_cast_level(10))
			else
				return "AC "..(10 + get_cast_level(50))
			end
	end,
	["desc"] =      {
			"Greatly increases your reflexes allowing you to block more melee blows.",
			"At level 30 it also makes you faster.",
			"Consumes the amount of mana each turn.",
	}
}

MUSIC_MIND = add_spell
{
	["name"] =      "Clairaudience(IV)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     49,
	["mana"] =      15,
	["mana_max"] =  30,
	["fail"] =      75,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 4 },
	["lasting"] =   function()
			set_tim_esp(5)
			if get_cast_level() >= 20 then
				fire_ball(dam.IDENTIFY, 0, 1, 1 + get_cast_level(3, 0))
			end
			return get_mana(MUSIC_MIND)
	end,
	["spell"] =     function()
			player.start_lasting_spell(MUSIC_MIND)
			return true
	end,
	["info"] =      function()
			if get_cast_level() >= 20 then
				return "rad "..(1 + get_cast_level(3, 0))
			else
				return ""
			end
	end,
	["desc"] =      {
			"Allows you to sense monster minds as long as you sing.",
			"At level 20 it identifies all objects in a radius on the floor,",
			"as well as probing monsters in that radius.",
			"Consumes the amount of mana each turn.",
	}
}

--- Horns

MUSIC_BLOW = add_spell
{
	["name"] =      "Blow(I)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     7,
	["mana"] =      3,
	["mana_max"] =  30,
	["fail"] =      20,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 1 },
	["spell"] =     function()
			fire_ball(dam.SOUND,
				  0,
				  rng.roll(2 + get_cast_level(10, 0), 4 + get_cast_level(40, 0)),
				  1 + get_cast_level(12, 0)
				 )
			return true
	end,
	["info"] =      function()
			return "dam "..rng.desc_roll(2 + get_cast_level(10, 0), 4 + get_cast_level(40, 0)).." rad "..(1 + get_cast_level(12, 0))
	end,
	["desc"] =      {
			"Produces a powerful, blowing, sound all around you.",
	}
}

MUSIC_WIND = add_spell
{
	["name"] =      "Gush of Wind(II)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     27,
	["mana"] =      15,
	["mana_max"] =  45,
	["fail"] =      30,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 2 },
	["spell"] =     function()
			fire_ball(dam.AWAY_ALL,
				  0,
				  10 + get_cast_level(40, 0),
				  1 + get_cast_level(12, 0)
				 )
			return true
	end,
	["info"] =      function()
			return "dist "..(10 + get_cast_level(40, 0)).." rad "..(1 + get_cast_level(12, 0))
	end,
	["desc"] =      {
			"Produces a outgoing gush of wind that sends monsters away.",
	}
}

MUSIC_YLMIR = add_spell
{
	["name"] =      "Horns of Ylmir(III)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     20,
	["mana"] =      25,
	["mana_max"] =  30,
	["fail"] =      20,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 3 },
	["spell"] =     function()
			earthquake(player.py, player.px, 2 + get_cast_level(10))
			return true
	end,
	["info"] =      function()
			return "rad "..(2 + get_cast_level(10))
	end,
	["desc"] =      {
			"Produces an earth shaking sound.",
	}
}

MUSIC_AMBARKANTA = add_spell
{
	["name"] =      "Ambarkanta(IV)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     49,
	["mana"] =      70,
	["mana_max"] =  70,
	["fail"] =      60,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 4 },
	["spell"] =     function()
			alter_reality()
			return true
	end,
	["info"] =      function()
			return ""
	end,
	["desc"] =      {
			"Produces a reality shaking sound that transports you to a nearly",
			"identical reality.",
	}
}


--[[
MUSIC_ = add_spell
{
	["name"] =      "(I)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     1,
	["mana"] =      0,
	["mana_max"] =  0,
	["fail"] =      20,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["flag"] =      { FLAG_MUSIC_LEVEL, 1 },
	["lasting"] =   function()
			return get_mana(MUSIC_)
	end,
	["spell"] =     function()
			player.start_lasting_spell(MUSIC_)
			return true
	end,
	["info"] =      function()
			return ""
	end,
	["desc"] =      {
			"",
			"Consumes the amount of mana each turn.",
	}
}

or

MUSIC_ = add_spell
{
	["name"] =      "(I)",
	["school"] =    {SCHOOL_MUSIC},
	["level"] =     1,
	["mana"] =      0,
	["mana_max"] =  0,
	["fail"] =      20,
	["stat"] =      A_CHR,
	["random"] =    "music",
	["pval"] =      1,
	["spell"] =     function()

			return true
	end,
	["info"] =      function()
			return ""
	end,
	["desc"] =      {
			"",
	}
}
]]
