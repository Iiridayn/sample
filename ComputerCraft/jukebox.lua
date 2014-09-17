local titles = {
}
--[[
  -- vanilla
  2256="13",
  2257="cat",
  2258="blocks",
  2259="chirp",
  2260="far",
  2261="mall",
  2262="mellohi",
  2263="stal",
  2264="strad",
  2265="ward",
  2266="11",
  2267="wait",
  -- PortalGun mod
  13657="StillAlive",
  13658="RadioLoop",
  13659="WantYouGone",
}
--]]

local box = peripheral.wrap("top")
local monitor = peripheral.wrap("bottom")
local disk = peripheral.wrap("front")
-- get width/height
local w,h = monitor.getSize()

local function findFirstEmpty()
  local size = box.getSizeInventory()
  for i=0,(size-1) do
    if not box.getStackInSlot(i) then
      return i
    end
  end
end

local function unloadDisk()
  -- suck and drop disk if one is there
  turtle.suck()
  if turtle.getItemCount(1) then
    turtle.dropUp()
  end
end

local function loadDisk(slot)
  unloadDisk()

  -- suck from slot 0, restore
  box.swapStacks(0,slot)
  turtle.suckUp()
  box.swapStacks(0,slot)

  -- put into drive
  turtle.drop()
end

local function scanDisks()
  local disks = {}
  box.condense() -- no empty slots

  for i=0,(findFirstEmpty()-1) do
    local item = box.getStackInSlot(i)
    if item.name == "Music Disc" then
      local name
      if titles[item.id] then
        name = titles[item.id]
      else
        loadDisk(i)
        name = string.sub(disk.getAudioTitle(), 8)
      end
      disks[i+1] = name
    end
  end

  -- loadDisk unloads, so just one last time
  unloadDisk()

  return disks
end

local disks
-- TODO - mark selected
local function printMenu()
  monitor.clear()
  local count = 0

  w = w/9 - 1 -- 8 chars + space per screen

  for slot, name in pairs(disks) do
    local x = 1+math.floor(count/h)*9
    local y = 1+count%h
    count = count+1

    monitor.setCursorPos(x, y)
    monitor.write(string.sub(name,1,8))
  end
end

-- flush inventory
for i=1,16 do
  if turtle.getItemCount(i) > 0 then
    turtle.select(i)
    turtle.dropUp()
  end
end

turtle.select(1)
unloadDisk()

monitor.clear()
disks = scanDisks()
printMenu()

while true do
  local event, side, x, y = os.pullEvent("monitor_touch")
  local record = math.floor(x/9)*h+y
  print("Playing "..disks[record])
  loadDisk(record-1)
  disk.playAudio()
end
