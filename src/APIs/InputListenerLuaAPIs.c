#include "InputListenerCore.h"
#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif



// static void log_debug(const char* message) {
//     FILE* f = fopen("reframework_plugin.log", "a");
//     if (f) {
//         fprintf(f, "%s\n", message);
//         fclose(f);
//     }
// }

// lua APIs start here.

// used by self to positivily print message in Lua.
EXPORT int lua_PrintMessage(lua_State *L, const char *print_msg) {
    lua_getglobal(L, "print");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        return luaL_error(L, "print function not found");
    }

    lua_pushstring(L, print_msg);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error calling print: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    return 0;
}

EXPORT int lua_getGamepadState(lua_State *L) {
    lua_newtable(L);
    GamepadState *tmp_gamepad_state;
    tmp_gamepad_state = getGamepadState();

    lua_pushstring(L, "buttons");
    lua_newtable(L);
    for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++) {
        lua_pushinteger(L, i);
        lua_pushboolean(L, tmp_gamepad_state->button_state[i]);
        lua_settable(L, -3);
    }
    lua_settable(L, -3);

    lua_pushstring(L, "axes");
    lua_newtable(L);
    for (int i = 0; i < SDL_GAMEPAD_AXIS_COUNT; i++) {
        lua_pushinteger(L, i);
        lua_pushinteger(L, tmp_gamepad_state->axis_value[i]);
        lua_settable(L, -3);
    }
    lua_settable(L, -3);

    return 1;
}

EXPORT int lua_listenGamepad(lua_State *L) {
    printf("InputListener getting started by lua...\n");
    lua_PrintMessage(L, "InputListener getting started by lua...");
    printf("InputListener getting started by lua...\n");
    lua_pushstring(L, "InputListener getting started by lua...\n");
    int execute = runGamepadListener();
    if(execute == -1){
        // printf("Error: %s\n", error_msg);
        // lua_pushstring(L, error_msg);
        lua_PrintMessage(L, error_msg);
        // return lua_error(L);
        return -1;
    }
    lua_pushboolean(L, 1);
    return 1;
}

EXPORT int lua_pauseGamepadListener(lua_State *L) {
    if(!getListenLoopTrigger()){
        lua_PrintMessage(L, "Invalid pause command! Gamepad listener already paused.");
        return -1;
    }
    pauseGamepadListener();
    lua_PrintMessage(L, "Gamepad listener paused.");
    return 0;
}

EXPORT int lua_resumeGamepadListener(lua_State *L) {
    if(getListenLoopTrigger()){
        lua_PrintMessage(L, "Invalid resume command! Gamepad listener already resumed.");
        return -1;
    }
    resumeGamepadListener();
    lua_PrintMessage(L, "Gamepad listener resumed.");
    return 0;
}

EXPORT int lua_stopGamepadListener(lua_State *L) {
    stopGamepadListener();
    // printf("Gamepad listener stopped.\n");
    // lua_pushstring(L, "Gamepad listener stopped.\n");
    lua_PrintMessage(L, "Gamepad listener stopped.");
    return 0;
}

EXPORT int luaopen_gamepadListener(lua_State *L) {
    // printf("[C Plugin] luaopen_InputListener called!\n");
    // log_debug("[C Plugin] luaopen_InputListener called!");

    lua_newtable(L);

    lua_pushcfunction(L, lua_getGamepadState);
    lua_setfield(L, -2, "getGamepadState");

    lua_pushcfunction(L, lua_listenGamepad);
    lua_setfield(L, -2, "listenGamepad");

    lua_pushcfunction(L, lua_pauseGamepadListener);
    lua_setfield(L, -2, "pauseGamepadListener");

    lua_pushcfunction(L, lua_resumeGamepadListener);
    lua_setfield(L, -2, "resumeGamepadListener");

    lua_pushcfunction(L, lua_stopGamepadListener);
    lua_setfield(L, -2, "stop");

    lua_pushvalue(L, -1);
    lua_setglobal(L, "InputListener");

    return 1;
}

// lua APIs end here.


