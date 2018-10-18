#pragma once
#include "QuadTree.h"
#include "engine_shared.h"
class Terrain;

namespace test
{

	struct Position
	{
		float x, y;
	};

	struct AABB
	{
		Position m_Pos;
		float m_Halfwidth;

		bool Intersect(Position position);
		bool Intersect(Position position, float radius);


	};

	struct Leaf
	{
		Leaf();
		~Leaf();

		AABB m_AABB;
		Leaf* m_Parent = nullptr;
		Leaf* m_Children[4];
		int m_Size = 0;
		int m_Depth = 0;
		int m_Index = 0;
		bool Insert(Position pos);
		void subdivide();

		void Render();
		void DrawShadow();
		bool Draw(bool shadow);


		void Reset();
		Terrain* m_Terrain = nullptr;

		bool isNeighbour(test::Leaf* leaf);

	};



	class QuadTree
	{
	public:
		QuadTree();

		void Init(Position xy);
		void Insert(Position xy);
		void Draw();
		void DrawShadow();


		void Update(float x, float y);
		bool m_RenderDepth = true;

	private:
		Leaf* m_Root = nullptr;

		CompiledShader* m_Shaders[4];

	};


}




class TerrainSystem
{
public:
	TerrainSystem();

	void Update();
	void DrawShadow();
	void Draw();


	float m_X = 0.f;
	float m_Y = 0.f;

	float GetHeight(int x, int y);

private:
	test::QuadTree m_Tree;
	struct 
	{
		u32 myWidth;
		u32 myDepth;
		u8* myData;
	} m_Heightmap;
	

};

