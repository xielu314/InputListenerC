# Version 1.0.000

# Execution OS
Windows10 or 11. Old versions of Windows have not been tested. Linux or MacOS have not been tested.

# Execution for test
You may use InputListenerLua.lua to test the features of this application, which the 5.4.2 or newer Lua is needed.
Or you can write another test program(script) in other languages by yourself.
You may execute InputListenerLua.lua while the gamepad accessed or not. 
When you execute, the script will last about 16 seconds, defaultly only listen to the buttons but no axes.
If you want it to listen to the axes, uncomment checkAllGamepadAxes() in main loop.
In about 5 second, the listener will be paused, and will be resumed in another 5 seconds. If you don't like this, comment them in main loop.

# APIs
Only Lua API for now. 
I tried to develop APIs for REFramework, a MOD framework for RE engine, but there are always compatibility issues.
Check the luaopen_gamepadListener(lua_State *L){} in .\src\APIs\InputListenerLuaAPIs.c for Lua APIs.

# GUI
No GUI for now. Maybe tomorrow :D.

# Use in independent environment
Make sure InputListenerLua.lua and folder /InputListenerC are in the same directory.
If you want the "InputListenerC" folder be in another place, or you want to develop a test script of yourselfs', fix the path of package.loadlib().

# Project structure
All source code are in /src and /include. In /src, the core features are wrote in InputListenerCore.c, and the APIs are wrote in /APIs.
Mixing the code of core features and APIs unrecommended.

# Dependencies of execution
Only SDL3.dll needed in the case of no Lua script for test. And which is integrated in /InputListenerC.

# Compile
You need SDL3 and Lua 5.4.2 includes and libs to compile. Also, indeed, the C compiler and cmake. Fix the paths in CMakeListes.txt.
64 bit includes and libs and compiler recommended. 32 bit version has not been tested.
Powershell Compile.ps1 to start the building. It will be automatically copied to /InputListenerC, so you may Lua the test script right after the compilation.
No script for Linux or MacOS, so you need to write it by yourself in this case.
