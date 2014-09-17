
valve = peripheral.wrap("back")

redstone.setBundledOutput("top", colors.red+colors.black)
tank = valve.getTanks("unknown")
low = tank[1].capacity*0.50
high = tank[1].capacity*0.75

while true do
  tank = valve.getTanks("unknown")

  value = 0
  if tank[1].amount > low then
    value = value + colors.black
  end
  if tank[1].amount < high then
    value = value + colors.red
  end

  redstone.setBundledOutput("top", value)

  sleep(60)
end
