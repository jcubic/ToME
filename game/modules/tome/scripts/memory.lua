-- Monster memory stuff

memory.entrydesc_add("death", memory.death) -- death and vengeance
memory.entrydesc_add("move", memory.move) -- movement
memory.entrydesc_add("exp", memory.exp) -- experience and type
--memory.entrydesc_add("", memory.aura)
memory.entrydesc_add("friends", memory.friends) -- escort and such
memory.entrydesc_add("spells", memory.spells)
memory.entrydesc_add("resists", memory.resists)
memory.entrydesc_add("toughness", memory.toughness) -- armor and hp
memory.entrydesc_add("misc", memory.misc)
memory.entrydesc_add("alertness", memory.alertness)
memory.entrydesc_add("drops", memory.drops)
memory.entrydesc_add("blows", memory.blows)

memory.flag_align_add {
	FLAG_ELDRITCH_HORROR,
	FLAG_ANIMAL,
	FLAG_EVIL,
	FLAG_GOOD,
	FLAG_UNDEAD,
}

memory.flag_race_add {
	FLAG_GIANT,
	FLAG_DEMON,
	FLAG_TROLL,
	FLAG_ORC,
	FLAG_SPIDER,
	FLAG_DRAGON,
	FLAG_THUNDERLORD,
	FLAG_NAZGUL,
}

memory.flagdesc_add{
	{FLAG_ELDRITCH_HORROR,"sanity blasting"},
	{FLAG_ANIMAL,"natural"},
}
