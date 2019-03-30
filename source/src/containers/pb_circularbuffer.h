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
	pb_circularbuffer() mMaxSize(S), mCurrentSize(0), mHeadNode(nullptr);
	~pb_circularbuffer()
	{
		Node* currentNode = mHeadNode;
		while (currentNode != nullptr)
		{
			Node* nextNode = currentNode->RightNode;
			delete currentNode;
			currentNode = nextNode;
		}
	};

	size_t Size() const { return mMaxSize; }

	///<summary>Add a new item to the collection. If the max size has already been met, the oldest item is removed.</summary>
	void Add(T data)
	{
		mCurrentSize++;

		Node* newNode = new Node(data);
		if (mHeadNode == nullptr)
		{
			mHeadNode = newNode;
		}		
		else {
			//Get the last item
			Node* currentNode = mHeadNode;
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
			Node* nextNode = mHeadNode->RightNode;
			delete mHeadNode;
			mHeadNode = nextNode;
			mHeadNode->LeftNode = nullptr;
		}
	}

	void Clear()
	{
		Node* currentNode = mHeadNode;
		while (currentNode != nullptr)
		{
			Node* nextNode = currentNode->RightNode;
			delete currentNode;
			currentNode = nextNode;
		}
		mHeadNode = nullptr;
		mCurrentSize = 0;
	}

	bool Contains(T data)
	{
		Node* currentNode = mHeadNode;
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

	size_t mMaxSize;
	size_t mCurrentSize;

	Node* mHeadNode;
};
