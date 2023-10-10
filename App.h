#pragma once

#include "olcConsoleGameEngine.h"
#include "dynahex/dynahex.h"
#include <fstream>
#include <strstream>
#include <string>

using namespace std;

struct vec2d
{
	float u = 0;
	float v = 0;
	float w = 1;
};

struct vec3d
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1; // Need a 4th term to perform sensible matrix vector multiplication

	vec3d operator-(const vec3d& other) const {
		return { x - other.x, y - other.y, z - other.z, w };
	}

	static vec3d ConvertVector3ToVec3d(dynahex::Vector3 vector3)
	{
		return vec3d{ vector3.x, vector3.y, vector3.z, 1.0f };
	}

	void MultiplyConst(const float k)
	{
		x *= k;
		y *= k;
		z *= k;
	}

	void reset() {
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}
};

struct triangle
{
	vec3d p[3];
	vec2d t[3]; // added a texture coord per vertex
	wchar_t sym;
	short col;
};

struct mesh
{
	vector<triangle> tris;

	bool LoadFromObjectFile(string sFilename, bool bHasTexture = false)
	{
		ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		vector<vec3d> verts;
		vector<vec2d> texs;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				if (line[1] == 't')
				{
					vec2d v;
					s >> junk >> junk >> v.u >> v.v;
					// A little hack for the spyro texture
					//v.u = 1.0f - v.u;
					//v.v = 1.0f - v.v;
					texs.push_back(v);
				}
				else
				{
					vec3d v;
					s >> junk >> v.x >> v.y >> v.z;
					verts.push_back(v);
				}
			}

			if (!bHasTexture)
			{
				if (line[0] == 'f')
				{
					int f[3];
					s >> junk >> f[0] >> f[1] >> f[2];
					tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
				}
			}
			else
			{
				if (line[0] == 'f')
				{
					s >> junk;

					string tokens[6];
					int nTokenCount = -1;


					while (!s.eof())
					{
						char c = s.get();
						if (c == ' ' || c == '/')
							nTokenCount++;
						else
							tokens[nTokenCount].append(1, c);
					}

					tokens[nTokenCount].pop_back();


					tris.push_back({ verts[stoi(tokens[0]) - 1], verts[stoi(tokens[2]) - 1], verts[stoi(tokens[4]) - 1],
						texs[stoi(tokens[1]) - 1], texs[stoi(tokens[3]) - 1], texs[stoi(tokens[5]) - 1] });

				}

			}
		}
		return true;
	}

	void LoadBox(float size = 1.0f)
	{
		tris = {
			// SOUTH
			{ 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, size, 0.0f, 1.0f,    size, size, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ 0.0f, 0.0f, 0.0f, 1.0f,    size, size, 0.0f, 1.0f,    size, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

			// EAST           																			   
			{ size, 0.0f, 0.0f, 1.0f,    size, size, 0.0f, 1.0f,    size, size, size, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ size, 0.0f, 0.0f, 1.0f,    size, size, size, 1.0f,    size, 0.0f, size, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

			// NORTH           																			   
			{ size, 0.0f, size, 1.0f,    size, size, size, 1.0f,    0.0f, size, size, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ size, 0.0f, size, 1.0f,    0.0f, size, size, 1.0f,    0.0f, 0.0f, size, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

			// WEST            																			   
			{ 0.0f, 0.0f, size, 1.0f,    0.0f, size, size, 1.0f,    0.0f, size, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ 0.0f, 0.0f, size, 1.0f,    0.0f, size, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

			// TOP             																			   
			{ 0.0f, size, 0.0f, 1.0f,    0.0f, size, size, 1.0f,    size, size, size, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ 0.0f, size, 0.0f, 1.0f,    size, size, size, 1.0f,    size, size, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

			// BOTTOM          																			  
			{ size, 0.0f, size, 1.0f,    0.0f, 0.0f, size, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ size, 0.0f, size, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    size, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
		};
	}

	void MoveMesh(const vec3d moveVector)
	{
		for (auto& tri : tris)
		{
			for (auto& vertex : tri.p)
			{
				vertex.x += moveVector.x;
				vertex.y += moveVector.y;
				vertex.z += moveVector.z;
			}
		}
	}

	void SetPosition(const vec3d& newPosition) {

		const vec3d positionDiff = newPosition - tris[0].p[0];

		for (auto& tri : tris) {
			for (auto& vertex : tri.p) {
				vertex.x += positionDiff.x;
				vertex.y += positionDiff.y;
				vertex.z += positionDiff.z;
			}
		}
	}
};

struct mat4x4
{
	float m[4][4] = { 0 };
};

class App : public olcConsoleGameEngine
{
public:
	App();

protected:
	mesh meshCube;
	mat4x4 matProj;	// Matrix that converts from view space to screen space
	vec3d vCamera;	// Location of camera in world space
	vec3d vLookDir;	// Direction vector along the direction camera points
	float fYaw;		// FPS Camera rotation in XZ plane
	float fTheta;	// Spins World transform

	olcSprite* sprTex1;

	vec3d Matrix_MultiplyVector(mat4x4& m, vec3d& i);
	mat4x4 Matrix_MakeIdentity();
	mat4x4 Matrix_MakeRotationX(float fAngleRad);
	mat4x4 Matrix_MakeRotationY(float fAngleRad);
	mat4x4 Matrix_MakeRotationZ(float fAngleRad);
	mat4x4 Matrix_MakeTranslation(float x, float y, float z);
	mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
	mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2);
	mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up);
	mat4x4 Matrix_QuickInverse(mat4x4& m);
	vec3d Vector_Add(vec3d& v1, vec3d& v2);
	vec3d Vector_Sub(vec3d& v1, vec3d& v2);
	vec3d Vector_Mul(vec3d& v1, float k);
	vec3d Vector_Div(vec3d& v1, float k);
	float Vector_DotProduct(vec3d& v1, vec3d& v2);
	float Vector_Length(vec3d& v);
	vec3d Vector_Normalise(vec3d& v);
	vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2);
	vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd, float& t);
	int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2);
	CHAR_INFO GetColour(float lum);
	float* pDepthBuffer = nullptr;

public:
	virtual bool OnUserCreate() override;
	virtual bool OnUserUpdate(float fElapsedTime) override;
	void DrawTrianglesFromMesh(mesh meshCube, mat4x4 matWorld, mat4x4 matView, vector<triangle>* vecTrianglesToRaster);
	void DrawRasterizedTriangles(vector<triangle>& vecTrianglesToRaster);
	void TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
		int x2, int y2, float u2, float v2, float w2,
		int x3, int y3, float u3, float v3, float w3,
		olcSprite* tex);
};
