-- Default bodies
constant("default_body", {})

default_body.none = { INVEN=23 MAINHAND=0 BODY=0 OUTER=0 LITE=0 AMMO=0 OFFHAND=0 HANDS=0 TOOL=0 RING=0 HEAD=0 NECK=0 FEET=0 BACKPACK=0 }
default_body.humanoid = { INVEN=23 MAINHAND=1 BODY=1 OUTER=1 LITE=1 OFFHAND=1 HANDS=1 TOOL=1 RING=2 HEAD=1 NECK=1 FEET=1 BACKPACK=1 }
default_body.blob = { INVEN=40 MAINHAND=0 BODY=2 OUTER=2 LITE=2 AMMO=0 OFFHAND=0 HANDS=0 TOOL=0 RING=6 HEAD=0 NECK=3 FEET=0 BACKPACK=1 }
default_body.worms = { INVEN=35 MAINHAND=1 BODY=1 OUTER=1 LITE=1 AMMO=0 OFFHAND=0 HANDS=0 TOOL=1 RING=3 HEAD=0 NECK=2 FEET=0 BACKPACK=1 }
default_body.insect = { INVEN=15 MAINHAND=0 BODY=1 OUTER=1 LITE=1 OFFHAND=0 HANDS=0 TOOL=0 RING=2 HEAD=1 NECK=1 FEET=0 BACKPACK=1 }
default_body.bird = { INVEN=20 MAINHAND=0 BODY=1 OUTER=1 LITE=1 OFFHAND=1 HANDS=1 TOOL=1 RING=0 HEAD=1 NECK=1 FEET=0 BACKPACK=1 }
default_body.quadruped = { INVEN=23 MAINHAND=0 BODY=1 OUTER=1 LITE=1 OFFHAND=0 HANDS=0 TOOL=0 RING=2 HEAD=1 NECK=1 FEET=0 BACKPACK=1 }
default_body.xorn = { INVEN=23 MAINHAND=0 BODY=1 OUTER=1 LITE=1 OFFHAND=0 HANDS=0 TOOL=0 RING=2 HEAD=1 NECK=1 FEET=0 BACKPACK=1 }
default_body.dragon = { INVEN=23 MAINHAND=0 BODY=1 OUTER=1 LITE=1 OFFHAND=0 HANDS=0 TOOL=0 RING=6 HEAD=1 NECK=1 FEET=0 BACKPACK=1 }
default_body.spider = { INVEN=23 MAINHAND=0 BODY=1 OUTER=1 LITE=1 OFFHAND=0 HANDS=0 TOOL=0 RING=2 HEAD=1 NECK=1 FEET=0 BACKPACK=1 }

-- Need a Player entry to be first
new_monster_race
{
	define_as = "RACE_PLAYER"	
	name = "player"
	race = '@'
	sval = 0
	color = color.WHITE
	speed = 0 life = { 1, 1 } ac = 0
	aaf = 0 sleep = 0
	level = 0 exp = 0
	rarity = 0
	weight = 100
	body = { INVEN=23 MAINHAND=1 BODY=1 OUTER=1 LITE=1 OFFHAND=1 HANDS=1 TOOL=1 RING=2 HEAD=1 NECK=1 FEET=1 MOUNT=1 BACKPACK=1 }
	flags = {
		UNIQUE=true
		DROP_CORPSE=true DROP_SKELETON=true
		OPEN_DOOR=true
		BASH_DOOR=true
		NEVER_GENE=true
	}
}

new_monster_race
{
	define_as = "RACE_DISEMBODIED"
	name = "disembodied player"
	race = 'G'
	sval = 1
	color = color.GREEN
	speed = 0 life = { 1, 1 } ac = 0
	aaf = 0 sleep = 0
	level = 0 exp = 0
	rarity = 0
	weight = 0
	body = { INVEN=23 MAINHAND=0 BODY=0 OUTER=0 LITE=0 OFFHAND=0 HANDS=0 TOOL=0 RING=0 HEAD=0 NECK=0 FEET=0 MOUNT=0 BACKPACK=0 }
	flags = {
		UNIQUE=true
		NEVER_GENE=true

		-- No body for symbiosis
		NO_SYMBIOSIS="You cannot @verb@ symbiosis without a body."

		-- Floats just over ground
		CAN_FLY=2

		-- Is a ghost
		COLD_BLOOD=true NO_CUT=true
		UNDEAD=true NONLIVING=true
		PASS_WALL=getter.flags{PASS_INCORP=10000}
		RESIST=getter.resists{COLD=100 POIS=100 CUT=100}
		METAB_PCT=0 -- No hunger

		-- Can't do much of anything
		NO_TAKE_ITEM=true

		EAT_PRE=
			function()
				message("You cannot eat while disembodied.")
				return true
			end

		PRE_QUAFF=
			function()
				message("You cannot drink while disembodied.")
				return true
			end

		PRE_READ=
			function()
				message("You cannot read scrolls while disembodied.")
				return true
			end

		SELF_KNOWLEDGE=
			function(list)
				tinsert(list, "You are disembodied.")
				tinsert(list, "You cannot eat.")
				tinsert(list, "You cannot drink.")
				tinsert(list, "You cannot read scrolls.")
			end
	}
}


tome_dofile_anywhere(TENGINE_DIR_DATA, "monsters/townspeople.lua")
tome_dofile_anywhere(TENGINE_DIR_DATA, "monsters/special.lua")
tome_dofile_anywhere(TENGINE_DIR_DATA, "monsters/global.lua")
tome_dofile_anywhere(TENGINE_DIR_DATA, "monsters/quests.lua")
tome_dofile_anywhere(TENGINE_DIR_DATA, "monsters/mounts.lua")
tome_dofile_anywhere(TENGINE_DIR_DATA, "monsters/barrow-downs.lua")
tome_dofile_anywhere(TENGINE_DIR_DATA, "monsters/old-forest.lua")
tome_dofile_anywhere(TENGINE_DIR_DATA, "monsters/mirkwood.lua")
