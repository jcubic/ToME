-- Equipment slots

declare_global_constants
{
	"INVEN_MAINHAND",
	"INVEN_OFFHAND",
	"INVEN_RING",
	"INVEN_NECK",
	"INVEN_LITE",
	"INVEN_BODY",
	"INVEN_OUTER",
	"INVEN_HEAD",
	"INVEN_HANDS",
	"INVEN_FEET",
	"INVEN_TOOL",
	"INVEN_QUIVER",
	"INVEN_AMMO",
	"INVEN_BACKPACK",
	"INVEN_MOUNT",
	"INVEN_SYMBIOTE",
}

INVEN_MAINHAND	= add_new_inventory_slot("hand",	"In hand", 	"wielding in your hand",	"You are wielding",	"You were wielding")
INVEN_OFFHAND	= add_new_inventory_slot("off hand",	"In off hand", "wielding in your off hand",	"You are wielding",	"You were wielding")
INVEN_RING	= add_new_inventory_slot("finger",	"On finger", 	"wearing on your finger",	"You are wearing",	"You were wearing")
INVEN_NECK	= add_new_inventory_slot("neck",	"Around neck",	"wearing around your neck",	"You are wearing",	"You were wearing")
INVEN_LITE	= add_new_inventory_slot("lite source",	"Light source","using to light the way",	"Your light source is",	"You were holding")
INVEN_BODY	= add_new_inventory_slot("body",	"On body", 	"wearing on your body",		"You are wearing",	"You were wearing")
INVEN_OUTER	= add_new_inventory_slot("cloak",	"About body",	"wearing on your back",		"You are wearing",	"You were wearing")
INVEN_HEAD	= add_new_inventory_slot("head",	"On head", 	"wearing on your head",		"You are wearing",	"You were wearing")
INVEN_HANDS	= add_new_inventory_slot("hands",	"On hands", 	"wearing on your hands",	"You are wearing",	"You were wearing")
INVEN_FEET	= add_new_inventory_slot("feet",	"On feet", 	"wearing on your feet",		"You are wearing",	"You were wearing")
INVEN_TOOL	= add_new_inventory_slot("tool",	"Using", 	"using",			"You are using",	"You were using")
INVEN_QUIVER	= add_new_inventory_slot("quiver",	"Quiver", 	"wearing as your quiver",	"In your quiver your have",	"You were carrying as your quiver")
INVEN_AMMO	= add_new_inventory_slot("quiver",	"Ammo", 	"wearing in your quiver",	"In your quiver your have",	"You were carrying in your quiver")
INVEN_BACKPACK	= add_new_inventory_slot("backpack",	"Backpack", 	"using as backpack",		"Your backpack is",	"Your backpack was")
INVEN_MOUNT	= add_new_inventory_slot("mount",	"Mount",	"riding",			"You are riding",	"You were riding")
INVEN_SYMBIOTE= add_new_inventory_slot("symbiote", "Symbiote", "in symbiosis with","You are in symbiosis with", "You were in symbiosis with")

-- INVEN_MAINHAND is the first slot to be considered equipment
INVEN_PACK = INVEN_MAINHAND
