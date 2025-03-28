#ifndef GAMEPAD_CORE_H
#define GAMEPAD_CORE_H

#include <SDL3/SDL.h>
// #include <windows.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

typedef struct {
    int button_state[SDL_GAMEPAD_BUTTON_COUNT];  
    int axis_value[SDL_GAMEPAD_AXIS_COUNT];  
} GamepadState;

static GamepadState gamepad_state;
static SDL_Gamepad *gamepad = NULL;
static char msg[1000] = "";
static char error_msg[1000] = "";
static int listen_loop_trigger = 1;
static int listen_stop_trigger = 0;
static int gamepad_connection = 0;

EXPORT int luaopen_gamepadListener(lua_State *L);
EXPORT int lua_listenGamepad(lua_State *L);

void *listenLoopThread(void *arg);
void *listenDeviceEventsThread(void *arg);

int getGamepadConnection();
void setGamepadConnectionOn();
void setGamepadConnectionOff();

int getListenLoopTrigger();
int getListenStopTrigger();
void setListenLoopTriggerOn();
void setListenLoopTriggerOff();
void setListenStopTriggerOn();

int initGamepad();
int listenGamepad();
void pauseGamepadListener();
void resumeGamepadListener();
int runGamepadListener();
void stopGamepadListener();

void updateGamepadState();
GamepadState* getGamepadState();
void zeroizeGamepadState();

#endif