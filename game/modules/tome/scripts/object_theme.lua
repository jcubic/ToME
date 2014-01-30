-- THEME_TREASURE
rand_obj.themed.register_new
(
	"THEME_TREASURE",
	function(obj_level, theme_flags)
		local obj = new_object()

		if not make_gold(obj) then
			delete_object(obj)
			return
		end
		return obj
	end
)
-- THEME_AXES
rand_obj.themed.register_new("THEME_AXES",
{
	[TV_AXE] = 1
})

-- THEME_COMBAT
rand_obj.themed.register_new("THEME_COMBAT",
{
	[TV_SHOT] = 1
	[TV_ARROW] = 1
	[TV_BOW] = 1
	[TV_SLING] = 1
	[TV_HAFTED] = 1
	[TV_POLEARM] = 1
	[TV_SWORD] = 1
	[TV_KNIFE] = 1
	[TV_AXE] = 1
	[TV_BOOTS] = 1
	[TV_GLOVES] = 1
	[TV_LIGHT_HELM] = 1
	[TV_HEAVY_HELM] = 1
	[TV_SHIELD] = 1
	[TV_CLOAK] = 1
	[TV_LIGHT_ARMOUR] = 1
	[TV_HEAVY_ARMOUR] = 1
	[TV_QUIVER] = 1
})

-- THEME_BOWS
rand_obj.themed.register_new("THEME_BOWS",
{
	[TV_SHOT] = 1
	[TV_ARROW] = 1
	[TV_BOW] = 1
	[TV_SLING] = 1
	[TV_QUIVER] = 1
})

-- THEME_ARMOR
rand_obj.themed.register_new("THEME_ARMOR",
{
	[TV_BOOTS] = 1
	[TV_GLOVES] = 1
	[TV_LIGHT_HELM] = 1
	[TV_HEAVY_HELM] = 1
	[TV_SHIELD] = 1
	[TV_CLOAK] = 1
	[TV_LIGHT_ARMOUR] = 1
	[TV_HEAVY_ARMOUR] = 1
})

-- THEME_CANTRIPS
rand_obj.themed.register_new("THEME_CANTRIPS",
{
	[TV_SCROLL] = 1
	[TV_POTION] = 1
})

-- THEME_HAFTED
rand_obj.themed.register_new("THEME_HAFTED",
{
	[TV_HAFTED] = 1
})

-- THEME_KNIFE
rand_obj.themed.register_new("THEME_KNIFE",
{
	[TV_KNIFE] = 1
})

--- THEME_DEMON
rand_obj.themed.register_new("THEME_DEMON",
{
	[TV_DEMON_ITEM] = 1
})

-- THEME_DEVICE
rand_obj.themed.register_new("THEME_DEVICE",
{
	[TV_AMULET] = 1
	[TV_RING] = 1
	[TV_WAND] = 1
	[TV_ORB] = 1
	[TV_GEMSTONE] = 1
})

-- THEME_MAGIC
rand_obj.themed.register_new("THEME_MAGIC",
{
	[TV_MSTAFF] = 1
	[TV_AMULET] = 1
	[TV_RING] = 1
	[TV_WAND] = 1
	[TV_ORB] = 1
	[TV_GEMSTONE] = 1
	[TV_SCROLL] = 1
	[TV_POTION] = 1
	[TV_BOOK] = 1
	[TV_RUNE1] = 1
	[TV_RUNE2] = 1
})

-- THEME_POLEARM
rand_obj.themed.register_new("THEME_POLEARM",
{
	[TV_POLEARM] = 1
})

-- THEME_SWORD
rand_obj.themed.register_new("THEME_SWORD",
{
	[TV_SWORD] = 1
})

-- THEME_TOOLS
rand_obj.themed.register_new("THEME_TOOLS",
{
	[TV_SPIKE] = 1
	[TV_TOOL] = 1
	[TV_LITE] = 1
	[TV_TRAPKIT] = 1
	[TV_BACKPACK] = 1
})

-- THEME_JUNK
rand_obj.themed.register_new("THEME_JUNK", { [TV_JUNK] = 1 })

-- THEME_FOOD
rand_obj.themed.register_new("THEME_FOOD", { [TV_FOOD] = 1 })

-- THEME_CHESTS
rand_obj.themed.register_new("THEME_CHESTS", { [TV_CHEST] = 1 })

-- THEME_GOLD
rand_obj.themed.register_new
(
	"THEME_GOLD",
	function(obj_level, theme_flags)
		local obj = new_object()

		if not make_gold(obj) then
			delete_object(obj)
			return
		end
		return obj
	end
)

-- Stores customization
flag_set(store.make_obj.full_random_theme, FLAG_THEME_COMBAT, 25)
flag_set(store.make_obj.full_random_theme, FLAG_THEME_ARMOR, 25)
flag_set(store.make_obj.full_random_theme, FLAG_THEME_MAGIC, 25)
flag_set(store.make_obj.full_random_theme, FLAG_THEME_TOOLS, 25)
