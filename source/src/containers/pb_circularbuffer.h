#pragma once

template <typename T>
struct Node
{
	T ThisNode;
	Node* LeftNode;
	Node* RightNode;

	Node(T data)
	{
		ThisNode = data;
		LeftNode = nullptr;
		RightNode = nullptr;
	}
};

template <typename T, size_t S>
class pb_circularbuffer
{
public:
	pb_circularbuffer() { mMaxSize = S; mCurrentSize = 0; mHeadNode = nullptr; };
	~pb_circularbuffer()
	{
		Node<T>* currentNode = mHeadNode;
		while (currentNode != nullptr)
		{
			Node<T>* nextNode = currentNode->RightNode;
			delete currentNode;
			currentNode = nextNode;
		}
	};

	size_t Size() const { return mMaxSize; }

	///<summary>Add a new item to the collection. If the max size has already been met, the oldest item is removed.</summary>
	void Add(T data, bool checkcontains = false)
	{
		if (!checkcontains || (checkcontains && !Contains(data)))
		{
			mCurrentSize++;

			Node<T>* newNode = new Node<T>(data);
			if (mHeadNode == nullptr)
			{
				mHeadNode = newNode;
			}
			else {
				//Get the last item
				Node<T>* currentNode = mHeadNode;
				while (currentNode->RightNode != nullptr)
				{
					currentNode = currentNode->RightNode;
				}
				currentNode->RightNode = newNode;
				newNode->LeftNode = currentNode;
				newNode->RightNode = nullptr;
			}

			//If the current size is now bigger than the max, delete the head node and reassign it the value of its child...
			//e.g. shifting all items left one.
			if (mCurrentSize > mMaxSize)
			{
				Node<T>* nextNode = mHeadNode->RightNode;
				delete mHeadNode;
				mHeadNode = nextNode;
				mHeadNode->LeftNode = nullptr;
				mCurrentSize--;
			}
		}
	}
		
	T At(int index)
	{
		Node<T>* currentNode = mHeadNode;
		int count = 0;
		while (count < index)
		{
			if (currentNode == nullptr)
				return nullptr;

			currentNode = currentNode->RightNode;
			count++;
		}
		return currentNode->ThisNode;
	}

	void Clear()
	{
		Node<T>* currentNode = mHeadNode;
		while (currentNode != nullptr)
		{
			Node<T>* nextNode = currentNode->RightNode;
			delete currentNode;
			currentNode = nextNode;
		}
		mHeadNode = nullptr;
		mCurrentSize = 0;
	}

	bool Contains(T data)
	{
		Node<T>* currentNode = mHeadNode;
		while (currentNode != nullptr)
		{
			if (currentNode->ThisNode == data)
			{
				return true;
			}
			currentNode = currentNode->RightNode;
		}
		return false;
	}


private:

	std::size_t mMaxSize;
	std::size_t mCurrentSize;

	Node<T>* mHeadNode;
};
