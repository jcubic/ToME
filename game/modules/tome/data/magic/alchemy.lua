--
-- alchemy.lua
--
-- Implements the special skills of the alchemist class
--
-- So far just potion mixing.
--
-- TODO:
-- * make some recipes just explode or other cool things
-- * keep track of which recipes player has discovered
-- * don't display identified type in recipe browser if not aware
-- * add failure chance
-- * other alchemy stuff (sticks and so on)
-- * decide if show_page and find_item_kidx deserve promotion to util.lua
-- * fix potion stacking bug
-- * describe new item
-- * separate browse action


declare_global_constants {
	"alchemy_make_potion_recipes",
	"alchemy_mix_potions",
	"alchemy_actions",
	"alchemy_action_shortinfo",
	"alchemy_browse",
	"show_page",
	"find_item_kidx"
}

declare_globals {
	"alchemy_potion_recipes",
	"alchemy_sources",
	"alchemy_sources_rev",
	"alchemy_dests",
	"_alchemy_inven_sources"
}

new_speed("ALCHEMY","alchemy")

add_mkey
{
	["mkey"] 	= "Alchemy",
	["type"]    = "skill",
	["fct"] 	= function()
		term.save()

		-- first set up _alchemy_inven_sources
		_alchemy_inven_sources = {}
		for i=1,player.inventory[INVEN_INVEN][0] do
			local obj = player.inventory[INVEN_INVEN][i]
			if obj and obj.flags[FLAG_ALCHEMY_SRC] then
				_alchemy_inven_sources[obj.k_idx] = obj.number
			end
		end


		local act =
			choose_from_list(alchemy_actions, "Actions",
			"Perform which alchemical action?",
			alchemy_action_shortinfo)

		if act and (act["do"]) then act["do"]() end


		term.load()
	end,
}

_alchemy_inven_sources = {}

alchemy_actions = {
	{
		["name"] = "Browse recipes available from inventory",
		["do"] = function()
			local recipes = {}
			for k,v in _alchemy_inven_sources do
				for kk,vv in _alchemy_inven_sources do
					tinsert(recipes,{alchemy_sources_rev[k],alchemy_sources_rev[kk]})
				end
			end
			alchemy_browse(recipes)
		end,
	},
	{
		["name"] = "Browse all known recipes",
		["do"] = function()
			local recipes = {}
			for i=0,getn(alchemy_sources)*getn(alchemy_sources)-1 do
				local a = i/(getn(alchemy_sources))
				local b = i - (a*getn(alchemy_sources))
				if alchemy_potion_recipes[alchemy_sources[a+1]]
					[alchemy_sources[b+1]].known then
					tinsert(recipes,{a+1,b+1})
				end
			end

			alchemy_browse(recipes)
		end,
	},
}

function alchemy_action_shortinfo (y, x, a) if a then term.blank_print(a.name, y, x) end end

function alchemy_browse(recipes)
	local page_size=19
	local first = 1
	local done = false
	local do_recipe = nil

	while (not done) do
		show_page(first,page_size,recipes)
		local key = term.inkey()
		if key == ESCAPE then done = true end
		if key == strbyte(" ") then first = first + page_size end
		if key >= strbyte("a") and
			key <= strbyte("a")+page_size-1 then
			done = true
			do_recipe = key - strbyte("a")
		end
	end

	if do_recipe then
		local actual_recipe = recipes[first+do_recipe];
		alchemy_mix_potions(actual_recipe[1],
					actual_recipe[2])
	end
end

function show_page(first,page_size,recipes)
	term.load()
	term.save()

	local total = getn(recipes);
	term.blank_print("  Alchemy Menu ("..first.."--"..(first+page_size-1).." of "..total..")",0,5)
	term.blank_print("",1,0)

	local recipe = first;
	for i=1,page_size do
		if recipe <= total then
			local a = recipes[recipe][1]
			local b = recipes[recipe][2]

			local k_idxs = {
				["a"] = alchemy_sources[a],
				["b"] = alchemy_sources[b]
			}
			if (alchemy_potion_recipes[k_idxs.a][k_idxs.b].known) then
				k_idxs.c = alchemy_potion_recipes[k_idxs.a][k_idxs.b].k
			else
				k_idxs.c = -1
			end

			local nums = {
				["a"] = _alchemy_inven_sources[k_idxs.a] or 0,
				["b"] = _alchemy_inven_sources[k_idxs.b] or 0
			}

			if (a == b) then nums.c = nums.a/2
			else nums.c = min(nums.a,nums.b) end

			local cols = {["a"]="",["b"]="",["c"]="#W"}
			local numstrs = {["a"]="",["b"]="",["c"]=""}
			local names = {}

			for k,v in {"a","b","c"} do
				if nums[v] > 0 then
					cols[v] = "#B"
					numstrs[v] = "("..nums[v]..")"
				end
				if k_idxs[v] > -1 then
					if k_info(k_idxs[v]).aware then
						names[v] = k_info(k_idxs[v]).name
					else
						names[v] = flavour.get("colors",k_info(k_idxs[v]).sval)
					end
				else
					names[v] = "unknown"
					if nums[v] > 0 then cols[v] = "#R" end
				end
			end

			term.blank_print("  ",i+1,2);
			term.text_out(cols.c..strchar(96+i)..") "..
				cols.a..names.a..numstrs.a..cols.c..
				" + "..cols.b..names.b..numstrs.b..cols.c..
				" = "..names.c..numstrs.c);
		else
		end
		recipe = recipe + 1;
	end
end

--
-- Ideas:
--
-- Browse by ingredients. Browse by product.
--
-- Certain percentage are bad recipes. Or other random effects? Damage monsters?
--

alchemy_potion_recipes = {}
alchemy_dests = {}
alchemy_sources = {}
alchemy_sources_rev = {}

add_loadsave("alchemy_potion_recipes", {})
add_loadsave("alchemy_sources", {})
add_loadsave("alchemy_sources_rev", {})
add_loadsave("alchemy_dests", {})



hook {
	-- Note the 'thaum_spells = {}' to zero the array if it has been
	-- loaded from the save file of a (dead) ancestor
	[hook.INIT_DONE] = function ()
		alchemy_potion_recipes = {}
		alchemy_dests = {}
		alchemy_sources = {}
		alchemy_sources_rev = {}
		alchemy_make_potion_recipes()
	end,
}

function alchemy_make_potion_recipes()
	local k_idx
	for k_idx = 1, max_k_idx-1 do
		if (flag_get(k_info(k_idx).flags,FLAG_ALCHEMY_SRC) == 1) then
--			message("Alchemy Source : "..k_info(k_idx).name);
			tinsert(alchemy_sources,k_idx);
			alchemy_sources_rev[k_idx] = getn(alchemy_sources)
		end
		if (flag_get(k_info(k_idx).flags,FLAG_ALCHEMY_DEST) == 1) then
--			message("Alchemy Dest : "..k_info(k_idx).name);
			tinsert(alchemy_dests,k_idx)
		end
	end

	local i,j
	for i=1,getn(alchemy_sources) do
		local asi = alchemy_sources[i]
		alchemy_potion_recipes[asi] = {}
		for j=1,getn(alchemy_sources) do
			local asj = alchemy_sources[j]
			-- todo: weight this so that:
			-- low-level potions tend to make low level potions
			-- some combinations just explode or do other weird stuff
			if asi <= asj then
				alchemy_potion_recipes[asi][asj] =
				{["k"]=alchemy_dests[rng(getn(alchemy_dests))]}
			else
				alchemy_potion_recipes[asi][asj] =
					alchemy_potion_recipes[asj][asi]
			end
		end
	end
end

function alchemy_mix_potions(a,b)
	local asa = alchemy_sources[a]
	local asb = alchemy_sources[b]

	local itema = find_item_kidx(asa);
	local itemb = find_item_kidx(asb);

	if not (itema and itemb) then
		message("Alchemy attempted when you don't have item! (BUG!)")
		return
	end

	if a == b and player.inventory[INVEN_INVEN][itema].number < 2 then
		message("Alchemy attempt when you don't have enough of item! (BUG!)")
		return
	end

	-- recipe is now known
	alchemy_potion_recipes[asa][asb].known = 1
	alchemy_potion_recipes[asb][asa].known = 1

	-- make a new object
	local obj = new_object()
	object_prep(obj, alchemy_potion_recipes[asa][asb].k)
	obj.number = 1


	-- Use up the potions
	item_increase(itema, -1)
	item_describe(itema)
	item_increase(itemb, -1)
	item_describe(itemb)
	-- 'optimize' the second once first
	if (itema > itemb) then
		item_optimize(itema)
		item_optimize(itemb)
	else
		item_optimize(itemb)
		item_optimize(itema)
	end

	-- Get the new potion
	local itemc = inven_carry(obj, true)
	item_describe(itemc)

	-- Notice new item (and lack of old item)
	player.notice = player.notice | PN_COMBINE | PN_REORDER

	-- Window stuff
	player.window[FLAG_PW_INVEN] = true
	player.window[FLAG_PW_EQUIP] = true
	player.window[FLAG_PW_PLAYER] = true

	-- use some energy
	energy_use = get_player_energy(SPEED_ALCHEMY)
end

-- given a k_idx, find the item in the inventory

function find_item_kidx(kidx)
	for i=1,player.inventory[INVEN_INVEN][0] do
		local obj = player.inventory[INVEN_INVEN][i]
		if obj and obj.k_idx == kidx then
			return i
		end
	end
	return nil
end

