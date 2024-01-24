

// LinkedList class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 { namespace fundamentals {
	template <typename T>
	class Node {
	public:
		Node(T data) {
			this->data = data;
			previous = nullptr;
			next = nullptr;
		}
		T data;
		Node<T>* previous;
		Node<T>* next;
	};

	template <typename T>
	class LinkedList {
		Node<T>* head;
		Node<T>* back;
	public:

		template <typename T>
		class Iterator {
		public:
			Iterator(Node<T>* point) {
				ptr = point;
			}
			T operator*() {
				return ptr->data;
			}
			Iterator<T>& operator++() {
				if (ptr)
					ptr = ptr->next;
				return *this;
			}
			bool operator==(Iterator<T> const& rhs) {
				if (ptr == rhs.ptr)
					return true;
				else
					return false;
			}
			bool operator!=(Iterator<T> const& rhs) {
				if (ptr == rhs.ptr)
					return false;
				else
					return true;
			}
		private:
			Node<T>* ptr;
		};


		LinkedList<T>() {
			head = nullptr;
			back = nullptr;
		}
		~LinkedList<T>() {
			clear();
		}
		Iterator<T> begin() const {
			return Iterator<T>(head);
		}
		Iterator<T> end() const {
			return Iterator<T>(nullptr);
		}
		bool isEmpty() const {
			if (head == nullptr)
				return true;
			return false;
		}
		T getFront() const {
			return head->data;
		}
		T getBack() const {
			return back->data;
		}
		void enqueue(T element) {
			if (head == nullptr) {
				head = new Node<T>(element);
				back = head;
			}
			else {
				Node<T>* temp = back;
				back = new Node<T>(element);
				back->previous = temp;
				temp->next = back;
			}
		}
		void dequeue() {
			// If there is only one element in the list
			if (head == back) {
				delete head;
				head = nullptr;
				back = nullptr;
			}
			else {
				Node<T>* temp = head;
				head = temp->next;
				head->previous = nullptr;
				delete temp;
			}
		}
		void pop() {
			if (head == back) {
				delete head;
				head = nullptr;
				back = nullptr;
			}
			else {
				Node<T>* temp = back;
				back = temp->previous;
				back->next = nullptr;
				delete temp;
			}
		}
		void clear() {
			Node<T>* temp = head;
			Node<T>* next = nullptr;
			while (temp != nullptr) {
				next = temp->next;
				delete temp;
				temp = next;
			}
			head = nullptr;
			back = nullptr;
		}
		bool contains(T element) const {
			Node<T>* temp = head;
			while (temp != nullptr) {
				if (temp->data == element)
					return true;
				temp = temp->next;
			}
			return false;
		}
		void remove(T element) {
			if (head == back) {
				delete head;
				head = nullptr;
				back = nullptr;
			}
			Node<T>* temp = head;
			while (temp != nullptr) {
				if (temp->data == element){
					if (temp == head) {
						head = temp->next;
						head->previous = nullptr;
						delete temp;
					}
					else if (temp == back) {
						back = temp->previous;
						back->next = nullptr;
						delete temp;
					}
					else {
						Node<T>* previousN = temp->previous;
						Node<T>* nextN = temp->next;
						delete temp;
						nextN->previous = previousN;
						previousN->next = nextN;
					}
					return;
				}
				temp = temp->next;
			}
		}
	};

} }  // namespace ufl_cap4053::fundamentals
