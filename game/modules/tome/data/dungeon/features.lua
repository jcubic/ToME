-- Floor/walls/... features definitions

new_feature_type
{
	define_as = "FEAT_MAP_BORDER"
	name = "Map Edge"
	display = ' ' color = color.DARK
	can_enter = function(mode)
		return false
	end
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true
		SUPPORT_LIGHT=true DONT_NOTICE_RUNNING=true
	}
}

new_feature_type
{
	define_as = "FEAT_FOUNTAIN"
	name = "fountain"
	on_walk = "The liquid here seems magical."
	display = '_' color = color.WHITE
	priority = 22
	flags =
	{
		FLOOR=true NOTICE=true REMEMBER=true CAN_RUN=true
		ON_MAKE =
			function(y, x, feat)
				local k_idx =
					rand_obj.rand_k_idx(object_level, 1,
										{features.fountain_potion_tester})

				if not k_idx then
					cave_set_feat(y, x, FEAT_EMPTY_FOUNTAIN)
					wiz_print("No suitable potions for fountain")
					return
				end

				local obj = new_object()
				object_prep(obj, k_idx)

				obj.number = rng.roll(3, 4)

				cave(y, x).flags[FLAG_FOUNTAIN] = obj
			end

		FEAT_DESC =
			function(y, x, feat)
				local c_ptr = cave(y, x)
				if not known_flag(c_ptr, FLAG_FOUNTAIN) then
					return
				end

				local obj = c_ptr.flags[FLAG_FOUNTAIN]

				if tag(obj) ~= TAG_OBJECT or obj.tval ~= TV_POTION then
					return "Terrain: fountain of something strange"
				end

				if is_aware(obj) then
					return "Terrain: fountain of " ..
						k_info(obj.k_idx).name
				else
					return "Terrain: fountain of " ..
						flavour.get_flavour(obj.k_idx) .. " liquid"
				end
			end
	}
}
new_feature_type
{
	define_as = "FEAT_EMPTY_FOUNTAIN"
	name = "empty fountain"
	on_walk = "The fountain seems empty."
	display = '_' color = color.LIGHT_DARK
	priority = 22
	flags =
	{
		FLOOR=true NOTICE=true REMEMBER=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_QUEST_ENTER"
	name = "quest entrance"
	display = '>' color = color.YELLOW
	flags =
	{
		FLOOR=true PERMANENT=true REMEMBER=true NOTICE=true CAN_RUN=true
		QUEST_CHANGER=true NO_TUNNEL_MSG="You cannot tunnel a quest entrance."
	}
}
new_feature_type
{
	define_as = "FEAT_QUEST_EXIT"
	name = "quest exit"
	display = '<' color = color.YELLOW
	flags =
	{
		FLOOR=true PERMANENT=true REMEMBER=true NOTICE=true CAN_RUN=true
		QUEST_CHANGER=-1 NO_TUNNEL_MSG="You cannot tunnel a quest exit."
	}
}
new_feature_type
{
	define_as = "FEAT_QUEST_DOWN"
	name = "quest down level"
	display = '>' color = color.RED
	flags =
	{
		FLOOR=true PERMANENT=true REMEMBER=true NOTICE=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_QUEST_UP"
	name = "quest up level"
	display = '<' color = color.RED
	flags =
	{
		FLOOR=true PERMANENT=true REMEMBER=true NOTICE=true CAN_RUN=true
	}
}
new_feature_type
{
	name = "town exit"
	display = '>' color = color.GREEN
	flags =
	{
		FLOOR=true PERMANENT=true REMEMBER=true NOTICE=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_WEB"
	name = "web"
	on_block = "a web blocking your way"
	display = '+' color = color.YELLOW
	flags =
	{
		WALL=true NOTICE=true TUNNELABLE={0,160} SUBST_WEB=true
		CAN_PASS=getter.flags{PASS_WEB=true, PASS_INCORP=1}
	}
}
new_feature_type
{
	define_as = "FEAT_TRAP"
	name = "trap"
	display = '^' color = color.WHITE
	flags =
	{
		FLOOR=true NOTICE=true REMEMBER=true
	}
}

features.magma_pass = {
	PASS_INCORP=1
	PASS_STONE=2
}

features.quartz_pass = {
	PASS_INCORP=1
	PASS_STONE=4
}

new_feature_type
{
	define_as = "FEAT_MAGMA"
	name = "magma vein"
	display = '%' color = color.SLATE
	priority = 12
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		SUBST_ROCK=true SUBST_MAGMA=true
		CAN_PASS=getter.flags(features.magma_pass)
		DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
	}
}
new_feature_type
{
	define_as = "FEAT_QUARTZ"
	name = "quartz vein"
	display = '%' color = color.WHITE
	priority = 11
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		CAN_PASS=getter.flags(features.quartz_pass)
		SUBST_QUARTZ=true SUBST_ROCK=true 
		DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
	}
}
new_feature_type
{
	define_as = "FEAT_MAGMA_H"
	name = "magma vein"
	display = '%' color = color.SLATE
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		CAN_PASS=getter.flags(features.magma_pass)
		SUBST_MAGMA=true SUBST_ROCK=true
		DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
	}
}
new_feature_type
{
	define_as = "FEAT_QUARTZ_H"
	name = "quartz vein"
	display = '%' color = color.WHITE
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		CAN_PASS=getter.flags(features.quartz_pass)
		SUBST_QUARTZ=true SUBST_ROCK=true
		DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
	}
}
new_feature_type
{
	define_as = "FEAT_MAGMA_K"
	name = "magma vein with treasure"
	display = '*' color = color.ORANGE
	priority = 19
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		CAN_PASS=getter.flags(features.magma_pass)
		SUBST_MAGMA=true SUBST_ROCK=true
		DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
	}
}
new_feature_type
{
	define_as = "FEAT_QUARTZ_K"
	name = "quartz vein with treasure"
	display = '*' color = color.ORANGE
	priority = 19
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		CAN_PASS=getter.flags(features.quartz_pass)
		SUBST_QUARTZ=true SUBST_ROCK=true
		DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
	}
}
new_feature_type
{
	define_as = "FEAT_SHAL_WATER"
	name = "stream of shallow water"
	display = '~' color = color.LIGHT_BLUE
	priority = 20
	shimmers = { color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.BLUE, }
	flags =
	{
		ATTR_MULTI=true FLOOR=true CAN_FLY=1 REMEMBER=true
		CAN_RUN=true DONT_NOTICE_RUNNING=true SUPPORT_GROWTH=true
		SUPPORT_LIGHT=getter.lit(nil, color.STEEL_BLUE)
		WATER=true
	}
}
new_feature_type
{
	define_as = "FEAT_DEEP_LAVA"
	name = "pool of deep lava"
	on_walk = "You move across the deep lava."
	display = '.' color = color.LIGHT_RED
	priority = 20
	can_enter = function(mode)
		if not mode then return true
		else
			-- In the overhead map
			return player.resists[dam.FIRE] >= 33
		end
	end
	flags =
	{
		FLOOR=true CAN_FLY=1 REMEMBER=true
		SUPPORT_LIGHT=getter.lit(nil, color.DARK_RED)
		LAVA=2
	}
	attacks =
	{
		[{dam.FIRE, 1}] = { 2,-1 }
	}
}
new_feature_type
{
	define_as = "FEAT_SHAL_LAVA"
	name = "stream of shallow lava"
	on_walk = "You move across the shallow lava."
	display = '.' color = color.RED
	priority = 20
	flags =
	{
		FLOOR=true CAN_FLY=1 REMEMBER=true
		SUPPORT_LIGHT=getter.lit(nil, color.DARK_RED)
		LAVA=true
	}
	attacks =
	{
		[{dam.FIRE, 1}] = { 1,-1 }
	}
}
new_feature_type
{
	define_as = "FEAT_DARK_PIT"
	name = "dark pit"
	on_walk = "Ohhh, it's dark and deep."
	on_block = "a deep chasm blocking your way"
	display = '#' color = color.LIGHT_DARK
	priority = 20
	flags =
	{
		CAN_FLY=2 NO_WALK=true DONT_NOTICE_RUNNING=true
		NO_TUNNEL_MSG="You cannot tunnel a @feat@."
	}
}
new_feature_type
{
	define_as = "FEAT_DIRT"
	name = "dirt"
	display = '.' color = color.LIGHT_UMBER
	priority = 20
	flags =
	{
		FLOOR=true CAN_RUN=true DONT_NOTICE_RUNNING=true SUPPORT_GROWTH=true
		SUPPORT_LIGHT=getter.lit(nil, color.UMBER)
	}
}
new_feature_type
{
	define_as = "FEAT_BARROW"
	name = "barrow"
	on_block = "the side of a rocky burial mound blocking your way"
	display = '^' color = color.LIGHT_UMBER
	priority = 20
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.UMBER)
		CAN_CLIMB=true CAN_FLY=15 WALL=true NO_WALK=true
		NO_VISION=true DONT_NOTICE_RUNNING=true PERMANENT=true
		CAN_PASS=getter.flags(features.pass_granite_wall)
		NO_TUNNEL_MSG="You cannot tunnel through this rock."
	}
}
new_feature_type
{
	define_as = "FEAT_SHORT_GRASS"
	name = "tuft of short grass"
	display = '.' color = color.DARK_KHAKI
	priority = 20
	flags =
	{
		FLOOR=true CAN_RUN=true DONT_NOTICE_RUNNING=true SUPPORT_GROWTH=true
		SUPPORT_LIGHT=getter.lit(nil, color.LIGHT_DARK)
	}
}
new_feature_type
{
	define_as = "FEAT_GRASS"
	name = "patch of grass"
	display = '.' color = color.LIGHT_GREEN
	priority = 20
	flags =
	{
		FLOOR=true CAN_RUN=true DONT_NOTICE_RUNNING=true SUPPORT_GROWTH=true
		SUPPORT_LIGHT=getter.lit(nil, color.GREEN)
	}
}
new_feature_type
{
	define_as = "FEAT_LONG_GRASS"
	name = "patch of long grass"
	display = '.' color = color.OLIVE_DRAB
	priority = 20
	flags =
	{
		FLOOR=true CAN_RUN=true DONT_NOTICE_RUNNING=true SUPPORT_GROWTH=true
		SUPPORT_LIGHT=getter.lit(nil, color.GREEN)
	}
}
new_feature_type
{
	define_as = "FEAT_TALL_GRASS"
	name = "patch of tall grass"
	display = ',' color = color.GREEN
	priority = 20
	on_walk = "The grass is taller than you."
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.DARK_GREEN)
		FLOOR=true CAN_RUN=true SUPPORT_GROWTH=true
		NO_VISION=true
	}
}
--from theme
new_feature_type
{
	define_as = "FEAT_ELANOR"
	name = "grass with Elanor flowers"
	display = ';' color = color.YELLOW
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
--from theme
new_feature_type
{
	define_as = "FEAT_FUMELLA"
	name = "grass with Fumella flowers"
	display = ';' color = color.RED
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_ANEMONES"
	name = "grass with anemones"
	display = ';' color = color.VIOLET
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_NIPHREDIL"
	name = "grass with Niphredil flowers"
	display = ';' color = color.WHITE
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_IRIS"
	name = "grass with irises"
	display = ';' color = color.BLUE
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_LILY"
	name = "water lily"
	display = ';' color = color.LIGHT_BLUE
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_KINGSFOIL"
	name = "grass with Kingsfoil flowers"
	display = ';' color = color.LIGHT_GREEN
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
new_feature_type
{
	define_as = "FEAT_ICE"
	name = "ice"
	display = '.' color = color.LIGHT_WHITE
	priority = 20
	flags =
	{
		FLOOR=true NOTICE=true
		MOVE_PRE_DIR = function()
			if not player.has_intrinsic(FLAG_FLY) and
				rng.percent(70 - player.lev / 2) then
				message("You slip on the icy floor.")
				return rng(9)
			end
		end
	}
	attacks =
	{
		[{dam.ICE, 50}] = { 1,20 }
	}
}
new_feature_type
{
	define_as = "FEAT_SAND"
	name = "sand"
	display = '.' color = color.YELLOW
	priority = 20
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
new_feature_type
{
	define_as = "FEAT_ASH"
	name = "ash"
	display = '.' color = color.SLATE
	priority = 20
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
new_feature_type
{
	define_as = "FEAT_MUD"
	name = "mud"
	display = '.' color = color.UMBER
	priority = 20
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
new_feature_type
{
	define_as = "FEAT_ICE_WALL"
	name = "ice wall"
	on_block = "an ice wall blocking your way"
	display = '#' color = color.LIGHT_WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.WHITE)
		WALL=true NO_WALK=true NO_VISION=true
		DONT_NOTICE_RUNNING=true TUNNELABLE={40,1600}
		DESTROY_INTO=FEAT_ICE SUBST_ICE=true
		CAN_PASS=getter.flags{PASS_ICE=true, PASS_INCORP=1}
		TUNNEL_WORKING_MSG="You tunnel into the ice wall... #BOh chilly#w."
	}
}

features.trees_pass = {
	PASS_TREES=1
	PASS_INCORP=1
	PASS_GASEOUS=1
	PASS_LIQUID=1
	PASS_CLIMB=2
}

new_feature_type
{
	define_as = "FEAT_DEAD_TREE"
	name = "dead tree"
	on_block = "a tree blocking your way"
	display = '#' color = color.LIGHT_DARK
	priority = 20
	flags =
	{
		CAN_FLY=3 WALL=true NO_WALK=true NO_VISION=true SUBST_TREE=true
		NOTICE=true DONT_NOTICE_RUNNING=true
		DESTROY_INTO=FEAT_GRASS CAN_PASS=getter.flags(features.trees_pass)
	}
}
new_feature_type
{
	define_as = "FEAT_TREES"
	name = "tree"
	on_block = "a tree blocking your way"
	display = '#' color = color.LIGHT_GREEN
	priority = 20
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.GREEN)
		CAN_FLY=3 WALL=true NO_WALK=true
		NO_VISION=true DONT_NOTICE_RUNNING=true
		DESTROY_INTO=FEAT_GRASS CAN_PASS=getter.flags(features.trees_pass)
		DEAD_TREE_FEAT=FEAT_DEAD_TREE SUBST_TREE=true
	}
}
new_feature_type
{
	define_as = "FEAT_DECAYING_TREES"
	name = "decaying tree"
	on_block = "a decaying tree blocking your way"
	display = '#' color = color.OLIVE_DRAB
	priority = 20
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.DARK_GREEN)
		CAN_FLY=3 WALL=true NO_WALK=true
		NO_VISION=true DONT_NOTICE_RUNNING=true
		DESTROY_INTO=FEAT_GRASS CAN_PASS=getter.flags(features.trees_pass)
		DEAD_TREE_FEAT=FEAT_DEAD_TREE SUBST_TREE=true
	}
}
new_feature_type
{
	define_as = "FEAT_DEAD_SMALL_TREE"
	name = "dead small tree"
	display = '#' color = color.LIGHT_DARK
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true
		REMEMBER=true NO_VISION=true
	}
}
new_feature_type
{
	define_as = "FEAT_SMALL_TREES"
	name = "small tree"
	display = '#' color = color.GREEN
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.DARK_GREEN)
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true
		REMEMBER=true NO_VISION=true DEAD_TREE_FEAT=FEAT_DEAD_SMALL_TREE
	}
}
new_feature_type
{
	define_as = "FEAT_DECAYING_SMALL_TREES"
	name = "decaying small tree"
	display = '#' color = color.DARK_GREEN
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true
		REMEMBER=true NO_VISION=true DEAD_TREE_FEAT=FEAT_DEAD_SMALL_TREE
	}
}
new_feature_type
{
	define_as = "FEAT_TREE_STUMP"
	name = "tree stump"
	display = ':' color = color.TAN
	priority = 20
	flags =
	{
		FLOOR=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_FIR_TREE"
	name = "fir tree"
	on_block = "a fir tree blocking your way"
	display = '#' color = color.GREEN
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.DARK_GREEN)
		CAN_FLY=3 WALL=true NO_WALK=true
		NO_VISION=true DONT_NOTICE_RUNNING=true TUNNELABLE={20,800}
		CAN_PASS=getter.flags(features.trees_pass)
		DEAD_TREE_FEAT=FEAT_DEAD_TREE SUBST_TREE=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_HILL"
	name = "low hill"
	on_walk = "You go over the hill."
	display = '^' color = color.GREEN
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true
		NO_VISION=true NO_TUNNEL_MSG="You cannot tunnel through that."
	}
}
new_feature_type
{
	define_as = "FEAT_MOUNTAIN"
	name = "mountain chain"
	on_block = "a hard stone block blocking your way"
	display = '^' color = color.LIGHT_UMBER
	priority = 20
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.UMBER)
		CAN_CLIMB=true CAN_FLY=15 WALL=true NO_WALK=true
		NO_VISION=true DONT_NOTICE_RUNNING=true PERMANENT=true
		CAN_PASS=getter.flags{
			PASS_STONE=15
			PASS_MOUNTAIN=15
			PASS_CLIMB=15
			PASS_INCORP=1
		}
		NO_TUNNEL_MSG="You cannot tunnel into such a hard stone."
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_SNOWY_PEAK"
	name = "snow-capped peak"
	on_block = "an unusually thick wall of ice barring your way"
	display = '^' color = color.WHITE
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		DONT_NOTICE_RUNNING=true PERMANENT=true CAN_FLY=30
		CAN_PASS=getter.flags{
			PASS_STONE=30
			PASS_MOUNTAIN=30
			PASS_CLIMB=30
			PASS_INCORP=1
		}
		NO_TUNNEL_MSG="The ice is too cold to tunnel through it."
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_DARK_MOUNTAIN"
	name = "dark mountain chain"
	on_block = "a very hard stone block blocking your way"
	display = '^' color = color.LIGHT_DARK
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true
		DONT_NOTICE_RUNNING=true CAN_FLY=25
		CAN_PASS=getter.flags{
			PASS_STONE=25
			PASS_MOUNTAIN=25
			PASS_CLIMB=25
			PASS_INCORP=1
		}
		NO_TUNNEL_MSG="This rock is far too hard to tunnel through."
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_BLUE_MOUNTAIN"
	name = "blue mountain chain"
	on_block = "a hard stone block blocking your way"
	display = '^' color = color.LIGHT_BLUE
	flags =
	{
		CAN_CLIMB=true WALL=true
		NO_WALK=true NO_VISION=true DONT_NOTICE_RUNNING=true
		PERMANENT=true CAN_FLY=20
		CAN_PASS=getter.flags{
			PASS_STONE=20
			PASS_MOUNTAIN=20
			PASS_CLIMB=20
			PASS_INCORP=1
		}
		NO_TUNNEL_MSG="You cannot tunnel through such hard stone."
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_GREY_MOUNTAIN"
	name = "grey mountain chain"
	on_block = "a hard stone block blocking your way"
	display = '^' color = color.SLATE
	flags =
	{
		CAN_CLIMB=true WALL=true
		NO_WALK=true NO_VISION=true DONT_NOTICE_RUNNING=true
		PERMANENT=true CAN_FLY=20
		CAN_PASS=getter.flags{
			PASS_STONE=20
			PASS_MOUNTAIN=20
			PASS_CLIMB=20
			PASS_INCORP=1
		}
		NO_TUNNEL_MSG="You cannot tunnel through such hard stone."
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_MOUNT_DOOM"
	name = "part of Mount Doom"
	on_block = "a stream of searing lava barring your way"
	display = '^' color = color.LIGHT_RED
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		DONT_NOTICE_RUNNING=true PERMANENT=true CAN_FLY=25
		NO_TUNNEL_MSG="You cannot tunnel through that."
	}
}

features.sandwall_pass = {
	PASS_INCORP=1
	PASS_STONE=1
}

new_feature_type
{
	define_as = "FEAT_SANDWALL"
	name = "sandwall"
	on_block = "a sandwall blocking your way"
	display = '#' color = color.YELLOW
	priority = 14
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true SUBST_SAND=true
		DONT_NOTICE_RUNNING=true EASY_DIG=true
		DESTROY_INTO=FEAT_SAND CAN_PASS=getter.flags(features.sandwall_pass)
	}
}
new_feature_type
{
	define_as = "FEAT_SANDWALL_H"
	name = "sandwall"
	on_block = "a sandwall blocking your way"
	display = '%' color = color.YELLOW
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true SUBST_SAND=true
		DONT_NOTICE_RUNNING=true EASY_DIG=true
		DESTROY_INTO=FEAT_SAND CAN_PASS=getter.flags(features.sandwall_pass)
	}
}
new_feature_type
{
	define_as = "FEAT_SANDWALL_K"
	name = "sandwall with treasure"
	on_block = "a sandwall blocking your way"
	display = '*' color = color.ORANGE
	priority = 19
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true SUBST_SAND=true
		DONT_NOTICE_RUNNING=true EASY_DIG=true
		DESTROY_INTO=FEAT_SAND CAN_PASS=getter.flags(features.sandwall_pass)
	}
}
new_feature_type
{
	define_as = "FEAT_HIGH_MOUNTAIN"
	name = "high mountain chain"
	on_walk = "Ohh you are flying very high!"
	on_block = "a very hard stone block blocking your way"
	display = '^' color = color.LIGHT_WHITE
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true
		DONT_NOTICE_RUNNING=true CAN_FLY=25
		NO_TUNNEL_MSG="This rock is far too hard to tunnel through."
	}
}
new_feature_type
{
	define_as = "FEAT_BETWEEN"
	name = "Void Jumpgate"
	on_walk = "A dark rift opens to the void here."
	display = '+' color = color.VIOLET
	flags =
	{
		FLOOR=true REMEMBER=true NOTICE=true PERMANENT=true CAN_RUN=true
		STAIR_DO = function(updown, ask_leave)
				if updown == "down" then
					features.do_between()
					return 0 -- Dungeon level changed by 0
				end
			end
		NO_RESTING = "Resting on a Void Jumpgate is too dangerous!"
	}
}
new_feature_type
{
	define_as = "FEAT_ALTAR_ERU"
	name = "Altar of Being"
	on_walk = "You feel at peace."
	display = '0' color = color.LIGHT_WHITE
	flags =
	{
		FLOOR=true REMEMBER=true NOTICE=true CAN_RUN=true
		ALTAR=god.ERU
	}
}
new_feature_type
{
	define_as = "FEAT_ALTAR_MANWE"
	name = "Altar of Winds"
	on_walk = "You grow a desire to become a bird."
	display = '0' color = color.LIGHT_BLUE
	flags =
	{
		FLOOR=true REMEMBER=true NOTICE=true CAN_RUN=true
		ALTAR=god.MANWE
	}
}
new_feature_type
{
	define_as = "FEAT_ALTAR_TULKAS"
	name = "Altar of Force"
	on_walk = "You grow a desire to fight evil."
	display = '0' color = color.LIGHT_RED
	flags =
	{
		FLOOR=true REMEMBER=true NOTICE=true CAN_RUN=true
		ALTAR=god.TULKAS
	}
}
new_feature_type
{
	define_as = "FEAT_ALTAR_MELKOR"
	name = "Altar of Darkness"
	on_walk = "Images of pain and death fill your mind."
	display = '0' color = color.LIGHT_DARK
	flags =
	{
		FLOOR=true REMEMBER=true NOTICE=true CAN_RUN=true
		ALTAR=god.MELKOR
	}
}
new_feature_type
{
	define_as = "FEAT_ALTAR_YAVANNA"
	name = "Altar of Nature"
	on_walk = "You feel the desire to walk in a great forest."
	display = '0' color = color.GREEN
	flags =
	{
		FLOOR=true REMEMBER=true NOTICE=true CAN_RUN=true
		ALTAR=god.YAVANNA
	}
}
new_feature_type
{
	define_as = "FEAT_ALTAR_ULMO"
	name = "Altar of Water"
	on_walk = "You feel the desire to see the sea."
	display = '0' color = color.BLUE
	flags =
	{
		FLOOR=true REMEMBER=true NOTICE=true CAN_RUN=true
		ALTAR=god.ULMO
	}
}
new_feature_type
{
	define_as = "FEAT_MARKER"
	name = "open floor"
	display = '.' color = color.WHITE
	flags =
	{
		FLOOR=true CAN_RUN=true DONT_NOTICE_RUNNING=true SUPPORT_GROWTH=true
	}
}
new_feature_type
{
	define_as = "FEAT_UNDERGROUND_TUNNEL"
	name = "Underground Tunnel"
	on_walk = "Oh, an underground tunnel!"
	display = '#' color = color.SLATE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.LIGHT_DARK)
		FLOOR=true REMEMBER=true DONT_NOTICE_RUNNING=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_TAINTED_WATER"
	name = "stream of tainted water"
	display = '~' color = color.UMBER
	flags =
	{
		FLOOR=true CAN_FLY=1 REMEMBER=true
		DONT_NOTICE_RUNNING=true
		WATER=true
	}
}
new_feature_type
{
	define_as = "FEAT_MON_TRAP"
	name = "monster trap"
	display = ';' color = color.VIOLET
	flags =
	{
		FLOOR=true
	}
}
new_feature_type
{
	define_as = "FEAT_BETWEEN2"
	name = "Void Jumpgate"
	on_walk = "A dark rift opens to the void here."
	display = '+' color = color.VIOLET
	flags =
	{
		FLOOR=true REMEMBER=true NOTICE=true PERMANENT=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_LAVA_WALL"
	name = "lava wall"
	on_block = "a lava wall blocking your way"
	display = '#' color = color.LIGHT_RED
	shimmers = { color.LIGHT_RED, color.LIGHT_RED, color.RED, color.RED, color.LIGHT_UMBER, color.UMBER, color.LIGHT_RED, }
	flags =
	{
		ATTR_MULTI=true WALL=true NO_WALK=true NO_VISION=true PERMANENT=true
		DONT_NOTICE_RUNNING=true
		NO_TUNNEL_MSG="The lava is far too hot to tunnel into it."
	}
}
new_feature_type
{
	define_as = "FEAT_GREAT_FIRE"
	name = "Great Fire"
	on_walk = "This fire is so powerful it could destroy even the most powerful artifacts."
	display = '%' color = color.VIOLET
	shimmers = { color.LIGHT_RED, color.LIGHT_RED, color.YELLOW, color.VIOLET, color.YELLOW, color.VIOLET, color.LIGHT_RED, }
	flags =
	{
		ATTR_MULTI=true FLOOR=true REMEMBER=true NOTICE=true PERMANENT=true
	}
	attacks =
	{
		[{dam.PURE, 1}] = { 150,2 }
	}
}
new_feature_type
{
	name = "field"
	display = ':' color = color.GREEN
	flags =
	{
		FLOOR=true PERMANENT=true NOTICE=true REMEMBER=true
		DONT_NOTICE_RUNNING=true
		NO_TUNNEL_MSG="You cannot tunnel a field."
	}
}
new_feature_type
{
	define_as = "FEAT_WORLD_BORDER"
	name = "Ekkaia, the Encircling Sea"
	display = '*' color = color.BLUE
	shimmers = { color.BLUE, color.BLUE, color.BLUE, color.BLUE, color.BLUE, color.BLUE, color.LIGHT_BLUE, }
	flags =
	{
		ATTR_MULTI=true WALL=true NO_WALK=true NO_VISION=true PERMANENT=true
		DONT_NOTICE_RUNNING=true
		NO_TUNNEL_MSG="You cannot tunnel the sea."
	}
}
new_feature_type
{
	name = "void"
	display = ' ' color = color.DARK
	flags =
	{
		FLOOR=true
	}
}
new_feature_type
{
	define_as = "FEAT_DEEP_WATER"
	name = "pool of deep water"
	display = '~' color = color.BLUE
	priority = 20
	shimmers = { color.BLUE, color.BLUE, color.BLUE, color.BLUE, color.BLUE, color.BLUE, color.LIGHT_BLUE, }
	can_enter = function(mode)
		if not mode then return true
		else
			-- In the overhead map
			return ((player.weight_limit() / 2) < calc_total_weight()) or player.has_intrinsic(FLAG_WALK_WATER) or player.has_intrinsic(FLAG_FLY) or player.has_intrinsic(FLAG_MAGIC_BREATH) or player.has_intrinsic(FLAG_WATER_BREATH)
		end
	end
	flags =
	{
		ATTR_MULTI=true FLOOR=true CAN_FLY=1 REMEMBER=true
		SUPPORT_LIGHT=getter.lit(nil, color.STEEL_BLUE)
		WATER=2
	}
	attacks =
	{
		[{dam.DROWN, 10}] = { -1,1 }
	}
}
new_feature_type
{
	define_as = "FEAT_GLASS_WALL"
	name = "glass wall"
	on_block = "a glass wall blocking your way"
	display = '.' color = color.LIGHT_BLUE
	flags =
	{
		NO_WALK=true WALL=true PERMANENT=true NOTICE=true
		DONT_NOTICE_RUNNING=true
		NO_TUNNEL_MSG="This glass seems to be totaly impenetrable."
	}
}
new_feature_type
{
	define_as = "FEAT_ILLUS_WALL"
	name = "illusion wall"
	on_walk = "Looks like this wall is not so real."
	display = '#' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		FLOOR=true NO_VISION=true REMEMBER=true DONT_NOTICE_RUNNING=true
		NO_TUNNEL_MSG="@Digger@ pass@es@ through the wall like it's not there!"
		DIGGER_MIMIC=FEAT_WALL_EXTRA
	}
}
new_feature_type
{
	define_as = "FEAT_GRASS_ROOF"
	name = "Grass roof"
	display = '#' color = color.YELLOW
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_GRASS_ROOF_TOP"
	name = "grass roof top"
	display = '#' color = color.YELLOW
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_GRASS_ROOF_CHIMNEY"
	name = "grass roof chimney"
	display = '#' color = color.YELLOW
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_BRICK_ROOF"
	name = "brick roof"
	display = '#' color = color.RED
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_BRICK_ROOF_TOP"
	name = "brick roof top"
	display = '#' color = color.RED
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_BRICK_ROOF_CHIMNEY"
	name = "brick roof chimney"
	display = '#' color = color.RED
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_WINDOW"
	name = "window"
	display = '#' color = color.WHITE
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_SMALL_WINDOW"
	name = "small window"
	display = '#' color = color.WHITE
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_RAIN_BARREL"
	name = "rain barrel"
	display = '#' color = color.WHITE
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true PERMANENT=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_FLOWER"
	name = "grass with flowers"
	display = ';' color = color.LIGHT_GREEN
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true SUPPORT_GROWTH=true
	}
}
new_feature_type
{
	define_as = "FEAT_COBBLESTONE_ROAD"
	name = "cobblestone road"
	display = '.' color = color.WHITE
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_COBBLESTONE_OUTLET"
	name = "cobblestone with outlet"
	display = '.' color = color.WHITE
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_TOWN"
	name = "town"
	display = '*' color = color.WHITE
	flags =
	{
		FLOOR=true NOTICE=true
	}
}
new_feature_type
{
	define_as = "FEAT_UNDERGROUND_TUNNEL_MOUNTAIN"
	name = "Underground Tunnel"
	on_walk = "Oh, an underground tunnel!"
	display = '^' color = color.LIGHT_UMBER
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.UMBER)
		FLOOR=true REMEMBER=true DONT_NOTICE_RUNNING=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_FIRE"
	name = "a blazing fire"
	on_walk = "The blazing fire burns you!"
	display = '%' color = color.YELLOW
	shimmers = { color.YELLOW, color.YELLOW, color.YELLOW, color.LIGHT_RED, color.RED, color.YELLOW, color.LIGHT_RED, }
	flags =
	{
		ATTR_MULTI=true FLOOR=true REMEMBER=true
		DONT_NOTICE_RUNNING=true
	}
	attacks =
	{
		[{dam.FIRE, 1}] = { -1,2 }
	}
}
new_feature_type
{
	define_as = "FEAT_PERM_RUBBLE"
	name = "pile of rubble"
	on_block = "some rubble blocking your way"
	display = ':' color = color.WHITE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		CAN_FLY=2 PERMANENT=true SUBST_ROCK=true SUBST_RUBBLE=true
		CAN_PASS=getter.flags{
			PASS_STONE=1
			PASS_CLIMB=1
			PASS_INCORP=1
			PASS_GASEOUS=1
			PASS_LIQUID=1
		}
		NO_TUNNEL_MSG="Looks like this pile of rubble is quite hard."
	}
}
new_feature_type
{
	define_as = "FEAT_ROCKY_GROUND"
	name = "rocky ground"
	display = '.' color = color.SLATE
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.LIGHT_DARK)
		FLOOR=true CAN_RUN=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_CLOUD"
	name = "cloud-like vapour"
	display = '.' color = color.LIGHT_WHITE
	shimmers = { color.LIGHT_WHITE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_WHITE, color.WHITE, color.LIGHT_WHITE, color.LIGHT_BLUE, }
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.WHITE)
		FLOOR=true ATTR_MULTI=true
		CAN_RUN=true DONT_NOTICE_RUNNING=true
	}
}
new_feature_type
{
	define_as = "FEAT_CONDENS_WATER"
	name = "condensing water"
	display = '~' color = color.LIGHT_BLUE
	shimmers = { color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.BLUE, }
	flags =
	{
		SUPPORT_LIGHT=getter.lit(color.LIGHT_STEEL_BLUE, color.BLUE)
		ATTR_MULTI=true FLOOR=true REMEMBER=true
		CAN_RUN=true DONT_NOTICE_RUNNING=true
		WATER=true
	}
}
new_feature_type
{
	define_as = "FEAT_DENSE_FOG"
	name = "dense fog"
	on_walk = "You wander through the fog."
	display = '#' color = color.WHITE
	shimmers = { color.WHITE, color.LIGHT_WHITE, color.SLATE, color.SLATE, color.SLATE, color.WHITE, color.WHITE, }
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.LIGHT_DARK)
		FLOOR=true NO_VISION=true REMEMBER=true ATTR_MULTI=true
		DONT_NOTICE_RUNNING=true
		NO_TUNNEL_MSG="You cannot tunnel through fog!"
	}
}
new_feature_type
{
	define_as = "FEAT_HAIL"
	name = "hail-stone wall"
	on_block = "a hail-stone wall blocking your way."
	display = '#' color = color.LIGHT_WHITE
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		DONT_NOTICE_RUNNING=true TUNNELABLE={40,1600}
		DESTROY_INTO=FEAT_ICE SUBST_ICE=true
		CAN_PASS=getter.flags{
			PASS_ICE=1
			PASS_INCORP=1
			PASS_GASEOUS=1
			PASS_LIQUID=1
		}
	}
}
new_feature_type
{
	define_as = "FEAT_COPPER_PILLAR"
	name = "copper pillar"
	on_block = "a copper pillar blocking your way."
	display = '#' color = color.UMBER
	shimmers = { color.UMBER, color.UMBER, color.UMBER, color.ORANGE, color.UMBER, color.UMBER, color.UMBER, }
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		DONT_NOTICE_RUNNING=true ATTR_MULTI=true PERMANENT=true
		CAN_PASS=getter.flags{PASS_INCORP=1}
		NO_TUNNEL_MSG="The copper is too tough to tunnel through."
	}
}
new_feature_type
{
	define_as = "FEAT_ETHEREAL"
	name = "ethereal wall"
	on_block = "an unseen force blocking your way"
	display = '.' color = color.WHITE
	flags =
	{
		NO_WALK=true WALL=true PERMANENT=true NOTICE=true
		DONT_NOTICE_RUNNING=true
		NO_TUNNEL_MSG="You can't even see your obstruction!"
	}
}

new_feature_type
{
	define_as = "FEAT_GLAC_WALL"
	name = "glacial wall"
	on_block = "a hard glacial wall blocking your way"
	display = '#' color = color.LIGHT_BLUE
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true
		DONT_NOTICE_RUNNING=true TUNNELABLE={40,1600}
		DESTROY_INTO=FEAT_ICE SUBST_ICE=true
		CAN_PASS=getter.flags{PASS_INCORP=1, PASS_ICE=1}
		TUNNEL_WORKING_MSG="You tunnel into the glacial wall... #BOh chilly#w."
	}
}
new_feature_type
{
	define_as = "FEAT_SLOT"
	name = "battlement"
	on_block = "a hard stone battlement blocking your way."
	display = '#' color = color.WHITE
	flags =
	{
		NO_WALK=true NOTICE=true SUBST_GRANITE=true
		DONT_NOTICE_RUNNING=true
		CAN_PASS=getter.flags{PASS_INCORP=1, PASS_STONE=8}
	}
}
new_feature_type
{
	define_as = "FEAT_TREE_MALLORN"
	name = "mallorn tree"
	on_block = "a mallorn tree is blocking your way"
	display = '#' color = color.YELLOW
	shimmers = { color.YELLOW, color.GOLD, color.YELLOW, color.GOLD, color.YELLOW, color.GOLD, color.YELLOW, }
	priority = 20
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.ORANGE)
		CAN_FLY=3 WALL=true NO_WALK=true ATTR_MULTI=true
		NO_VISION=true DONT_NOTICE_RUNNING=true
		DESTROY_INTO=FEAT_GRASS CAN_PASS=getter.flags(features.trees_pass)
		DEAD_TREE_FEAT=FEAT_DEAD_TREE SUBST_TREE=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_DEAD_MARSHES"
	name = "part of the Dead Marshes"
	display = '~' color = color.LIGHT_GREEN
	attacks =
	{
		[{dam.CONFUSION, 6}] = { 10,10 }
	}
	flags =
	{
		FLOOR=true CAN_LEVITATE=true CAN_FLY=true REMEMBER=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_SWAMP_POOL"
	name = "swamp pool"
	display = '~' color = color.GREEN
	attacks =
	{
		[{dam.POIS, 1}] = { 1,8 }
	}
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.DARK_GREEN)
		FLOOR=true CAN_LEVITATE=true CAN_FLY=true REMEMBER=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_ANDUIN"
	name = "stream of the Anduin river"
	display = '~' color = color.LIGHT_BLUE
	attacks =
	{
		[{dam.WATER, 1}] = { 1,1 }
	}
	shimmers = { color.WHITE, color.WHITE, color.WHITE, color.BLUE, color.WHITE, color.WHITE, color.BLUE, }
	flags =
	{
		ATTR_MULTI=true FLOOR=true CAN_LEVITATE=true CAN_FLY=true REMEMBER=true
		CAN_RUN=true DONT_NOTICE_RUNNING=true SUPPORT_GROWTH=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_DENSE_MIST"
	name = "dense mist"
	on_walk = "You wander through the mist."
	display = '#' color = color.WHITE
	shimmers = { color.WHITE, color.LIGHT_WHITE, color.SLATE, color.SLATE, color.SLATE, color.WHITE, color.WHITE, }
	flags =
	{
		SUPPORT_LIGHT=getter.lit(nil, color.LIGHT_DARK)
		FLOOR=true NO_VISION=true REMEMBER=true
		ATTR_MULTI=true DONT_NOTICE_RUNNING=true
		NO_TUNNEL_MSG="You cannot tunnel through mist!"
	}
}
-- from theme (overland view only)
new_feature_type
{
	define_as = "FEAT_RIVER"
	name = "river"
	display = '~' color = color.WHITE
	shimmers = { color.WHITE, color.WHITE, color.WHITE, color.LIGHT_BLUE, color.WHITE, color.WHITE, color.LIGHT_BLUE, }
	flags =
	{
		ATTR_MULTI=true FLOOR=true CAN_LEVITATE=true CAN_FLY=true
		REMEMBER=true CAN_RUN=true DONT_NOTICE_RUNNING=true
		SUPPORT_GROWTH=true
	}
}
-- from theme
new_feature_type
{
	define_as = "FEAT_BLACK_GATE"
	name = "Black Gate"
	on_block = "a closed Black Gate blocking your way"
	display = '+' color = color.LIGHT_DARK
	flags =
	{
		CAN_CLIMB=true WALL=true
		NO_WALK=true NO_VISION=true NOTICE=true DONT_NOTICE_RUNNING=true
		PERMANENT=true CAN_PASS=getter.flags{PASS_INCORP=1, PASS_STONE=8}
		NO_TUNNEL_MSG="You cannot tunnel through that."
	}
}
-- Small rubble, a auto-drop container
new_feature_type
{
	define_as = "FEAT_SMALL_RUBBLE"
	name = "small pile of rubble"
	display = ':' color = color.WHITE
	flags =
	{
		FLOOR=true NOTICE=true PERMANENT=true
		SUPPORT_LIGHT=getter.lit(color.YELLOW, color.LIGHT_DARK)
		MOVE_POST_DEST = function()
			local grid = cave(player.py, player.px)
			if grid.flags[FLAG_DROP_OBJECTS] then
				message("You have found something inside the small pile rubble!")
				foreach_flags(grid.flags[FLAG_DROP_OBJECTS], function(flags, key)
					local obj = object_dup(flags[key])
					drop_near(obj, -1, player.py, player.px)
				end)
				grid.flags[FLAG_DROP_OBJECTS] = nil
			end
		end
	}
}

-- For damage types temporary features
new_feature_type
{
	define_as = "FEAT_BURNING_GROUND"
	name = "burning ground"
	on_walk = "You move across the flames."
	display = '.' color = color.CRIMSON
	priority = 20
	flags =
	{
		FLOOR=true REMEMBER=true
		SUPPORT_LIGHT=getter.lit(nil, color.DARK_RED)
	}
	attacks =
	{
		[{dam.FIRE, 1}] = { 1,-1 }
	}
}
new_feature_type
{
	define_as = "FEAT_POND_WATER"
	name = "small pond of water"
	display = '~' color = color.LIGHT_BLUE
	priority = 20
	shimmers = { color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.LIGHT_BLUE, color.BLUE, }
	flags =
	{
		ATTR_MULTI=true FLOOR=true REMEMBER=true
		CAN_RUN=true DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(nil, color.STEEL_BLUE)
		WATER=true
	}
}
new_feature_type
{
	define_as = "FEAT_ICE_SHARDS"
	name = "ice shards"
	display = '.' color = color.LIGHT_WHITE
	priority = 20
	flags =
	{
		FLOOR=true NOTICE=true
	}
	attacks =
	{
		[{dam.CUT, 50}] = { 1,-1 }
	}
}
new_feature_type
{
	define_as = "FEAT_FROZEN_GROUND"
	name = "frozen ground"
	display = '.' color = color.LIGHT_WHITE
	priority = 20
	flags =
	{
		FLOOR=true NOTICE=true
		MOVE_PRE_DIR = function()
			if not player.has_intrinsic(FLAG_FLY) and rng.percent(70 - player.lev / 2) then
				message("You slip on the frozen ground.")
				return rng(9)
			end
		end
	}
}


--------------------------
-- Dwarven Architecture --
--------------------------
new_feature_type
{
	define_as = "FEAT_WATERFALL"
	name = "waterfall"
	display = '~' color = color.LIGHT_BLUE
	priority = 20
	shimmers = { color.WHITE, color.LIGHT_BLUE, color.WHITE, color.LIGHT_BLUE, color.WHITE, color.LIGHT_BLUE, color.WHITE, }
	flags =
	{
		ATTR_MULTI=true CAN_FLY=1 REMEMBER=true
		CAN_RUN=true DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(nil, color.STEEL_BLUE)
		WATER=true
	}
}
new_feature_type
{
	define_as = "FEAT_STONE_PILLAR"
	name = "stone pillar"
	on_block = "an engraved stone pillar here"
	display = 'O' color = color.WHITE
	priority = 20
	flags =
	{
		WALL=true NO_WALK=true NO_VISION=true NOTICE=true
		DONT_NOTICE_RUNNING=true ATTR_MULTI=true PERMANENT=true
		CAN_PASS=getter.flags{PASS_INCORP=1}
		SUPPORT_LIGHT=getter.lit(nil, color.STEEL_BLUE)
		-- It might be fun to change this so tunneling it actually
		-- causes a cave-in. Remove the pillar, but drop heavy stone
		-- and rubble on everything in the area
		NO_TUNNEL_MSG="Removing this structural support might cause a cave-in"
	}
}
new_feature_type
{
	define_as = "FEAT_BRIDGE_LEFT"
	name = "bridge"
	display = '/' color = color.UMBER
	priority = 20
	flags =
	{
		FLOOR=true CAN_RUN=true DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(nil, color.STEEL_BLUE)
	}
}
new_feature_type
{
	define_as = "FEAT_BRIDGE_RIGHT"
	name = "bridge"
	display = '\\' color = color.UMBER
	priority = 20
	flags =
	{
		FLOOR=true CAN_RUN=true DONT_NOTICE_RUNNING=true
		SUPPORT_LIGHT=getter.lit(nil, color.STEEL_BLUE)
	}
}

------------------------------
-- End Dwarven Architecture --
------------------------------

-----------------------------------------------------------------------------
-- Glyphs and other magical features
-----------------------------------------------------------------------------

new_feature_type
{
	define_as = "FEAT_GLYPH_WARDING"
	name = "glyph of warding"
	on_walk = "There is a mighty spell of protection here."
	display = ';' color = color.YELLOW
	flags =
	{
		FLOOR=true NOTICE=true REMEMBER=true CAN_RUN=true 
		SUPPORT_LIGHT=getter.lit(nil, color.LIGHT_DARK)
		MONST_NO_PLACE=true

		ON_MAKE =
			function(y, x, feat, old_feat)
				local flags = cave(y, x).flags

				if flags[FLAG_ORIG_FEAT] or flags[FLAG_GLYPH_POWER] then
					cave(y, x).feat = old_feat
					return
				end

				flags[FLAG_ORIG_FEAT]   = old_feat
				flags[FLAG_GLYPH_POWER] = 550
			end

		MONST_CAN_ENTER =
			function(y, x, do_attack, monst)
				local msg = "The rune of protection was broken!"

				return features.try_break_glyph(y, x, monst, msg)
			end

		MONST_CAN_ATTACK =
			function(y, x, do_move, monst)
				local msg   = "The rune of protection was broken!"
				if not do_move then
					return features.try_break_glyph(y, x, monst, msg)
				end
				return false
			end
	}
} -- FEAT_GLYPH

new_feature_type
{
	define_as = "FEAT_EXPLOSIVE_RUNE"
	name = "explosive rune"
	on_walk = "This rune seems unstable."
	display = ';' color = color.LIGHT_RED
	flags =
	{
		FLOOR=true NOTICE=true REMEMBER=true CAN_RUN=true 
		SUPPORT_LIGHT=getter.lit(nil, color.LIGHT_DARK)
		MONST_NO_PLACE=true

		ON_MAKE =
			function(y, x, feat, old_feat)
				local flags = cave(y, x).flags

				if flags[FLAG_ORIG_FEAT] or flags[FLAG_GLYPH_POWER] then
					cave(y, x).feat = old_feat
					return
				end

				flags[FLAG_ORIG_FEAT]   = old_feat
				flags[FLAG_GLYPH_POWER] = 99
			end

		MONST_CAN_ENTER =
			function(y, x, do_attack, monst)
				if do_attack then
					return true
				end

				local msg = "An explosive rune was disarmed."
				return features.try_break_glyph(y, x, monst, msg)
			end

		MONST_CAN_ATTACK =
			function(y, x, do_move, monst)
				local msg = "The rune explodes!"
				local broke = 
					features.try_break_glyph(y, x, monst, msg)

				if broke then
					fire_ball(dam.PURE, 0,
							  2 * (player.lev / 2) + rng.roll(7, 7), 2, 0)
				end
				return broke
			end
	}
} -- FEAT_EXPLOSIVE_RUNE
