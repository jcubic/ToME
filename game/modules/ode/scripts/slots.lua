-- Equipment slots

declare_global_constants
{
	"INVEN_MAINHAND",
	"INVEN_BODY",
	"INVEN_LITE",
}

INVEN_MAINHAND	= add_new_inventory_slot("hand",	"In hand", 	"wielding in your hand",	"You are wielding",	"You were wielding")
INVEN_BODY	= add_new_inventory_slot("body",	"On body", 	"wearing on your body",		"You are wearing",	"You were wearing")
INVEN_LITE	= add_new_inventory_slot("lite source",	"Light source","using to light the way",	"Your light source is",	"You were holding")

-- INVEN_MAINHAND is the first slot to be considered equipment
INVEN_PACK = INVEN_MAINHAND
