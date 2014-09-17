local count = 1
local args = {...}
if #args == 1 then
  count = args[1]
end

local function checkSupplies()
  return turtle.getItemCount(1) < 4 or
    turtle.getItemCount(2) < 3 or
    turtle.getFuelLevel() < 12
end

local function wither()
  turtle.select(1)
  for i=1,2 do
    turtle.up()
    turtle.place()
  end
  turtle.up()
  turtle.forward()
  turtle.turnLeft()
  turtle.forward()
  turtle.placeDown()
  turtle.back()
  turtle.select(2)
  turtle.place()
  turtle.select(1)
  turtle.back()
  turtle.placeDown()
  turtle.forward()
  turtle.turnLeft()
  turtle.turnLeft()
  turtle.select(2)
  turtle.place()
  turtle.turnLeft()
  turtle.back()
  turtle.place()
  for i=1,4 do turtle.down() end

  redstone.setOutput("front", true)
  sleep(50)
  redstone.setOutput("front", false)
end

-- todo: resupply, grab star, wait long enough to refil capacitor

for i=1,count do
  if checkSupplies() then
    print("Out of supplies")
    return
  else
    wither()
  end
end
