-- Floor/walls/... features definitions

-- Dungeons of doom
new_feature_type
{
	define_as = "FEAT_FLOOR_ROOM"
	name = "open floor"
	display = '.' color = color.WHITE
	priority = 5
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_FLOOR_TUNNEL"
	name = "open floor"
	display = '#' color = color.WHITE
	priority = 5
	flags =
	{
		FLOOR=true DONT_NOTICE_RUNNING=true CAN_RUN=true
	}
}
new_feature_type
{
	define_as = "FEAT_WALL_LR"
	name = "granite wall"
	display = '-' color = color.WHITE
	flags =
	{
		WALL=true NO_WALK=true CAN_PASS=true NO_VISION=true SUPPORT_LIGHT=true
		DONT_NOTICE_RUNNING=true PERMANENT=true
	}
}
new_feature_type
{
	define_as = "FEAT_WALL_UD"
	name = "granite wall"
	display = '|' color = color.WHITE
	flags =
	{
		WALL=true NO_WALK=true CAN_PASS=true NO_VISION=true SUPPORT_LIGHT=true
		DONT_NOTICE_RUNNING=true PERMANENT=true
	}
}
new_feature_type
{
	define_as = "FEAT_WALL"
	name = "granite wall"
	display = ' ' color = color.WHITE
	flags =
	{
		WALL=true NO_WALK=true CAN_PASS=true NO_VISION=true SUPPORT_LIGHT=true
		DONT_NOTICE_RUNNING=true PERMANENT=true
	}
}
