
#include "App.cpp"
#include "demos/Firework.cpp"

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

class FireworkDemo : public App {
private:
	struct FireworkData
	{
		Firework firework;
		mesh fireworkMesh;
	};

	const static unsigned maxFireworks = 256;
	FireworkData fireworks[maxFireworks];
	unsigned nextFirework;
	const static unsigned ruleCount = 9;
	FireworkRule rules[ruleCount];

	void create(unsigned type, const Firework* parent = NULL)
	{
		// Get the rule needed to create this firework
		FireworkRule* rule = rules + (type - 1);

		// Create the firework
		rule->create(&fireworks[nextFirework].firework, parent);

		// Increment the index for the next firework
		nextFirework = (nextFirework + 1) % maxFireworks;
	}

	void create(unsigned type, unsigned number, const Firework* parent) 
	{
		for (unsigned i = 0; i < number; i++)
		{
			create(type, parent);
		}
	}

	void initFireworkRules() 
	{
		// Go through the firework types and create their rules.
		rules[0].init(2);
		rules[0].setParameters(
			1, // type
			0.5f, 1.4f, // age range
			dynahex::Vector3(-5, 25, -5), // min velocity
			dynahex::Vector3(5, 28, 5), // max velocity
			0.1 // damping
		);
		rules[0].payloads[0].set(3, 5);
		rules[0].payloads[1].set(5, 5);

		rules[1].init(1);
		rules[1].setParameters(
			2, // type
			0.5f, 1.0f, // age range
			dynahex::Vector3(-5, 10, -5), // min velocity
			dynahex::Vector3(5, 20, 5), // max velocity
			0.8 // damping
		);
		rules[1].payloads[0].set(4, 2);

		rules[2].init(0);
		rules[2].setParameters(
			3, // type
			0.5f, 1.5f, // age range
			dynahex::Vector3(-5, -5, -5), // min velocity
			dynahex::Vector3(5, 5, 5), // max velocity
			0.1 // damping
		);

		rules[3].init(0);
		rules[3].setParameters(
			4, // type
			0.25f, 0.5f, // age range
			dynahex::Vector3(-20, 5, -5), // min velocity
			dynahex::Vector3(20, 5, 5), // max velocity
			0.2 // damping
		);

		rules[4].init(1);
		rules[4].setParameters(
			5, // type
			0.5f, 1.0f, // age range
			dynahex::Vector3(-20, 2, -5), // min velocity
			dynahex::Vector3(20, 18, 5), // max velocity
			0.01 // damping
		);
		rules[4].payloads[0].set(3, 5);

		rules[5].init(0);
		rules[5].setParameters(
			6, // type
			3, 5, // age range
			dynahex::Vector3(-5, 5, -5), // min velocity
			dynahex::Vector3(5, 10, 5), // max velocity
			0.95 // damping
		);

		rules[6].init(1);
		rules[6].setParameters(
			7, // type
			4, 5, // age range
			dynahex::Vector3(-5, 50, -5), // min velocity
			dynahex::Vector3(5, 60, 5), // max velocity
			0.01 // damping
		);
		rules[6].payloads[0].set(8, 10);

		rules[7].init(0);
		rules[7].setParameters(
			8, // type
			0.25f, 0.5f, // age range
			dynahex::Vector3(-1, -1, -1), // min velocity
			dynahex::Vector3(1, 1, 1), // max velocity
			0.01 // damping
		);

		rules[8].init(0);
		rules[8].setParameters(
			9, // type
			3, 5, // age range
			dynahex::Vector3(-15, 10, -5), // min velocity
			dynahex::Vector3(15, 15, 5), // max velocity
			0.95 // damping
		);
		// ... and so on for other firework types ...
	}

	void createFirework(const unsigned char key)
	{
		switch (key)
		{
			case '1': create(1, 1, NULL); break;
			case '2': create(2, 1, NULL); break;
			case '3': create(3, 1, NULL); break;
			case '4': create(4, 1, NULL); break;
			case '5': create(5, 1, NULL); break;
			case '6': create(6, 1, NULL); break;
			case '7': create(7, 1, NULL); break;
			case '8': create(8, 1, NULL); break;
			case '9': create(9, 1, NULL); break;
		}
	}

	void UpdateFireworks(float fElapsedTime) 
	{
		for (auto* firework = fireworks; firework < fireworks + maxFireworks; firework++)
		{
			// Check if we need to process this firework && Does it need removing?
			if (firework->firework.type > 0 && firework->firework.update(fElapsedTime))
			{
				// Find the appropriate rule
				FireworkRule* rule = rules + (firework->firework.type - 1);

				// Delete the current firework
				firework->firework.type = 0;

				// Add the payload
				for (unsigned i = 0; i < rule->payloadCount; i++)
				{
					FireworkRule::Payload* payload = rule->payloads + i;
					create(payload->type, payload->count, &firework->firework);
				}
			}
		}
	}

	void GetKeys() 
	{
		// Quite ugly D:
		if (GetKey(L'1').bPressed)
		{
			createFirework(L'1');
		}
		else if (GetKey(L'2').bPressed)
		{
			createFirework(L'2');
		}
		else if (GetKey(L'3').bPressed)
		{
			createFirework(L'3');
		}
		else if (GetKey(L'4').bPressed)
		{
			createFirework(L'4');
		}
		else if (GetKey(L'5').bPressed)
		{
			createFirework(L'5');
		}
		else if (GetKey(L'6').bPressed)
		{
			createFirework(L'6');
		}
		else if (GetKey(L'7').bPressed)
		{
			createFirework(L'7');
		}
		else if (GetKey(L'8').bPressed)
		{
			createFirework(L'8');
		}
		else if (GetKey(L'9').bPressed)
		{
			createFirework(L'9');
		}
	}

public:
	FireworkDemo() : nextFirework(0)
	{
		m_sAppName = L"Firework Demo";

		// Make all shots unused
		for (FireworkData* firework = fireworks; firework < fireworks + maxFireworks;firework++)
		{
			firework->firework.type = 0;
		}

		// Create the firework types
		initFireworkRules();
	}


	bool OnUserCreate() override
	{
		for (auto firework : fireworks)
		{
			firework.fireworkMesh.LoadBox(0.15f);
		}

		pDepthBuffer = new float[ScreenWidth() * ScreenHeight()];
		// Projection Matrix
		matProj = Matrix_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override 
	{
		if (GetKey(VK_UP).bHeld)
			vCamera.y += 8.0f * fElapsedTime;	// Travel Upwards

		if (GetKey(VK_DOWN).bHeld)
			vCamera.y -= 8.0f * fElapsedTime;	// Travel Downwards


		// Dont use these two in FPS mode, it is confusing :P
		if (GetKey(VK_LEFT).bHeld)
			vCamera.x -= 8.0f * fElapsedTime;	// Travel Along X-Axis

		if (GetKey(VK_RIGHT).bHeld)
			vCamera.x += 8.0f * fElapsedTime;	// Travel Along X-Axis
		///////


		vec3d vForward = Vector_Mul(vLookDir, 8.0f * fElapsedTime);

		// Standard FPS Control scheme, but turn instead of strafe
		if (GetKey(L'W').bHeld)
			vCamera = Vector_Add(vCamera, vForward);

		if (GetKey(L'S').bHeld)
			vCamera = Vector_Sub(vCamera, vForward);

		if (GetKey(L'A').bHeld)
			fYaw -= 2.0f * fElapsedTime;

		if (GetKey(L'D').bHeld)
			fYaw += 2.0f * fElapsedTime;

		// Set up "World Tranmsform" though not updating theta 
		// makes this a bit redundant
		mat4x4 matRotZ, matRotX;
		//fTheta += 1.0f * fElapsedTime; // Uncomment to spin me right round baby right round
		matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
		matRotX = Matrix_MakeRotationX(fTheta);

		mat4x4 matTrans;
		matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);

		mat4x4 matWorld;
		matWorld = Matrix_MakeIdentity();	// Form World Matrix
		matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX); // Transform by rotation
		matWorld = Matrix_MultiplyMatrix(matWorld, matTrans); // Transform by translation

		// Create "Point At" Matrix for camera
		vec3d vUp = { 0,1,0 };
		vec3d vTarget = { 0,0,1 };
		mat4x4 matCameraRot = Matrix_MakeRotationY(fYaw);
		vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);
		vTarget = Vector_Add(vCamera, vLookDir);
		mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

		// Make view matrix from camera
		mat4x4 matView = Matrix_QuickInverse(matCamera);

		// Store triagles for rastering later
		vector<triangle> vecTrianglesToRaster;

		// UpdateFireworks(fElapsedTime);
		GetKeys();

		for (int i = 0; i < maxFireworks; i++)
		{
			auto firework = fireworks[i];
			firework.firework.update(fElapsedTime);
			if (firework.firework.type > 0)
			{
				const dynahex::Vector3& pos = firework.firework.getPosition();
				vec3d moveVector = vec3d::ConvertVector3ToVec3d(pos);
				moveVector.MultiplyConst(0.15f);
				firework.fireworkMesh.LoadBox();
				firework.fireworkMesh.MoveMesh(moveVector);
			}
			DrawTrianglesFromMesh(firework.fireworkMesh, matWorld, matView, &vecTrianglesToRaster);
		}

		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_CYAN);

		// Clear Depth Buffer
		for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
			pDepthBuffer[i] = 0.0f;

		DrawRasterizedTriangles(vecTrianglesToRaster);

		return true;
	}

};

int main()
{
	BulletDemo demo;
	if (demo.ConstructConsole(256, 240, 4, 4))
		demo.Start();
	return 0;
}
