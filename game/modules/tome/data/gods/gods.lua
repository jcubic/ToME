hook.new_hook_type("VALARIN_FAVOR")

-- Utility
function god.wisdom_scale(max)
	return (player.stat(A_WIS) * max) / stats.MAX_VALUE
end

--
-- Define ToME gods
--

god.ERU = god.new
{
	name = "Eru Iluvatar"
	desc =
	{
		"He is the supreme god, he created the world, and most of its inhabitants.",
	}
	hooks =
	{
		[hook.VALARIN_FAVOR] = function(level)
			if player.god(god.ERU) then
				--player.mhp(9999)
			end
		end
                [hook.PLAYER_USED_ENERGY] = function()
                	god.inc_piety(god.ERU, god.wisdom_scale(10))
                end
	}
}

god.MANWE = god.new
{
	name = "Manwe Sulimo"
	desc =
	{
		"He is the king of the Valar, most powerful of them after Melkor.",
	}
	hooks =
	{
	}
}

god.TULKAS = god.new
{
	name = "Tulkas Astaldo"
	desc =
	{
		"He is the last of the Valar that came to the world, and the fiercest fighter.",
	}
	hooks =
	{
	}
}

god.MELKOR = god.new
{
	name = "Melkor Bauglir"
	desc =
	{
		"He is the most powerful of the Valar. He became corrupted and he's now ",
		"the greatest threat of Arda, he is also known as Morgoth, the dark enemy.",
	}
	hooks =
	{
		[hook.SACRIFICE_GOD] = function()
			if player.pgod == god.MELKOR then
				-- One can sacrifice some HP for piety or damage
				if (player.mhp() > 10) and (player.chp() > 10) and get_check("Do you want to sacrifice a part of yourself? ") then
					-- 10 HP = 300 * wis piety
					if get_check("Do you want to sacrifice for more piety instead of damage? ") then
						local x = wisdom_scale(6)
						if x < 1 then x = 1 end

						player.hp_mod = player.hp_mod - 10
						take_hit(10, "self sacrifice to Melkor")
						msg_print("Your life slips away, Melkor seems happier.")
						god.inc_piety(god.MELKOR, x * 300)
						player.update = player.update | PU_HP;
					-- 10 HP = +wis damage
					else
						take_hit(10, "self sacrifice to Melkor")
						msg_print("Your life slips away, your arms grow stronger.")
						player.melkor_sacrifice = player.melkor_sacrifice + 1
						player.update = player.update | (PU_BONUS | PU_HP);
					end
				else
					-- Get an item
					local ret, item = get_item("Sacrifice which item? ", "You have nothing to sacrifice.", USE_INVEN,
						function(obj)
							return obj.tval == TV_CORPSE and
								obj.sval == SV_CORPSE_CORPSE and
								obj.flags[FLAG_MONSTER_OBJ]
						end)
					if not ret then return end
					local o_ptr = get_object(item)
					local m_ptr = o_ptr.flags[FLAG_MONSTER_OBJ]
					local race  = m_ptr.sr_ptr or r_info(m_ptr.r_idx)

					-- Piety for corpses is based on monster level
					god.inc_piety(god.MELKOR, 2 * race.level)

					-- Remove the item
					item_increase(item, -1)
					item_describe(item)
					item_optimize(item)
				end
			end
		end
	}
}

god.YAVANNA = god.new
{
	name = "Yavanna Kementari"
	desc =
	{
		"She is the Vala of nature, protectress of the great forests of ",
		"Middle-earth.",
	}
	hooks =
	{
		[hook.PLAYER_CAN_ENTER] = function(feat, pass_wall, only_wall)
			if feat == FEAT_TREES then
				if player.grace() >= 9000 and player.praying and player.pgod == god.YAVANNA then
					return true, true
				end
			end
		end
	}
}

god.ULMO = god.new
{
	name = "Ulmo",
	desc =
	{
		"Ulmo is called Lord of Waters, he rules all that is water on Arda."
	},
	hooks =
	{
	},
}
