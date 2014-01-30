-- Sentient object system

constant("sentient", {})
settag(sentient, TAG_NAMESPACE) -- Tag as a namespace

sentient.subsystems = {}
sentient.tvals      = {}

sentient.grant_when = {}
sentient.grant_when.ALWAYS          = 0  -- 0x00
sentient.grant_when.OBJ_NOT_HAVE    = 1  -- 0x01
sentient.grant_when.PLAYER_NOT_HAVE = 2  -- 0x02
sentient.grant_when.OBJ_HAS_LESS    = 4  -- 0x04
sentient.grant_when.PLAYER_HAS_LESS = 8  -- 0x08
sentient.grant_when.IS_ART          = 16 -- 0x10
sentient.grant_when.NOT_ART         = 32 -- 0x20

sentient.grant_type = {}
sentient.grant_type.SET  = 1
sentient.grant_type.ADD  = 2
sentient.grant_type.FUNC = 3

new_flag("SENTIENT")
safe_new_flag("POINTS")
safe_new_flag("REALM")

-- We take care of the leveling for sentient items, rather than
-- the LEVELS flag.  But the LEVELS flag still must exist for this
-- hook to be called.
hook(hook.OBJECT_LEVEL_DO,
function(obj)
	if obj.flags[FLAG_SENTIENT] then
		sentient.do_gain_level(obj)
		return true
	end

	return false
end)

function sentient.do_gain_level(obj)
	if obj.exp < 1 then
		message(color.VIOLET, "ERROR: " .. object_desc(obj, 0) ..
				" has no experience")
		return
	end

	sentient.check_init(obj)

	local subsys = sentient.get_subsys(obj)

	if not subsys then
		message(color.VIOLET, "ERROR: No sentient subsys registered " ..
				"for tval " .. tval_to_str(obj.tval))
		return
	end

	local flags  = obj.flags[FLAG_SENTIENT]

	local subsys_name = subsys.name
	local gains       = subsys.gains

	if type(gains) == "function" then
		gains(obj)
		return
	end

	local total_chance = 0
	local alloc_array  = {}

	for i = 1, getn(gains) do
		total_chance = total_chance + gains[i].chance
		tinsert(alloc_array, total_chance)
	end

	local chance = rng(total_chance) - 1
	local gain

	for i = 1, getn(alloc_array) do
		if chance < alloc_array[i] then
			gain = gains[i]
			break
		end
	end

	obj.to_h = obj.to_h + (gain.to_h or 0)
	obj.to_d = obj.to_d + (gain.to_d or 0)
	obj.to_a = obj.to_a + (gain.to_a or 0)

	if gain.points then
		flags[FLAG_POINTS] = flags[FLAG_POINTS] + gain.points
		wiz_print("#ySentient object now has " ..
				  flags[FLAG_POINTS] .. " point(s).#w")
	end

	if gain.realm_chance and rng.percent(gain.realm_chance) then
		sentient.buy_random_realm(obj)
	end

	if gain.power_chance and rng.percent(gain.power_chance) then
		local realms = sentient.get_realms_by_name(obj)

		if getn(realms) == 0 then
			sentient.buy_random_realm(obj)
		end

		sentient.gain_random_power(obj)
	end

	if gain.pval_chance and rng.percent(gain.pval_chance) then
		sentient.increase_pval(obj, subsys)
	end
end -- do_gain_level()

function sentient.buy_random_realm(obj)
	wiz_print("Attempt to buy realm.")

	local flags  = obj.flags[FLAG_SENTIENT]
	local realms = flags[FLAG_REALM]
	local points = flags[FLAG_POINTS]
	local subsys = sentient.get_subsys(obj)

	if subsys.num_realms == 0 then
		message(color.VIOLET, "ERROR: subsystem '" .. subsys.name ..
				" has no realms!")
		return
	end

	local avail_realms = {}

	for name, realm in subsys.realms do
		avail_realms[name] = realm.cost
	end

	-- Narrow list of available realms
	for name, cost in avail_realms do
		if cost > points then
			avail_realms[name] = nil
		end

		-- We already have that realm, or its been excluded for this
		-- object
		if realms[subsys.realms[name].index] then
			avail_realms[name] = nil
		end
	end

	-- List has been narrowed
	local realm_names = {}
	for name, cost in avail_realms do
		tinsert(realm_names, name)
	end

	if getn(realm_names) == 0 then
		wiz_print("#RNo realms avialable for buying.#w")
		return
	end

	wiz_print(getn(realm_names) .. " realms to buy from.")

	-- Choose realm
	local realm_name = realm_names[rng(getn(realm_names))]

	-- Remember that we have this realm
	if sentient.acquire_realm(obj, realm_name) then
		-- Consume points
		flags[FLAG_POINTS] = flags[FLAG_POINTS] - avail_realms[realm_name]

		if is_known(obj) then
			local color = subsys.realms[realm_name].color

			message(strcap(object_desc(obj)) .. " has gained access to the "
					.. color .. realm_name .. "#w realm!")
		end
	end
end -- sentient.buy_realm()

function sentient.acquire_realm(obj, realm_name)
	local subsys = sentient.get_subsys(obj)

	if not subsys.realms[realm_name] then
		message(color.VIOLET, "ERROR: subsystem '" .. subsys.name ..
				"' has no realm '" .. realm_name .. "'")
		return false
	end

	sentient.check_init(obj)

	local realms = obj.flags[FLAG_SENTIENT][FLAG_REALM]
	realms[subsys.realms[realm_name].index] = flag_new()

	return true
end

function sentient.gain_random_power(obj)
	wiz_print("Attempting to gain a random power.")

	local realms = sentient.get_realms_by_name(obj)

	if getn(realms) == 0 then
		wiz_print("#RNo realms from which to gain a power.#w")
		return
	end

	while getn(realms) > 0 do
		local arr_idx = rng(getn(realms))
		local realm   = realms[arr_idx]

		if sentient.gain_random_power_from_realm(obj, realm) then
			return
		end

		-- That realm had nothing, remove it from the list and
		-- select again at random
		tremove(realms, arr_idx)
	end

	wiz_print("#RNo new powers to gain from any of the available realms.#w")
end -- sentient.gain_random_power(obj)

function sentient.gain_random_power_from_realm(obj, realm_name)
	wiz_print("Attempting to gain random power from realm " ..
			  realm_name .. ".")

	local subsys = sentient.get_subsys(obj)
	local realm  = subsys.realms[realm_name]
	local powers = obj.flags[FLAG_SENTIENT][FLAG_REALM][realm.index]

	local avail_powers = {}

	if getn(realm.powers) == 0 then
		message(color.VIOLET, "Realm '" .. realm_name .. "' of subsys '" ..
				subsys.name .. "' has no powers!")
		return false
	end

	for i = 1, getn(realm.powers) do
		if powers[i] then
			-- Already have that power
		elseif sentient.can_grant_power(obj, realm, realm.powers[i]) then
			tinsert(avail_powers, i)
		end
	end

	if getn(avail_powers) == 0 then
		wiz_print("#RNo powers available from realm " .. realm_name .. ".#w")
		return false
	end

	-- Select power and grant it
	local power_idx  = avail_powers[rng(getn(avail_powers))]
	local power      = realm.powers[power_idx]
	local power_name = power.name

	wiz_print("#yGaining power " .. power_name .. ".#w")

	-- Remember that we have it
	powers[power_idx] = true

	if power.grant_type == sentient.grant_type.SET then
		flag_add(obj.flags, power.grant)
	elseif power.grant_type == sentient.grant_type.ADD then
		flag_add_increase(obj.flags, power.grant)
	elseif power.grant_type == sentient.grant_type.FUNC then
		power.grant(obj, realm_name)
	else
		message(color.VIOLET, "ERROR: power " .. power_name .. " of realm " ..
				realm_name .. "of subsys " .. subsys.name ..
					" has unkown grant type!")
		return false
	end

	if power.msg then
		message(power.msg)
	end

	return true
end -- sentient.gain_random_power_from_realm()

function sentient.can_grant_power(obj, realm, power)
	local when = power.grant_when

	if tag(when) == TAG_FUNCTION then
		return when(obj, realm, power)
	end

	if when == sentient.grant_when.ALWAYS then
		return true
	end

	if tag(power.grant) == TAG_FUNCTION then
		message(color.VIOLET, "ERROR: A sentient power which is granted "
				.. " through a Lua function must either have its " ..
				"'grant_when' be a function or ALWAYS")
		return false
	end

	if tag(power.grant) ~= TAG_FLAG then
		message(color.VIOLET, "ERROR: Don't know how to grant a power " ..
				" of type " .. type(power.grant))
		return false
	end

	if (when & sentient.grant_when.OBJ_NOT_HAVE) ~= 0 then
		if flag_contains(obj.flags, power.grant) then
			wiz_print("#RNot granting power '" .. power.name .. "': " ..
					  "object already has it.#w")
			return false
		end
	end

	if (when & sentient.grant_when.PLAYER_NOT_HAVE) ~= 0 then
		if flag_contains(player.intrinsic_flags, power.grant) then
			wiz_print("#RNot granting power '" .. power.name .. "': " ..
					  "player already has it.#w")
			return false
		end
	end

	if (when & sentient.grant_when.OBJ_HAS_LESS) ~= 0 then
		if not sentient.flagset_is_less(obj.flags, power.grant) then
			wiz_print("#RNot granting power '" .. power.name .. "': " ..
					  "object already has at least that amount.#w")
			return false
		end
	end

	if (when & sentient.grant_when.PLAYER_HAS_LESS) ~= 0 then
		if not sentient.flagset_is_less(player.intrinsic_flags,
										power.grant) then
			wiz_print("#RNot granting power '" .. power.name .. "': " ..
					  "player already has at least that amount.#w")
			return false
		end
	end

	if (when & sentient.grant_when.IS_ART) ~= 0 then
		if not is_artifact(obj) then
			wiz_print("#RNot granting power '" .. power.name .. "': " ..
					  "object isn't an artifact.#w")
			return false
		end
	end
	
	if (when & sentient.grant_when.NOT_ART) ~= 0 then
		if is_artifact(obj) then
			wiz_print("#RNot granting power '" .. power.name .. "': " ..
					  "object is an artifact.#w")
			return false
		end
	end

	return true
end -- sentient.can_grant_power()

function sentient.increase_pval(obj, subsys)
	print("Inc pval!")
end

-------------------------------------------------------------------------
-- Helper functions
-------------------------------------------------------------------------
function sentient.flagset_is_less(lesser, greater)
	for i = 1, flag_max_key(greater) do
		if greater[i] and lesser[i] then
			if tag(lesser[i]) == TAG_FLAG then
				if not sentient.flagset_is_less(lesser[i], greater[i]) then
					return false
				end
			elseif type(lesser[i] == "number") then
				if lesser[i] >= greater[i] then
					return false
				end
			end
		end
	end

	return true
end -- sentient.flagset_is_less()

function sentient.check_init(obj)
	assert(obj.flags[FLAG_SENTIENT],
		   object_desc(obj, 0) .. " doesn't have SENTIENT flag defined.")
	assert(obj.flags[FLAG_LEVELS],
		   object_desc(obj, 0) .. " doesn't have LEVELS flag defined.")

	local subsys = 'melee'
	if tag(obj.flags[FLAG_SENTIENT]) ~= TAG_FLAG then
		-- Have to restore known status, since changing a flag's
		-- value un-learns it.
		subsys = obj.flags[FLAG_SENTIENT]
		local known = flag_is_known(obj.flags, FLAG_SENTIENT)
		obj.flags[FLAG_SENTIENT] = flag_new()
		flag_learn(obj.flags, FLAG_SENTIENT, known)
	end

	if not obj.flags[FLAG_SENTIENT][FLAG_POINTS] then
		obj.flags[FLAG_SENTIENT][FLAG_POINTS] = 0
	end

	if not obj.flags[FLAG_SENTIENT][FLAG_REALM] then
		obj.flags[FLAG_SENTIENT][FLAG_REALM] = flag_new()
	end

	if not obj.flags[FLAG_SENTIENT][FLAG_SENTIENT] then
		obj.flags[FLAG_SENTIENT][FLAG_SENTIENT] = subsys
	end
end -- sentient.check_init()

function sentient.get_subsys(obj)
	local subsys_name = obj.flags[FLAG_SENTIENT][FLAG_SENTIENT]

	if not subsys_name or not sentient.subsystems[subsys_name] then
		message(color.VIOLET, "ERROR: No sentient subsys registered " ..
				"for name " .. tostring(subsys_name))
	end

	return sentient.subsystems[subsys_name]
end -- sentient.get_subsys()

function sentient.get_realms_by_name(obj)
	local subsys = sentient.get_subsys(obj)
	local realms = obj.flags[FLAG_SENTIENT][FLAG_REALM]
	local list   = {}

	for i = 1, flag_max_key(realms) do
		if tag(realms[i]) == TAG_FLAG then
			tinsert(list, subsys.realms_by_index[i].name)
		end
	end

	return list
end -- sentient.get_realms_by_name()

-------------------------------------------------------------------------
-- Functions for registering subsystems and realms
-------------------------------------------------------------------------
function sentient.add_subsystem(t)
	assert(t.name, "New sentient subsystem has no name.")
	assert(not sentient.subsystems[t.name],
		   "Sentient subsystem '" .. t.name .. "' already exists")
	assert(t.tvals, "Sentient subsystem '" .. t.name .. "' has " ..
		   "no tvals.")
	assert(t.gains, "Sentient subsystem '" .. t.name .. "' has " ..
		   "no gains.")

	sentient.subsystems[t.name] = t

	local tvals = t.tvals
	t.tvals = {}
	for i = 1, getn(tvals) do
		local tval = tvals[i]

		assert(not sentient.tvals[tval],
			   "Sentient subsystem '" .. (sentient.tvals[tval] or "") ..
				   "' already registed for tval " .. tval_to_str(tval))
			
		sentient.tvals[tval] = t.name
		t.tvals[tval]        = true
	end

	t.num_realms      = 0
	t.realms_by_index = {}

	t.realms = t.realms or {}

	local realms = t.realms
	t.realms = {}
	for i = 1, getn(realms) do
		realms[i].subsys = t.name
		sentient.add_realm(realms[i])
	end
end -- sentient.add_subsystem()

function sentient.add_realm(t)
	local subsys_name = t.subsys

	assert(t.name, "New realm has no name.")
	assert(subsys_name, "Realm '" .. t.name .. "' has no subsystem.")
	assert(sentient.subsystems[subsys_name],
		   "No such subsystem as '" .. subsys_name .. "'")

	local subsys = sentient.subsystems[subsys_name]

	assert(not subsys.realms[t.name],
		   "Subsystem '" .. subsys_name .. "' already has a realm named " ..
			   "'" .. t.name .. "'")

	assert(t.cost, "Realm '" .. t.name .. "' for subsystem '" ..
		   subsys_name .. "' has no cost.")

	subsys.num_realms = subsys.num_realms + 1
	t.index           = subsys.num_realms

	subsys.realms[t.name]                     = t
	subsys.realms_by_index[subsys.num_realms] = t

	t.color = t.color or "#w"

	t.powers = t.powers or {}

	local powers = t.powers
	t.powers = {}
	for i = 1, getn(powers) do
		powers[i].realm  = t.name
		powers[i].subsys = subsys_name
		sentient.add_power(powers[i])
	end
end -- sentient.add_realm()

function sentient.add_power(t)
	local subsys_name = t.subsys
	assert(sentient.subsystems[subsys_name],
		   "No such subsystem as '" .. subsys_name .. "'")

	local subsys      = sentient.subsystems[subsys_name]
	local realm_name  = t.realm

	assert(subsys.realms[realm_name], "Subsystem '" .. subsys_name ..
		   "' has no realm '" .. realm_name .. "'")

	local str = "Power for subsystem '" .. subsys_name .. "', realm '" ..
		realm_name .. "' "

	assert(t.grant, str .. " has no grant")
	assert(t.grant_when, str .. " has no grant_when")
	assert(t.grant_type, str .. " has no grant_type")

	if type(t.grant) == "table" then
		t.grant = getter.flags(t.grant)
	end

	local realm = subsys.realms[realm_name]

	tinsert(realm.powers, t)

	if not t.name then
		t.name = getn(realm.powers)
	end
end -- sentient.add_power()

-------------------------------------------------------------------------
-- Describe sentient objects.
-------------------------------------------------------------------------
function desc_obj.desc_sentient(obj, trim_down)
	if not known_flag(obj, FLAG_SENTIENT) then
		return
	end

	sentient.check_init(obj)

	desc_obj.text_out("It is sentient. ")

	local realms = sentient.get_realms_by_name(obj)

	if getn(realms) == 0 then
		return
	end

	local subsys = sentient.get_subsys(obj)

	desc_obj.text_out("It has access to the ")
	for i = 1, getn(realms) do
		local realm = realms[i]
		desc_obj.text_out(subsys.realms[realm].color .. realm .. "#w")

		if i < (getn(realms) - 1) then
			desc_obj.text_out(", ")
		elseif i < getn(realms) then
			desc_obj.text_out(" and ")
		end
	end

	if getn(realms) == 1 then
		desc_obj.text_out(" realm. ")
	else
		desc_obj.text_out(" realms. ")
	end
end

-------------------------------------------------------------------------
-- Meele subsystem and realms
-------------------------------------------------------------------------
sentient.add_subsystem{
	name = "melee"
	gains = {
		{chance = 1, to_h = 1, to_d = 1},
		{chance = 1, to_h = 2, to_d = 1},
		{chance = 2, to_h = 1, points = 1, realm_chance = 40},
		{chance = 2, pval_chance = 100, power_chance = 100}
	}
	tvals = {TV_POLEARM, TV_AXE, TV_HAFTED, TV_KNIFE, TV_SWORD}
}

-- Fire
sentient.add_realm{
	subsys = "melee"
	name   = "fire"

	cost = 1

	color = "#r"

	powers = {
		{
			name  = "Add to light",
			msg   = "Your weapon glows brightly!",
			grant = {LITE = 1},
			grant_when = sentient.grant_when.ALWAYS
			grant_type = sentient.grant_type.ADD
		},

		{
			name  = "Fire brand",
			msg   = "Flames surround your weapon!",
			grant = {BRAND = getter.array{[dam.FIRE] = 3}}
			grant_when = sentient.grant_when.OBJ_HAS_LESS
			grant_type = sentient.grant_type.SET
		}

		{
			name  = "Fire resistance",
			grant = {RESIST = getter.resists{FIRE = 66}}
			grant_when = (sentient.grant_when.OBJ_HAS_LESS |
						  sentient.grant_when.PLAYER_HAS_LESS)
			grant_type = sentient.grant_type.SET
		}

		{
			name  = "Ignore fire",
			grant = {IGNORE = getter.resists{FIRE = true}}
			grant_when = (sentient.grant_when.OBJ_NOT_HAVE |
						  sentient.grant_when.NOT_ART)
			grant_type = sentient.grant_type.SET
		}
	} -- powers
} -- Fire

-- Cold
sentient.add_realm{
	subsys = "melee"
	name   = "cold"

	cost = 1

	color = "#B"

	powers = {
		{
			name  = "Invisibility",
			msg   = "You feel more hidden!",
			grant = {INVISIBLE = 30},
			grant_when = sentient.grant_when.ALWAYS
			grant_type = sentient.grant_type.ADD
		},

		{
			name  = "Cold brand",
			msg   = "Your weapon is covered in glittering frost!",
			grant = {BRAND = getter.array{[dam.COLD] = 3}}
			grant_when = sentient.grant_when.OBJ_HAS_LESS
			grant_type = sentient.grant_type.SET
		}

		{
			name  = "Cold resisance",
			grant = {RESIST = getter.resists{COLD = 66}}
			grant_when = (sentient.grant_when.OBJ_HAS_LESS |
						  sentient.grant_when.PLAYER_HAS_LESS)
			grant_type = sentient.grant_type.SET
		}

		{
			name  = "Ignore cold",
			grant = {IGNORE = getter.resists{COLD = true}}
			grant_when = (sentient.grant_when.OBJ_NOT_HAVE |
						  sentient.grant_when.NOT_ART)
			grant_type = sentient.grant_type.SET
		}
	} -- powers
} -- Cold
