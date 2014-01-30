-- Handles the loading of towns

do

-- Add towns in here
local towns =
{
	[1] = { "towns/bree.map", 	"towns/d_bree.map" }
	[2] = { "towns/hobbiton.map",	"towns/hobbiton.map" }
	[4] = { "towns/lothlorien.map", "towns/d_lothlorien.map" }
	[5] = { "towns/rivendell.map", 	"towns/d_rivendell.map" }
}

assert(towns[player.town_num], "unknown town "..player.town_num)

-- Selects the correct map based on destroyed state
if town(player.town_num).destroyed then
	map.import(towns[player.town_num][2])
else
	map.import(towns[player.town_num][1])
end

end
