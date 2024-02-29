#include "../platform.h" // This file will make exporting DLL symbols simpler for students.
#include "../Framework/TileSystem/TileMap.h"
#include "../PriorityQueue.h"
#include <map>

using namespace std;



namespace ufl_cap4053
{
	namespace searches
	{

		class PlannerNode {
		public:
			Tile* _self;
			PlannerNode* _parent;

			float hCost;
			float gCost;
			float fCost;


			PlannerNode(Tile* tile, PlannerNode* _p, float _g, float _h) {
				_self = tile;
				_parent = _p;
				gCost = _g;
				hCost = _h;
				fCost = _g + _h;
			}
		};


		class PathSearch
		{
			
			private:

				TileMap* _tileMap;
				
				Tile* _start;
				Tile* _goal;

				std::vector<Tile const*> _solution;

				PriorityQueue<PlannerNode*> _queue;

				map<Tile*, PlannerNode*> visited;
				// Store a vector of a neighboring tiles and the est dist to goal for it
				map<Tile*, map<Tile*, float>> neighbors;

				
			public:
				DLLEXPORT PathSearch();
				DLLEXPORT ~PathSearch();
				DLLEXPORT void load(TileMap* _TileMap);
				DLLEXPORT void initialize(int startRow, int startCol, int goalRow, int goalCol);
				DLLEXPORT void update(long timeslice);
				DLLEXPORT void shutdown();
				DLLEXPORT void unload();
				DLLEXPORT bool isDone() const;
				DLLEXPORT std::vector<Tile const*> const getSolution() const;
			};
	}
}  // close namespace ufl_cap4053::searches
