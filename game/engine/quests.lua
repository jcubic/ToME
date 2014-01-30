-- Quest helper files

declare_global_constants {
	"quest_data",
	"add_quest",
	"acquire_quest",
	"change_quest_status",
	"trigger_quest_hook",
}
declare_globals {
	"__quest_data",
	"__quest_callbacks",
	"__quest_callbacks_max",
	"__quest_dynamic_desc",
	"__quest_hook",
}

-- Quest structs

__quest_data = {}
__quest_hook = {}
__quest_callbacks = {}
__quest_callbacks_max = 0
__quest_dynamic_desc = {}

function add_quest(q)
	local i, index, d, z, qq

	assert(q.global, "No quest global name")
	assert(q.name, "No quest name")
	assert(q.desc, "No quest desc")
	assert(q.level, "No quest level")
	assert(q.hooks, "No quest hooks")

	i = new_quest(q.name);
	globals()[q.global] = i

	-- Make it save & load
	add_loadsave("quest("..q.global..").status", QUEST_STATUS_UNTAKEN)

	if type(q.desc) == "table" then
		z = 0
		for index, d in q.desc do
			quest_set_desc(i, z, d)
			z = z + 1
		end
	else
		__quest_dynamic_desc[i] = q.desc
		quest(i).dynamic_desc = true
	end
	quest(i).level = q.level
	if not q.silent then
		quest(i).silent = FALSE
	else
		quest(i).silent = q.silent
	end
	__quest_hook[i] = q.hooks
	for index, d in q.hooks do
		if tag(index) == TAG_NUMBER then
			local hookname = "__lua__quest_callback"..__quest_callbacks_max
			declare_global_constants {hookname}
			hook.add_script(index, hookname, hookname)
			setglobal(hookname, d)
			__quest_callbacks_max = __quest_callbacks_max + 1
		end
	end
	if q.data then
		__quest_data[i] = q.data
		add_loadsave("__quest_data["..i.."]", {})
	end
	return i
end

function acquire_quest(q, silent)
	assert(quest(q).status == QUEST_STATUS_UNTAKEN, "Quest "..q.." cannot be acquired, status is not untaken!")
	change_quest_status(q, QUEST_STATUS_TAKEN, silent)
	if __quest_hook[q][hook.INIT_QUEST] then __quest_hook[q][hook.INIT_QUEST]() end
	trigger_quest_hook(q, "init")
end

function change_quest_status(q, status, silent)
	local name = quest(q).name.." (Level "..quest(q).level..")"
	local txt =
	{
		[QUEST_STATUS_IGNORED] = "Quest '"..name.."' ignored."
		[QUEST_STATUS_UNTAKEN] = "Quest '"..name.."' unassigned."
		[QUEST_STATUS_TAKEN] = "Quest '"..name.."' #Gassigned#w."
		[QUEST_STATUS_COMPLETED] = "Quest '"..name.."' #Gcompleted#w."
		[QUEST_STATUS_REWARDED] = "Quest '"..name.."' #Grewarded#w."
		[QUEST_STATUS_FAILED] = "Quest '"..name.."' #Rfailed#w."
		[QUEST_STATUS_FINISHED] = "Quest '"..name.."' #Gfinished#w."
		[QUEST_STATUS_FAILED_DONE] = "Quest '"..name.."' #Rfinished in failure#w."
	}
	quest(q).status = status
	if not silent then message(txt[status]) end
end

function trigger_quest_hook(q, hook, ...)
	if __quest_hook[q][hook] then __quest_hook[q][hook](unpack(arg)) end
end

function quest_data(i)
	return __quest_data[i]
end

-- Reset status
hook(hook.INIT_DONE, function()
	for i = 1, max_q_idx - 1 do
		quest(i).status = QUEST_STATUS_UNTAKEN
	end
end)
