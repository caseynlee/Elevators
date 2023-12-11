/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Move.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * Erin Xu, Elaina Kern, Kyra Kendall, Casey Lee
 * erinhua, kernmela, kyrakend, leecasey
 *
 * Final Project - Elevators
 */
 
#include <cmath>
#include <sstream>
#include <stdio.h>      
#include <stdlib.h>
#include "Move.h"

using namespace std;

Move::Move(string commandString) : Move() {
    if (commandString == ""){
        isPass = true;
        return;
    }
    else if (commandString == "S" || commandString == "s"){
        isSave = true;
        return;
    }
    else if (commandString == "Q" || commandString == "s"){
        isQuit = true;
        return;
    }
    else{
        stringstream iFS(commandString);
        char c;
        char modified;
        iFS >> c >> elevatorId >> modified;
        if (modified == 'f'){
            iFS >> targetFloor;
        }
        else if (modified == 'p'){
            isPickup = true;
        }
        return;
    }
}
/**
 * Requires: elevators represents the elevator servicing states
 * Modifies: nothing
 * Effects:  returns true if this Move instance is valid, false otherwise
 *
 *           The following are the criterion for validity:
 *
 *           If a move is a Pass Move, a Quit Move, or a Save Move it is valid
 *           For both Pickup Moves and Servicing Moves:
 *               0 <= elevatorId < NUM_ELEVATORS
 *               The corresponding Elevator is currently not
 *                  servicing a request.
 *           For Servicing Moves only:
 *               0 <= targetFloor < NUM_FLOORS
 *               targetFloor is different from the corresponding
 *                  Elevator's currentFloor
 */
bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
    if (isPass || isQuit || isSave){
        return true;
    }
    else if (elevatorId >= 0 && elevatorId < NUM_ELEVATORS) {
        if (!elevators[elevatorId].isServicing()) {
            if (isPickup) {
                return true;
            }
            else if (targetFloor >= 0 && targetFloor < NUM_FLOORS) {
                if (targetFloor != elevators[elevatorId].getCurrentFloor()) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, const Floor& pickupFloor) {
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
    int furthestDistance = 0;
    targetFloor = 0;
    for (int i = 0; i < pickupList.length(); i++) {
        peopleToPickup[i] += pickupList[i] - '0';
        numPeopleToPickup ++;
        totalSatisfaction += MAX_ANGER - pickupFloor.getPersonByIndex(peopleToPickup[i]).getAngerLevel();
        if(furthestDistance < abs(pickupFloor.getPersonByIndex(peopleToPickup[i]).getTargetFloor()
                                  - currentFloor)){
            furthestDistance = abs(pickupFloor.getPersonByIndex(peopleToPickup[i]).getTargetFloor()
                                   - currentFloor);
            targetFloor = pickupFloor.getPersonByIndex(peopleToPickup[i]).getTargetFloor();
        }
    }
    
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Move::Move() {
    elevatorId = -1;
    targetFloor = -1;
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
	isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
}

bool Move::isPickupMove() const {
    return isPickup;
}

bool Move::isPassMove() const {
	return isPass;
}

bool Move::isSaveMove() const {
	return isSave;
}

bool Move::isQuitMove() const {
	return isQuit;
}

int Move::getElevatorId() const {
    return elevatorId;
}

int Move::getTargetFloor() const {
    return targetFloor;
}

int Move::getNumPeopleToPickup() const {
    return numPeopleToPickup;
}

int Move::getTotalSatisfaction() const {
    return totalSatisfaction;
}

void Move::setTargetFloor(int inTargetFloor) {
    targetFloor = inTargetFloor;
}

void Move::copyListOfPeopleToPickup(int newList[MAX_PEOPLE_PER_FLOOR]) const {
    for (int i = 0; i < numPeopleToPickup; ++i) {
        newList[i] = peopleToPickup[i];
    }
}
