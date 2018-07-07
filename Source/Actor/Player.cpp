//-----------------------------------------------------------
// File: Player.cpp
//-----------------------------------------------------------

#include "Player.h"

#include "../Graphics/Model2D.h"
#include "../Content/AudioManager.h"


// Initialize the static variables
const float Player::FRICTION_FACTOR = 0.975f;
const float Player::ACCELERATION = 100.0f;
const float Player::FORCE_VALUE = 100.0f;
const float Player::FIRING_COOLDOWN = 0.2f;

const float Player::TURNING_SPEED = 1.3f;

const int Player::STARTING_LIVES = 3;

const float Player::DAMAGE_COOLDOWN_SECONDS = 2;

Player::Player() noexcept : 
	thrustSound(nullptr)
{
	lives = STARTING_LIVES;
	startingWorld = World();
	startingVelocity = Vector2(0, 0);
}

Player::Player(const Vector2& pos, const Vector2& size, float rot,
               const Model2D& model, const Vector2& velocity, float mass,
               AudioManager* audioManager) noexcept :
	ActorBase(pos, size, rot, model, velocity, mass), audioManager(audioManager)
{
	bulletCooldown = 0;
	lives = STARTING_LIVES;

	startingWorld = World(pos, size, rot);
	startingVelocity = velocity;

	invulnerable = false;
	pulseSeconds = 0.2f;
	alpha = 255.0f;

	thrustSound = nullptr;
	playerBoosting = false;
}

void Player::Update(float dt)
{
	if(Alive())
	{
		if(invulnerable)
		{
			// Update the invulnerability animation
			TickInvulnerability(dt);
		}

		velocity *= FRICTION_FACTOR;
		world.pos += velocity * dt;

		WrapPosition();
	}

	// Update all of the bullets that are alive
	for(auto& i : bulletsArray)
	{
		i.Update(dt);
	}

	// Decrease the bulletCooldown
	if(bulletCooldown > 0)
	{
		bulletCooldown -= dt;
	}
}

void Player::Render(const Camera& camera) const
{
	if(Alive())
	{
		ActorBase::Render(camera);
	}
}

void Player::TurnClockwise(float dt)
{
	world.rot += PI * TURNING_SPEED * dt;

	// Ensure the rotation wraps around 360
	// Shift into the positive space
	world.rot += 2 * PI;
	// Mod to leave in [0-2PI] space
	world.rot = fmod(world.rot, 2 * PI);
}

void Player::TurnAntiClockwise(float dt)
{
	world.rot -= PI * TURNING_SPEED * dt;

	// Ensure the rotation wraps around 360
	// Shift into the positive space
	world.rot += 2 * PI;
	// Mod to leave in [0-2PI] space
	world.rot = fmod(world.rot, 2 * PI);
}

void Player::Boost(float dt)
{
	Vector2 direction(0, 1);

	direction = RotatePoint(direction, world.rot);
	velocity += direction * ACCELERATION * dt;

	// Flip to indicate boosting for starting the boost sound
	if(!playerBoosting)
	{
		playerBoosting = true;
		thrustSound = audioManager->PlaySoundByName("Thrust");
	}
}

// Function to stop the boost sound if one is playing
void Player::EndBoost()
{
	if(playerBoosting)
	{
		playerBoosting = false;
		if(thrustSound)
		{
			thrustSound->Stop();
			thrustSound = nullptr;
		}
	}
}

void Player::FireBullet()
{
	// Make sure that the cooldown is not 0
	if(bulletCooldown > 0)
	{
		return;
	}

	// Search for a bullet that isn't alive
	for(auto& i : bulletsArray)
	{
		if(!i.IsAlive()) // bullet not alive
		{
			// Get the player direction
			Vector2 direction(0, 1);
			direction = RotatePoint(direction, world.rot);
			// Create new bullet
			i = Bullet(world.pos, Vector2(1, 1), world.rot,
			                         model,
			                         direction * Bullet::FIRING_SPEED);

			// Reset the cooldown
			bulletCooldown = FIRING_COOLDOWN;

			// Play the fire sound
			audioManager->PlaySoundByName("FireAlt");

			// Quit loop to stop firing bullets
			break;
		}
	}
}

Bullet* Player::GetBullets()
{
	return bulletsArray;
}

// Override the default
MathTypes::Circle Player::GetCircle() const
{
	return MathTypes::Circle(world.pos, model.GetModelRadius() * world.scale.Length());
}

void Player::RemoveLife()
{
	if(!invulnerable) // may be unncessary but this will stay
	{
		lives--;

		// Play collision sound
		audioManager->PlaySoundByName("Bang3");
	}
}

void Player::AddLife()
{
	lives++;
}

bool Player::Alive() const
{
	return lives > 0;
}

void Player::KillPlayer()
{
	lives = 0;
}

void Player::StartInvulnerability()
{
	invulnerable = true;
	invulnerabilityClock.Start();
	pulseClock.Start();
}

void Player::StopSounds()
{
	if(thrustSound)
	{
		thrustSound->Stop();
		thrustSound = nullptr;
	}
}

// Resets all member variables including lives
void Player::Reset()
{
	ResetWorld();
	lives = STARTING_LIVES;
}

// Reset all member variables excluding lives
void Player::ResetWorld()
{
	world = startingWorld;
	velocity = startingVelocity;

	invulnerabilityClock.Stop();
	invulnerable = false;
	pulseClock.Stop();
	pulseSeconds = 0.2f;
	alpha = 255.0f;

	if(thrustSound)
	{
		thrustSound->Stop();
		thrustSound = nullptr;
	}

	// Need to remove bullets between levels
	for(auto& i : bulletsArray)
	{
		i.KillBullet();
	}
}

void Player::TickInvulnerability(float dt)
{
	enum InvulState { inPulse, pulseComplete, stopped };
	InvulState state;
	invulnerabilityClock.Tick(dt);
	pulseClock.Tick(dt);

	// Get the correct state
	if(invulnerabilityClock.GetDeltaTime() >= DAMAGE_COOLDOWN_SECONDS)
	{
		if(pulseClock.GetDeltaTime() >= pulseSeconds)
		{
			state = stopped;
		}
		else
		{
			state = inPulse;
		}
	}
	else
	{
		if(pulseClock.GetDeltaTime() >= pulseSeconds)
		{
			state = pulseComplete;
		}
		else
		{
			state = inPulse;
		}
	}

	// Perform the correct action for the state
	switch(state)
	{
	case inPulse:
	{
		// recalculate the alpha
		const float ratio = pulseClock.GetDeltaTime() / pulseSeconds;
		alpha = 255.0f * sin(ratio) + 64.0f;
		if(alpha > 255.0f) alpha = 255.0f;
		break;
	}

	case pulseComplete:
	{
		// Reset the pulse clock with a larger period
		pulseClock.Stop();
		pulseClock.Start();
		pulseSeconds *= 1.5f;
		break;
	}

	case stopped:
	{
		// Reset all the invulnerability stuff
		invulnerable = false;
		invulnerabilityClock.Stop();
		pulseClock.Stop();
		alpha = 255.0f;
		pulseSeconds = 0.2f;

		break;
	}

	default:
		break;
	}

	// Set the model color
	const float alphaFactor = alpha / 255.0f;
	const Color col(1.0f * alphaFactor, 1.0f * alphaFactor, 1.0f * alphaFactor, 1.0f);
	model.SetColor(col);
}
