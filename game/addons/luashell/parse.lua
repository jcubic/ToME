shell.parser = {}

function shell.parser_print_grammar(g) 
	local line
	for prod,alts in g do
		line = prod .. " := "
		for i=1,getn(alts) do
			for j=1,getn(alts[i]) do
				if (type(alts[i][j]) == "string") then
					line = line .. alts[i][j].." "
				elseif alts[i][j].text then
					line = line .. "<"..alts[i][j].text..":"..alts[i][j].tok.."> "
				else
					line = line .. "<"..alts[i][j].tok.."> "
				end
			end
			if (i<getn(alts)) then line = line .. " | " end
		end
		_ALERT(line)
	end
end



-- shell.parse_as(shell.tokenise("a+b"),"exp",1)
-- shell.parse_as(shell.tokenise("a+b"),key("nil"),1)

function shell.parser_test_parse(s,p)
	shell.debug = 1
	local ret =  shell.parse_as(shell.tokenise(s),p,1,true)
	shell.debug=nil
	return ret
end

-- parse_as
--
-- Crude (i.e. exponential time) recursive descent parser
--
-- toks: list of tokens to parse
-- prod: production to parse as
-- pos: position in token list to start at
-- all: true/FALSE: must we consume all tokens?
-- indent: just for debugging messages
--
-- returns
--
-- list (array) of tables {
--   tree = the parse tree
--   pos = position parsed up to
-- }

function shell.parse_as(toks,prod)
	local cache = {}

	for i=1,getn(toks)+1 do cache[i] = {} end

	return shell.parse_as_aux(toks,prod,1,true,"",cache)
end

function shell.parse_as_incomplete(toks,prod)
	local cache = {}

	for i=1,getn(toks)+1 do cache[i] = {} end

	return shell.parse_as_aux(toks,prod,1,true,"",cache,1)
end

function shell.parse_as_aux(toks,prod,pos,all,indent,cache,wants_unfinished)
		-- HACK: skip space tokens
		local spacebefore = ""
		while getn(toks) >= pos and toks[pos].tok=="space" do
			spacebefore = spacebefore..toks[pos].text
			pos = pos + 1
		end

--	shell.debug_print(indent.."spa ",pos," : ",prod)
	if (type(prod) == "string") then -- production
		if cache[pos][prod] then
			return cache[pos][prod]
		end

		local prodparses = {} -- all parses under this production
		local incomplete_parses = {} 


		if not shell.grammar[prod] then
			shell.debug_print("Ooops, "..prod.." does not exist")
			return {}
		end

		local lookahead
		-- Lookahead at first token
		if getn(toks) >= pos then
			lookahead = shell.terminal(toks[pos])
		else
			lookahead = "::epsilon" -- nothing left
		end

		if (shell.debug) then
			shell.debug_print(indent.."Parsing as "..prod.." at pos "..pos.." lookahead "..lookahead)
		end

		local valid_alts = shell.opt[prod][lookahead]
		if not valid_alts then valid_alts = shell.opt[prod]["::epsilon"] end
		--valid_alts = shell.grammar[prod]

		if getn(valid_alts) == 0 and lookahead == "::epsilon" then
			-- nothing fits but we're at end of token stream
			return {},{{}}
		end

		
		for i=1,getn(valid_alts) do
			-- for each alternative
			local alt = valid_alts[i]
			local altparses = {} -- possible parse trees under this alt
			local success = true
			local altpos = pos

			altparses[1]={["tree"]={},["pos"]=pos,["spacebefore"]=spacebefore} -- start off

			for j=1,getn(alt) do
				-- for each part of this alternative
				local part = alt[j]
				local newparses = {} -- collect the new parses

				-- for each parse so far
				for k=1,getn(altparses) do
					local rec,incomplete = 
						shell.parse_as_aux(toks,part,altparses[k].pos,FALSE,indent.."  ",cache,wants_unfinished)

					for m=1,getn(rec) do -- for each successful sub-parse
						tinsert(newparses,
							{["tree"] = shell.util_join_array(altparses[k].tree,
								{{["prod"]=part,
								  ["tree"]=rec[m].tree,
						      ["spacebefore"]=rec[m].spacebefore}}),
								["pos"] = rec[m].pos,
								["spacebefore"] = spacebefore
							})
					end

					-- were there any incomplete ones?
					if incomplete then
						for m=1,getn(incomplete) do -- for each successful sub-parse
							if incomplete[m].tree then 
								tinsert(incomplete_parses,
												{["tree"] = shell.util_join_array(altparses[k].tree,
																							 {{["prod"]=part,
																									 ["tree"]=incomplete[m].tree}})
												})
							else
								tinsert(incomplete_parses,
												{["tree"] = shell.util_join_array(altparses[k].tree,
																							 {{["INCOMPLETE"]=part}})
												})
							end
						end
					end
				end
				altparses = newparses
			end

			if (all == FALSE) then
				-- append all parses for this alternative to our list
				prodparses = shell.util_join_array(prodparses,altparses)
			else
				-- need to filter out ones that don't use all tokens
				for j=1,getn(altparses) do
					if (altparses[j].pos == getn(toks)+1) then
						tinsert(prodparses,altparses[j])
					end
				end
			end
		end

		if (shell.debug) then
			shell.debug_print(indent..prod," at ",pos," returns ",getn(prodparses)," parses")
		end
		cache[pos][prod] = prodparses
		return prodparses,incomplete_parses
	elseif (type(prod) == "table") then -- token
		shell.token_count = shell.token_count+1
		local s
		if (shell.debug and getn(toks) >= pos) then
			local a,b
			if (prod.text) then a="<"..prod.text..":"..prod.tok..">"
			else a="<"..prod.tok..">" end
			b="<"..toks[pos].text..":"..toks[pos].tok..">"
			s= indent.."Looking for "..a.." at "..b
		end
		if getn(toks) >= pos then -- enough room
			if (toks[pos].tok == prod.tok) and -- right kind
				((not prod.text) or toks[pos].text == prod.text) then
				-- successfully match single token in exactly one way
				if (shell.debug and s) then shell.debug_print(s.." succeeded") end
				return {{["pos"]=pos+1,["tree"]=toks[pos],["spacebefore"]=spacebefore}}
			else
				if (shell.debug and s) then shell.debug_print(s.." failed") end
				return {}
			end
		else -- end of token stream
			if wants_unfinished then
				shell.print("returninf unfinished parse")
				return {},{{}}
			else
				return {}
			end
		end
	end
end

function shell.print_parse(p)
	for i=1,getn(p) do
		local line = shell.print_parse_tree(p[i].tree)
		shell.print(line)
	end
end

function shell.print_parse_tree(t)
	local s = ""
	for j=1,getn(t) do
		if  j ~= 1 then s = s.." " end
		s = s .. shell.print_parse_aux(t[j])
	end
	return s
end

function shell.print_parse_aux(q)
	if q.INCOMPLETE then
		if type(q.INCOMPLETE) == "string" then -- non-terminal
			return "INCOMPLETE("..q.INCOMPLETE..")"
		else -- terminal
			return "INCOMPLETE("..q.INCOMPLETE.tok..")"
		end
	end
	if type(q.prod) == "string" then -- non-terminal
		return q.prod.."<"..shell.print_parse_tree(q.tree)..">"
	else -- terminal
		return q.tree.text
	end
end


-- Recursive descent parsers loop on left-recursion.
-- Fortunately it can be algorithmically resolved by preprocessing the
-- grammar
					
function shell.parser_resolve_leftrec(grammar)
	local ret = {}
--	_ALERT("Res. leftrec called on")
--	shell.parser_print_grammar(grammar)

	for prod,alts in grammar do -- for each production
		local lrs = {} -- left-rec alternatives
		local nlrs = {} -- non-left-rec alts

		for i=1,getn(alts) do -- for each alternative
			if alts[i][1] == prod then
				-- recursive
				tinsert(lrs,alts[i])
			else
				tinsert(nlrs,alts[i])
			end
		end

		if (getn(lrs) > 0) then
			if (getn(nlrs) <= 0) then
				error("Recursion can't be factored out, no 'base' cases")
			end
			ret[prod] = {} 
			ret[prod.."_"] = {} 

			for i=1,getn(lrs) do
				local tmp = {}
				for k=2,getn(lrs[i]) do
					tmp[k-1]=lrs[i][k]
				end
				tinsert(tmp,prod.."_")
				
				tinsert(ret[prod.."_"],tmp)
			end

			-- slight hack, put empty alternative at end
			-- so it has lowest priority
			tinsert(ret[prod.."_"],{})
			
			for j=1,getn(nlrs) do
				local tmp = {}
				for k=1,getn(nlrs[j]) do
					tmp[k] = nlrs[j][k]
				end
				tinsert(tmp,prod.."_")
				
				tinsert(ret[prod],tmp)
			end
		else
			ret[prod] = alts
		end
	end

	return ret
end

-- Worse still, you can have indirect left-recursion.  This one
-- sorts that out

function shell.parser_resolve_cyclic(grammar)
	local done = {}
	local ret = {}

--	shell.parser_print_grammar(grammar)

	for gg=1,getn(grammar) do
		local prod,alts = grammar[gg][1],grammar[gg][2]
		ret[prod] = {}
		local alt_new = {} -- by default, nothing changes
		local ind = 1

		local cyclesremoved
		local iters = 0

		repeat 
			cyclesremoved = 0
			for i=1,getn(alts) do
				local alt = alts[i]

				if (type(alt[1]) == "string" and
						done[alt[1]]) then -- left-cycle spotted
					cyclesremoved = cyclesremoved + 1

					local sub = ret[alt[1]]

					--				shell.debug_print("Doing ",prod," subbing ",sub," in ",alt)
					-- get rid of the recursion by expanding out

					for k=1,getn(sub) do -- for each alt. expansion
						alt_new[ind] = shell.util_copy_of(alt)

						tremove(alt_new[ind],1) -- zap the 1st elt

						for j=1,getn(sub[k]) do
							tinsert(alt_new[ind],j,sub[k][j]) -- by its expansion
						end
						
						ind = ind + 1
					end				
				else
					alt_new[ind] = shell.util_copy_of(alt)
					ind = ind +1
				end
			end

			alts = alt_new -- and iterate..
			alt_new = {}
			ind = 1
			iters = iters+1
		until iters > 5 or cyclesremoved == 0

		-- check for left recursion
		local g = shell.parser_resolve_leftrec({[prod]=alts}) 

		ret[prod] = g[prod]

		if g[prod.."_"] then
			ret[prod.."_"] = g[prod.."_"]
		end
																			
		done[prod] = 1
	end

	return ret
end

function shell.parser_build_firsts(grammar) 
	local firsts={}

	local added = 0

	-- first we get tokens and epsilons only

	for prod,alts in grammar do -- for each production in the grammar
		firsts[prod] = {}
		for i=1,getn(alts) do -- for each alternative
			if (getn(alts[i]) >= 1) then -- at least one part
				local firstpart = alts[i][1]
				if type(firstpart) ~= "string" then -- alt. begins with token
					firsts[prod][shell.terminal(firstpart)]=1
				end
			else
				firsts[prod]["::epsilon"] = 1
			end
		end
	end

	-- now we ripple up through productions
	repeat
		added = 0
		for prod,alts in grammar do -- for each production in the grammar
			for i=1,getn(alts) do -- for each alternative
				if (getn(alts[i]) >= 1) then -- at least one part
					local firstpart = alts[i][1]
					if type(firstpart) == "string" then -- alt. begins with prodn
						for k,v in firsts[firstpart] do
							if not firsts[prod][k] then
								firsts[prod][k] = 1
								added = added + 1
							end
						end
					end
				end
			end
		end
	until added == 0

	return firsts
end

function shell.parser_build_optimised(grammar)
	local firsts = shell.parser_build_firsts(grammar)

	-- grammar has the form
  -- {prod ==> array of alts}
	-- each alts is array of parts
	-- each part is prod or token

	-- we build optimised
	-- {prod ==> table of indexed_alts}
	-- indexed_alts is {firsttoken ==> array of alts}
  -- these alts are as before

	local optimised = {}

	for prod,alts in grammar do -- for each production
		optimised[prod] = {}
		for i=1,getn(alts) do -- for each alternative
			if (getn(alts[i]) >= 1) then -- at least one part
				local firstpart = alts[i][1]
				if type(firstpart) == "string" then -- alt. begins with prodn
					for t,_ in firsts[firstpart] do
						-- for each possible first token t
						if not optimised[prod][t] then
							optimised[prod][t] = {}
						end
						tinsert(optimised[prod][t],alts[i])
					end
				else -- alt begins with token
					local t = shell.terminal(firstpart)
					if not optimised[prod][t] then
						optimised[prod][t] = {}
					end
					tinsert(optimised[prod][t],alts[i])
				end
			else -- no parts, i.e. empty alternative, store under epsilon
				-- XXX this is wrong!
				local t = "::epsilon"
				if not optimised[prod][t] then
					optimised[prod][t] = {}
				end
				tinsert(optimised[prod][t],alts[i])
			end
		end

		if not optimised[prod]["::epsilon"] then
			optimised[prod]["::epsilon"] = {}
		end

		-- now add the epsilon parts to each one
		for t,_ in optimised[prod] do
			if t ~= "::epsilon" then
				for i=1,getn(optimised[prod]["::epsilon"]) do
					tinsert(optimised[prod][t],optimised[prod]["::epsilon"][i])
				end
			end
		end

	end

	return optimised
end

		

function shell.parser_time_trial()
	shell.parser_time_one("a+b+c+d+e+f+g+h+i","exp")
	shell.parser_time_one("a+b+c+d+e+f+g+h+i","stat")

	shell.parser_time_one("a+b+c+d+e+f+g+h+i+j+k","exp")
	shell.parser_time_one("a+b+c+d+e+f+g+h+i+j+k","stat")

	shell.parser_time_one("1+2+3+4+5+6+7+8+9","exp")
	shell.parser_time_one("1+2+3+4+5+6+7+8+9","stat")

	shell.parser_time_one("{1,2,3,4,5,6,7,8,a,b,c}","exp")
	shell.parser_time_one("{1,2,3,4,5,6,7,8,a,b,c}","stat")

	shell.parser_time_one("fold({1,2,3,4,5,6,7},0,function (a,b) return a+b end)","exp")
	shell.parser_time_one("fold({1,2,3,4,5,6,7},0,function (a,b) return a+b end)","stat")

	shell.parser_time_one("f(a.b.c,{},function (d,e) g(d,h(e)) return e end)","exp")
	shell.parser_time_one("f(a.b.c,{},function (d,e) g(d,h(e)) return e end)","stat")

	shell.parser_time_one("f(a.b.c,{},function (d,e) return e end)","exp")
	shell.parser_time_one("f(a.b.c,{},function (d,e) return e end)","stat")

	shell.parser_time_one("f(a,{},function (d,e) g(d,h(e)) return e end)","exp")
	shell.parser_time_one("f(a,{},function (d,e) g(d,h(e)) return e end)","stat")

	shell.parser_time_one("f(function (d,e) g(d) end)","exp")

	shell.parser_time_one("f(function (d,e) g(d,h(e)) end)","exp")
	shell.parser_time_one("f(function (e) g(d,h(e)) end)","exp")
	shell.parser_time_one("f(function () g(d,h(e)) end)","exp")

	shell.parser_time_one("f(function (d,e) g(d,h()) end)","exp")
	shell.parser_time_one("f(function (e) g(d,h()) end)","exp")
	shell.parser_time_one("f(function () g(d,h()) end)","exp")

	shell.parser_time_one("f(function (d,e) g(d,h(e)) return e end)","exp")

	shell.parser_time_one("a()","exp")
	shell.parser_time_one("a(a())","exp")
	shell.parser_time_one("a(a(a()))","exp")
	shell.parser_time_one("a(a(a(a())))","exp")
	shell.parser_time_one("a(a(a(a(a()))))","exp")

	shell.parser_time_one("a.a.a.a.a","exp")
	shell.parser_time_one("a.a.a.a","exp")
	shell.parser_time_one("a.a.a","exp")
	shell.parser_time_one("a.a","exp")
	shell.parser_time_one("a","exp")
end

shell.token_count = 0
function shell.parser_time_one(s,p)
	local time = clock()
	shell.token_count = 0
	local num = getn(shell.parse_as(shell.tokenise(s),p,1))
	shell.print("Parsing "..s.." as "..p.." took "..(clock()-time).." seconds "
						.."returning "..num.." parses with "..shell.token_count.." token comparisons")
end

function shell.parser_decorate_tree(p)
	for i=1,getn(p.tree) do
		if i~=1 then p.tree[i].left = p.tree[i-1] end
		if i~=getn(p.tree) then p.tree[i].right = p.tree[i+1] end
		p.tree[i].up = p
		p.tree[i].index = i
		if (getn(p.tree[i].tree) > 0) then shell.parser_decorate_tree(p.tree[i]) end
	end
end

function shell.parser_find_rightmost_nonempty(p)
	local n = getn(p.tree)

	if (n == 0) then return p end

	for i=n,1,-1 do
		if not shell.parser_epsilon_node(p.tree[i]) then
			return shell.parser_find_rightmost_nonempty(p.tree[i])
		end
	end

	return p
end

function shell.parser_epsilon_node(p) 
	if next(p.tree,nil) == nil then return 1 
	else return nil end
end
	
function shell.parser_tree_fulltext(t)
	local text = ""
	for i=1,getn(t.tree) do
		text = text..t.tree[i].spacebefore
		if t.tree[i].tree.tok then
			text = text..t.tree[i].tree.text
		else
			text = text..shell.parser_tree_fulltext(t.tree[i])
		end
	end

	return text
end

	
