local function getItem()
  for i=1,16 do
    if turtle.getItemCount(i) > 0 then
      turtle.select(i)
      return
    end
  end
end

local canFindSpot = true

while canFindSpot do
  canFindSpot = false
  while turtle.back() do
    canFindSpot = true
    getItem()
    turtle.place()
  end
  turtle.turnRight()

--[[
  canFindSpot = false
  for i=1,3 do
    turtle.turnLeft()
    if not turtle.detect() then
      canFindSpot = true
      turtle.turnRight()
      turtle.turnRight()
      break
    end
  end
--]]
end

turtle.up()
getItem()
turtle.placeDown()
