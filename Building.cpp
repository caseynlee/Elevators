/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Building.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * Erin Xu, Elaina Kern, Kyra Kendall, Casey Lee
 * erinhua, kernmela, kyrakend, leecasey
 *
 * Final Project - Elevators
 */

#include "Building.h"

using namespace std;

void Building::spawnPerson(Person newPerson){
    int currentFloor = newPerson.getCurrentFloor();
    int targetFloor = newPerson.getTargetFloor();
    int request = targetFloor - currentFloor;
    floors[currentFloor].addPerson(newPerson, request);
}

void Building::update(Move move){
    int elevatorId = move.getElevatorId();
    int currentFloor = elevators[elevatorId].getCurrentFloor();
    int targetFloor = move.getTargetFloor();

    //Checks if move is pass move
    if (move.isPassMove()) {
        return;
    }
    //Checks if move is pickup move
    else if (move.isPickupMove()) {

        //Copies the list of people to pickup into an array
        int numOfPeopleToPickup = move.getNumPeopleToPickup();
        int peopleToPickup[MAX_PEOPLE_PER_FLOOR];
        move.copyListOfPeopleToPickup(peopleToPickup);

        //Calls removePeople() on the appropriate floor
        floors[currentFloor].removePeople(peopleToPickup, numOfPeopleToPickup);

     
    }
    //Elevator sent to service the targetFloor
    elevators[elevatorId].serviceRequest(targetFloor);
}

int Building::tick(Move move){
    //Increments time
    time++;

    //Calls update() on input move
    update(move);

    //Ticks all the elevators with the new time
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        elevators[i].tick(time);
    }

    //Ticks all of the buildingFloors, keeping track of new exploded people
    int total = 0;
    for (int i = 0; i < NUM_FLOORS; i++) {
        total += floors[i].tick(time);
    }
    //Returns the total number of people that exploded in this tick
    return total;
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Building::Building() {
    time = 0;
}

void Building::setElevator(string elevatorInfo, int elevatorId) {
    int i = 0;
    string currPosString = "";
    string destPosString = "";

    while(elevatorInfo[i] != 's' && elevatorInfo[i] != 'w'){
        currPosString += elevatorInfo[i];
        i++;
    }
    elevators[elevatorId] = Elevator();

    elevators[elevatorId].setCurrentFloor(stoi(currPosString));

    if (elevatorInfo[i] == 's') {
        i++;
        while (i < (int)elevatorInfo.size()) {
            destPosString += elevatorInfo[i];
            i++;
        }
        elevators[elevatorId].serviceRequest(stoi(destPosString));
    }
}

int Building::getTime() const {
    return time;
}

void Building::setTime(int timeIn) {
    time = timeIn;
}

void Building::prettyPrintBuilding(ostream& outs) const {
    //-------
    //| | | | 🔼 1 4 2
    //| | |E| 🔽 o o o
    //-------
    //| | | | 🔼   1
    //| |E| |      o
    //-------
    //| | | |
    //|E| | |
    for (int i = NUM_FLOORS - 1; i >= 0; --i) {
            outs << "   ";
            for (int j = 0; j < 2 * NUM_ELEVATORS + 1; ++j) {

                outs << '-';
            }

            outs << endl << "   ";

            for (int j = 0; j <  NUM_ELEVATORS; ++j) {
                char floorIndicatorIcon = ' ';

                if (elevators[j].getCurrentFloor() == i &&
                    elevators[j].isServicing()) {

                    char floorServiceIndicator = ('0' + elevators[j].getTargetFloor());
                    floorIndicatorIcon = floorServiceIndicator;
                }

                outs << "|" << floorIndicatorIcon;
            }
            outs << "|";

            floors[i].prettyPrintFloorLine1(outs);
            outs << i << "  ";
            for (int j = 0; j <  NUM_ELEVATORS; ++j) {

                outs << "|" << (elevators[j].getCurrentFloor() == i ? 'E' : ' ');
            }
            outs << "|";

            floors[i].prettyPrintFloorLine2(outs);
        }
        outs << "   ";
        for (int j = 0; j < 2 * NUM_ELEVATORS + 1; ++j) {
            outs << '-';
        }
        outs << endl << "   ";
        for (int j = 0; j < NUM_ELEVATORS; ++j) {
            outs << " " << j;
        }
        outs << endl;
}

Elevator Building::getElevatorById(int elevatorId) const {
    return elevators[elevatorId];
}

Floor Building::getFloorByFloorNum(int floorNum) const {
    return floors[floorNum];
}

BuildingState Building::getBuildingState() const {
    BuildingState buildingState;
    buildingState.turn = time;

    for (int i = 0; i < NUM_FLOORS; ++i) {
        buildingState.floors[i].floorNum = i;
        buildingState.floors[i].hasDownRequest = floors[i].getHasDownRequest();
        buildingState.floors[i].hasUpRequest = floors[i].getHasUpRequest();

        int & np = buildingState.floors[i].numPeople;

        for (int j = 0; j < floors[i].getNumPeople(); ++j) {

            Person p = floors[i].getPersonByIndex(j);
            buildingState.floors[i].people[np++].angerLevel = p.getAngerLevel();
        }
    }

    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        Elevator e = elevators[i];

        buildingState.elevators[i].elevatorId = i;
        buildingState.elevators[i].targetFloor = e.getTargetFloor();
        buildingState.elevators[i].currentFloor = e.getCurrentFloor();
        buildingState.elevators[i].isServicing = e.isServicing();
    }

    return buildingState;
}
