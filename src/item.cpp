#include "item.h"
#include "person.h"
#include <iostream>
bool item::operator==(const item& other)
{
    if(name==other.name && value==other.value && weight==other.weight)
    {
        return true;
    }
    return false;
}

item::item(int weight, int value, int count, std::string name) : weight(weight), value(value), count(count), name(name){}

inventory::inventory(int itemLimit, int weightLimit, person * assignedPerson) : weightLimit(weightLimit), itemLimit(itemLimit), assignedPerson(assignedPerson){}

void inventory::addItem(item * toAdd)
{
    currentWeight+=toAdd->weight * toAdd->count;
    bool alreadyHadItem = false;
    bool itemDropped = false;
    for(item * existingItem : items)
    {
        if(*existingItem == *toAdd)
        {
            alreadyHadItem = true;
            existingItem->count += toAdd->count;
            continue;
        }
    }
    if(!alreadyHadItem)
    {
        numItems++;
        items.push_back(toAdd);
    }
    if(numItems > itemLimit)
    {
        dropItem(*toAdd, -1);
        itemDropped = true;
    }
    if(currentWeight > weightLimit)
    {
        dropItem(*toAdd, ((currentWeight - weightLimit) / toAdd->weight) + 1);
        itemDropped = true;
    }
    if(alreadyHadItem && !itemDropped)
    {
        delete toAdd;
    }
}

void inventory::dropItem(item toDrop, int count)
{
    tile * currentTile = assignedPerson->attachedMap->tileAt(assignedPerson->xPos,assignedPerson->yPos);
    for(int i = 0; i < items.size(); i++)
    {
        if(toDrop == *(items[i]))
        {
            if(count==items[i]->count || count<0){
                currentTile->itemsOnGround.push_back(items[i]);
                items.erase(items.begin()+i);     
                continue;
            }
            else
            {
                items[i]->count-=count;
                item * duplicate = (item *) malloc(sizeof(item));
                *duplicate = *(items[i]);
                duplicate->count=count;
                currentTile->itemsOnGround.push_back(duplicate);
                continue;
            }
        }
    }
    printf(("\n" + assignedPerson->name + " couldn't carry " + toDrop.name + " so they dropped ").c_str());
    if(count < 0)
    {
        printf("all");
    }
    else
    {
        printf("%d", count);
    }
    printf(" of them\n");
}

item::item() {}
item::~item() {}