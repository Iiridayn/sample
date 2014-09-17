turtle.select(1)

while true do
  local cell = peripheral.wrap("front")

  if cell and not (cell.getMaxEnergyStored() > tonumber(cell.getEnergyStored())) then
    cell = nil
    turtle.dig()
    turtle.dropUp()
  end

  if not cell and turtle.suckDown() then
    turtle.place()
  end

  sleep(10)
end
