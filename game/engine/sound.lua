-----------------------------------------------------------
------------------ Sound/music system ---------------------
-----------------------------------------------------------

sound.__musics = {}
sound.__samples = {}

--- @var
-- @brief Modules are welcome to change order/edit/whatever
sound.music_formats_order =
{
	".it"
	".mod"
	".s3m"
	".mid"
	".ogg"
}

--- @var
-- @brief Modules are welcome to change order/edit/whatever
sound.sound_formats_order =
{
	".ogg"
	".wav"
}

--- @fn
-- @brief Loads a music from an ogg file in /media/music/
function sound.load_music(name, name_as)
	for i = 1, getn(sound.music_formats_order) do
		if fs.file_exist("/media/music/"..name..sound.music_formats_order[i]) then
			sound.__musics[name_as or name] = sound.__load_music("/media/music/"..name..sound.music_formats_order[i])
			if sound.__musics[name_as or name] then return true end
		end
	end
end

--- @fn
-- @brief Plays a previously loaded music
function sound.play_music(name, loop, fade_in_time)
	if not sound.__musics[name] then return end
	loop = loop or 1
	fade_in_time = fade_in_time or 0
	local ret = sound.__play_music(sound.__musics[name], loop, fade_in_time)
	return ret
end

--- @fn
-- @brief Loads a sample from a file in /media/sound/
function sound.load_sample(name, name_as)
	for i = 1, getn(sound.sound_formats_order) do
		if fs.file_exist("/media/sound/"..name..sound.sound_formats_order[i]) then
			sound.__samples[name_as or name] = sound.__load_sample("/media/sound/"..name..sound.sound_formats_order[i])
			if sound.__samples[name_as or name] then return true end
		end
	end
end

--- @fn
-- @brief Plays a previously loaded sample
function sound.play_sample(name, channel, loop, fade_in_time)
	if not sound.__samples[name] then return end
	channel = channel or -1
	loop = loop or 0
	fade_in_time = fade_in_time or 0
	return sound.__play_sample(sound.__samples[name], channel, loop, fade_in_time)
end

-- Void all functions if sound is disabled
-- DO NOT declare sound functions below this.
if sound.not_available then
	local to_void = {}
	for k, e in sound do
		if type(e) == "function" then
			tinsert(to_void, k)
		end
	end
	for i = 1, getn(to_void) do
		sound[to_void[i]] = function() end
	end
end

-----------------------------------------------------------
---------------------Sound system events-------------------
-----------------------------------------------------------

sound.events = {}
hook.new_hook_type("SOUND_CHANNEL_STOP")
hook.new_hook_type("MUSIC_STOP")

function sound.events.music_stop()
	local hook_ret = hook.process(hook.MUSIC_STOP)

	if not hook_ret and sound.ambient.cur_music and sound.ambient.cur_music ~= "" then
		sound.play_music(sound.ambient.cur_music, sound.ambient.default_loop, sound.ambient.cur_music_changed and sound.ambient.fade_in or 0)
		if sound.ambient.next_music then sound.ambient.cur_music = sound.ambient.next_music end
		sound.ambient.cur_music_changed = false
	end
end
function sound.events.channel_stop(channel)
	local hook_ret = hook.process(hook.SOUND_CHANNEL_STOP, channel)
end

-----------------------------------------------------------
---------------------Ambient music system------------------
-----------------------------------------------------------
new_flag("AMBIENT_MUSIC")

sound.ambient = { cur_music = "" default_loop = 1 enabled = false fade_in = 1000 fade_out = 1000 }

sound.ambient.change_music = function(force, amb, temporary)
	if not sound.ambient.enabled then return end
	local amb = amb or level_flags.flags[FLAG_AMBIENT_MUSIC] or dungeon(current_dungeon_idx).flags[FLAG_AMBIENT_MUSIC] or "default"
	if amb then
		if sound.ambient.cur_music == amb then return end
		if not force and sound.is_playing_music() then
			if sound.is_fading_music() ~= 0 then
				sound.stop_music()
			else
				sound.fade_out_music(sound.ambient.fade_out)
			end
		else
			sound.pause_music()
			sound.play_music(amb, sound.ambient.default_loop, sound.ambient.fade_in)
		end
		if temporary then sound.ambient.next_music = sound.ambient.cur_music end
		sound.ambient.cur_music = amb
		sound.ambient.cur_music_changed = true
	end
end

hook(hook.LEVEL_END_GEN, sound.ambient.change_music)
hook(hook.GAME_START, function() if character_dungeon then sound.ambient.change_music(true) end end)


-----------------------------------------------------------
---------------------Ambient sounds system-----------------
-----------------------------------------------------------
new_flag("AMBIENT_SOUNDS")
new_flag("AMBIENT_SOUNDS_FREQUENCY")

sound.ambient_sounds = { default_loop = 1 enabled = false fade_in = 3000 }

sound.ambient_sounds.random = function(force)
	if not sound.ambient_sounds.enabled then return end
	local amb = level_flags.flags[FLAG_AMBIENT_SOUNDS] or dungeon(current_dungeon_idx).flags[FLAG_AMBIENT_SOUNDS]
	local freq = level_flags.flags[FLAG_AMBIENT_SOUNDS_FREQUENCY] or dungeon(current_dungeon_idx).flags[FLAG_AMBIENT_SOUNDS_FREQUENCY]
	if not freq or not amb then return end
        if rng(freq) ~= 1 then return end

	sound.play_sample(amb[rng(flag_max_key(amb))])
end

hook(hook.GAME_START, function()
	if sound.ambient_sounds.enabled then
		time.rt.add(1000, sound.ambient_sounds.random)
	end
end)
