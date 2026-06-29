//
// Created by itari on 2025-11-20.
//

#ifndef IRON_TIDE_COMPONENTS_H
#define IRON_TIDE_COMPONENTS_H

// Common components
struct Position { float x, y; };
struct Rotation { float theta; };
struct Scale { float x, y; };

struct Render { uint8_t texture_id; };

struct Velocity { float x, y; };

// All Ships Local
struct StatusEffects{ bool burning, sinking; };
struct Health{ uint32_t max_health, current_health; };

// All Personal Ship Local
struct Physics{ float mass, inertia; };
struct Collider{ float radius; };

// Online and Personal ship stats
struct ShipStats{ uint32_t max_speed, turning_speed, acceleration; };
struct ResourcePool { uint32_t max_boast, current_boast; };
struct AttackPattern { uint8_t attack_id; };

// Online Only
struct Hitbox { float rad_out, rad_center, dist; };

// Bullet only
struct ShipId { uint8_t id; };

// Online Ship only
struct BulletEffects { uint32_t burning, breaking; };

#endif //IRON_TIDE_COMPONENTS_H