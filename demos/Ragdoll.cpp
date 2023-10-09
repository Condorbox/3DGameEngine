#include "dynahex/collide_narrow.h"

#define NUM_BONES 12
#define NUM_JOINTS 11

class Bone : public dynahex::CollisionBox {
public:
	Bone() {
		body = new dynahex::RigidBody();
	}

	~Bone() {
		delete body;
	}

	/**
	 * We use a sphere to collide bone on bone to allow some limited
	 * interpenetration.
	 */
	dynahex::CollisionSphere getCollisionSphere() const {
		dynahex::CollisionSphere sphere;
		sphere.body = body;
		sphere.radius = halfSize.x;
		sphere.offset = dynahex::Matrix4();
		if (halfSize.y < sphere.radius) sphere.radius = halfSize.y;
		if (halfSize.z < sphere.radius) sphere.radius = halfSize.z;
		sphere.calculateInternals();
		return sphere;
	}

	/** Sets the bone to a specific location. */
	void setState(const dynahex::Vector3& position, const dynahex::Vector3& extents) {
		body->setPosition(position);
		auto q = dynahex::Quaternion();
		body->setOrientation(q);
		body->setVelocity(dynahex::Vector3());
		body->setRotation(dynahex::Vector3());
		halfSize = extents;

		dynahex::real mass = halfSize.x * halfSize.y * halfSize.z * 8.0f;
		body->setMass(mass);

		dynahex::Matrix3 tensor;
		tensor.setBlockInertiaTensor(halfSize, mass);
		body->setInertiaTensor(tensor);

		body->setLinearDamping(0.95f);
		body->setAngularDamping(0.8f);
		body->clearAccumulators();
		body->setAcceleration(dynahex::Vector3::GRAVITY);

		body->setCanSleep(false);
		body->setAwake();

		body->calculateDerivedData();
		calculateInternals();
	}
};