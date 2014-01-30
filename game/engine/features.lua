-- Do this, instead of using constant("define_features", {}), because this
-- file is only run when generating the features.raw file.  If we
-- used contant(), then sometimes the rest of the Lua code would find
-- features to already be defined, but other times it wouldn't.
globals()["define_features"] = {}

--
-- Define some default features that are required by the default level
-- generator.  This file could be overriden by a module to not contain
-- any defaults if the default generator is not used, but it really
-- should not cause any trouble to have them defined, so why not ?
--

new_feature_type
{
	__index__ = 0
	define_as = "FEAT_NONE"
	name = "nothing"
	display = ' ' color = color.DARK
}
new_feature_type
{
	define_as = "FEAT_FLOOR"
	name = "open floor"
	display = '.' color = color.WHITE
	priority = 5
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
	}
}
new_feature_type
{
	define_as = "FEAT_LESS"
	name = "up staircase"
	on_walk = "There is an up staircase here."
	display = '<' color = color.WHITE
	priority = 25
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		FLOOR=true PERMANENT=true NOTICE=true REMEMBER=true
		CAN_RUN=true LEVEL_CHANGER=-1
	}
}
new_feature_type
{
	define_as = "FEAT_MORE"
	name = "down staircase"
	on_walk = "There is a down staircase here."
	display = '>' color = color.WHITE
	priority = 25
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		FLOOR=true PERMANENT=true NOTICE=true REMEMBER=true
		CAN_RUN=true LEVEL_CHANGER=1
	}
}
new_feature_type
{
	define_as = "FEAT_WAY_MORE"
	name = "path to the next area"
	on_walk = "There is a path leading to the next area here."
	display = '>' color = color.WHITE
	priority = 25
	flags =
	{
		FLOOR=true PERMANENT=true NOTICE=true REMEMBER=true CAN_RUN=true
		LEVEL_CHANGER=1
	}
}
new_feature_type
{
	define_as = "FEAT_WAY_LESS"
	name = "path to the previous area"
	on_walk = "There is a path leading to the previous area here."
	display = '<' color = color.WHITE
	priority = 25
	flags =
	{
		FLOOR=true PERMANENT=true NOTICE=true REMEMBER=true CAN_RUN=true
		LEVEL_CHANGER=-1
	}
}
new_feature_type
{
	define_as = "FEAT_SHAFT_DOWN"
	name = "shaft down"
	on_walk = "There is a shaft down here."
	display = '>' color = color.LIGHT_UMBER
	priority = 25
	flags =
	{
		FLOOR=true PERMANENT=true REMEMBER=true NOTICE=true CAN_RUN=true
		LEVEL_CHANGER=getter.random(2, 4)
	}
}
new_feature_type
{
	define_as = "FEAT_SHAFT_UP"
	name = "shaft up"
	on_walk = "There is a shaft up here."
	display = '<' color = color.LIGHT_UMBER
	priority = 25
	flags =
	{
		FLOOR=true PERMANENT=true REMEMBER=true NOTICE=true CAN_RUN=true
		LEVEL_CHANGER=getter.random(-4, -2)
	}
}

new_open_pair_feature_type
{
	open = {
		define_as = "FEAT_TRAP_DOOR_OPEN"
		name = "open trap door"
		on_walk="An open trap door is underneath you."
		display = ">" color = color.WHITE
		flags =
		{
			SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
			FLOOR=true PERMANENT=true NOTICE=true REMEMBER=true
			CAN_RUN=true LEVEL_CHANGER=1
		}
	},
	close = {
		define_as = "FEAT_TRAP_DOOR"
		name = "trap door"
		on_walk = "A closed trap door is underneath you."
		display = '>' color = color.LIGHT_DARK
		flags =
		{
			SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
			FLOOR=true PERMANENT=true NOTICE=true REMEMBER=true
			CAN_RUN=true
			STAIR_DO=
				function(updown, ask_leave)
					message("You must open the trap door before going " ..
							"through it.")
					return 0
				end
		}
	}
}

new_feature_type
{
	define_as = "FEAT_BROKEN"
	name = "broken door"
	display = "'" color = color.LIGHT_UMBER
	priority = 15
	flags =
	{
		FLOOR=true NOTICE=true REMEMBER=true CAN_RUN=true DOOR=true
		OPEN="The door appears to be broken."
	}
}

define_features.door_pass = {
	PASS_INCORP = 1,
	PASS_LIQUID = 1,
	PASS_GASEOUS = 1,
	PASS_DOOR = 1
}

function define_features.auto_open_door(y, x, dir)
	if game_options.easy_open then
		do_cmd_open_aux(y, x, dir)
		return true
	end
end

new_open_pair_feature_type
{
	open = {
		define_as = "FEAT_OPEN"
		name = "open door"
		display = "'" color = color.LIGHT_UMBER
		priority = 15
		flags =
		{
			FLOOR=true NOTICE=true REMEMBER=true CAN_RUN=true DOOR=true
		}
	},
	close = {
		define_as = "FEAT_DOOR"
		name = "door"
		on_block = "a closed door blocking your way"
		display = '+' color = color.LIGHT_UMBER
		priority = 17
		flags =
		{
			WALL=true NO_WALK=true NO_VISION=true NOTICE=true
			REMEMBER=true DOOR=true OPEN_DOOR=true BASH_DOOR=50
			DESTROY_INTO=FEAT_BROKEN SUBST_DOOR=true
			CAN_PASS=getter.flags(define_features.door_pass)
			MOVE_PRE_DEST = define_features.auto_open_door
		}
	}
}
new_feature_type
{
	define_as = "FEAT_DOOR_LOCKED_HEAD"
	name = "locked door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		REMEMBER=true DOOR=true LOCKED=4 CLOSED=FEAT_OPEN
		DESTROY_INTO=FEAT_BROKEN SUBST_DOOR=true
		OPEN_DOOR=true BASH_DOOR=50
		CAN_PASS=getter.flags(define_features.door_pass)
		MOVE_PRE_DEST = define_features.auto_open_door
	}
}
new_feature_type
{
	name = "locked door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		REMEMBER=true DOOR=true LOCKED=8 CLOSED=FEAT_OPEN
		DESTROY_INTO=FEAT_BROKEN SUBST_DOOR=true
		OPEN_DOOR=true BASH_DOOR=50
		CAN_PASS=getter.flags(define_features.door_pass)
		MOVE_PRE_DEST = define_features.auto_open_door
	}
}
new_feature_type
{
	name = "locked door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		REMEMBER=true DOOR=true LOCKED=12 CLOSED=FEAT_OPEN
		DESTROY_INTO=FEAT_BROKEN SUBST_DOOR=true
		OPEN_DOOR=true BASH_DOOR=50
		CAN_PASS=getter.flags(define_features.door_pass)
		MOVE_PRE_DEST = define_features.auto_open_door
	}
}
new_feature_type
{
	name = "locked door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		REMEMBER=true DOOR=true LOCKED=16 CLOSED=FEAT_OPEN
		DESTROY_INTO=FEAT_BROKEN SUBST_DOOR=true
		OPEN_DOOR=true BASH_DOOR=50
		CAN_PASS=getter.flags(define_features.door_pass)
		MOVE_PRE_DEST = define_features.auto_open_door
	}
}
new_feature_type
{
	name = "locked door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		REMEMBER=true DOOR=true LOCKED=20 CLOSED=FEAT_OPEN
		DESTROY_INTO=FEAT_BROKEN SUBST_DOOR=true
		OPEN_DOOR=true BASH_DOOR=50
		CAN_PASS=getter.flags(define_features.door_pass)
		MOVE_PRE_DEST = define_features.auto_open_door
	}
}
new_feature_type
{
	name = "locked door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		REMEMBER=true DOOR=true LOCKED=24 CLOSED=FEAT_OPEN
		DESTROY_INTO=FEAT_BROKEN SUBST_DOOR=true
		OPEN_DOOR=true BASH_DOOR=50
		CAN_PASS=getter.flags(define_features.door_pass)
		MOVE_PRE_DEST = define_features.auto_open_door
	}
}
new_feature_type
{
	define_as = "FEAT_DOOR_LOCKED_TAIL"
	name = "locked door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		REMEMBER=true DOOR=true LOCKED=28 CLOSED=FEAT_OPEN
		DESTROY_INTO=FEAT_BROKEN SUBST_DOOR=true
		OPEN_DOOR=true BASH_DOOR=50
		CAN_PASS=getter.flags(define_features.door_pass)
		MOVE_PRE_DEST = define_features.auto_open_door
	}
}
new_feature_type
{
	define_as = "FEAT_DOOR_JAMMED_HEAD"
	name = "jammed door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true REMEMBER=true
		NOTICE=true JAMMED=true CLOSED=FEAT_OPEN BASH_DOOR=50
		DESTROY_INTO=FEAT_BROKEN DOOR=true SUBST_DOOR=true
		CAN_PASS=getter.flags(define_features.door_pass)
	}
}
new_feature_type
{
	name = "jammed door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true REMEMBER=true
		NOTICE=true JAMMED=true CLOSED=FEAT_OPEN BASH_DOOR=50
		DESTROY_INTO=FEAT_BROKEN DOOR=true SUBST_DOOR=true
		CAN_PASS=getter.flags(define_features.door_pass)
	}
}
new_feature_type
{
	name = "jammed door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true REMEMBER=true
		NOTICE=true JAMMED=true CLOSED=FEAT_OPEN BASH_DOOR=50
		DESTROY_INTO=FEAT_BROKEN DOOR=true SUBST_DOOR=true
		CAN_PASS=getter.flags(define_features.door_pass)
	}
}
new_feature_type
{
	name = "jammed door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true REMEMBER=true
		NOTICE=true JAMMED=true CLOSED=FEAT_OPEN BASH_DOOR=50
		DESTROY_INTO=FEAT_BROKEN DOOR=true SUBST_DOOR=true
		CAN_PASS=getter.flags(define_features.door_pass)
	}
}
new_feature_type
{
	name = "jammed door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true REMEMBER=true
		NOTICE=true JAMMED=true CLOSED=FEAT_OPEN BASH_DOOR=50
		DESTROY_INTO=FEAT_BROKEN DOOR=true SUBST_DOOR=true
		CAN_PASS=getter.flags(define_features.door_pass)
	}
}
new_feature_type
{
	name = "jammed door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true REMEMBER=true
		NOTICE=true JAMMED=true CLOSED=FEAT_OPEN BASH_DOOR=50
		DESTROY_INTO=FEAT_BROKEN DOOR=true SUBST_DOOR=true
		CAN_PASS=getter.flags(define_features.door_pass)
	}
}
new_feature_type
{
	name = "jammed door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true REMEMBER=true
		NOTICE=true JAMMED=true CLOSED=FEAT_OPEN BASH_DOOR=50
		DESTROY_INTO=FEAT_BROKEN DOOR=true SUBST_DOOR=true
		CAN_PASS=getter.flags(define_features.door_pass)
	}
}
new_feature_type
{
	define_as = "FEAT_DOOR_JAMMED_TAIL"
	name = "jammed door"
	on_block = "a closed door blocking your way"
	display = '+' color = color.LIGHT_UMBER
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true REMEMBER=true
		NOTICE=true JAMMED=true CLOSED=FEAT_OPEN BASH_DOOR=50
		DESTROY_INTO=FEAT_BROKEN DOOR=true SUBST_DOOR=true
		CAN_PASS=getter.flags(define_features.door_pass)
	}
}
new_feature_type
{
	define_as = "FEAT_SECRET"
	name = "secret door"
	display = '#' color = color.WHITE
	priority = 10
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true DOOR=true
		DESTROY_INTO=FEAT_BROKEN SECRET=FEAT_DOOR SUBST_DOOR=true
		DONT_NOTICE_RUNNING=true CLOSED=FEAT_OPEN
		OPEN_DOOR=true BASH_DOOR=50
		CAN_PASS=getter.flags(define_features.door_pass)
	}
}

new_feature_type
{
	define_as = "FEAT_RUBBLE"
	name = "pile of rubble"
	on_block = "some rubble blocking your way"
	display = ':' color = color.WHITE
	priority = 13
	flags =	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		SUBST_ROCK=true SUBST_RUBBLE=true
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		CAN_FLY=2
		CAN_PASS=getter.flags{
			PASS_INCORP=1
			PASS_STONE=1
			PASS_LIQUID=1
			PASS_GASEOUS=1
			PASS_CLIMB=1
		}
	}
}

define_features.pass_granite_wall = {
	PASS_INCORP=1
	PASS_STONE=8
}

new_feature_type
{
	define_as = "FEAT_WALL_EXTRA"
	name = "granite wall"
	display = '#' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		WALL=true NO_WALK=true NO_VISION=true
		SUBST_ROCK=true SUBST_GRANITE=true
		DONT_NOTICE_RUNNING=true
		CAN_PASS=getter.flags(define_features.pass_granite_wall)
	}
}
new_feature_type
{
	define_as = "FEAT_WALL_INNER"
	name = "granite wall"
	display = '#' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		WALL=true NO_WALK=true NO_VISION=true 
		SUBST_ROCK=true SUBST_GRANITE=true
		DONT_NOTICE_RUNNING=true
		CAN_PASS=getter.flags(define_features.pass_granite_wall)
	}
}
new_feature_type
{
	define_as = "FEAT_WALL_OUTER"
	name = "granite wall"
	display = '#' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		WALL=true NO_WALK=true NO_VISION=true 
		SUBST_ROCK=true SUBST_GRANITE=true
		DONT_NOTICE_RUNNING=true
		CAN_PASS=getter.flags(define_features.pass_granite_wall)
	}
}
new_feature_type
{
	define_as = "FEAT_WALL_SOLID"
	name = "granite wall"
	display = '#' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		WALL=true NO_WALK=true NO_VISION=true 
		SUBST_ROCK=true SUBST_GRANITE=true
		DONT_NOTICE_RUNNING=true
		CAN_PASS=getter.flags(define_features.pass_granite_wall)
	}
}
new_feature_type
{
	define_as = "FEAT_PERM_EXTRA"
	name = "permanent wall"
	display = '#' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true 
		DONT_NOTICE_RUNNING=true SUBST_ROCK=true SUBST_GRANITE=true
	}
}
new_feature_type
{
	define_as = "FEAT_PERM_INNER"
	name = "permanent wall"
	display = '#' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true 
		DONT_NOTICE_RUNNING=true SUBST_ROCK=true SUBST_GRANITE=true
	}
}
new_feature_type
{
	define_as = "FEAT_PERM_OUTER"
	name = "permanent wall"
	display = '#' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true 
		DONT_NOTICE_RUNNING=true SUBST_ROCK=true SUBST_GRANITE=true
	}
}
new_feature_type
{
	define_as = "FEAT_PERM_SOLID"
	name = "permanent wall"
	display = '#' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true 
		DONT_NOTICE_RUNNING=true SUBST_ROCK=true SUBST_GRANITE=true
	}
}
new_feature_type
{
	define_as = "FEAT_WORLD_BORDER"
	name = "the end of the world"
	display = '*' color = color.DARK
	shimmers = { color.DARK, color.DARK, color.DARK, color.DARK, color.DARK, color.DARK, color.LIGHT_DARK, }
	flags =
	{
		ATTR_MULTI=true WALL=true NO_WALK=true NO_VISION=true PERMANENT=true
		DONT_NOTICE_RUNNING=true SUBST_ROCK=true SUBST_GRANITE=true
	}
}
new_feature_type
{
	define_as = "FEAT_SHOP"
	name = "Building"
	display = '1' color = color.LIGHT_UMBER
	flags =
	{
		FLOOR=true PERMANENT=true REMEMBER=true NOTICE=true CAN_RUN=true
		CONTAINS_BUILDING=true SUBST_ROCK=true SUBST_GRANITE=true
	}
}
