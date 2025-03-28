local is_using_InputListener = false
local gamepad_state
local InputListener

local GAMEPAD_BUTTON = {
    ["LB"] = 9,
        ["L1"] = 9,
    ["RB"] = 10,
        ["R1"] = 10,
    ["BACK"] = 4,
        ["SHARE"] = 4,
        ["CREATE"] = 4,
    ["START"] = 6,
        ["OPTIONS"] = 6,
    ["TOUCHPAD"] = 20,
        ["TOUCH_PAD"] = 20,
        ["PAD"] = 20,
        ["BOARD"] = 20,
    ["DPAD_UP"] = 11,
        ["UP"] = 11,
    ["DPAD_DOWN"] = 12,
        ["DOWN"] = 12,
    ["DPAD_LEFT"] = 13,
        ["LEFT"] = 13,
    ["DPAD_RIGHT"] = 14,
        ["RIGHT"] = 14,
    ["A"] = 0,
        ["CROSS"] = 0,
    ["B"] = 1,
        ["CIRCLE"] = 1,
    ["X"] = 2,
        ["SQUARE"] = 2,
    ["Y"] = 3,
        ["TRIANGLE"] = 3,
    ["LEFT_STICK_BUTTON"] = 7,
        ["LEFT_STICK"] = 7,
        ["L_STICK"] = 7,
    ["RIGHT_STICK_BUTTON"] = 8,
        ["RIGHT_STICK"] = 8,
        ["R_STICK"] = 8,
    ["HOME"] = 5,
        ["LOGO"] = 5,
        ["SONY"] = 5,
        ["XBOX"] = 5,
    ["MUTE"] = 15,
        ["VOICE"] = 15,
        ["VOICE_CHAT"] = 15,

    -- ["LEFT_STICK_X"] = 0,
    --     ["L_STICK_X"] = 0,
    -- ["LEFT_STICK_Y"] = 1,
    --     ["L_STICK_Y"] = 1,
    -- ["RIGHT_STICK_X"] = 2,
    --     ["R_STICK_X"] = 2,
    -- ["RIGHT_STICK_Y"] = 3,
    --     ["R_STICK_Y"] = 3,
    -- ["LT"] = 4,
    --     ["L2"] = 4,
    -- ["RT"] = 5,
    --     ["R2"] = 5,
}

local GAMEPAD_AXIS = {
    ["LEFT_STICK_X"] = 0,
        ["L_STICK_X"] = 0,
    ["LEFT_STICK_Y"] = 1,
        ["L_STICK_Y"] = 1,
    ["RIGHT_STICK_X"] = 2,
        ["R_STICK_X"] = 2,
    ["RIGHT_STICK_Y"] = 3,
        ["R_STICK_Y"] = 3,
    ["LT"] = 4,
        ["L2"] = 4,
    ["RT"] = 5,
        ["R2"] = 5,
}

local function initializeInputListener()
    local SDL3, err = package.loadlib("InputListenerC/SDL3.dll", "*") 
    if not SDL3 then
        print("Failed to load", "SDL3.dll", ", Error:", err)
        return false
    else
        print("SDL3.dll", "loaded successfully!")
    end

    local dll_path = "InputListenerC/InputListener.dll"
    local InputListener0, err = package.loadlib(dll_path, "luaopen_gamepadListener")
    print("InputListener0 type:", type(InputListener0))
    if not InputListener0 then
        print("Failed to load", dll_path, ", Error:", err)
        return false
    else
        print(dll_path, "loaded successfully!")
    end

    InputListener = InputListener0()
    if not InputListener then
        print("Failed to initialize InputListener module")
        return false
    end

    if InputListener.listenGamepad() then
        is_using_InputListener = true
        print("Gamepad listener started.")
        return true
    else
        print("Failed to start gamepad listener.")
        return false
    end
end

local function getGamepadState(arg)
    if is_using_InputListener then
        local state = InputListener.getGamepadState()

        return state
    end
end

local function checkGamepadButton(button)
    if type(button) == "string" then
        button = GAMEPAD_BUTTON[button]
    end
    if gamepad_state then
        return gamepad_state.buttons[button]
    end
end

local function checkGamepadAxis(axis)
    if type(axis) == "string" then
        axis = GAMEPAD_AXIS[axis]
    end
    if gamepad_state then
        return gamepad_state.axes[axis]
    end
end

local function checkAllGamepadButtons()
    if checkGamepadButton("A") then
        print("A is pressed")
    end

    if checkGamepadButton("B") then
        print("B is pressed")
    end

    if checkGamepadButton("X") then
        print("X is pressed")
    end

    if checkGamepadButton("Y") then
        print("Y is pressed")
    end

    if checkGamepadButton("LB") then
        print("LB is pressed")
    end

    if checkGamepadButton("RB") then
        print("RB is pressed")
    end

    if checkGamepadButton("BACK") then
        print("BACK is pressed")
    end

    if checkGamepadButton("START") then
        print("START is pressed")
    end

    if checkGamepadButton("TOUCHPAD") then
        print("TOUCHPAD is pressed")
    end

    if checkGamepadButton("DPAD_UP") then
        print("DPAD_UP is pressed")
    end

    if checkGamepadButton("DPAD_DOWN") then
        print("DPAD_DOWN is pressed")
    end

    if checkGamepadButton("DPAD_LEFT") then
        print("DPAD_LEFT is pressed")
    end

    if checkGamepadButton("DPAD_RIGHT") then
        print("DPAD_RIGHT is pressed")
    end

    if checkGamepadButton("LEFT_STICK_BUTTON") then
        print("LEFT_STICK_BUTTON is pressed")
    end

    if checkGamepadButton("RIGHT_STICK_BUTTON") then
        print("RIGHT_STICK_BUTTON is pressed")
    end

    if checkGamepadButton("HOME") then
        print("HOME is pressed")
    end

    if checkGamepadButton("MUTE") then
        print("MUTE is pressed")
    end
end

local function checkAllGamepadAxes()
    if checkGamepadAxis("L2") > 0 then
        print("L2: ", checkGamepadAxis("L2"))
    end

    if checkGamepadAxis("R2") > 0 then
        print("R2: ", checkGamepadAxis("R2"))
    end

    if checkGamepadAxis("L_STICK_X") ~= 0 or checkGamepadAxis("L_STICK_Y") ~= 0 then
        print("L_STICK_X: ", checkGamepadAxis("L_STICK_X"), "L_STICK_Y: ", checkGamepadAxis("L_STICK_Y"))
    end

    if checkGamepadAxis("R_STICK_X") ~= 0 or checkGamepadAxis("R_STICK_Y") ~= 0 then
        print("R_STICK_X: ", checkGamepadAxis("R_STICK_X"), "R_STICK_Y: ", checkGamepadAxis("R_STICK_Y"))
    end
end







if not initializeInputListener() then
    print("Failed to initialize InputListener module.")
    return
end

-- main loop , simulate on_frame()
-- which is called every frame in the game loop
local i = 0
for i=0,1000 do 
    gamepad_state = getGamepadState()

    checkAllGamepadButtons()
    -- checkAllGamepadAxes()

    if i == 300 then
        InputListener.pauseGamepadListener()
        print("Gamepad listener paused.")
    end

    if i == 600 then
        InputListener.resumeGamepadListener()
        print("Gamepad listener resumed.")
    end

    os.execute("sleep 0.016")  
end

InputListener.stop()

os.execute("sleep 1")






return {
    initializeInputListener = initializeInputListener,
    getGamepadState = getGamepadState,
    checkGamepadButton = checkGamepadButton,
    checkGamepadAxis = checkGamepadAxis,
    checkAllGamepadButtons = checkAllGamepadButtons,
    checkAllGamepadAxes = checkAllGamepadAxes,
}


