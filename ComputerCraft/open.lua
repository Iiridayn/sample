-- direction from pim to turtle
local direction = "north"

local pim = peripheral.wrap("front")
local inventory = peripheral.wrap("top")

local function findFirstEmpty(inventory)
  local size = inventory.getSizeInventory()
  for i=0,(size-1) do
    if not inventory.getStackInSlot(i) then
      return i
    end
  end
end

local function getBag()
  -- if we have a bag, use it first
  for i=1,16 do
    if turtle.getItemCount(i) > 0 then
      turtle.select(i)
      return i
    end
  end

  turtle.select(1)
  turtle.suckDown()
  if turtle.getItemCount(1) > 0 then
    return 1
  end
end

while true do
  local name = pim.getInvName()
  if name ~= "EmptyInventory" then
    local slot = findFirstEmpty(pim)
    if slot then
      local bagSlot = getBag()
      if bagSlot then
        -- cycle through player inventory
        pim.pullIntoSlot(direction, bagSlot-1, 1, slot)
        pim.pushIntoSlot(direction, slot, 1, bagSlot-1)

        -- right click into container
        turtle.placeUp()

        -- remove fake stone blocks by reorganizing
        inventory.condense()
      end
    end
  end
  sleep(0.25) -- same speed as export bus
end
