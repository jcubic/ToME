--
-- The auto wield starting objects subsystem
--
-- Load it with: load_subsystem("birth_auto_wield")
--
-- Make sure the inventory subsystem is already loaded, or an equivalent
-- inventory.wield(item, silent) function is available.

-------------
-- Auto wield
-------------
hook(get_subsystem_param("birth_auto_wield", "hook") or hook.INIT_DONE, function()
	local items = { list = { } }
	for_inventory(player, INVEN_INVEN, INVEN_PACK, function(obj, i, j, s)
		tinsert(%items.list, 1, s)
	end)
	if has_subsystem("combat_weapon_default") then combat.default_weapon.bypass_dualwield_check = true end
	for item in items.list do
		inventory.wield(item, true)
	end
	if has_subsystem("combat_weapon_default") then combat.default_weapon.bypass_dualwield_check = false end
end)
