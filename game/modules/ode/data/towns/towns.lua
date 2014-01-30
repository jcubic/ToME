-- Handles the loading of "town"

map.define("town.map")

-- Dungeon entrance
map.symbol{ symbol='>' feat=FEAT_MORE info=(CAVE_GLOW|CAVE_MARK) special=DUNGEON_ODE }

-- Floor
map.symbol{ symbol='.' feat=FEAT_FLOOR_ROOM info=(CAVE_GLOW|CAVE_MARK) }

-- Permanent wall
map.symbol{ symbol='-' feat=FEAT_WALL_LR info=(CAVE_GLOW|CAVE_MARK) }
map.symbol{ symbol='|' feat=FEAT_WALL_UD info=(CAVE_GLOW|CAVE_MARK) }


---------------- Town Layout ----------------
map.map
{
[[                      ]],
[[ ---------------------]],
[[ |...................|]],
[[ |...................|]],
[[ |...................|]],
[[ |...................|]],
[[ |.........>.........|]],
[[ |...................|]],
[[ |...................|]],
[[ |...................|]],
[[ ---------------------]],
}

---------------- Starting position ----------------
map.set_player(10, 10)
