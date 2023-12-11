/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Floor.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * Erin Xu, Elaina Kern, Kyra Kendall, Casey Lee
 * erinhua, kernmela, kyrakend, leecasey
 *
 * Final Project - Elevators
 */


#include "Floor.h"

using namespace std;

/*
 * Requires: nothing
 * Modifies: people
 * Effects:  Ticks each person on this floor
 *           Also removes any Person who explodes
 *           Returns the number of exploded people
 */
 int Floor::tick(int currentTime) {
     int j = 0;
     int explodedCount = 0;
     int peopleToRemove[MAX_PEOPLE_PER_FLOOR];
     for (int i = 0; i < numPeople; i++) {
         if (people[i].tick(currentTime)) {
             peopleToRemove[j] = i;
             j++;
             explodedCount++;
         }
     }
     removePeople(peopleToRemove, explodedCount);
     resetRequests();
     return explodedCount;
 }


/*
 * Requires: request != 0
 * Modifies: hasUpRequest, hasDownRequest, numPeople, people
 * Effects:  If there is still room, add newPerson to people.
 *           Updates hasUpRequest or hasDownRequest based on value of request
 */
void Floor::addPerson(Person newPerson, int request) {
    if (numPeople < MAX_PEOPLE_PER_FLOOR) {
        people[numPeople++] = newPerson;
        if (request > 0) {
            hasUpRequest = true;
        }
        else if (request < 0) {
            hasDownRequest = true;
        }
    }
}

/*
 * Requires: numPeopleToRemove >= 0, numPeopleToRemove <= MAX_PEOPLE_PER_FLOOR,
 *           numPeopleToRemove >= 0, numPeopleToRemove <= numPeople,
 *           for all values of i such that 0 <= i < numPeopleToRemove, indicesToRemove[i] < numPeople
 * Modifies: people[], numPeople, hasUpRequest, hasDownRequest
 * Effects:  Removes objects from people[] at indices specified in indicesToRemove[].
 *           The size of indicesToRemove[] is given by numPeopleToRemove.
 *           After removals, calls resetRequests() to update hasUpRequest and hasDownRequest.
 */
 void Floor::removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR], int numPeopleToRemove) {
     
     Person peopleToKeep[MAX_PEOPLE_PER_FLOOR];
     int indexToKeep = 0;
     
     for (int i = 0; i < numPeople; i++) {
         bool keepPerson = 1;
         for (int j = 0; j < numPeopleToRemove; j++) {
             if (i == indicesToRemove[j]) {
                 keepPerson = 0;
             }
         }
                 if (keepPerson) {
                     peopleToKeep[indexToKeep] = people[i];
                     indexToKeep++;
                 }
     }
     for (int i = 0; i < indexToKeep; i++) {
         people[i] = peopleToKeep[i];
     }
     numPeople = indexToKeep;
     resetRequests();
         
}

/*
 * Requires: nothing
 * Modifies: hasUpRequest, hasDownRequest
 * Effects: Search through people to find if there are any
 *          pending up requests or down requests. Set the
 *          values of hasUpRequest and hasDownRequest appropriately.
 *          This function is used to recalculate requests whenever
 *          the people on this floor are added or removed.
 */
void Floor::resetRequests() {
    hasUpRequest = false;
    hasDownRequest = false;

    for (int i = 0; i < numPeople; ++i) {
        int targetFloor = people[i].getTargetFloor();
        int currentFloor = people[i].getCurrentFloor();

        if (targetFloor > currentFloor) {
            hasUpRequest = true;
        }
        else if (targetFloor < currentFloor) {
            hasDownRequest = true;
        }
    }
}


//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Floor::Floor() {
    hasDownRequest = false;
    hasUpRequest = false;
    numPeople = 0;
}

void Floor::prettyPrintFloorLine1(ostream& outs) const {
	string up = "U";
	outs << (hasUpRequest ? up : " ") << " ";
	for (int i = 0; i < numPeople; ++i){
		outs << people[i].getAngerLevel();
		outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
	}
	outs << endl;
}

void Floor::prettyPrintFloorLine2(ostream& outs) const {
	string down = "D";
	outs << (hasDownRequest ? down : " ") << " ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "o   ";
	}
	outs << endl;
}

void Floor::printFloorPickupMenu(ostream& outs) const {
	cout << endl;
	outs << "Select People to Load by Index" << endl;
	outs << "All people must be going in same direction!";
	/*  O   O
	// -|- -|-
	//  |   |
	// / \ / \  */
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " O   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "-|-  ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " |   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "/ \\  ";
	}
	outs << endl << "INDEX:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << i << "   ";
	}
	outs << endl << "ANGER:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getAngerLevel() << "   ";
	}
	outs << endl << "TARGET FLOOR: ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getTargetFloor() << "   ";
	}
}

void Floor::setHasUpRequest(bool hasRequest) {
    hasUpRequest = hasRequest;
}

bool Floor::getHasUpRequest() const {
	return hasUpRequest;
}

void Floor::setHasDownRequest(bool hasRequest) {
    hasDownRequest = hasRequest;
}

bool Floor::getHasDownRequest() const {
	return hasDownRequest;
}

int Floor::getNumPeople() const {
    return numPeople;
}

Person Floor::getPersonByIndex(int index) const {
    return people[index];
}
