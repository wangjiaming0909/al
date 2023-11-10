require('util')
local gdb_state = {'INITIAL', 'STARTED', 'STOPPED'}
GDBState = CreateEnum(gdb_state)
GDBStateInfo = {GDBState = nil}
GDBIntance = {id = 0, pid = 0, GDBState = nil}

local function find_gdb()
  local res = os.execute('which gdb')
  if res ~= 0 then
    return false
  end
  return true
end

PreCheck = function ()
  if not find_gdb() then
    print('gdb not found')
    return false
  end
end

local function open_tty()
  return 11
end
RunGDB = function (params)
  local gdb_arg = " --interpreter=mi3 -q"
  return 1
end

StopGDB = function()
  return 1
end
