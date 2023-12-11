/*
 * Copyright 2023 University of Michigan EECS183
 *
 * AI.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */

#include "AI.h"
#include <cassert>

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core

string getAIMoveString(const BuildingState& buildingState) {
    bool isAllFloorEmpty = true;
    //Pass move if all floors are empty
    for (int i = 0; i < NUM_FLOORS; i++){
        if (buildingState.floors[i].numPeople != 0){
            isAllFloorEmpty = false;
        }
    }
    if (isAllFloorEmpty == true){
        return "";
    }
    
    bool e0IsServing = buildingState.elevators[0].isServicing;
    bool e1IsServing = buildingState.elevators[1].isServicing;
    bool e2IsServing = buildingState.elevators[2].isServicing;
    //Pass move if all elevators are busy
    //Only E0 works for floor 0 and 9
    if (e0IsServing && e1IsServing && e2IsServing){
        return "";
    }
    
    int floorwithMostPeople = 0;
    
    if (!e0IsServing){
        //Pick up people in current floor if there are
        if (buildingState.floors[buildingState.elevators[0].currentFloor].numPeople != 0){
            return "e0p";
        }
        //Go to the floor with most people if there are not people in current floor
        else {
            int mostPeopleonFloor = 0;
            for (int i = 0; i < NUM_FLOORS; i++){
                //Make sure target floor for each elevator is not the same
                if ((buildingState.floors[i].numPeople > mostPeopleonFloor) && (buildingState.elevators[1].targetFloor != i) && (buildingState.elevators[2].targetFloor != i)){
                    floorwithMostPeople = i;
                    mostPeopleonFloor = buildingState.floors[i].numPeople;
                }
            }
            if (mostPeopleonFloor == 0){
                return "";
            }
            else {
                string targetFloor = to_string(floorwithMostPeople);
                return "e0f" + targetFloor;
            }
        }
    }
    
    
    else if (!e1IsServing){
        //Pick up people in current floor if there are
        if (buildingState.floors[buildingState.elevators[1].currentFloor].numPeople != 0 && buildingState.elevators[1].currentFloor != 0 && buildingState.elevators[1].currentFloor != 9){
            return "e1p";
        }
        //Go to the floor with most people if there are not people in current floor
        else {
            int mostPeopleonFloor = 0;
            for (int i = 0; i < NUM_FLOORS; i++){
                //Make sure target floor for each elevator is not the same
                if ((buildingState.floors[i].numPeople > mostPeopleonFloor) && (buildingState.elevators[0].targetFloor != i) && (buildingState.elevators[2].targetFloor != i)){
                    floorwithMostPeople = i;
                    mostPeopleonFloor = buildingState.floors[i].numPeople;
                }
            }
            //Avoid always working for 0 & 9
            if ((buildingState.elevators[1].currentFloor == 0 && buildingState.floors[0].numPeople == mostPeopleonFloor && mostPeopleonFloor != 0) || (buildingState.elevators[1].currentFloor == 9 && buildingState.floors[9].numPeople == mostPeopleonFloor && mostPeopleonFloor != 0)){
                return "e1p";
            }
            else{
                if (mostPeopleonFloor == 0){
                    return "";
                }
                else {
                    string targetFloor = to_string(floorwithMostPeople);
                    return "e1f" + targetFloor;
                }
            }
        }
    }
    
    else {
        //Pick up people in current floor if there are
        if (buildingState.floors[buildingState.elevators[2].currentFloor].numPeople != 0 && buildingState.elevators[1].currentFloor != 0 && buildingState.elevators[1].currentFloor != 9){
            return "e2p";
        }
        //Go to the floor with most people if there are not people in current floor
        else {
            int mostPeopleonFloor = 0;
            for (int i = 0; i < NUM_FLOORS; i++){
                //Make sure target floor for each elevator is not the same
                if ((buildingState.floors[i].numPeople > mostPeopleonFloor) && (buildingState.elevators[1].targetFloor != i) && (buildingState.elevators[0].targetFloor != i)){
                    floorwithMostPeople = i;
                    mostPeopleonFloor = buildingState.floors[i].numPeople;
                }
            }
            //Avoid always working for 0 & 9
            if ((buildingState.elevators[1].currentFloor == 0 && buildingState.floors[0].numPeople == mostPeopleonFloor && mostPeopleonFloor != 0) || (buildingState.elevators[1].currentFloor == 9 && buildingState.floors[9].numPeople == mostPeopleonFloor && mostPeopleonFloor != 0)){
                return "e2p";
            }
            else{
                if (mostPeopleonFloor == 0){
                    return "";
                }
                else {
                    string targetFloor = to_string(floorwithMostPeople);
                    return "e2f" + targetFloor;
                }
            }
        }
    }
}




string getAIPickupList(const Move& move, const BuildingState& buildingState, 
                       const Floor& floorToPickup) {
    string pickUpList = "";
    int angelforUp = 0;
    int angelforDown = 0;
    string pickUpforUp = "";
    string pickUpforDown = "";
        //Decide the request direction with larger angel level
    for (int i = 0; i < floorToPickup.getNumPeople(); i++){
        if (floorToPickup.getPersonByIndex(i).getTargetFloor() > floorToPickup.getPersonByIndex(i).getCurrentFloor()){
                angelforUp += floorToPickup.getPersonByIndex(i).getAngerLevel();
                pickUpforUp += to_string(i);
        }
        else if (floorToPickup.getPersonByIndex(i).getTargetFloor() < floorToPickup.getPersonByIndex(i).getCurrentFloor()){
                angelforDown += floorToPickup.getPersonByIndex(i).getAngerLevel();
                pickUpforDown += to_string(i);
        }
    }
    //pick Up if only Uprequest
    if (floorToPickup.getHasUpRequest() && !floorToPickup.getHasDownRequest()){
        pickUpList = pickUpforUp;
    }
    //pick Down if only Downrequest
    else if (!floorToPickup.getHasUpRequest() && floorToPickup.getHasDownRequest()){
        pickUpList = pickUpforDown;
    }
    else{
        //Go up if people with up request have more angel level
        if (angelforUp > angelforDown){
            pickUpList = pickUpforUp;
        }
        //Go down if people with down request have more angel level
        else if (angelforDown > angelforUp){
            pickUpList = pickUpforDown;
        }
        //if equal, then decide the number of Uprequest or Downrequest
        else {
            int numUpRequest = 0;
            int numDownRequest = 0;
            for (int i = 0; i < NUM_FLOORS; i++){
                if (buildingState.floors[i].hasUpRequest){
                    numUpRequest ++;
                }
                else if (buildingState.floors[i].hasDownRequest){
                    numDownRequest ++;
                }
            }
            if (numUpRequest >= numDownRequest){
                pickUpList = pickUpforDown;
            }
            else {
                pickUpList = pickUpforUp;
            }
        }
    }
        return pickUpList;
    }


