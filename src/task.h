#ifndef TASK_H
#define TASK_H
#include <queue>
#include <vector>
class person;

typedef enum direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST
}direction;

class task 
{
    public:
        int timeToComplete;
        person* assignedPerson;
        task();
        task(int timeToComplete, person* assignedPerson);
		virtual int getTimeRemaining();
        virtual int doTask() = 0;
};

class moveTask : virtual public task
{
    public:
        direction directionToMove;
        moveTask(int timeToComplete, person* assignedPerson, direction directionToMove);
		int getTimeRemaining();
        int doTask();

};



#endif