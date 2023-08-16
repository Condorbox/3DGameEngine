
#include "App.cpp"

class BulletDemo : public App
{
private:
	enum ShotType
	{
		UNUSED = 0,
		PISTOL,
		ARTILLERY,
		FIREBALL,
		LASER
	};

	ShotType type = PISTOL;

	dynahex::Particle bullet;

	void fire(unsigned char key)
	{
		bullet.setPosition(0.0f, 0.0f, 0.0f);

		switch (key)
		{
		case '1':
			type = PISTOL;
			bullet.setMass(2.0f);
			bullet.setVelocity(0.0f, 0.0f, 35.0f / 64.0f);
			bullet.setAcceleration(0.0f, -1.0f / 64.0f, 0.0f);
			bullet.setDamping(0.99f);
			break;
		case '2':
			type = ARTILLERY;
			bullet.setMass(200.0f);
			bullet.setVelocity(0.0f, 30.0f / 64.0f, 40.0f / 64.0f);
			bullet.setAcceleration(0.0f, -20.0f / 64.0f, 0.0f);
			bullet.setDamping(0.99f);
			break;
		case '3':
			type = FIREBALL;
			bullet.setMass(1.0f);
			bullet.setVelocity(0.0f, 0.0f, 10.0f / 64.0f);
			bullet.setAcceleration(0.0f, 0.6f / 64.0f, 0.0f);
			bullet.setDamping(0.9f);
			break;
		case '4':
			type = LASER;
			bullet.setMass(0.1f);
			bullet.setVelocity(0.0f, 0.0f, 100.0f / 64.0f);
			bullet.setAcceleration(0.0f, 0.0f, 0.0f);
			bullet.setDamping(0.99f);
			break;
		default:
			break;
		}

		meshCube.LoadBox(0.15f);
	}

public:
	BulletDemo()
	{
		m_sAppName = L"Bullet Demo";
	}

	bool OnUserCreate() override
	{
		App::OnUserCreate();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Bullet Controller
		if (GetKey(L'1').bPressed)
		{
			fire('1');
		}
		else if (GetKey(L'2').bPressed)
		{
			fire('2');
		}
		else if (GetKey(L'3').bPressed)
		{
			fire('3');
		}
		else if (GetKey(L'4').bPressed)
		{
			fire('4');
		}

		bullet.integrate(fElapsedTime);
		vec3d moveVector = vec3d::ConvertVector3ToVec3d(bullet.getPosition());
		moveVector.MultiplyConst(0.15f);  // Make bullet go slower
		meshCube.MoveMesh(moveVector);

		App::OnUserUpdate(fElapsedTime);

		return true;
	}
};
