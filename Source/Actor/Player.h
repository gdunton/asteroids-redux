//-----------------------------------------------------------
// File: Player.h
// Description: Player inherits PhysicsObject with functionality
//				for controls and lives
//-----------------------------------------------------------

#ifndef PLAYER_H
#define PLAYER_H

#include "ActorBase.h"
#include "../Graphics/Camera.h"
#include "Bullet.h"
#include "../EventManager/AnimationClock.h"

class AudioManager;
class Channel;
class Model2D;

const int MAX_BULLETS = 5;

class Player : public ActorBase
{
public:
	Player();
	Player(const Vector2& pos, const Vector2& size, float rot,
	       const Model2D& model, const Vector2& velocity, float mass,
	       AudioManager* audioManager);

	void Update(float dt) override;
	void Render(const Camera& camera) const override;

	// Override for the collision circle accessor
	MathTypes::Circle GetCircle() const override;

	// Accessor for the bullet list
	Bullet* GetBullets();

	// Contol functions
	void TurnClockwise(float dt);
	void TurnAntiClockwise(float dt);
	void Boost(float dt);
	void EndBoost();
	void FireBullet();

	// Life related functions
	void RemoveLife();
	void AddLife();
	bool Alive() const;
	void KillPlayer();

	// Functions to reset the values on the player
	void Reset();
	void ResetWorld();

	// Sets the invulnerability flag and starts the animation
	void StartInvulnerability();

	// Stops any associated sounds
	void StopSounds();

	static float GetForceValue() { return FORCE_VALUE; }
	int GetLives() const { return lives; }

	bool GetInvulnerable() const { return invulnerable; }

private:

	void TickInvulnerability(float dt);

	// Constant for movement
	static const float FRICTION_FACTOR;
	static const float ACCELERATION;
	static const float FORCE_VALUE;
	static const float FIRING_COOLDOWN;
	static const float TURNING_SPEED;

	static const float DAMAGE_COOLDOWN_SECONDS;

	static const int STARTING_LIVES;

	Bullet bulletsArray[MAX_BULLETS];

	float bulletCooldown = 0;

	int lives = 0;

	// Variables for resetting the player
	World startingWorld;
	Vector2 startingVelocity;

	// Variables for invulnerability 
	bool invulnerable = false;
	AnimationClock invulnerabilityClock;
	AnimationClock pulseClock;
	float pulseSeconds = 0;
	float alpha = 0;

	Channel* thrustSound;
	bool playerBoosting = false;

	AudioManager* audioManager = nullptr;
};

#endif
