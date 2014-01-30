-- This file takes care of manipulating lua bytecode

bytecode.merge_functions = function(...)
	local codes = arg
	return function(...)
		local codes = %codes
		for i = 1, getn(codes) do
			local fct = bytecode.load(codes[i])
			call(fct, arg)
		end
	end
end
