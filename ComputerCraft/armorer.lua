local pim = peripheral.wrap("top")
local stand = "south" -- direction from PIM

local function findFirstEmpty()
  local size = pim.getSizeInventory()
  for i=0,(size-1) do
    if not pim.getStackInSlot(i) then
      return i
    end
  end
end

local function equip()
  slot = findFirstEmpty()
  if not slot then
    return false
  end

  for i=0,3 do
    pim.swapStacks(39-i, slot)
    pim.pullIntoSlot(stand, i, 1, 39-i)
    pim.pushIntoSlot(stand, slot, 1, i)
  end

  return true
end

local equipped = false
while true do
  name = pim.getInvName()
  if not equipped and name ~= "EmptyInventory" then
    sleep(1) -- no accidental steps
    if name ~= "EmptyInventory" and equip() then
      equipped = true
    end
  elseif equipped and name == "EmptyInventory" then
    equipped = false
  end
  sleep(1)
end
