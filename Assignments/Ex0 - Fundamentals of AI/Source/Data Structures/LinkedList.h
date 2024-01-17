

// LinkedList class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 { namespace fundamentals {
	template <typename T>
	class Node {
	public:
		T data;
		Node* previous;
		Node* next;
	};

	template <typename T>
	class Iterator : public LinkedList {
		friend class LinkedList;
	public:
		T operator*();
		Iterator& operator++();
		bool operator==(Iterator const& rhs);
		bool operator!=(Iterator const& rhs);
	};

	template <typename T>
	class LinkedList {
		friend class Iterator;
		Node head;
	public:
		LinkedList<T>() {
			head = nullptr;
		}
		Iterator begin();
		Iterator end();
		bool isEmpty() const {
			if (head == nullptr)
				return true;
			return false;
		}
		T getFront() const {
			return head.data;
		}
		T getBack() const;
		void enqueue(T element);
		void dequeue();
		void pop();
		void clear();
		bool contains(T element) const;
		void remove(T element);
	};

} }  // namespace ufl_cap4053::fundamentals
