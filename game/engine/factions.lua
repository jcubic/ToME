-- Engine factions functions

factions.__next = 100
function factions.new(name, desc)
	globals()['FACTION_'..name] = factions.__next
	factions.faction_names[factions.__next] = desc
	factions.__next = factions.__next + 1
end

player.faction = FACTION_PLAYER
