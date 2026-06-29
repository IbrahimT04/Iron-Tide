//
// Created by itari on 2025-11-06.
//

#ifndef IRON_TIDE_CAR_H
#define IRON_TIDE_CAR_H

#include <SDL3/SDL.h>
#include <cmath> // for std::cos and std::sin

class Car {
public:
    Car(int *win_x, int *win_y, float x, float y, float direction);
    Car(int *win_x, int *win_y, float x, float y);
    Car(int *win_x, int *win_y);

    // Update with delta time (seconds per frame)
    int update(double delta);
    int right_turn();
    int left_turn();
    int accelerate();
    int decelerate();
    int draw_car(SDL_Renderer* renderer);

private:
    // Movement parameters
    float pos_x = 200.0f;
    float pos_y = 200.0f;
    float dir = 0.0f;                 // direction in radians

    float speed = 0.0f;               // current velocity (pixels/second)
    float max_speed = 500.0f;         // max forward speed
    float car_power = 200.0f;          // acceleration power per second
    float turn_speed = 0.0025f * (M_PI / 180.0f); // turning speed in radians/frame
    float car_size = 30.0f;           // car's visual size

    int* window_x = nullptr;
    int* window_y = nullptr;

    SDL_FRect rect_car;
};

#endif // IRON_TIDE_CAR_H
