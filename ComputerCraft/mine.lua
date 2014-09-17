local water = 1
local fresh = 2
local spent = 3
local charge = 4
local items = 5
local quarry = 16
local chest = 15

local function empower ()
  turtle.select(fresh)
  turtle.placeUp()
  turtle.suckUp()
  while turtle.getItemCount(fresh) < 1 do
    sleep(10)
    turtle.suckUp()
  end
  turtle.placeDown()
  turtle.digUp()
end

local function changePower ()
  turtle.forward()
  turtle.select(spent)
  turtle.placeUp()
  turtle.digDown()
  turtle.dropUp()
  turtle.digUp()
  empower()
  turtle.back()

  -- try to grab that one item we drop
  turtle.select(16)
  for i=1,4 do
    turtle.suck()
    turtle.turnLeft()
  end
end

local function buffer ()
  local q = peripheral.wrap("bottom")
  -- quarry done - 20 seconds
  local dry = false
  local count = 0
  while count < 20 do
    sleep(0)
    if tonumber(q.getEnergyStored()) <= 0 then
      changePower()
    end
    dry = true
    for i=items,16 do
      sleep(0)
      if turtle.getItemCount(i) > 0 then
        dry = false
        turtle.select(i)
        turtle.dropUp()
      end
    end
    if dry then
      count = count + 1
      sleep(1)
    else
      count = 0
    end
  end
end

local function recharge()
  turtle.select(charge)
  turtle.up()
  turtle.placeDown()

  local new = turtle.getFuelLevel()
  local fuel
  repeat
    fuel = new
    sleep(1)
    new = turtle.getFuelLevel()
  until fuel == new

  turtle.digDown()
  turtle.down()
end

local function setup ()
  -- quarry
  turtle.select(quarry)
  turtle.placeDown()

  -- power
  turtle.turnRight()
  turtle.forward()
  empower()
  turtle.back()
  turtle.turnLeft()

  -- water
  turtle.select(water)
  for i=1,3 do turtle.up() end
  if turtle.detectUp() then
    turtle.digUp()
  end
  turtle.up()
  for i=1,5 do
    if turtle.detect() then
      turtle.dig()
    end
    turtle.forward()
  end
  turtle.place()
  for i=1,5 do turtle.back() end
  for i=1,4 do turtle.down() end

  -- turtle buffer
  turtle.select(chest)
  turtle.placeUp()
  turtle.turnRight()
end

local function teardown ()
  turtle.forward()
  local cell = peripheral.wrap("bottom")
  if turtle.getFuelLevel() < 10000 and tonumber(cell.getEnergyStored()) > 10000 then
    recharge()
  end
  -- remove spent cell
  turtle.select(spent)
  turtle.placeUp()
  turtle.digDown()
  turtle.dropUp()
  turtle.digUp()

  turtle.back()
  turtle.turnLeft()

  turtle.select(quarry)
  turtle.digDown()
  turtle.select(chest)
  turtle.digUp()

  -- water
  for i=1,6 do sleep(0) turtle.forward() end
  for i=1,3 do sleep(0) turtle.up() end
  turtle.select(water)
  turtle.placeUp()

  -- move into position for the next
  for i=1,3 do sleep(0) turtle.down() turtle.forward() end
end

-- if starting up in buffer mode
-- handle it and continue
local args = { ... }
if turtle.getItemCount(5) > 0 then
  buffer()
  teardown()
--  exit()
elseif #args < 1 or args[1] ~= "start" then
  print("I'm lost!")
  return
end

while true do
  setup()
  -- TODO - find better method than timeout
  sleep(30)
  buffer()
  teardown()
end
