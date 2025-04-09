#include "task.h"
#include "person.h"
#include "battle.h"
#include <queue>

task::task(){
    
}

task::task(int timeToComplete, person* assignedPerson)
{
    this->timeToComplete = timeToComplete;
    this->assignedPerson = assignedPerson;
}



moveTask::moveTask(int timeToComplete, person* assignedPerson, direction directionToMove)
{
    this->directionToMove = directionToMove;
    this->timeToComplete = timeToComplete;
    this->assignedPerson = assignedPerson;

}

digTask::digTask(int timeToComplete, person* assignedPerson)
{
    this->timeToComplete = timeToComplete;
    this->assignedPerson = assignedPerson;
}

int digTask::doTask()
{
    assignedPerson->dig();
    return 1;
}



//returns 1 if task is to be removed
int moveTask::doTask()
{
    if(assignedPerson->attributes->stamina<=0) return 0;
    assignedPerson->attributes->stamina--;
    int startingXPos = assignedPerson->xPos;
    int startingYPos = assignedPerson->yPos;
    int landingXPos = assignedPerson->xPos;
    int landingYPos = assignedPerson->yPos;
    switch(directionToMove)
    {
        case NORTH:
            landingYPos--;
            break;
        case SOUTH:
            landingYPos++;
            break;
        case EAST:
            landingXPos++;
            break;
        case WEST:
            landingXPos--;
            break;
        default:
            return 1;
    }
    tile * startingTile = assignedPerson->attachedMap->tileAt(startingXPos, startingYPos);
    tile * landingTile = assignedPerson->attachedMap->tileAt(landingXPos,landingYPos);
    //landingTile will be NULL if moving over edge of map
    if(landingTile == NULL) return 1;
    person * personAtDestination = landingTile->personHere;
    if(personAtDestination==NULL)
    {
        startingTile->personHere=NULL;
        landingTile->personHere=assignedPerson;
        assignedPerson->xPos = landingXPos;
        assignedPerson->yPos = landingYPos;
        assignedPerson->attachedMap->updateTileRender(startingXPos,startingYPos);
        assignedPerson->attachedMap->updateTileRender(landingXPos,landingYPos);
        return 1;
    }
    if(personAtDestination->getIsFriendly()==assignedPerson->getIsFriendly()) return 0;
    if(assignedPerson->getIsFriendly())
    {
        bool playerLoses = initBattle(assignedPerson,personAtDestination);
        if(!playerLoses)
        {
            startingTile->personHere=NULL;
            landingTile->personHere=assignedPerson;
            assignedPerson->xPos = landingXPos;
            assignedPerson->yPos = landingYPos;
            assignedPerson->attachedMap->updateTileRender(startingXPos,startingYPos);
            assignedPerson->attachedMap->updateTileRender(landingXPos,landingYPos);
            return 1;
        }
    }
    else
    {
        bool playerLoses = initBattle(personAtDestination,assignedPerson);
        if(playerLoses)
        {            
            startingTile->personHere=NULL;
            landingTile->personHere=assignedPerson;
            assignedPerson->xPos = landingXPos;
            assignedPerson->yPos = landingYPos;
            assignedPerson->attachedMap->updateTileRender(startingXPos,startingYPos);
            assignedPerson->attachedMap->updateTileRender(landingXPos,landingYPos);
            return 1;
        }
    }
    return 1;
}

int moveTask::getTimeRemaining()
{
	return timeToComplete;
}

int task::getTimeRemaining()
{
	return timeToComplete;
}

task::~task()
{

}

moveTask::~moveTask()
{

}

