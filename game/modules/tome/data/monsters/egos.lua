-- Monster egos definitions

-------------------- TOWN EGOS ------------------
new_monster_ego
{
	define_as = "RACE_EGO_HUMAN_TOWN"
	name = "human" place = "before"
	level = mods.add(0) rarity = 15
	speed = mods.add(0) life = {mods.add(0),mods.add(0)} ac = mods.add(0)
	aaf = mods.add(0) sleep = mods.add(0)
	exp = mods.add(0)
	weight = mods.add(0)
	need_flags =
	{
		TOWN_PEOPLE_CAN_HUMAN=1
	}
}
new_monster_ego
{
	define_as = "RACE_EGO_ELVEN_TOWN"
	name = "elven" place = "before"
	level = mods.add(0) rarity = 15
	speed = mods.add(3) life = {mods.add(0),mods.add(0)} ac = mods.add(0)
	aaf = mods.add(0) sleep = mods.add(0)
	exp = mods.add(0)
	weight = mods.add(0)
	need_flags =
	{
		TOWN_PEOPLE_CAN_ELF=1
	}
}
new_monster_ego
{
	define_as = "RACE_EGO_DWARVEN_TOWN"
	name = "dwarven" place = "before"
	level = mods.add(0) rarity = 15
	speed = mods.add(0) life = {mods.add(0),mods.add(1)} ac = mods.add(5)
	aaf = mods.add(0) sleep = mods.add(0)
	exp = mods.add(0)
	weight = mods.add(0)
	need_flags =
	{
		TOWN_PEOPLE_CAN_DWARF=1
	}
}
new_monster_ego
{
	define_as = "RACE_EGO_HOBBIT_TOWN"
	name = "hobbit" place = "before"
	level = mods.add(0) rarity = 25
	speed = mods.add(1) life = {mods.add(1),mods.add(0)} ac = mods.add(0)
	aaf = mods.add(5) sleep = mods.add(0)
	exp = mods.add(0)
	weight = mods.add(0)
	need_flags =
	{
		TOWN_PEOPLE_CAN_HOBBIT=1
	}
}
