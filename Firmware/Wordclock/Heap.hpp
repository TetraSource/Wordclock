#pragma once

//#define HEAP_DEBUG

namespace Wordclock
{
	#define POW2(n) (1 << (n))
	#define PARENT(child) (child >> 1)
	#define CHILD1(parent) (((parent+1) << 1)-1)
	#define CHILD2(parent) ((parent+1) << 1)

	// Data must implement the functions
	// Data(const Data &data)
	//   The copy constructor.
	// bool operator < (const Data &data)
	//   Returns whether this object has a lower priority than data.
	// void freeze()
	//   Is called before a sequence of operator < is called.
	//   Fill it with content
	template<typename SizeType, class Data, bool min = true>
	class Heap {
	protected:
		Data **items;
		SizeType size;
		uint8_t deepth;

		void reallocate();
		void swap(const SizeType &idx0, const SizeType &idx1);
		void removeRoot(SizeType root);
		void heapifyUp(SizeType root);
		void heapifyDown(SizeType root);
	public:
		Heap();
		~Heap();
		Data *getMin();
		bool isEmpty();
		void removeMin();
		void insert(const Data &data);
	#ifdef HEAP_DEBUG
		void print(void (*printItem)(Data*)) const;
	#endif
	};

#ifdef HEAP_DEBUG
#define REP(s, n) { \
	uint8_t m = n; for (uint8_t i = 0; i < m; i++) Serial.print(s); \
}

	template<typename SizeType, class Data, bool min>
	void Heap<SizeType, Data, min>::print(void (*printItem)(Data*)) const
	{
		if (size == 0) {
			Serial.println("~");
		}
		else {
			for (uint8_t i = 0; i < deepth; i++) {
				REP(' ', POW2(deepth - i)-2);
				SizeType j = 0;
				while (true) {
					(*printItem)(items[POW2(i) + j - 1]);
					j++;
					if (j < POW2(i) && POW2(i) + j <= size) {
						REP(' ', POW2(deepth - i + 1)-1);
					}
					else
						break;
				}
				Serial.println();
			}
		}
		REP('-', POW2(deepth+1)); Serial.println();
	}

//#undef REP
#endif

	template<typename SizeType, class Data, bool min>
	Heap<SizeType, Data, min>::Heap()
	{
		items = nullptr;
		size = 0;
		deepth = 0;
	}

	template<typename SizeType, class Data, bool min>
	Heap<SizeType, Data, min>::~Heap()
	{
		for (SizeType i = 0; i < size; i++)
			delete items[i];
		delete[] items;
	}

	template<typename SizeType, class Data, bool min>
	void Heap<SizeType, Data, min>::reallocate()
	{
		Data **newItems = new Data*[POW2(deepth)-1];
		for (SizeType i = 0; i < size; i++)
			newItems[i] = items[i];
		delete[] items;
		items = newItems;
	}

	template<typename SizeType, class Data, bool min>
	void Heap<SizeType, Data, min>::swap(
		const SizeType &idx1, const SizeType &idx2)
	{
		Data *temp = items[idx1];
		items[idx1] = items[idx2];
		items[idx2] = temp;
	}

	template<typename SizeType, class Data, bool min>
	void Heap<SizeType, Data, min>::heapifyUp(SizeType root)
	{
		while (root > 0 && (
			min && *items[root] < *items[PARENT(root)] ||
			!min && *items[PARENT(root)] < *items[root]))
		{
			swap(PARENT(root), root);
			root = PARENT(root);
		}
	}

	template<typename SizeType, class Data, bool min>
	void Heap<SizeType, Data, min>::heapifyDown(SizeType root)
	{
		while (CHILD1(root) < size) {
			if (CHILD2(root) >= size ||
				min && *items[CHILD1(root)] < *items[CHILD2(root)] ||
				!min && *items[CHILD2(root)] < *items[CHILD1(root)])
			{
				if (min && *items[CHILD1(root)] < *items[root] ||
					!min && *items[root] < *items[CHILD1(root)])
				{
					swap(root, CHILD1(root));
					root = CHILD1(root);
				}
				else
					break;
			}
			else {
				if (min && *items[CHILD2(root)] < *items[root] ||
					!min && *items[root] < *items[CHILD2(root)])
				{
					swap(root, CHILD2(root));
					root = CHILD2(root);
				}
				else
					break;
			}
		}
	}

	template<typename SizeType, class Data, bool min>
	void Heap<SizeType, Data, min>::removeRoot(SizeType root)
	{
		if (root >= size)
			return;
		delete items[root];
		if (--size == 0) {
			delete[] items;
			items = nullptr;
			deepth = 0;
		}
		else {
			if(root != size) {
				items[root] = items[size];
				heapifyDown(root);
			}
			if (size <= POW2(deepth-1)-1) {
				deepth--;
				reallocate();
			}
		}
	}

	template<typename SizeType, class Data, bool min>
	void Heap<SizeType, Data, min>::insert(const Data &data)
	{
		if (size == POW2(deepth)-1) {
			if (deepth+1 == sizeof(SizeType) << 3)
				return;
			deepth++;
			reallocate();
		}
		items[size] = new Data(data);
		heapifyUp(size);
		size++;
	}

	template<typename SizeType, class Data, bool min>
	inline bool Heap<SizeType, Data, min>::isEmpty()
	{
		return size == 0;
	}

	template<typename SizeType, class Data, bool min>
	inline Data *Heap<SizeType, Data, min>::getMin()
	{
		return !isEmpty() ? items[0] : nullptr;
	}

	template<typename SizeType, class Data, bool min>
	inline void Heap<SizeType, Data, min>::removeMin()
	{
		removeRoot(0);
	}
}