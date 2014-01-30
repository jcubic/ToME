--
-- Informations for the various store owners
--

new_store_owner
{
	define_as = "store.OWNER_DEFAULT"
	name = "Narok(Human)"
	max_cost = 10000
	min_inflate = 108
	max_inflate = 170
	haggle_per = 1
	insult_max = 1
	costs = { 120, 100, 80 }
}

new_store_owner
{
	define_as = "store.OWNER_PLAYER"
	name = "Player(You)"
	max_cost = 0
	min_inflate = 100
	max_inflate = 100
	haggle_per = 1
	insult_max = 1
	costs = { 100, 100, 100 }
}

new_store_owner
{
	define_as = "store.OWNER_BEASTMASTER"
	name = "Sharra (human)"
	max_cost = 1500
	min_inflate = 100
	max_inflate = 100
	haggle_per = 0
	insult_max = 0
	costs = { 1500, 1500, 1500 }
}

--- Bree
new_store_owner
{
	define_as = "store.OWNER_MARLO"
	name = "Marlo Brockhouse(Hobbit)"
	max_cost = 15000
	min_inflate = 105
	max_inflate = 120
	haggle_per = 6
	insult_max = 16
	costs = { 120, 100, 80 }
}

--- Lothlorien
new_store_owner
{
	define_as = "store.OWNER_CELEBORN"
	name = "Celeborn(Eldar)"
	max_cost = 15000
	min_inflate = 105
	max_inflate = 120
	haggle_per = 6
	insult_max = 16
	costs = { 120, 100, 80 }
}
