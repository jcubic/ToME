--
-- Handle calculations of vital stats
--

--
-- Calculate the players (maximal) hit points
-- Adjust current hitpoints if necessary
--
function player.calc_hitpoints()
	local mhp

	-- Calculate hitpoints
	-- 12 at first + 1d10 + 2*(CON - 5)
	mhp = player_hp[player.lev] + 2 + 2 * (player.stat(A_CON) - 5)

	-- New maximum hitpoints
	if player.mhp() ~= mhp then
		-- Enforce maximum
		if player.chp() >= mhp then
			player.chp(mhp)
		end

		-- Save the new max-hitpoints
		player.mhp(mhp)

		-- Display hitpoints (later)
		player.redraw[FLAG_PR_HP] = true

		-- Window stuff
		player.window[FLAG_PW_PLAYER] = true
	end
end

--
-- Calculate maximum mana.  You do not need to know any spells.
-- Note that mana is lowered by heavy (or inappropriate) armor.
--
function player.calc_mana()
	-- no mana
end


--
-- Calculate the player's sanity
--
function player.calc_sanity()
	-- no sanity
end

--
-- Computes current weight limit.
--
function player.weight_limit()
	local i = 0

	-- Weight limit based only on strength
	i = player.adj_str_wgt[player.stat(A_STR)] * 100

	-- Sanity check
	if i < 10 then i = 10 end

	-- Return the result
	return i
end

--
-- Return a price factor(in percents) for buying/selling stuff
--
function player.calc_price_factor()
	return 100
end


--
-- Apply those flags to the player
--
function player.apply_flags(flags, src)
	src = src or {}

	-- Affect stats
	if flag_exists(flags, FLAG_STATS) then
		flag_add_increase(player.stat_add, flags[FLAG_STATS])
	end

	-- Sustain stats
	if flag_exists(flags, FLAG_SUST_STATS) then
		flag_add_increase(player.intrinsic_flags[FLAG_SUST_STATS], flags[FLAG_SUST_STATS])
	end
end


--
-- Compute the player "state" from various data
--
function player.calc_bonuses(silent)
	-- Call the "normal" initialisation
	player.basic_calc_bonuses(silent)

	-- Set HP regen
	player.inc_intrinsic(FLAG_REGEN_LIFE, (player.mhp() * 1000) / 200)

	-- Handles the defense skill
	player.to_a = player.to_a + get_skill(SKILL_DEFENSE)
end




---------------------------- Various usefull stats tables -----------------------
-- Stat Table (STR) -- weight limit in deca-pounds
player.adj_str_wgt =
{
	2,
	5,
	10,
	15,
	20,
	30,
	32,
	33,
	35,
	35,
	37,
	39,
	42,
	44,
}
-- Stat Table (STR) -- bonus to dam
player.adj_str_td =
{
	-5,

	-3,
	-2,
	-1,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
}
