
do
	local key = function(s) return {["tok"]="keyword",["text"]=s} end
	local op = function(s) return {["tok"]="op",["text"]=s} end
	local name = {["tok"]="identifier"}
	local number = {["tok"]="number"}
	local string = {["tok"]="string"}

	shell.grammar = shell.parser_resolve_cyclic(
	{
		{"chunk",{
				{},
				{"stat","chunk"},
				{"stat",op(";"),"chunk"},
		}},
		{"block",{{"chunk"}}},
		{"stat",{
				{"varlist1",op("="),"explist1"},
				{"functioncall"},
				{key("do"),"block",key("end")},
				{key("while"),"exp1",key("do"),"block",key("end")},
				{key("repeat"),"block",key("until"),"exp1"},
				{key("if"),"exp1",key("then"),"block","elseif","else",key("end")},
				{key("return")},
				{key("return"),"explist1"},
				{key("break")},
				{key("for"),name,op("="),"exp1",op(","),"exp1",
					key("do"),"block",key("end")},
				{key("for"),name,op("="),"exp1",op(","),"exp1",op(","),"exp1",
					key("do"),"block",key("end")},
				{key("for"),name,op(","),name,key("in"),"exp1",
					key("do"),"block",key("end")},
				{key("function"),"funcname",op("("),"parlist1",op(")"),"block",key("end")},
				{key("function"),"funcname",op("("),op(")"),"block",key("end")},
				{key("local"),"declist"},
				{key("local"),"declist","init"},
		}},
		{"elseif",{
				{},
				{key("elseif"),"exp1",key("then"),"block","elseif"},
		}},
		{"else",{
				{},
				{key("else"),"block"},
		}},
		{"funcname",{
				{name},
				{name,op("."),name},
				{name,op(":"),name}
		}},
		{"varlist1",{
				{"var"},
				{"var",op(","),"varlist1"}
		}},
		{"varorfunc",{
				{"var"},
				{"functioncall"}
		}},
		{"declist",{
				{name},
				{name,op(","),"declist"}
		}},
		{"init",{
				{op("="),"explist1"},
		}},
		{"explist1",{
				{"exp"},
				{"exp",op(","),"explist1"}
		}},
		{"exp1",{{"exp"}}},
		--	{"exp",{
		--			{key("nil")},
		--			{number},
		--			{string},
		--			{"var"},
		--			{"function"},
		--			{"upvalue"},
		--			{"functioncall"},
		--			{"tableconstructor"},
		--			{op("("),"exp",op(")")},
		--			{"unop","exp"},
		--			{"exp","binop","exp"}
		--	}},
		{"exp",{
				{"simpleexp"},
				{"simpleexp","binop","exp"}
		}},
		{"simpleexp",{
				{key("nil")},
				{number},
				{string},
				{"var"},
				{"function"},
				{"upvalue"},
				{"functioncall"},
				{"tableconstructor"},
				{op("("),"exp",op(")")},
				{"unop","exp"}
		}},
		{"var",{
				{name},
				{"varorfunc",op("["),"exp",op("]")},
				{"varorfunc",op("."),name}
		}},
		{"functioncall",{
				{"varorfunc","args"},
				{"varorfunc",op(":"),name,"args"}
		}},
		{"args",{
				{op("("),op(")")},
				{op("("),"explist1",op(")")},
				{"tableconstructor"},
				{string}
		}},
		{"function",{
				{key("function"),op("("),"parlist1",op(")"),"block",key("end")},
				{key("function"),op("("),op(")"),"block",key("end")},
		}},
		{"parlist1",{
				{op("...")},
				{name},
				{name,op(","),"parlist1"},
		}},
		{"upvalue",{{op("%"),name}}},
		{"tableconstructor",{{op("{"),"fieldlist",op("}")}}},
		{"fieldlist",{
				{"lfieldlist"},
				{"ffieldlist"},
				{"lfieldlist",op(";"),"ffieldlist"},
				{"ffieldlist",op(";"),"lfieldlist"},
		}},
		{"lfieldlist",{
				{},
				{"exp"},
				{"exp",op(","),"lfieldlist"},
		}},
		{"ffieldlist",{
				{},
				{"ffield"},
				{"ffield",op(","),"ffieldlist"},
		}},
		{"ffield",{
				{op("["),"exp",op("]"),op("="),"exp"},
				{name,op("="),"exp"},
		}},
		{"binop",{
				{op("+")},
				{op("-")},
				{op("*")},
				{op("/")},
				{op("^")},
				{op("..")},
				{key("and")},
				{key("or")},
				{op("<")},
				{op("<=")},
				{op(">")},
				{op(">=")},
				{op("==")},
				{op("~=")},
		}},
		{"unop",{
				{op("-")},
				{key("not")}
		}},
	})


	shell.firsts = shell.parser_build_firsts(shell.grammar)
	shell.opt = shell.parser_build_optimised(shell.grammar)
end
