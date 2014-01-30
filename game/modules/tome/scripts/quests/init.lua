--
-- Load all quests
--

-- Utiliy functions for quests
tome_dofile("quests/util.lua")


-- The main plot
tome_dofile("quests/shards_of_narsil.lua")
tome_dofile("quests/isengard_investigate.lua")


tome_dofile("quests/necromancer.lua")
tome_dofile("quests/one_ring.lua")
tome_dofile("quests/sauron.lua")


-- Bree quests
tome_dofile("quests/thieves.lua")
tome_dofile("quests/king_of_barrows.lua")

-- The beastmaster bounty quest
tome_dofile("quests/bounty.lua")

tome_dofile("quests/khazad.lua")		-- Khazad-dum quests
tome_dofile("quests/goblin.lua")		-- The Great Goblin quest (placeholder)