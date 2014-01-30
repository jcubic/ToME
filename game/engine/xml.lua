-- The xml module

declare_globals {
	"xml",
}

xml = {}

function xml:parseargs (s)
  local arg = {}
  gsub(s, "(%w+)=([\"'])(.-)%2", function (w, _, a)
    %arg[w] = a
  end)
  return arg
end

-- s is a xml stream, returns a table
function xml:collect (s)
  local stack = {n=0}
  local top = {n=0}
  tinsert(stack, top)
  local ni,c,label,args, empty
  local i, j = 1, 1
  while 1 do
    ni,j,c,label,args, empty = strfind(s, "<(%/?)([%w_]+)(.-)(%/?)>", j)
    if not ni then break end
    local text = strsub(s, i, ni-1)
    if not strfind(text, "^%s*$") then
      tinsert(top, text)
    end
    if empty == "/" then  -- empty element tag
      tinsert(top, {n=0, label=label, args=xml:parseargs(args), empty=1})
    elseif c == "" then   -- start tag
      top = {n=0, label=label, args=xml:parseargs(args)}
      tinsert(stack, top)   -- new level
    else  -- end tag
      local toclose = tremove(stack)  -- remove top
      top = stack[stack.n]
      if stack.n < 1 then
	error("xml: nothing to close with "..label)
      end
      if toclose.label ~= label then
	error("xml: trying to close tag "..toclose.label.." with tag "..label)
      end
      tinsert(top, toclose)
    end 
    i = j+1
  end
  local text = strsub(s, i)
  if not strfind(text, "^%s*$") then
    tinsert(stack[stack.n], text)
  end
  if stack.n > 1 then
    error("xml: unclosed "..stack[stack.n].label)
  end
  return stack[1]
end

-- Viewport coordinates
xml.write_out_y = 0
xml.write_out_x = 0
xml.write_out_h = 24
xml.write_out_w = 80

-- Offsets
xml.write_off_y = 0
xml.write_off_x = 0

-- Current position
xml.write_y = 0
xml.write_x = 0

xml.write_screen = function(color, s)
	local i
	for i = 1, strlen(s) do
		local c = strsub(s, i, i + 1)
		if c ~= "\n" then
			if xml.write_y - xml.write_off_y >= 0 and xml.write_y - xml.write_off_y < xml.write_out_h and xml.write_x - xml.write_off_x >= 0 and xml.write_x - xml.write_off_x < xml.write_out_w then
				term.putch(xml.write_x - xml.write_off_x + xml.write_out_x, xml.write_y - xml.write_off_y + xml.write_out_y, color, strbyte(c))
			end
			xml.write_x = xml.write_x + 1
		else
			xml.write_x = 0
			xml.write_y = xml.write_y + 1
		end
	end
end

xml.write_file = function (color, s)
	hook.print(s)
end

xml.write = xml.write_screen

xml.rule2string = {
    ['name'] = {"Its ", "name", " is"},
    ['contain'] = {"Its ", "name", " contains"},
    ['symbol'] = {"Its ", "symbol", " is"},
    ['inscribed'] = {"Its ", "inscription", " contains"},
    ['state'] = {"Its ", "state", " is"},
    ['status'] = {"Its ", "status", " is"},
    ['tval'] = {"Its ", "tval", " is"},
    ['race'] = {"Your ", "race", " is"},
    ['subrace'] = {"Your ", "subrace", " is"},
    ['class'] = {"Your ", "class", " is"},
    ['foo1'] = {"The result of ", "test 1 ", "is"},
    ['foo2'] = {"The result of ", "test 2 ", "is"},
    ['foo3'] = {"The result of ", "test 3 ", "is"},
}

xml.display_english = 1
function xml:display_xml(t, tab)
    if xml.display_english then
	xml:english_xml(t, tab)
    else
	xml:print_xml(t, tab)
    end
end

function xml:english_xml(t, tab, not_flag)
	local i, k, e
	local pre, post, recurse
	local children_not_flag
	local nextlevel
	local bcol, ecol = color.LIGHT_GREEN, color.GREEN

	if xml.write_active and t == auto_aux.rule then bcol, ecol = color.VIOLET, color.VIOLET end

	nextlevel = tab .. "    "

	recurse = 1

	if t.label == "rule" then
	    if t.args.type == "inscribe" then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "A rule named \"")
		xml.write(color.WHITE, tostring(t.args.name))
		xml.write(ecol, "\" to ")
		xml.write(bcol, "inscribe")
		xml.write(ecol, " an item with \"")
		xml.write(color.WHITE, t.args.inscription)
		xml.write(ecol, "\" when")
		xml.write(color.WHITE, "\n")
	    else	
		xml.write(color.WHITE, tab)
		xml.write(ecol, "A rule named \"")
		xml.write(color.WHITE, tostring(t.args.name))
		xml.write(ecol, "\" to ")
		xml.write(bcol, t.args.type)
		xml.write(ecol, " when")
		xml.write(color.WHITE, "\n")
	    end
	elseif t.label == "and" then
	    if not_flag then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "At least one of the following is false:")
		xml.write(color.WHITE, "\n")
	    else
		xml.write(color.WHITE, tab)
		xml.write(ecol, "All of the following are true:")
		xml.write(color.WHITE, "\n")
	    end
	elseif t.label == "or" then
	    if not_flag then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "All of the following are false:")
		xml.write(color.WHITE, "\n")
	    else
		xml.write(color.WHITE, tab)
		xml.write(ecol, "At least one of the following are true:")
		xml.write(color.WHITE, "\n")
	    end
	elseif t.label == "not" then
	    if bcol == color.VIOLET or getn(t) == 0 then
		xml.write(ecol, "(a negating rule)")
		xml.write(color.WHITE, "\n")
	    else
		nextlevel = tab
	    end
	    children_not_flag = not nil
	elseif t.label == "inventory" then
	    if not_flag then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Nothing in your ")
		xml.write(bcol, "inventory")
		xml.write(ecol, " matches the following:")
		xml.write(color.WHITE, "\n")
	    else
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Something in your ")
		xml.write(bcol, "inventory")
		xml.write(ecol, " matches the following:")
		xml.write(color.WHITE, "\n")
	    end
	elseif t.label == "equipment" then
	    if not_flag then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Nothing in your ")
		xml.write(bcol, "equipment")
		xml.write(ecol, " matches the following:")
		xml.write(color.WHITE, "\n")
	    else
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Something in your ")
		xml.write(bcol, "equipment")
		xml.write(ecol, " matches the following:")
		xml.write(color.WHITE, "\n")
	    end
	elseif t.label == "comment" then
		xml.write(color.WHITE, tab)
		xml.write(color.WHITE, "(" .. t[1] .. ")")
		xml.write(color.WHITE, "\n")
	elseif t.label == "skill" then
	    local s = t[1]
	    if not_flag then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Your skill in ")
		xml.write(bcol, s)
		xml.write(ecol, " is not from ")
		xml.write(color.WHITE, tostring(t.args.min))
		xml.write(ecol, " to ")
		xml.write(color.WHITE, tostring(t.args.max))
		xml.write(color.WHITE, "\n")
	    else
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Your skill in ")
		xml.write(bcol, s)
		xml.write(ecol, " is from ")
		xml.write(color.WHITE, tostring(t.args.min))
		xml.write(ecol, " to ")
		xml.write(color.WHITE, tostring(t.args.max))
		xml.write(color.WHITE, "\n")
	    end
	elseif t.label == "ability" then
	    local s = t[1]
	    if not_flag then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "You do not have the ")
		xml.write(bcol, s)
		xml.write(ecol, " ability")
		xml.write(color.WHITE, "\n")
	    else
		xml.write(color.WHITE, tab)
		xml.write(ecol, "You have the ")
		xml.write(bcol, s)
		xml.write(ecol, " ability")
		xml.write(color.WHITE, "\n")
	    end
	elseif t.label == "level" then
	    if not_flag then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Your ")
		xml.write(bcol, "level")
		xml.write(ecol, " is not from ")
		xml.write(color.WHITE, tostring(t.args.min))
		xml.write(ecol, " to ")
		xml.write(color.WHITE, tostring(t.args.max))
		xml.write(color.WHITE, "\n")
	    else
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Your ")
		xml.write(bcol, "level")
		xml.write(ecol, " is from ")
		xml.write(color.WHITE, tostring(t.args.min))
		xml.write(ecol, " to ")
		xml.write(color.WHITE, tostring(t.args.max))
		xml.write(color.WHITE, "\n")
	    end
	elseif t.label == "sval" then
	    if not_flag then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Its ")
		xml.write(bcol, "sval")
		xml.write(ecol, " is not from ")
		xml.write(color.WHITE, tostring(t.args.min))
		xml.write(ecol, " to ")
		xml.write(color.WHITE, tostring(t.args.max))
		xml.write(color.WHITE, "\n")
	    else
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Its ")
		xml.write(bcol, "sval")
		xml.write(ecol, " is from ")
		xml.write(color.WHITE, tostring(t.args.min))
		xml.write(ecol, " to ")
		xml.write(color.WHITE, tostring(t.args.max))
		xml.write(color.WHITE, "\n")
	    end
	elseif t.label == "discount" then
	    if not_flag then
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Its ")
		xml.write(bcol, "discount")
		xml.write(ecol, " is not from ")
		xml.write(color.WHITE, tostring(t.args.min))
		xml.write(ecol, " to ")
		xml.write(color.WHITE, tostring(t.args.max))
		xml.write(color.WHITE, "\n")
	    else
		xml.write(color.WHITE, tab)
		xml.write(ecol, "Its ")
		xml.write(bcol, "discount")
		xml.write(ecol, " is from ")
		xml.write(color.WHITE, tostring(t.args.min))
		xml.write(ecol, " to ")
		xml.write(color.WHITE, tostring(t.args.max))
		xml.write(color.WHITE, "\n")
	    end
	else 
	    if xml.rule2string[t.label] then
		local rule = xml.rule2string[t.label]
		local a, b, c = rule[1], rule[2], rule[3]
		if not_flag then c = c .. " not" end
		xml.write(color.WHITE, tab)
		xml.write(ecol, a) 
		xml.write(bcol, b) 
		xml.write(ecol, c) 
		xml.write(ecol, " \"") 
		xml.write(color.WHITE, t[1])
		xml.write(ecol, "\"") 
		xml.write(color.WHITE, "\n")
	    else
		if not_flag then
		    xml.write(bcol, "Not:\n")
		    tab = tab .. "    "
		xml:print_xml(t, tab)
		return
		end
	    end
	end

	for i = 1, getn(t) do
		if type(t[i]) == "string" then
			-- xml.write(color.WHITE, t[i].."\n")
		else
			xml:english_xml(t[i], nextlevel, children_not_flag)
		end
	end
end

function xml:print_xml(t, tab)
	local i, k, e
	local inside = nil
	local bcol, ecol = color.LIGHT_GREEN, color.GREEN

	if xml.write_active and t == auto_aux.rule then bcol, ecol = color.VIOLET, color.VIOLET end

	xml.write(bcol, tab.."<"..t.label)
	for k, e in t.args do
		xml.write(color.LIGHT_BLUE, " "..k)
		xml.write(color.WHITE, "=\"")
		xml.write(color.YELLOW, e)
		xml.write(color.WHITE, "\"")
	end
	xml.write(bcol, ">")

	for i = 1, getn(t) do
		if type(t[i]) == "string" then
			xml.write(color.WHITE, t[i])
		else
			if not inside then xml.write(color.WHITE, "\n") end
			inside = not nil
			xml:print_xml(t[i], tab.."    ")
		end
	end

	if not inside then
		xml.write(ecol, "</"..t.label..">\n")
	else
		xml.write(ecol, tab.."</"..t.label..">\n")
	end
end

-- t is a table representing xml, outputs the xml code via xml.write()
function xml:output(t)
	local i
	for i = 1, getn(t) do
		xml:print_xml(t[i], "")
	end
end
