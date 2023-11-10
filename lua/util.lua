
CreateEnum = function (tal, index)
  local e_table = {}
  local enum_index = index or 0
  for i, v ipairs(tal) do
    e_table[v] = enum_index + i
  end
  return e_table
end
