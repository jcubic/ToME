dam.PURE = dam.add
{
	gfx	= 0,
	desc	= "damage",
	color	= { color.SLATE },
}
dam.MELEE = dam.add
{
	gfx	= 0,
	desc	= "melee",
	color	= { color.SLATE },
}

dam.FIRE = dam.add
{
	gfx	= 0,
	desc	= "fire",
	color	= function() return iif(rng(6)<4,color.yellow, iif(rng(4)==1,color.RED, color.LIGHT_RED)) end,
	player	= function(state)
		return { fuzzy="You are hit by fire!" }
	end,
}

dam.COLD = dam.add
{
	color	= function() return iif(rng(6)<4,color.WHITE,color.LIGHT_WHITE) end,
	gfx	= 1,
	desc	= "cold",
	player	= function(state)
		return { fuzzy="You are hit by cold!" }
	end,
}
