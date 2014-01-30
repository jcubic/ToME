global("__building_actions", {})

declare_global_constants
{
	"add_building_action",
	"__bact_activate",
}

function add_building_action(a)
        assert(a.index, "No building action index")
        assert(a.action, "No building action action")
        __building_actions[a.index] = a.action
end

function __bact_activate(bact)
        if __building_actions[bact] then
                return __building_actions[bact]()
        end
end

hook.add_script(hook.BUILDING_ACTION, "__bact_activate", "__bact_activate")
