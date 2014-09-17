local args = { ... }
if #args < 1 then
  print( "Usage: platform out [left]" )
  error()
end

local out = tonumber(args[1])

if out > 32 then
  print( "Turtle inventory too small!" )
  error()
end

local left = out
if #args > 1 then
  left = tonumber(args[2])
end

local function getItem()
  for i=1,16 do
    if turtle.getItemCount(i) > 0 then
      turtle.select(i)
      return
    end
  end
end

local function placeBlock()
  turtle.forward()
  getItem()
  turtle.placeDown()
end

for i=1,out do
  placeBlock()
end

turtle.turnLeft()
for i=1,left-1 do
  placeBlock()
end

turtle.turnLeft()
for i=1,out-1 do
  placeBlock()
end

local function turn(row)
  if row - math.floor(row/2)*2 > 0 then
    turtle.turnLeft()
  else
    turtle.turnRight()
  end
end

for i=1,left-2 do
  turn(i)
  placeBlock()
  turn(i)
  for j=1,out-2 do
    placeBlock()
  end
end

turtle.turnLeft()
turtle.forward()
turtle.turnRight()
turtle.forward()
turtle.turnRight()
turtle.turnRight()
