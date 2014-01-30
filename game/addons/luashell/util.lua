shell.util={}

-- join two arrays together

function shell.util_join_array(a,b) 
	local ret = {}

	for i=1,getn(a) do tinsert(ret,a[i]) end
	for i=1,getn(b) do tinsert(ret,b[i]) end

	return ret
end

-- check for elt in array

function shell.util_in_array(a,x)
	for i=1,getn(a) do if (a[i]==x) then return 1 end end
	return nil
end

-- deep copy

function shell.util_copy_of(s)
	local ret

	if (type(s) == "table") then 
		ret = {}
		for k,v in s do
			ret[k] = shell.util_copy_of(v)
		end
	else
		ret = s
	end
	
	return ret
end

