#include <vector>
#include <queue>

// TreeNode class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 { namespace fundamentals {
	template <typename T>
	class TreeNode {
	private:
		std::vector<TreeNode<T>*> children;
		T data;
	public:
		TreeNode<T>() {

		}
		TreeNode<T>(T element) {
			data = element;
		}
		const T& getData() const {
			return data;
		}
		size_t getChildCount() const {
			return children.size();
		}
		TreeNode<T>* getChild(size_t index) {
			return children[index];
		}
		TreeNode<T>* getChild(size_t index) const {
			return children[index];
		}
		void addChild(TreeNode<T>* child) {
			children.push_back(child);
		}
		TreeNode<T>* removeChild(size_t index) {
			TreeNode<T>* temp = children[index];
			children.erase(children.begin()+index);
			return temp;
		}
		void breadthFirstTraverse(void (*dataFunction)(const T)) const {
			std::queue<TreeNode<T>*> list;

			// Deal with root node, then continue to children and so on
			dataFunction(data);

			for (auto child : children) {
				list.push(child);
			}

			while (!list.empty()) {
				TreeNode<T>* current = list.front();
				list.pop();
				dataFunction(current->data);

				for (auto child : current->children) {
					list.push(child);
				}
			}
			
		}
		void preOrderTraverse(void (*dataFunction)(const T)) const {
			dataFunction(data);
			for (auto child : children) {
				child->preOrderTraverse(dataFunction);
			}
		}
		void postOrderTraverse(void (*dataFunction)(const T)) const {
			for (auto child : children) {
				child->postOrderTraverse(dataFunction);
			}
			dataFunction(data);
		}
	};	

}}  // namespace ufl_cap4053::fundamentals
