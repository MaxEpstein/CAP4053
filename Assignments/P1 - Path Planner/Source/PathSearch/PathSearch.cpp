#include "./PathSearch.h"
#include <chrono>
#include <iostream>

#define blue 0xff2816fa
#define yellow 0xffcfea2d
#define red 0xfffa1e60
#define PURPLE 0xff9370db


namespace ufl_cap4053
{
	namespace searches
	{

		float estToGoal(Tile* initial, Tile* goal) {
			// x^2 + y^2 = z^2
			// z = sqrt(x^2 + y^2)
			return (sqrt(pow(goal->getXCoordinate() - initial->getXCoordinate(), 2) + pow(goal->getYCoordinate() - initial->getYCoordinate(), 2)));
		}

		bool pqFunc(PlannerNode* const& lhs, PlannerNode* const& rhs) {
			return lhs->fCost > rhs->fCost;
		}


		// CLASS DEFINITION GOES HERE
		PathSearch::PathSearch() : _queue(pqFunc) {
			_tileMap = nullptr;
			_start = nullptr;
			_goal = nullptr;
		}

		PathSearch::~PathSearch(){
			unload();
		}

		void PathSearch::load(TileMap* _TileMap) {
			_tileMap = _TileMap;
			_start = _TileMap->getStartTile();
			_goal = _TileMap->getGoalTile();

			for (int row = 0; row < _tileMap->getRowCount(); row++) {
				for (int col = 0; col < _tileMap->getColumnCount(); col++) {
					bool evenRow = false;
					if (row % 2 == 0)
						evenRow = true;
					
					Tile* currentTile = _tileMap->getTile(row, col);
					

					//has a right tile column
					if (currentTile->getColumn() < _tileMap->getColumnCount() - 1 && currentTile->getWeight() != 0) {
						// get the tile to the right
						if ((_tileMap->getTile(currentTile->getRow(), currentTile->getColumn() + 1))->getWeight() != 0) {
							currentTile->setFill(blue);
							Tile* right = _tileMap->getTile(currentTile->getRow(), currentTile->getColumn() + 1);
							neighbors[currentTile][right] = estToGoal(right, _goal);
							currentTile->addLineTo(right, PURPLE);
						}
					}
					//has a left tile column
					if (currentTile->getColumn() > 0 && currentTile->getWeight() != 0) {
						// get left tile
						if (_tileMap->getTile(currentTile->getRow(), currentTile->getColumn() - 1)->getWeight() != 0) {
							currentTile->setFill(blue);
							Tile* left = _tileMap->getTile(currentTile->getRow(), currentTile->getColumn() - 1);
							neighbors[currentTile][left] = estToGoal(left, _goal);
							currentTile->addLineTo(left, PURPLE);
						}
					}
					//has a upper tile row
					if (currentTile->getRow() > 0 && currentTile->getWeight() != 0) {
						if (_tileMap->getTile(currentTile->getRow() - 1, currentTile->getColumn())->getWeight() != 0) {
							currentTile->setFill(blue);
							Tile* top = _tileMap->getTile(currentTile->getRow() - 1, currentTile->getColumn());
							neighbors[currentTile][top] = estToGoal(top, _goal);
							currentTile->addLineTo(top, PURPLE);
						}
					}
					//has a lower tile row
					if (currentTile->getRow() < _tileMap->getRowCount() - 1 && currentTile->getWeight() != 0) {
						if (_tileMap->getTile(currentTile->getRow() + 1, currentTile->getColumn())->getWeight() != 0) {
							currentTile->setFill(blue);
							Tile* bottom = _tileMap->getTile(currentTile->getRow() + 1, currentTile->getColumn());
							neighbors[currentTile][bottom] = estToGoal(bottom, _goal);
							currentTile->addLineTo(bottom, PURPLE);
						}

					}

					//even row corners
					if (evenRow && currentTile->getWeight() != 0) {
						currentTile->setFill(blue);
						//has a top left tile
						if (currentTile->getRow() > 0 && currentTile->getColumn() > 0) {
							if (_tileMap->getTile(currentTile->getRow() - 1, currentTile->getColumn() - 1)->getWeight() != 0) {
								Tile* topLeft = _tileMap->getTile(currentTile->getRow() - 1, currentTile->getColumn() - 1);
								neighbors[currentTile][topLeft] = estToGoal(topLeft, _goal);
								currentTile->addLineTo(topLeft, PURPLE);
							}

						}
						// has a bottom right tile 
						if (currentTile->getRow() < _TileMap->getRowCount() - 1 && currentTile->getColumn() > 0) {
							if (_tileMap->getTile(currentTile->getRow() + 1, currentTile->getColumn() - 1)->getWeight() != 0) {
								Tile* bottomLeft = _tileMap->getTile(currentTile->getRow() + 1, currentTile->getColumn() - 1);
								neighbors[currentTile][bottomLeft] = estToGoal(bottomLeft, _goal);
								currentTile->addLineTo(bottomLeft, PURPLE);
							}

						}
					}
					//odd row 
					else if (!evenRow && currentTile->getWeight() != 0){
						currentTile->setFill(blue);
						//has a top right tile
						if (currentTile->getRow() > 0 && currentTile->getColumn() < _TileMap->getColumnCount() - 1) {
							if (_tileMap->getTile(currentTile->getRow() - 1, currentTile->getColumn() + 1)->getWeight() != 0) {
								Tile* topRight = _tileMap->getTile(currentTile->getRow() - 1, currentTile->getColumn() + 1);
								neighbors[currentTile][topRight] = estToGoal(topRight, _goal);
								currentTile->addLineTo(topRight, PURPLE);
							}

						}
						// has a bottom right tile
						if (currentTile->getRow() < _TileMap->getRowCount() - 1 && currentTile->getColumn() < _TileMap->getColumnCount() - 1) {
							if (_tileMap->getTile(currentTile->getRow() + 1, currentTile->getColumn() + 1)->getWeight() != 0) {
								Tile* bottomRight = _tileMap->getTile(currentTile->getRow() + 1, currentTile->getColumn() + 1);
								neighbors[currentTile][bottomRight] = estToGoal(bottomRight, _goal);
								currentTile->addLineTo(bottomRight, PURPLE);
							}

						}
					}
				}
			}
		}

		void PathSearch::initialize(int startRow, int startCol, int goalRow, int goalCol) {
			_start = _tileMap->getTile(startRow, startCol);
			_goal = _tileMap->getTile(goalRow, goalCol);

			_solution.clear();
			_queue.clear();
			visited.clear();

			PlannerNode* start = new PlannerNode(_start, nullptr, 0, estToGoal(_start, _goal));

			visited[_start] = start;
			_queue.push(start);
		}

		void PathSearch::update(long timeslice) {
			bool doOne = false;
			if (timeslice == 0)
				doOne = true;

			if (doOne) {
				// if the queue is empty, a solution has been found and there are no more steps
				if (!_queue.empty()) {
					PlannerNode* current = _queue.front();
					current->_self->setFill(blue);
					_queue.pop();

					if (current->_self == _goal) {
						// need parent so that we can draw line without drawing to nullptr
						while (current->_parent != nullptr) {
							_solution.push_back(current->_self);
							current->_self->addLineTo(current->_parent->_self, red);
							current = current->_parent;
						}

						_solution.push_back(current->_self);

						return;
						
					}

					for (auto it = neighbors[current->_self].begin() ; it != neighbors[current->_self].end(); it++) {
						Tile* successor = it->first;
						float newCost = current->gCost + (successor->getWeight()*2*_tileMap->getTileRadius());
						
						// if the successor tile has not yet been visited
						if (visited[successor] == NULL) {
							PlannerNode* successorPlanner = new PlannerNode(successor, current, newCost, it->second);
							successorPlanner->_self->setFill(PURPLE);

							visited[successor] = successorPlanner;
							_queue.push(successorPlanner);
						}
						else {
							if (newCost < visited[successor]->gCost) {
								_queue.remove(visited[successor]);
								visited[successor]->gCost = newCost;
								visited[successor]->fCost = visited[successor]->hCost + newCost;
								visited[successor]->_parent = current;
								_queue.push(visited[successor]);
							}
						}
					}
				}
			}
			else {
				auto start = chrono::high_resolution_clock::now();
				auto current = chrono::high_resolution_clock::now();

				while (chrono::duration_cast<chrono::milliseconds>(current-start).count() < timeslice) {
					if (!_queue.empty()) {
						PlannerNode* current = _queue.front();
						current->_self->setFill(blue);
						_queue.pop();

						if (current->_self == _goal) {
							// need parent so that we can draw line without drawing to nullptr
							while (current->_parent != nullptr) {
								_solution.push_back(current->_self);
								current->_self->addLineTo(current->_parent->_self, red);
								current = current->_parent;
							}

							_solution.push_back(current->_self);

							return;

						}

						for (auto it = neighbors[current->_self].begin(); it != neighbors[current->_self].end(); it++) {
							Tile* successor = it->first;
							float newCost = current->gCost + (successor->getWeight() * 2 * _tileMap->getTileRadius());

							// if the successor tile has not yet been visited
							if (visited[successor] == NULL) {
								PlannerNode* successorPlanner = new PlannerNode(successor, current, newCost, it->second);
								successorPlanner->_self->setFill(PURPLE);

								visited[successor] = successorPlanner;
								_queue.push(successorPlanner);
							}
							else {
								if (newCost < visited[successor]->gCost) {
									_queue.remove(visited[successor]);
									visited[successor]->gCost = newCost;
									visited[successor]->fCost = visited[successor]->hCost + newCost;
									visited[successor]->_parent = current;
									_queue.push(visited[successor]);
								}
							}
						}
					}
					
					current = chrono::high_resolution_clock::now();
				}
			}
		}

		void PathSearch::shutdown() {
			for (auto i = visited.begin(); i != visited.end(); i++) {
				delete i->second;
			}
			visited.clear();
			_queue.clear();
		}

		void PathSearch::unload() {
			shutdown();

			_tileMap = nullptr;
			_start = nullptr;
			_goal = nullptr;
			_solution.clear();
			_queue.clear();
			visited.clear();
			neighbors.clear();
		}

		bool PathSearch::isDone() const {
			return !_solution.empty();
		}

		std::vector<Tile const*> const PathSearch::getSolution() const{
			return _solution;
		}
	}
}  // close namespace ufl_cap4053::searches
