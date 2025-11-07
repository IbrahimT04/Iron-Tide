//
// Created by itari on 2025-11-06.
//

#include <SDL3/SDL.h>
#include "Car.h"
#include <cmath>

Car::Car(int *win_x, int *win_y, const float x, const float y, const float direction)
    : pos_x(x), pos_y(y), dir(direction), window_x(win_x), window_y(win_y) {}

Car::Car(int *win_x, int *win_y, const float x, const float y)
    : pos_x(x), pos_y(y), window_x(win_x), window_y(win_y) {}

Car::Car(int *win_x, int *win_y)
    : window_x(win_x), window_y(win_y) {}

int Car::update(double delta)
{
    // Move based on current speed and direction
    pos_x += std::sin(dir) * speed * delta;
    pos_y -= std::cos(dir) * speed * delta; // minus because y increases downward in SDL

    // Apply friction
    speed *= 0.98f;

    // Clamp position inside window bounds
    if (pos_x < car_size) pos_x = car_size;
    if (pos_y < car_size) pos_y = car_size;
    if (pos_x > *window_x - car_size) pos_x = *window_x - car_size;
    if (pos_y > *window_y - car_size) pos_y = *window_y - car_size;

    return 0;
}

int Car::right_turn()
{
    dir += turn_speed;
    return 0;
}

int Car::left_turn()
{
    dir -= turn_speed;
    return 0;
}

int Car::accelerate()
{
    speed += car_power * 0.016f;
    if (speed > max_speed) speed = max_speed;
    return 0;
}

int Car::decelerate()
{
    speed -= car_power;
    if (speed < -max_speed / 4.0f) speed = -max_speed / 4.0f; // limit reverse speed
    return 0;
}

int Car::draw_car(SDL_Renderer* renderer)
{
    rect_car.x = pos_x - car_size / 2.0f;
    rect_car.y = pos_y - car_size;
    rect_car.w = car_size;
    rect_car.h = car_size * 2.0f;

    SDL_SetRenderDrawColor(renderer, 255, 0, 100, 255);
    SDL_RenderFillRect(renderer, &rect_car);
    return 0;
}
