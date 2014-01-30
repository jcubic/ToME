shell.lexical = {
	{"keyword",
		{
			["match"] = "[%a_][%w_]*",
			["restrict"] = 
				{"and","break","do","else","elseif","end","for",
				"function","if","in","local","nil","not","or","repeat",
				"return","then","until","while"}
		}
	},
	{"op", 
		{"~=","<=",">=","<",">","==","=","+","-","*","/",
			"(",")","{","}","[","]",";",",","...","..",".","%"}
	},
	{"identifier",{["match"] = "[%a_][%w_]*"}},
	{"number",{["match"] = "%d+"}},
	{"string",{["fct"] =
			function (s,pos) 
					local first = strsub(s,pos,pos)
					if not (first == '"' or first == "'") then
						return FALSE
					else
						local i = pos+1
						while i <= strlen(s) do
							if strsub(s,i,i) == first then return true,i end
							if strsub(s,i,i) == "\\" then
								i = i+1 -- skip past char after backslash
							end
							i=i+1
						end
						return FALSE -- incomplete, actually
					end
				end
			}
		},
}

function shell.terminal(t) 
	local s = t.tok
	if (t.tok == "keyword" or t.tok == "op") then
		s = s..t.text
	end
	return s
end

function shell.tokenise(s,spaces)
	local pos = 1
	local tokens = {}

	while pos <= strlen(s) do 
		-- try to get next token

		local ret,newtoken

		ret,newtoken,pos = shell.tokenise_once(s,pos)

		if (ret == FALSE) then -- failed
			return nil
		else
			tinsert(tokens,newtoken)
			
			-- skip spaces after each token (hack?)
			
			local spstart,spend = strfind(s,"%s+",pos)

			if spstart == pos then
				pos = spend + 1
				if (spaces) then
				  tinsert(tokens,{["tok"]="space",["text"]=strsub(s,spstart,spend)})
				end
			end
		end
	end

	-- successful tokenise

	return tokens
end

function shell.tokenise_once(s,pos)
	for j=1,getn(shell.lexical) do
		local k,v = shell.lexical[j][1],shell.lexical[j][2]

		if v.match then -- token is an RE
			local mstart,mend = strfind(s,v.match,pos)

			if mstart == pos then -- only want match at beginning
				-- sometimes restrict to a set of literals also
				if not v.restrict or shell.util_in_array(v.restrict,strsub(s,mstart,mend)) then
					return true,{["tok"]=k,["text"]=strsub(s,mstart,mend)},mend+1
				end
			end
		elseif v.fct then -- token is defined by a function
			local ret,mend = v.fct(s,pos)

			if ret == true then
				return true,{["tok"]=k,["text"]=strsub(s,pos,mend)},mend+1
			end
		else -- token is array of literals
			for i = 1,getn(v) do
				if (strsub(s,pos,pos+strlen(v[i])-1) == v[i]) then
					return true,{["tok"]=k,["text"]=v[i]},pos+strlen(v[i])
				end
			end
		end
	end

	-- failed match

	return FALSE
end


