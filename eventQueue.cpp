#include <iostream>
#include "eventQueue.h"

using namespace std;

struct Node {
	const Event* e;
	Node* next;
};

// Enqueue event e
// Return true if sucessful and false otherwise

Node* head = 0;

bool enqueue (const Event* e){
	
	// Negative time error
	if (e == 0 || e->time < 0)
	{
		// cerr << "Error: Negative time detected" << endl;
		return false;
	}
	
	// Creating a new node and setting it's next pointer to head
	Node* inputNode = new Node;
	inputNode->e = e;
	// inputNode->next = 0;
	// cout<<"test"<<endl;
	
	// When list is empty
	if (head == 0)
	{
		inputNode->next = 0;
		head = inputNode;
		return true;
	}
	
	Node* currentNode = head;
	
	//Inseting before the head
	if (head->e->time > e->time)
	{
		inputNode->next = head;
		head = inputNode;
		return true;
	}
	
	// Scrolling through the list
	while ((currentNode->next != 0) && (currentNode->next->e->time <= e->time))
	{
		currentNode = currentNode->next;
	}
	
	// Inserting in the list
	inputNode->next = currentNode->next;
	currentNode->next = inputNode;
	
	return true;
}

// Dequeue
// Return the next event on the queue, ordered by time
// Lowest value of time is output first
// Return 0 if queue is empty

const Event* dequeue()
{
	// When there are no elements in the list
	if (head == 0)
	{
		return 0;
	}
	
	Node* temp = 0;
	
	// When there is only one element in the list
	if (head->next == 0)
	{
		temp = head;
		head = 0;
		return temp->e;
	}
	
	// When there's more than one element in the list
	temp = head;
	head = temp->next;
	return temp->e;
}