----------------------------------------------
-- Pickle.lua
-- An table serialization utility for lua 4
-- Steve Dekorte, http://www.dekorte.com, April 2000
-- Public Domain
----------------------------------------------

declare_globals
{
	"pickle",
	"unpickle",
	"Pickle",
}

function pickle(t)
  return Pickle:clone():pickle_(t)
end

Pickle = {
  clone = function (t) local nt={}; for i, v in t do nt[i]=v end return nt end
}

function Pickle:pickle_(root)
  if type(root) ~= "table" then
    error("can only pickle tables, not ".. type(root).."s")
  end
  self._tableToRef = {}
  self._refToTable = {}
  local savecount = 0
  self:ref_(root)
  local s = ""

  while getn(self._refToTable) > savecount do
    savecount = savecount + 1
    local t = self._refToTable[savecount]
    s = s.."{\n"
    for i, v in t do
        s = format("%s[%s]=%s,\n", s, self:value_(i), self:value_(v))
    end
    s = s.."},\n"
  end

  return format("{%s}", s)
end

function Pickle:value_(v)
  local vtype = type(v)
  if     vtype == "string" then return format("%q", v)
  elseif vtype == "number" then return v
  elseif vtype == "table" then return "{"..self:ref_(v).."}"
  else --error("pickle a "..type(v).." is not supported")
  end
end

function Pickle:ref_(t)
  local ref = self._tableToRef[t]
  if not ref then
    if t == self then error("can't pickle the pickle class") end
    tinsert(self._refToTable, t)
    ref = getn(self._refToTable)
    self._tableToRef[t] = ref
  end
  return ref
end

----------------------------------------------
-- unpickle
----------------------------------------------

function unpickle(s)
  local tables = s
  if type(s) == "string" then
	tables = dostring("return "..s)
  end

  for tnum = 1, getn(tables) do
    local t = tables[tnum]
    local tcopy = {}; for i, v in t do tcopy[i] = v end
    for i, v in tcopy do
      local ni, nv
      if type(i) == "table" then ni = tables[i[1]] else ni = i end
      if type(v) == "table" then nv = tables[v[1]] else nv = v end
      t[ni] = nv
    end
  end
  return tables[1]
end
