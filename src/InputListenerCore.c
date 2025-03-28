#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <SDL3/SDL.h>
#include "InputListenerCore.h"

// static void log_debug(const char* message) {
//     FILE* f = fopen("reframework_plugin.log", "a");
//     if (f) {
//         fprintf(f, "%s\n", message);
//         fclose(f);
//     }
// }



int getListenLoopTrigger(){
    return listen_loop_trigger;
}

int getListenStopTrigger(){
    return listen_stop_trigger;
}

void setListenLoopTriggerOn(){
    listen_loop_trigger = 1;
}

void setListenLoopTriggerOff(){
    listen_loop_trigger = 0;
}

void setListenStopTriggerOn(){
    listen_stop_trigger = 1;
}

int getGamepadConnection(){
    return gamepad_connection;
}

void setGamepadConnectionOn(){
    gamepad_connection = 1;
}

void setGamepadConnectionOff(){
    gamepad_connection = 0;
}

void zeroizeGamepadState(){
    for(int i=0; i<SDL_GAMEPAD_BUTTON_COUNT; i++){
        gamepad_state.button_state[i] = 0;
    }
    for(int i=0; i<SDL_GAMEPAD_AXIS_COUNT; i++){
        gamepad_state.axis_value[i] = 0;
    }
}

void printGamepadState(){
    for(int i=0; i<SDL_GAMEPAD_BUTTON_COUNT; i++){
        if(gamepad_state.button_state[i]){
            printf("Button %d\n", i);
        }
    }
    for(int i=0; i<SDL_GAMEPAD_AXIS_COUNT; i++){
        if(gamepad_state.axis_value[i] > 0){
            printf("Axis %d = %d\n", i, gamepad_state.axis_value[i]);
        }
    }
}

void updateGamepadState() {
    for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++) {
        gamepad_state.button_state[i] = SDL_GetGamepadButton(gamepad, i);
    }
    for (int i = 0; i < SDL_GAMEPAD_AXIS_COUNT; i++) {
        gamepad_state.axis_value[i] = SDL_GetGamepadAxis(gamepad, i);
    }
}

GamepadState* getGamepadState() {
    return &gamepad_state;
}

/**
 * Initializes the gamepad subsystem and attempts to connect to the first available gamepad.
 *
 * @return int 
 *         - Returns 0 if the initialization and connection are successful.
 *         - Returns -1 if an error occurs during initialization or if no gamepads are detected.
 *
 * The function performs the following steps:
 * 1. Checks if the SDL gamepad subsystem is already initialized. If not, it initializes it.
 *    - If SDL initialization fails, the error message is stored in `error_msg` and -1 is returned.
 * 2. Retrieves the list of connected gamepads.
 *    - If no gamepads are detected, an error message is stored in `error_msg` and -1 is returned.
 * 3. Attempts to open the first detected gamepad.
 *    - If opening the gamepad fails, the error message is stored in `error_msg` and -1 is returned.
 *    - If successful, a success message is stored in `msg` containing the name of the connected gamepad.
 * 4. Frees the memory allocated for the list of gamepads.
 *
 * Note:
 * - The function uses SDL2's gamepad API for initialization and management.
 * - The global variables `error_msg` and `msg` are used to store error and success messages, respectively.
 */
int initGamepad(){
    int execute = 0;

    if (SDL_WasInit(SDL_INIT_GAMEPAD) == 0) {  // **如果 SDL 没有初始化，才初始化**
        if (SDL_Init(SDL_INIT_GAMEPAD) < 0) {
            // log_debug("[C Plugin] SDL_Init failed: ");
            // log_debug(SDL_GetError());
            strcpy(error_msg, SDL_GetError());
            return -1;
        }
    } else {
        // log_debug("[C Plugin] SDL already initialized by the game/REF.");
    }

    int count;
    SDL_JoystickID *gamepads = SDL_GetGamepads(&count);
    if (!gamepads || count < 1) {
        // printf("ERROR: No gamepads detected.\n");
        // log_debug("ERROR: No gamepads detected.");
        strcpy(error_msg, "No gamepads detected.");
        // SDL_Quit();
        execute = -1;
    }

    gamepad = SDL_OpenGamepad(gamepads[0]);
    if (!gamepad) {
        // printf("ERROR: %s\n", SDL_GetError());
        strcpy(error_msg, SDL_GetError());
        // SDL_Quit();
        execute = -1;
    }
    else{
        strcpy(msg, "CONNECTED: ");	
        strcat(msg, SDL_GetGamepadNameForID(gamepads[0]));
        fflush(stdout);
    }
    
    

    SDL_free(gamepads);
    return execute;
}

int listenGamepad() {
    int execute = initGamepad();
    if(execute){
        setGamepadConnectionOn();
    }
    
    return execute;
}

/**
 * @brief Thread function that continuously listens for gamepad input and updates the gamepad state.
 *
 * This function runs in an infinite loop, checking for gamepad connections and triggers to update
 * the gamepad state. It also listens for a stop trigger to terminate the loop.
 *
 * @param arg A pointer to any arguments passed to the thread function (unused in this implementation).
 * @return Always returns NULL upon exiting the loop.
 *
 * The loop performs the following actions:
 * - Checks if a gamepad is connected using `getGamepadConnection()`.
 * - If connected and the listen loop trigger is active (`getListenLoopTrigger()`), it updates the
 *   gamepad state by calling `SDL_UpdateGamepads()` and `updateGamepadState()`.
 * - Checks if the stop trigger (`getListenStopTrigger()`) is active, and if so, exits the loop.
 * - Delays execution for 5 milliseconds using `SDL_Delay(5)` to reduce CPU usage.
 *
 * Note: Debugging print statements are commented out in the current implementation.
 */
void *listenLoopThread(void *arg) {
    while(1){
        if(getGamepadConnection()){
            if(getListenLoopTrigger()){
                SDL_UpdateGamepads();
                updateGamepadState(); 
                // printGamepadState();
            }
        }
        if(getListenStopTrigger()){
            // printf("listen_stop triggered.\n"); // debug
            break;
        }
        SDL_Delay(5);
    }
    // printf("listenLoopThread stopped.\n"); // debug
    return NULL;
}

/**
 * @brief Thread function to listen for gamepad connection and disconnection events.
 *
 * This function continuously monitors the state of connected gamepads using SDL's
 * gamepad API. It detects when a gamepad is connected or disconnected and triggers
 * appropriate actions to handle these events.
 *
 * @param arg A pointer to any arguments passed to the thread (unused in this function).
 * @return Always returns NULL.
 *
 * The function performs the following steps in an infinite loop:
 * 1. Retrieves the current list of connected gamepads and their count.
 * 2. Compares the current count of gamepads with the previous count (`past_count`).
 * 3. If the count decreases, it assumes a gamepad was disconnected and:
 *    - Prints a debug message.
 *    - Calls `setGamepadConnectionOff()` to update the connection state.
 *    - Calls `zeroizeGamepadState()` to reset the gamepad state.
 * 4. If the count increases, it assumes a gamepad was connected and:
 *    - Prints a debug message.
 *    - Calls `SDL_UpdateGamepads()` to refresh the gamepad state.
 *    - Calls `initGamepad()` to initialize the new gamepad.
 *    - Calls `setGamepadConnectionOn()` to update the connection state.
 * 5. Updates `past_count` with the current count.
 * 6. Delays for 20 milliseconds before repeating the loop.
 *
 * Note: This function assumes SDL is properly initialized and that the gamepad
 *       API is supported on the platform.
 */
void *listenDeviceEventsThread(void *arg) {
    // printf("listenDeviceEventsThread activated.\n"); // debug
    int past_count = 0;
    while (1) {
        int count;
        SDL_JoystickID *gamepads = SDL_GetGamepads(&count);
        // printf("%d", count); // debug
        SDL_free(gamepads); 
        if (past_count > count) {
            printf("Gamepad disconnected!\n"); // debug
            setGamepadConnectionOff(); 
            zeroizeGamepadState();
        } else if (past_count < count) {
            printf("Gamepad reconnected!\n"); // debug
            SDL_UpdateGamepads();
            initGamepad();
            setGamepadConnectionOn();
        }

        past_count = count;
        SDL_Delay(20);
    }
    return NULL;
}

/**
 * @brief Starts the gamepad listener by initializing necessary threads and handling gamepad events.
 *
 * This function sets up the environment for listening to gamepad inputs and device events.
 * It creates two separate threads: one for listening to gamepad inputs and another for
 * handling device events. The function also ensures that the gamepad state is initialized
 * and ready for use.
 *
 * @return int Returns 0 on success, or -1 if there is an error during thread creation or execution.
 *
 * @note The function uses SDL to allow joystick events in the background and initializes
 *       the gamepad state before starting the threads.
 *
 * @error If thread creation fails, an error message is printed to the console, and the
 *        function returns -1.
 */
int runGamepadListener(){
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    zeroizeGamepadState();
    pthread_t listen_thread;
    int err = pthread_create(&listen_thread, NULL, listenLoopThread, NULL);
    if (err != 0) {
        printf("Error: Failed to create listenLoopThread.\n");
        strcpy(error_msg, "Error: Failed to create listenLoopThread.\n");
        return -1;
    }
    pthread_detach(listen_thread);

    pthread_t events_thread;
    err = pthread_create(&events_thread, NULL, listenDeviceEventsThread, NULL);
    if (err != 0) {
        printf("Error: Failed to create listenLoopThread.\n");
        strcpy(error_msg, "Error: Failed to create listenDeviceEventsThread.\n");
        return -1;
    }
    pthread_detach(events_thread);

    int execute = listenGamepad();
    if(execute == -1){
        printf("%s", error_msg);
        // return -1;
    }

    return 0;
}

void pauseGamepadListener(){
    setListenLoopTriggerOff();
    zeroizeGamepadState();
}

void resumeGamepadListener(){
    setListenLoopTriggerOn();
}

void stopGamepadListener(){
    setListenLoopTriggerOff();
    setListenStopTriggerOn();
    SDL_CloseGamepad(gamepad);
    SDL_Quit();
    // printf("Gamepad listener stopped.\n");
    strcpy(msg, "Gamepad listener stopped.\n");
}



// Core features(logics) end here. Don't write them after this line.






// PS5 Controller
// Buttons
// 
// L1 9
// R1 10
// create(share) 4
// start 6
// board_L 20
// board_R 20
// L_up 11
// L_left 13
// L_down 12
// L_right 14
// R_triangle 3
// R_square 2
// R_cross 0
// R_circle 1
// joystick_L_button 7
// joystick_R_button 8
// Sony_mark 5
// voice_chat 15
// 
// Axis
// 
// L2 4
// R2 5
// joystick_L_x 0
// joystick_L_y 1
// joystick_R_x 2
// joystick_R_y 3
// 
// for joysticks: left and up are minus, right and down are plus.
