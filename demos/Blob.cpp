
#include "dynahex/dynahex.h"

#define BLOB_COUNT 5
#define PLATFORM_COUNT 10
#define BLOB_RADIUS 0.4f

class Platform : public dynahex::ParticleContactGenerator {
public:
	dynahex::Vector3 start;
	dynahex::Vector3 end;

	/** Holds a pointer to the particles we are cheking. */
	dynahex::Particle* particles;

	unsigned addContact(dynahex::ParticleContact* contact, unsigned limit) const override {
		const static dynahex::real restitution = 0.0f;

		unsigned used = 0;

		for (unsigned i = 0; i < BLOB_COUNT; i++) {
			if (used >= limit) break;

			dynahex::Vector3 toParticle = particles[i].getPosition() - start;
			dynahex::Vector3 lineDirection = end - start;
			dynahex::real projected = toParticle * lineDirection;
			dynahex::real platformSqLength = lineDirection.squareMagnitude();

			if (projected <= 0) {
				// the blob is nearest to the start point
				if (toParticle.squareMagnitude() < BLOB_RADIUS * BLOB_RADIUS) {
					// We have a collision
					contact->contactNormal = toParticle.unit();
					contact->contactNormal.z = 0;
					contact->restitution = restitution;
					contact->particle[0] = particles + i;
					contact->particle[1] = 0;
					contact->penetration = BLOB_RADIUS - toParticle.magnitude();
					used++;
					contact++;
				}
			}
			else if (projected >= platformSqLength) {
				// The blob is nearest to the end point
				toParticle = particles[i].getPosition() - end;
				if (toParticle.squareMagnitude() < BLOB_RADIUS * BLOB_RADIUS) {
					// We have a collision
					contact->contactNormal = toParticle.unit();
					contact->contactNormal.z = 0;
					contact->restitution = restitution;
					contact->particle[0] = particles + i;
					contact->particle[1] = 0;
					contact->penetration = BLOB_RADIUS - toParticle.magnitude();
					used++;
					contact++;
				}
			}
			else {
				// The blob is nearest to the middle.
				dynahex::real distanceToPlatform = toParticle.squareMagnitude() - (projected * projected) / platformSqLength;
				if (distanceToPlatform < BLOB_RADIUS * BLOB_RADIUS) {
					// We have a collision 
					dynahex::Vector3 closestPoint = start + lineDirection * (projected / platformSqLength);
					contact->contactNormal = (particles[i].getPosition() - closestPoint).unit();
					contact->contactNormal.z = 0;
					contact->restitution = restitution;
					contact->particle[0] = particles + i;
					contact->particle[1] = 0;
					contact->penetration = BLOB_RADIUS - real_sqrt(distanceToPlatform);
					used++;
					contact++;
				}
			}
		}

		return used;
	}
};

/** A force generator for proximal attraction. */
class BlobForceGenerator : public dynahex::ParticleForceGenerator {
public:
	// Holds a pointer to the particles we might be attracting.
	dynahex::Particle* particles;

	// The maximum force used to push the particles apart.
	dynahex::real maxReplusion;

	// The maximum force used to pull particles together.
	dynahex::real maxAttraction;

	// The separation between particles where there is no force.
	dynahex:: real minNaturalDistance, maxNaturalDistance;

	// The force with which to float the head particle, if it is joined to others.
	dynahex::real floatHead;

	// The maximum number of particles in the blob before the head is floated at maximum force.
	unsigned maxFloat;

	// The separation between particles after which they 'break' apart and there is no force.
	dynahex::real maxDistance;

	void updateForce(dynahex::Particle* particle, dynahex::real duration) override {
		unsigned joinCount = 0;
		for (unsigned i = 0; i < BLOB_COUNT; i++) {
			// Don't attract yourself
			if (particles + i == particle) continue;

			// Work out the separation distance
			dynahex::Vector3 separation = particles[i].getPosition() - particle->getPosition();
			separation.z = 0.0f;
			dynahex::real distance = separation.magnitude();

			if (distance < minNaturalDistance) {
				// Use a repulsion force.
				distance = 1.0f - distance / minNaturalDistance;
				particle->addForce(separation.unit() * (1.0f - distance) * maxReplusion * -1.0f);
				joinCount++;
			}
			else if (distance > maxNaturalDistance && distance < maxDistance)
			{
				// Use an attraction force.
				distance = (distance - maxNaturalDistance) / (maxDistance - maxNaturalDistance);
				particle->addForce(separation.unit() * distance * maxAttraction);
				joinCount++;
			}
		}

		// If the particle is the head, and we've got a join count, then float it.
		if (particle == particles && joinCount > 0 && maxFloat > 0)
		{
			dynahex::real force = dynahex::real(joinCount / maxFloat) * floatHead;
			if (force > floatHead) force = floatHead;
			particle->addForce(dynahex::Vector3(0, force, 0));
		}
	}
};