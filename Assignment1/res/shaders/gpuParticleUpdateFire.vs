#version 410

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Velocity;
layout(location=2) in float Lifetime;
layout(location=3) in float Lifespan;

out vec3 position;
out vec3 velocity;
out float lifetime;
out float lifespan;

uniform float time;
uniform float deltaTime;
uniform float lifeMin;
uniform float lifeMax;
uniform float velocityMin;
uniform float velocityMax;

uniform vec3 emitterPosition;

uniform float spread;

uniform int emitterType;

const float INVERSE_MAX_UINT = 1.0f / 4294967295.0f;
float rand(uint seed, float range)
{
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float(range * i) * INVERSE_MAX_UINT;
}

void main()
{
	uint seed = uint(time * 1000.0) + uint(gl_VertexID);

	position = Position + Velocity * deltaTime;	
	velocity = Velocity;
	
	vec3 targetPosition = emitterPosition;
	if(emitterType != 0) {
		targetPosition.x += (tan(time) * spread);
		targetPosition.z += (cos(time) * spread);
	}

	if(position.x > targetPosition.x + 0.1f)
		velocity.x -= deltaTime;
	else if(position.x < targetPosition.x - 0.1f)
		velocity.x += deltaTime;
	else
		velocity.x += velocity.x * 5 * -deltaTime;

	if(position.z > targetPosition.z + 0.1f)
		velocity.z -= deltaTime;
	else if(position.z < targetPosition.z - 0.1f)
		velocity.z += deltaTime;
	else
		velocity.z += velocity.z * 5 * -deltaTime;

	velocity.y += deltaTime;
	
	lifetime = Lifetime + deltaTime;
	lifespan = Lifespan;

	if(lifetime > lifespan)
	{
		velocity.x = rand(seed++, 1) - 0.5f;
		velocity.y = rand(seed++, 1) - 0.125;
		velocity.z = rand(seed++, 1) - 0.5f;

		position = emitterPosition;

		if(emitterType == 1) {
			float theta = 2 * 3.14 * rand(seed++, 1);
			float phi = 3.14 * rand(seed++, 1);

			position.x = cos(theta) * sin(phi) * spread;
			position.z = cos(phi) * spread;
		} else if(emitterType == 2) {
			float theta = 2 * 3.14 * rand(seed++, 1);
			float phi = 3.14 * rand(seed++, 1);
			
			position.x = cos(theta) * sin(phi) * spread;
			position.y = sin(theta) * sin(phi) * spread;
			position.z = cos(phi) * spread;
		} else if(emitterType == 3) {
			position.x += (rand(seed++, 1) - 0.5f) * spread;
			position.z += (rand(seed++, 1) - 0.5f) * spread;
		} else if(emitterType == 4) {
			position.x += (rand(seed++, 1) - 0.5f) * spread;
			position.y += (rand(seed++, 1) - 0.5f) * spread;
			position.z += (rand(seed++, 1) - 0.5f) * spread;
		}
		//Sphere
		//
		//

		lifetime = 0;
		lifespan = (rand(seed++, lifeMax - lifeMin) + lifeMin);
	}
}