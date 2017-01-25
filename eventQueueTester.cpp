#include <iostream>
#include <cstdlib>
#include "eventQueue.cpp"

#define INT_MAX 32767

using namespace std;

//////////////////////////////////////////////
//
// Unrequired helper routine stringToInt()
// Converts a null-terminated string to an int
// If conversion is valid, it returns true
// Otherwise it returns false
//

bool stringToInt (char* toConvert, int& convertedInt) {

  enum IntStates { INITIAL, SIGN, DIGIT };
  IntStates state = INITIAL;
  int value = 0;
  int sign = +1;
  char* pInput = toConvert;

  while (*pInput != 0) {
    switch (state) {
    case INITIAL:
      if (*pInput == '+') {
	sign = +1;
	state = SIGN;
      } else if (*pInput == '-') {
	sign = -1;
	state = SIGN;
      } else if ((*pInput >= '0') && (*pInput <= '9')) {
	state = DIGIT;
	value = ((int)((*pInput) - '0'));
      } else {
	return false;
      }
      ++pInput;
      break;

    case DIGIT:
      if ((*pInput >= '0') && (*pInput <= '9')) {
	state = DIGIT;
	if (value > INT_MAX/10) { // We will overflow if the digit is > 7 (or 8 if sign is -ve)
	  return false;           // Code for this corner case is more than I'm doing now
	}                         // And I refuse to use the cheesy "long long" solution.
	value = 10*value + ((int)((*pInput) - '0'));
      } else {
	return false;
      }
      ++pInput;
      break;

    case SIGN:
      if ((*pInput >= '0') && (*pInput <= '9')) {
	state = DIGIT;
	value = ((int)((*pInput) - '0'));
      } else {
	return false;
      }
      ++pInput;
      break;

    default:
      return false;
      break;
    }
  }
  if (state != DIGIT) {
    return false;
  } else {
    convertedInt = value * sign;
    return true;
  }
}

///////////////////////////////////////////////////////////////////////
//
// EventQueue Tester
// Generate and enqueue random events
// Then dequeue and display them
//

int main(int argc, char* argv[]) {
  int randomSeed;
  int numberEvents;

  if (argc != 3) {
    cerr << "Usage Error: " << argv[0] << " : <random seed> <number of queue elements>" << endl;
    exit(-1);
  }
  if (!stringToInt(argv[1], randomSeed)) {
    cerr << "Usage Error: " << argv[1] << " : <random seed> <number of queue elements>\nArgument 1 was not an integer" << endl;
    exit(-1);
  }
  if (!stringToInt(argv[2], numberEvents) || numberEvents < 1) {
    cerr << "Usage Error: " << argv[2] << " : <random seed> <number of queue elements>\nArgument 2 was not an positive integer" << endl;
    exit(-1);
  }

  srand(randomSeed);
  
  for (int i = 0; i < numberEvents; ++i) {
    // Generate a random number in the range [0-999]
    // create an event
    // Rotate between the three event types
    // Assign the time to that event
    // enqueue the event

    Event* e = new Event;
    e->time = rand()%1000;
    if (i%3 == 0) {
      e->type = LOADSTORE;
    } else if (i%3 == 1) {
      e->type = ALU;
    } else if (i%3 == 2) {
      e->type = COMPAREJUMP;
    }
    if (!enqueue(e)) {
      cerr << "Error: unable to enqueue event.  Exiting" << endl;
      exit(-1);
    }
    cout << "Event " << i << " of type ";
    switch (e->type) {
    case LOADSTORE: 
      cout << "Load/Store";
      break;
      
    case ALU:
      cout << "ALU";
      break;

    case COMPAREJUMP:
      cout << "Compare/Jump";
      break;

    default:
      cout << "Unknown";
      break;
    }
    cout << " to occur at simulated time " << e->time << " has been generated and enqueued" << endl;
  }

  bool done = false;
  int eventNumber = 0;
  const Event* currentEvent = 0;

  while (!done) {
    currentEvent = dequeue();
    if (currentEvent == 0) {
      done = true;
    }
    else {
      cout << "Event " << eventNumber << " occured at simulated time " << currentEvent->time << " and was of type ";
      switch (currentEvent->type) {
      case LOADSTORE: 
	cout << "Load/Store";
	break;
	
      case ALU:
	cout << "ALU";
	break;
	
      case COMPAREJUMP:
	cout << "Compare/Jump";
	break;
	
      default:
	cout << "Unknown";
	break;
      }
      cout << endl;
      delete currentEvent;   // Clean up the event
      ++eventNumber;
    }
  }
}
