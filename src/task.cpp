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

int moveTask::doTask()
{
    bool battleResult = false;
	if(assignedPerson->attributes->stamina < 1) return 0;
    switch (directionToMove)
    {
    case NORTH:
        if (assignedPerson->yPos == 0) return 1;
        if (assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos - 1)->personHere != NULL && assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos - 1)->personHere->getIsFriendly()) return 0;
        if(assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos - 1)->personHere != NULL && !assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos - 1)->personHere->getIsFriendly()) battleResult = initBattle(assignedPerson, assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos - 1)->personHere);
        assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos)->personHere = NULL;
        assignedPerson->attachedMap->updateTileRender(assignedPerson->xPos, assignedPerson->yPos);
        if(battleResult) return 1;
        assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos - 1)->personHere = assignedPerson;
        assignedPerson->yPos--;
        assignedPerson->attachedMap->updateTileRender(assignedPerson->xPos, assignedPerson->yPos);
		assignedPerson->attributes->stamina--;
        return 1;
        break;
    case SOUTH:
        if (assignedPerson->yPos == assignedPerson->attachedMap->verticalSize - 1) return 1;
        if (assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos + 1)->personHere != NULL && assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos + 1)->personHere->getIsFriendly()) return 0;
        if(assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos + 1)->personHere != NULL && !assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos + 1)->personHere->getIsFriendly()) battleResult = initBattle(assignedPerson, assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos + 1)->personHere);
        assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos)->personHere = NULL;
        assignedPerson->attachedMap->updateTileRender(assignedPerson->xPos, assignedPerson->yPos);
        if(battleResult) return 1;
        assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos + 1)->personHere = assignedPerson;
        assignedPerson->yPos++;
        assignedPerson->attachedMap->updateTileRender(assignedPerson->xPos, assignedPerson->yPos);
		assignedPerson->attributes->stamina--;
        return 1;
        break;
    case WEST:
        if (assignedPerson->xPos == 0) return 1;
        if (assignedPerson->attachedMap->tileAt(assignedPerson->xPos - 1, assignedPerson->yPos)->personHere != NULL && assignedPerson->attachedMap->tileAt(assignedPerson->xPos - 1, assignedPerson->yPos)->personHere->getIsFriendly()) return 0;
        if(assignedPerson->attachedMap->tileAt(assignedPerson->xPos - 1, assignedPerson->yPos)->personHere != NULL && !assignedPerson->attachedMap->tileAt(assignedPerson->xPos - 1, assignedPerson->yPos)->personHere->getIsFriendly()) battleResult = initBattle(assignedPerson, assignedPerson->attachedMap->tileAt(assignedPerson->xPos - 1, assignedPerson->yPos)->personHere);
        assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos)->personHere = NULL;
        assignedPerson->attachedMap->updateTileRender(assignedPerson->xPos, assignedPerson->yPos);
        if(battleResult) return 1;
        assignedPerson->attachedMap->tileAt(assignedPerson->xPos - 1, assignedPerson->yPos)->personHere = assignedPerson;
        assignedPerson->xPos--;
        assignedPerson->attachedMap->updateTileRender(assignedPerson->xPos, assignedPerson->yPos);
		assignedPerson->attributes->stamina--;
        return 1;
        break;
    case EAST:
        if (assignedPerson->xPos == assignedPerson->attachedMap->horizontalSize-1) return 1;
        if (assignedPerson->attachedMap->tileAt(assignedPerson->xPos + 1, assignedPerson->yPos)->personHere != NULL && assignedPerson->attachedMap->tileAt(assignedPerson->xPos + 1, assignedPerson->yPos)->personHere->getIsFriendly()) return 0;
        if(assignedPerson->attachedMap->tileAt(assignedPerson->xPos + 1, assignedPerson->yPos)->personHere != NULL && !assignedPerson->attachedMap->tileAt(assignedPerson->xPos + 1, assignedPerson->yPos)->personHere->getIsFriendly()) battleResult = initBattle(assignedPerson, assignedPerson->attachedMap->tileAt(assignedPerson->xPos+1, assignedPerson->yPos)->personHere);
        assignedPerson->attachedMap->tileAt(assignedPerson->xPos, assignedPerson->yPos)->personHere = NULL;
        assignedPerson->attachedMap->updateTileRender(assignedPerson->xPos, assignedPerson->yPos);
        if(battleResult) return 1;
        assignedPerson->attachedMap->tileAt(assignedPerson->xPos + 1, assignedPerson->yPos)->personHere = assignedPerson;
        assignedPerson->xPos++;
        assignedPerson->attachedMap->updateTileRender(assignedPerson->xPos, assignedPerson->yPos);
		assignedPerson->attributes->stamina--;
        return 1;
        break;
    default:
        return 0;
    }
}

int moveTask::getTimeRemaining()
{
	return timeToComplete;
}

int task::getTimeRemaining()
{
	return timeToComplete;
}




