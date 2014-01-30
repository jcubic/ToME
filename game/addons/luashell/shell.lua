-- shell = {} XXX inited in patch.lua

shell.version = "1.0.1CVS"

shell.lines = {
	"ToME Lua Shell v"..shell.version.." copyright 2003 Jules Bean",
	"Type a lua expression and the shell will show the result.",
	"Type a lua statement and the shell will execute it.",
	"Errors will be safely caught.",
	"",
	"ESC to return to ToME, ? for help",
	"",
}

shell.help = {
	"ToME Lua Shell v"..shell.version,
	"",
	"Copyright 2003 Jules Bean",
	"",
	"This code may be copied and distributed under the GNU Public License or the "..
		"Angband License, at your option.",

	"",
	"Welcome to the ToME Lua Shell.",
	"",
	"This shell enables you to explore ToME and Lua. It is intended for "..
		"module and script authors to use it for testing and enlightenment "..
		":-)",
	"",
	"Just type in lua expressions, and it will show you the result. Lua "..
		"statements are executed. <TAB> will do completion on globals and "..
		"sometimes even within tables. Any LUA errors you may cause are caught, and the error "..
		"messages appear in the shell screen. If your expression evaluates to "..
		"a large table or array, you get -- more -- style paging.",
	"",
	"Before executing anything, the screen flicks back to the ToME screen, "..
		"so any ToME functions you might call will have their normal onscreen "..
		"effect.  The ToME display system is very efficient, and on a fast "..
		"computer you won't even see the screen flicker if your function has no "..
		"side-effects.",
	"",
	"There is some simple history support on CTRL-P and CTRL-N.",
	"",
	"There is basic command-line editing on the emacs-style keys CTRL-{B,F,A,E,K,D}.",
	"",
	"Some examples you might want to try (without the comments!): ",
	"",
	"  > 3+4",
	"  > rng.roll(3,6)",
	"  > rng.roll(3,6) -- yes, again!",
	"  > player",
	"  > monster(1)",
	"  > project_meteor(20,dam.FIRE,200,PROJECT_KILL)",
	"  > for i=1,10 do shell.print(i) end",
	"    -- shell.print is an internal print routine, but it can be useful",
	"    -- for debugging purposes",
	"",
	"To Do:",
	"",
	" * Optimize parser",
	" * Context-sensitive completion, e.g. for fields in tables (some support in place)",
	" * Allow multiline entry or at least long line entry (long lines done)",
	"",
	"I welcome constructive criticism and suggestions (suggestions that "..
		"aren't already above in the TODO list!). Of course, I want to hear "..
		"about bugs. In particular, you may see some strange parse errors "..
		"if you try complex expressions, I want to know about these, they "..
		"indicate bugs in my LUA parser.  I already know it doesn't support "..
		"the [[ ]] and -- constructions.",
	"",
	"I'm not particularly interested in reports that my code is ugly, "..
		"unless they are accompanied by patches which fix the problem ;-)",
	"",
	"Please make comments, bug reports, or feature requests in the first "..
		"instance via the ToME T-Engine/Lua & Modules forum at "..
		"http://www.t-o-m-e.net/forum/",
	"",
}

shell.changelog = {
	" * 2003-08-08 Version 1.0.1 released ",
	" * 2003-08-05 Namespace cleanups ",
	" * 2003-07-18 Beginnings of support for incomplete parsing, which",
	"   will be used to permit completion of incomplete expressions",
	" * 2003-07-18 Limited support for completion, only on parsable exps",
	" * 2003-07-18 Optimised parser considerably",
	" * 2003-07-17 Support for custom pretty printers for particular ",
	"   data-types. Implemented for now is a flag printer for ToME CVS",
	" * 2003-07-17 Under CVS ToME, the shell now uses HOOK_COMMAND_DEBUG ",
	"   to bind itself to ^A^L",
	" * 2003-07-16 Bug in parlist in grammar prevented functions with more ",
	"   than one arg",
	" * 2004-07-02 Basic command-line editing support",
	" * 2004-07-05 Added ctrl-D (fwd delete), fixed history some",
	" * 2004-07-05 Long-line editing (multi line should be easy now) ",
	"   Syntax colouring in actual edit buffer."
}


shell.numlines = 22
shell.cols = 80

shell.buf = ""
shell.buflines = 1
shell.curs = 0

shell.top = getn(shell.lines)+1 -- skip past intro text

shell.RETURN = strbyte("\r")
shell.QUESTION = strbyte("?")
shell.DELETE = 127
shell.BACKSPACE = 8
shell.CTRL = function(c) return strbyte(strupper(c)) - 64 end
shell.TAB = 9

shell.col = {}
shell.col.userinput = color.LIGHT_BLUE
shell.col.prompt = color.WHITE
shell.col.output = color.YELLOW
shell.col.completions = color.GREEN

function shell.debug_cmd_hook(d)
	if d == shell.CTRL("L") then
		shell.run()
		return true
	end
	return FALSE
end

hook(hook.DEBUG_COMMAND, shell.debug_cmd_hook)

function shell.run()
	term.save()

	term.clear()
	shell.draw()

	local ret,wid,hgt = term.get_size(0,0)

	shell.numlines=hgt-2
	shell.cols=wid

	local running = 1

	while running do
		shell.printinput()

		-- allocate a history space for this command
		shell.new_history()

		while not nil do
			local c = term.inkey()

			if (c == ESCAPE) then running = nil; break end
			if (c == strbyte("\r")) then break end

			if (c == shell.DELETE or c == shell.BACKSPACE) then
				if (shell.curs>0) then
					shell.buf=strsub(shell.buf,1,shell.curs-1)..strsub(shell.buf,shell.curs+1)
					shell.curs=shell.curs-1
					shell.printinput()
				end
			elseif (c==shell.CTRL("D")) then
				if (shell.curs<strlen(shell.buf)) then
					shell.buf=strsub(shell.buf,1,shell.curs)..strsub(shell.buf,shell.curs+2)
					shell.printinput()
				end
			elseif (c==shell.CTRL("P")) or (c==strbyte('8') and term.inkey_dir) then
				shell.set_history(shell.buf)
				shell.buf = shell.history_back()
				shell.curs = strlen(shell.buf)
				shell.printinput()
			elseif (c==shell.CTRL("N")) or (c==strbyte('2') and term.inkey_dir) then
				shell.set_history(shell.buf)
				shell.buf = shell.history_fwd()
				shell.curs = strlen(shell.buf)
				shell.printinput()
			elseif (c==shell.CTRL("B")) or (c==strbyte('4') and term.inkey_dir) then
				if (shell.curs > 0) then
					shell.curs = shell.curs - 1
					shell.printinput()
				end
			elseif (c==shell.CTRL("F")) or (c==strbyte('6') and term.inkey_dir) then
				if (shell.curs < strlen(shell.buf)) then
					shell.curs = shell.curs + 1
					shell.printinput()
				end
			elseif (c==shell.CTRL("A")) then
				shell.curs = 0
				shell.printinput()
			elseif (c==shell.CTRL("E")) then
				shell.curs = strlen(shell.buf)
				shell.printinput()
			elseif (c==shell.CTRL("K")) then
				shell.buf = strsub(shell.buf,1,shell.curs)
				shell.curs = strlen(shell.buf)
				shell.printinput()
			elseif (c==shell.TAB) then
				shell.draw()

				local matches = {}
				local maxlen = 0
				local bestmatch
				local initial
				local completing
				local table
				local ind = 0


				term.blank_print("",shell.numlines,0)

				if shell.tokenise(shell.buf) then
					local t = shell.tokenise(shell.buf,1)
					local p = shell.parse_as(t,"exp")

					if (getn(p) == 0) then
						p = shell.parse_as(t,"chunk")
					end

					if getn(p) > 0 then
						local q = p[1]

						shell.print_parse(p)

						shell.parser_decorate_tree(q) -- add backpointers, up-pointers, etc
						local r = shell.parser_find_rightmost_nonempty(q)
						term.blank_print("Rightmost was "..r.tree.tok..":"..r.tree.text,shell.numlines-(shell.buflines-1),0)
						if (r.tree.tok == "identifier") then
							local v = r.tree.text -- base name
							local tab = "" -- table name
							local s = r
							while not (s.index == nil or s.prod == "var") do
								local ind = s.index-1
								s = s.up
								for i=ind,1,-1 do
									local t = s.tree[i]
									if (t.tree.tok) then
										tab = t.tree.text..tab
									else
										tab = shell.parser_tree_fulltext(t)..tab
									end
								end
							end

							initial=""
							while not (s.index == nil) do
								local ind = s.index-1
								s = s.up
								for i=ind,1,-1 do
									local t = s.tree[i]
									if (t.tree.tok) then
										initial = t.tree.text..initial
									else
										initial = shell.parser_tree_fulltext(t)..initial
									end
									initial = t.spacebefore..initial
								end
								initial = s.spacebefore..initial
							end


							completing = v

							if tab == "" then
								table = globals()
							else
								--table = globals()[strsub(tab,1,strlen(tab)-1)] --chop off final .
								local save_alert = _ALERT
								_ALERT = function () end
								table = dostring("return "..strsub(tab,1,strlen(tab)-1))
								_ALERT = save_alert

								if not table then
									term.blank_print("No such global : "..strsub(tab,1,strlen(tab)-1),shell.numlines,0)
									completing = nil
								elseif type(table) ~= "table" and type(table) ~= "userdata" then
									term.blank_print("Not a table : "..strsub(tab,1,strlen(tab)-1),shell.numlines,0)
									completing = nil
								end
							end
							initial = initial..tab -- XXX
						end
					end
--					print ("parsing results, init = "..initial.." compl = "..completing)
				end

				if (completing == nil) then -- try old style
					while (strfind(shell.buf,"[%(%,%[%+%-%*% ]",ind+1)) do
						ind = strfind(shell.buf,"[%(%,%[%+%-%*% ]",ind+1)
					end

					initial = strsub(shell.buf,1,ind)
					if not initial then initial = "" end
					completing = strsub(shell.buf,ind+1)
					table = globals()
				end

				if (completing) then
					if type(table) == "userdata" then
						local faketable = {}
						if table[".get"] then -- tolua style userdata
							for k,v in table[".get"] do
								faketable[k] = 1
							end
							table=faketable
						end
					end

					for k,v in table do
						if strsub(k,1,strlen(completing)) == completing then
							tinsert(matches,k)
							if (strlen(k) > maxlen) then maxlen = strlen(k) end
						end
					end

					if (getn(matches) == 0) then
						-- nothing
					elseif (getn(matches) == 1) then
						completing = matches[1]
					else -- 2 or more matches
						bestmatch = matches[1]
						for i = 1,getn(matches) do
							for j = 1,strlen(bestmatch) do
								if strsub(matches[i],1,j) ~= strsub(bestmatch,1,j) then
									bestmatch = strsub(bestmatch,1,j-1)
									break
								end
							end
						end


						local numcols = shell.cols/(maxlen+1)
						local colwidth = shell.cols/numcols

						local numrows = ((getn(matches)-1)/numcols)+1

						local firstrow

						if (numrows > (shell.numlines - (shell.buflines-1))) then
							numrows = shell.numlines-(shell.buflines-1)-1
							firstrow = shell.numlines-(shell.buflines-1)-1-numrows
						else
							firstrow = shell.numlines-(shell.buflines-1)-numrows
						end

						sort(matches)

						if firstrow >= 1 then term.blank_print("",firstrow-1,0) end

						for col = 1,numcols do
							for i = 1,numrows do
								if matches[(col-1)*numrows+i] then
									term.blank_print(shell.col.completions,matches[(col-1)*numrows+i],i+firstrow-1,((col-1)*colwidth))
								end
							end
						end

						if (getn(matches) > numcols*numrows) then
							term.blank_print("",shell.numlines-2,0)
							term.blank_print(shell.col.completions,"("..(getn(matches)-(numcols*numrows)).." more omitted matches)",shell.numlines-1,0)
						end

						completing=bestmatch
					end

					shell.buf = initial..completing
					shell.curs = strlen(shell.buf)
				end


				-- XXX syntax highlight here please

				shell.printinput()
			elseif c == shell.QUESTION and shell.buf=="" then
				shell.do_help()
				shell.printinput()
			else
				local cstr = format("%c",c)
				shell.buf=strsub(shell.buf,1,shell.curs)..cstr..strsub(shell.buf,shell.curs+1)
				shell.curs=shell.curs+1

				shell.printinput()
			end
		end



		if running then
			shell.final_history(shell.buf)

			local save_alert = _ALERT
			_ALERT = shell.alert
			term.load()

			local toks = shell.tokenise(shell.buf)
			local printresult=FALSE
			local result

			if toks then
				if (shell.dontparse or getn(shell.parse_as(toks,"exp",1)) > 0) then
					result = shell.parse_tuple(dostring("return 1,"..shell.buf))
					printresult=true
				elseif (getn(shell.parse_as(toks,"chunk",1)) > 0) then
					dostring(shell.buf)
				else
					shell.print("Parse error, don't know if it's a statement or an expression")
				end
			else
				shell.print("Tokenising error, don't know if it's a statement or an expression")
			end

			term.save()
			_ALERT = save_alert

			if (printresult==true and result.status == "OK") then
				shell.pretty_print(shell.buf,result.value)
			else
				shell.print(shell.buf)
			end

			term.clear()
			shell.draw()
			shell.buf = ""
			shell.curs = 0
		end
	end

	term.load()
end

function shell.printinput()
	local tokens = shell.tokenise(shell.buf,1)

	local printcolsub = function(a,b,y,x)
	if (%tokens and getn(%tokens)>0) then
		local col_table = {
			["keyword"]=color.RED,
			["number"]=color.LIGHT_BLUE,
			["op"]=color.YELLOW,
			["identifier"]=color.WHITE,
			["string"]=color.GREEN,
			["default"]=color.ORANGE,
		}
		local pos = 0
		local i = 1
		while %tokens[i] and pos+strlen(%tokens[i].text) < a do
			pos = pos + strlen(%tokens[i].text)
			i = i+1
		end

		if not %tokens[i] then return end

		local colour = col_table[%tokens[i].tok]
		if not colour then colour = col_table.default end

		term.blank_print(colour,strsub(%tokens[i].text,a-pos),y,x)

		pos = pos + strlen(%tokens[i].text)
		x = x + (pos-a) + 1
		i = i+1

		if not %tokens[i] then return end

		while(%tokens[i] and pos < b) do
			colour = col_table[%tokens[i].tok]
			if not colour then colour = col_table.default end
			term.blank_print(colour,%tokens[i].text,y,x)
			pos = pos + strlen(%tokens[i].text)
			x = x+strlen(%tokens[i].text)
			i = i+1
		end
	else
		term.print(color.WHITE,strsub(shell.buf,a,b),y,x)
	end
	end

	if (strlen(shell.buf) < shell.cols-2) then
		term.blank_print(shell.col.prompt,"> ",shell.numlines+1,0)
		printcolsub(1,strlen(shell.buf),shell.numlines+1,2)
		term.gotoxy(shell.numlines+1,shell.curs+2)
	else
		shell.buflines = 2 + ((strlen(shell.buf) - (shell.cols-2))/shell.cols)
		local i
		local strpos
		local curs_x,curs_y

		term.blank_print(shell.col.prompt,"> ",shell.numlines+1-(shell.buflines-1),0)
		printcolsub(1,shell.cols-2,shell.numlines+1-(shell.buflines-1),2)

		strpos = shell.cols-1
		for i=2,shell.buflines do
			printcolsub(strpos,strpos+shell.cols-1,shell.numlines+1-(shell.buflines-1)+i-1,0)
			strpos = strpos + shell.cols
		end

		if (shell.curs < shell.cols-2) then
			term.gotoxy(shell.numlines+1-(shell.buflines-1),shell.curs+2)
		else
			local curs_line
			local curs_x,curs_y

			curs_line = (shell.curs-(shell.cols-2))/shell.cols + 1
			curs_y = shell.numlines+1-(shell.buflines-1)+curs_line
			curs_x = shell.curs - (shell.cols-2) - (shell.cols*(curs_line))
			term.gotoxy(curs_y,curs_x)
		end
	end
end

--- helper function to cope with multiple return values
--- and weird behaviour of dostring()

function shell.parse_tuple(...)
	local ret

	assert(arg.n > 0)

	if (arg[1] == nil) then return {["status"] = "error"} end
	if arg.n == 1 then ret = nil
	elseif arg.n == 2 then ret = arg[2]
	else
		ret = {}
		for i=2,arg.n do ret[i-1] = arg[i] end
	end

	return {["status"]="OK",["value"]=ret}
end

shell.history = {}
shell.history.i = 1
shell.history.max = 100

function shell.new_history()
	shell.history.i = imod(shell.history.i + 1,shell.history.max)
	shell.history[shell.history.i] = ""
	shell.history_copy={}
	for k,v in shell.history do
	  shell.history_copy[k] = v
	end
end

function shell.set_history(s)
	shell.history_copy[shell.history_copy.i] = s
end

function shell.history_back()
	shell.history_copy.i = imod(shell.history_copy.i + shell.history.max - 1,shell.history.max)

	local ret = shell.history_copy[shell.history_copy.i]

	if ret then return ret else return "" end
end

function shell.history_fwd()
	shell.history_copy.i = imod(shell.history_copy.i + 1,shell.history_copy.max)

	local ret = shell.history_copy[shell.history_copy.i]

	if ret then return ret else return "" end
end

function shell.final_history(s)
	shell.history[shell.history.i] = s
end

function shell.print(s)
	if (strlen(s) < shell.cols) then shell.printline(s) return end

	local lastspace = nil
	local pos = 1
	local space = strfind(s,"%s",pos)

	while not nil do
		if not space then -- space not found, finish
			shell.printline(strsub(s,pos))
			break
		else
			-- found a space
			if (space >= pos+shell.cols) then -- past the end
				if (lastspace) then
					shell.printline(strsub(s,pos,lastspace-1))
					pos = lastspace+1
				else -- no previous space
					shell.printline(strsub(s,pos,space-1))
					pos = space+1
				end
			end
			lastspace=space
			space = strfind(s,"%s",space+1) -- find next space
		end
	end
end

function shell.printline(s)
	tinsert(shell.printbuffer,s)
end

shell.printbuffer = {}

function shell.draw()
	local newlines = 0
	local escaped

	for i=1,getn(shell.printbuffer) do
		if (newlines >= shell.numlines -1) then
			-- XXX (more)
			shell._draw()
			if not escaped then
				term.blank_print(color.LIGHT_BLUE,"-- (more, ESC to skip to end) --",shell.numlines,0)
				if (term.inkey() == ESCAPE) then escaped = 1 end
			end
			newlines=0
		end

		local s = shell.printbuffer[i]

		shell.top = imod(shell.top + 1,shell.numlines)

		local bottom = imod(shell.top+shell.numlines-1,shell.numlines)

		shell.lines[bottom] = s

		newlines = newlines + 1
	end

	shell.printbuffer = {}

	term.blank_print("",shell.numlines,0)

	shell._draw()
end

function shell._draw()
	for i = 1,shell.numlines do
		local ind = imod(shell.top+i-1,shell.numlines)
		if shell.lines[ind] then
			term.blank_print(shell.col.output,shell.lines[ind],i-1,0)
		else
			term.blank_print("",i-1,0)
		end
	end
	shell.newlines = 0
end

function shell.alert(s)
	local init = 1

	while not nil do
		local nl = strfind(s,"\n",init,1)
		if nl then
			shell.print(strsub(s,init,nl-1))
			init = nl+1
		else
			shell.print(strsub(s,init))
			break
		end
	end
end


function shell.pretty_print(n,x) shell.pretty_print_aux(n,x,"",{}) end

function shell.pretty_print_aux(n,x,indent,visited)
	if (shell.util_in_array(visited,x)) then
		shell.print(indent..n.." has already been displayed, avoiding recursion")
		return
	end
	if (type(x) == "number") then
		shell.print(indent..n.." = "..x)
	elseif type(x) == "string" then
		shell.print(indent..n.." = "..'"'..x..'"')
	elseif x == nil then
		shell.print(indent..n.." is nil")
	elseif type(x) == "function" then
		local inf = getinfo(x)
		local auxinf = ""
		if (inf.what == "Lua") then
			auxinf = "[Lua: line "..inf.linedefined.." of "..inf.source.."]"
		else auxinf = "["..inf.what.."]" end

		shell.print(indent..n.." is a function "..auxinf)
	elseif type(x) == "table" then
		if not next(x,nil) then
			shell.print(indent..n.." = {}") -- empty
		else
			local doarray = 1
			local eltcount = 0

			for k,v in x do
				if type(k) ~= "number" and k ~= "n" then
					doarray = nil
					break
				end
			end

			if doarray then
				for i=1,getn(x) do if x[i] then eltcount = eltcount +1 end end
			end

			if eltcount*2 > getn(x) then -- print as array
				shell.print(indent..n.." = {")
				for i=1,getn(x) do
					if x[i] then
						shell.pretty_print_aux("["..i.."]",x[i],indent.."  ",
																	 shell.util_join_array(visited,{x}))
					end
				end
				shell.print(indent.."}")

			else -- print as table
				shell.print(indent..n.." = {")
				for k,v in x do
					shell.pretty_print_aux(k,v,indent.."  ",shell.util_join_array(visited,{x}))
				end
				shell.print(indent.."}")
			end
		end
	elseif type(x) == "userdata" then

		if shell.pretty_print_special[tolua.type(x)] then
			shell.pretty_print_special[tolua.type(x)](indent,n,x)
		else
			local toluatype = globals()[tolua.type(x)]

			if toluatype and (type(toluatype) == "table") then
				shell.print(indent..n.." is of tolua [C] type "..tolua.type(x).." = {")
				for k,v in toluatype[".get"] do
					shell.pretty_print_aux(k,x[k],indent.."  ",shell.util_join_array(visited,{x}))
				end
				shell.print(indent.."}")
			else
				shell.print(indent..n.." is of userdata type "..tolua.type(x))
			end
		end
	else
		shell.print(indent..n.." is of unknown type "..type(x))
	end
end

shell.pretty_print_special = {
	["flags_type"] = function (indent,n,f)
										 local flag_descs = {}
										 for i=1,f.size do
											 local n = f.node[i]
											 -- XXX fixme flag_foreach
											 if (n.flags & FLAG_FLAG_USED) > 0 then
												 local flag = find_flag(n.key)
												 local flagname
												 if flag then flagname = flag.name
												 else flagname=n.key end
												 if (n.flags & FLAG_FLAG_FLAGS) > 0 then
												   tinsert(flag_descs,flagname..": {..flags..}")
												 elseif (n.flags & FLAG_FLAG_PTR) > 0 then
												   tinsert(flag_descs,flagname..": [ptr]")
												 else
												   tinsert(flag_descs,flagname..":"..flag_get(f,n.key))
												 end
											 end
										 end
										 if getn(flag_descs)>0 then
											 sort(flag_descs)
											 local s = ""
											 for i=1,getn(flag_descs) do
												 s = s..flag_descs[i]
												 if i ~= getn(flag_descs) then s = s.." " end
											 end
											 shell.print(indent..n.." = "..s)
										 else
											 shell.print(indent..n.." = (no flags)")
										 end
									 end
}

function shell.do_help()
	for i=1,getn(shell.help) do
		shell.print(shell.help[i])
	end
	shell.draw()
end

function shell.debug_print(...)
	_ALERT(shell.debug_print_aux(arg))
end

function shell.debug_print_aux(arg)
	local ret = ""
	for i=1,getn(arg) do
		ret = ret..shell.debug_print_one(arg[i])
	end

	return ret
end

function shell.debug_print_one(x)
	if (type(x) == "string") then return x end
	if (type(x) == "number") then return ""..x end
	if (type(x) == "table") then
		local array=true
		local ret
		for k,v in x do
			if type(k) ~= "number" and k ~= "n" then
				array=FALSE
			end
		end

		if array == true then
			ret="{"
			for k=1,getn(x) do
				if (x[k]) then
					ret = ret..shell.debug_print_one(x[k])..","
				else
					ret = ret.."nil"..","
				end
			end
			return ret.."}"
		else
			ret="{"
			for k,v in x do
				ret = ret..k.."="..shell.debug_print_one(v)..","
			end
			return ret.."}"
		end
	end
	return type(x)
end

