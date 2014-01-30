--
-- Helper functions for magic powers
--

declare_global_constants {
	"__get_magic_info",
	"add_magic",
	"execute_magic",
	"get_level_power",
	"magic_power_sucess",
}
declare_globals {
	"__magic_powers",
}

__magic_powers = {}

function add_magic(m)
	local i, ret

	if type(m.spell_list) ~= "table" then
		error("add_magic called without a table")
	end

	-- Ok iterate over all the powers to add
	local index, p, max

	-- First, count them
	max = 0
	for index, p in m.spell_list do
		max = max + 1
	end

	-- Now register it
	ret = {}
	i = new_magic_power(max)
	ret.spells = i
	ret.max = max
	ret.fail_fct = m.fail
	if m.stat then
		ret.stat = m.stat
	else
		ret.stat = A_INT
	end
	if m.get_level then
		ret.get_current_level = m.get_level
	else
		ret.get_current_level = function()
			return player.lev
		end
	end

	-- And add each spells
	max = 0
	ret.info = {}
	ret.spell = {}
	for index, p in m.spell_list do
		assert(p.name, "No name for the spell!")
		assert(p.desc, "No desc for the spell!")
		assert(p.mana, "No mana for the spell!")
		assert(p.level, "No level for the spell!")
		assert(p.fail, "No fail for the spell!")
		assert(p.info, "No info for the spell!")
		assert(p.spell, "No spell for the spell!")

		get_magic_power(i, max).name = p.name
		get_magic_power(i, max).desc = p.desc
		get_magic_power(i, max).mana_cost = p.mana
		get_magic_power(i, max).min_lev = p.level
		get_magic_power(i, max).fail = p.fail
		ret.info[max] = p.info
		ret.spell[max] = p.spell

		max = max + 1
	end

	return ret
end

function __get_magic_info(power)
	return __current_magic_power_info[power]()
end

function execute_magic(m)
	local sn, ret

	-- Ask for a spell
	__current_magic_power_info = m.info
	ret, sn = select_magic_power(0, m.spells, m.max, "__get_magic_info", m.get_current_level(), m.stat)
	if not ret then return end
		
	-- Verify mana needs
	if (get_magic_power(m.spells, sn).mana_cost > player.csp()) then message("Not enough mana!") return end

	-- Get infos
	local fail_adj, fail_min = player.spell_chance_get_fails(stat)

	-- Verify failure(second parameter is optional)
	if m.fail then
		__current_magic_power_fail = m.fail_fct
		if not magic_power_sucess(get_magic_power(m.spells, sn), fail_adj, fail_min, __current_magic_power_fail) then return end
	else
		if not magic_power_sucess(get_magic_power(m.spells, sn), fail_adj, fail_min) then return end
	end

	-- Actually cast the spells
	m.spell[sn]()

	-- use up some mana
	increase_mana(-get_magic_power(m.spells, sn).mana_cost)
end

-- Get the level of a power
function get_level_power(s, max, min)
	if not max then max = 50 end
	if not min then min = 1 end

	return value_scale(s.get_current_level(), 50, max, min)
end

function magic_power_sucess(spell, fail_adj, fail_min, oups_fct)
	local chance
	local minfail = 0

	-- Spell failure chance
	chance = spell.fail

	-- Reduce failure rate by "effective" level adjustment
	chance = chance - 3 * (player.lev - spell.min_lev)

	-- Reduce failure rate by INT/WIS adjustment
	chance = chance - 3 * (fail_adj - 1)

	-- Not enough mana to cast
	if (spell.mana_cost > player.csp) then
		chance = chance + 5 * (spell.mana_cost - player.csp)
	end

	-- Extract the minimum failure rate
	minfail = fail_min

	-- Minimum failure rate
	if (chance < minfail) then chance = minfail end

	-- Always a 5 percent chance of working
	if (chance > 95) then chance = 95 end

	-- Failed spell
	if (rand_int(100) < chance) then
		if (flush_failure) then flush() end
		message("You failed to concentrate hard enough!")

		if not oups_fct then
			oups_fct(chance)
		end
		return false
	end
	return true
end

