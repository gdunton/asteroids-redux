//-----------------------------------------------------------
// File: Player.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Player.h"

#include "..\Graphics\Model2D.h"
#include "..\Content\ModelManager.h"
#include "..\Content\AudioManager.h"

#include "..\GameMain\Globals.h"

// Initialize the static variables
const float Player::FRICTION_FACTOR = 0.975f;
const float Player::ACCELERATION = 100.0f;
const float Player::FORCE_VALUE = 100.0f;
const float Player::FIRING_COOLDOWN = 0.2f;

const float Player::TURNING_SPEED = 1.3f;

const int Player::STARTING_LIVES = 3;

const float Player::DAMAGE_COOLDOWN_SECONDS = 2;

Player::Player()
{
	lives = STARTING_LIVES;
	startingWorld = World();
	startingVelocity = MyVector2(0,0);
}

Player::Player( const MyVector2& _pos, const MyVector2& _size, float _rot,
		Model2D* _model, const MyVector2& _velocity, float _mass,
		int _WORLD_WIDTH, int _WORLD_HEIGHT )
		: PhysicsObject( _pos, _size, _rot, _model, _velocity, _mass )
{
	bulletCooldown = 0;
	lives = STARTING_LIVES;

	startingWorld = World( _pos, _size, _rot );
	startingVelocity = _velocity;

	invulnerable = 0;
	pulseSeconds = 0.2f;
	alpha = 255;

	thrustSound = 0;
	playerBoosting = false;
}

void Player::Update( float dt )
{
	if( Alive() )
	{
		if( invulnerable )
		{
			// Update the invulnerability animation
			TickInvulnerability( dt );
		}

		velocity *= FRICTION_FACTOR;
		world.pos += velocity * dt;

		WrapPosition();	
	}

	// Update all of the bullets that are alive
	for( int i = 0; i < MAX_BULLETS; ++i )
	{
		bulletsArray[i].Update( dt );
	}

	// Decrease the bulletCooldown
	if( bulletCooldown > 0 )
	{
		bulletCooldown -= dt;
	}
}

void Player::Render( Camera& camera )
{
	if( Alive() )
	{
		GameEntity::Render( camera );
	}
}

void Player::TurnClockwise( float dt )
{
	world.rot += PI * TURNING_SPEED * dt;

	// Ensure the rotation wraps around 360
	// Shift into the positive space
	world.rot += 2*PI;
	// Mod to leave in [0-2PI] space
	world.rot = fmod( world.rot, 2*PI );
}

void Player::TurnAntiClockwise( float dt )
{
	world.rot -= PI * TURNING_SPEED * dt;

	// Ensure the rotation wraps around 360
	// Shift into the positive space
	world.rot += 2*PI;
	// Mod to leave in [0-2PI] space
	world.rot = fmod( world.rot, 2*PI );
}

void Player::Boost( float dt )
{
	MyVector2 direction( 0, 1 );

	direction = RotatePoint( direction, world.rot );
	velocity += direction * ACCELERATION * dt;

	// Flip to indicate boosting for starting the boost sound
	if( !playerBoosting )
	{
		playerBoosting = true;
		thrustSound = AudioManager::PlaySoundByName( "Thrust" );
	}
}

// Function to stop the boost sound if one is playing
void Player::EndBoost()
{
	if( playerBoosting )
	{
		playerBoosting = false;
		if( thrustSound ) 
		{
			thrustSound->VStop();
			thrustSound = 0;
		}
	}
}

void Player::FireBullet()
{
	// Make sure that the cooldown is not 0
	if( bulletCooldown > 0 )
	{
		return;
	}

	// Search for a bullet that isn't alive
	for( int i = 0; i < MAX_BULLETS; ++i )
	{
		if( !bulletsArray[i].GetAlive() ) // bullet not alive
		{
			// Get the player direction
			MyVector2 direction( 0, 1 );
			direction = RotatePoint( direction, world.rot );
			// Create new bullet
			bulletsArray[i] = Bullet( world.pos, MyVector2( 1,1 ), world.rot, 
				ModelManager::GetInstance().GetModel("Player"), 
				direction * Bullet::FIRING_SPEED );

			// Reset the cooldown
			bulletCooldown = FIRING_COOLDOWN;

			// Play the fire sound
			AudioManager::PlaySoundByName( "FireAlt" );

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
	return MathTypes::Circle( world.pos, model->GetModelRadius() * Length( world.scale ) );
}

void Player::RemoveLife()
{
	if( !invulnerable ) // may be unncessary but this will stay
	{
		lives--;

		// Play collision sound
		AudioManager::PlaySoundByName( "Bang3" );
	}
}

void Player::AddLife()
{
	lives++;
}

bool Player::Alive()
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
	if( thrustSound )
	{
		thrustSound->VStop();
		thrustSound = 0;
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
	alpha = 255;

	if( thrustSound )
	{
		thrustSound->VStop();
		thrustSound = 0;
	}

	// Need to remove bullets between levels
	for( int i = 0; i < MAX_BULLETS; ++i )
	{
		bulletsArray[i].KillBullet();
	}
}

void Player::TickInvulnerability( float dt )
{
	enum InvulState { inPulse, pulseComplete, stopped };
	InvulState state;
	invulnerabilityClock.Tick( dt );
	pulseClock.Tick( dt );

	// Get the correct state
	if( invulnerabilityClock.GetDeltaTime() >= DAMAGE_COOLDOWN_SECONDS )
	{
		if( pulseClock.GetDeltaTime() >= pulseSeconds )
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
		if( pulseClock.GetDeltaTime() >= pulseSeconds )
		{
			state = pulseComplete;
		}
		else
		{
			state = inPulse;
		}
	}

	// Perform the correct action for the state
	switch( state )
	{
	case inPulse:
		{
			// recalculate the alpha
			float ratio = pulseClock.GetDeltaTime() / pulseSeconds;
			alpha = 255.0f * (float)sin(ratio) + 64;
			if( alpha > 255 ) alpha = 255;
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
			alpha = 255;
			pulseSeconds = 0.2f;
			
			break;
		}

	default:
		break;
	}

	// Set the model color
	float alphaFactor = static_cast<float>(alpha) / 255.0f;
	const Color col(1.0f * alphaFactor, 1.0f * alphaFactor, 1.0f * alphaFactor, 1.0f);
	model->SetColor( col );
}