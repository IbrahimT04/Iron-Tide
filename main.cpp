#include <cmath>
#include <cstdio>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>

#include "Car.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;


int win_w = WINDOW_WIDTH;
int win_h = WINDOW_HEIGHT;
auto car = Car(&win_w, &win_h, 600.0f, 600.0f);

bool key_w = false;
bool key_s = false;
bool key_a = false;
bool key_d = false;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    // 800x450 is 16:9
    if (SDL_CreateWindowAndRenderer("hello SDL3 with cmake", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    // return success!
    return SDL_APP_CONTINUE;
}

// This function runs when a new event occurs
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_EVENT_QUIT:
            // end the program, reporting success to the OS
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_DOWN:

            switch (event->key.key) {
                case SDLK_W: case SDLK_UP:      key_w = true; break;
                case SDLK_S: case SDLK_DOWN:    key_s = true; break;
                case SDLK_A: case SDLK_LEFT:    key_a = true; break;
                case SDLK_D: case SDLK_RIGHT:   key_d = true; break;
                case SDLK_ESCAPE: return SDL_APP_SUCCESS;
                default: break;
            }
            break;

        case SDL_EVENT_KEY_UP:

            switch (event->key.key) {
            case SDLK_W: case SDLK_UP:      key_w = false; break;
            case SDLK_S: case SDLK_DOWN:    key_s = false; break;
            case SDLK_A: case SDLK_LEFT:    key_a = false; break;
            case SDLK_D: case SDLK_RIGHT:   key_d = false; break;
            default: break;
            }
            break;

        default:
            break;
    }

    // return continue to continue
    return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void *appstate)
{
    static Uint64 last_time = 0;
    static Uint64 frame_count = 0;
    static double fps_timer = 0.0;
    static char title[128];

    const Uint64 now = SDL_GetPerformanceCounter();

    // Initialize last_time on first frame
    if (last_time == 0)
        last_time = now;

    const double delta = (now - last_time) / static_cast<double>(SDL_GetPerformanceFrequency());
    last_time = now;

    fps_timer += delta;
    frame_count++;

    if (fps_timer >= 1.0)
    {
        double fps = frame_count / fps_timer;
        snprintf(title, sizeof(title), "Iron Tide - FPS: %.1f", fps);
        SDL_SetWindowTitle(window, title);
        fps_timer = 0.0;
        frame_count = 0;
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Draw test rect
    SDL_FRect rect{100, 100, 400, 300};
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
    SDL_RenderFillRect(renderer, &rect);

    // Car controls
    if (key_w) car.accelerate();
    if (key_s) car.decelerate();
    if (key_a) car.left_turn();
    if (key_d) car.right_turn();

    car.update(delta);
    car.draw_car(renderer);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}


// This function runs once at shutdown
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // SDL will clean up the window/renderer for us.
}