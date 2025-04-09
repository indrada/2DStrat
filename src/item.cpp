#include "item.h"
#include "person.h"
#include "tile.h"
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

inventory::inventory(int itemLimit, int weightLimit, locatable* attached) : itemLimit(itemLimit), weightLimit(weightLimit), attached(attached) {}

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
        dropItem(*toAdd, -1, attached->getTile());
        itemDropped = true;
    }
    if(currentWeight > weightLimit)
    {
        dropItem(*toAdd, ((currentWeight - weightLimit) / toAdd->weight) + 1, attached->getTile());
        itemDropped = true;
    }
    if(alreadyHadItem && !itemDropped)
    {
        delete toAdd;
    }
}

void inventory::dropItem(item toDrop, int count, tile * location)
{
    for(int i = 0; i < items.size(); i++)
    {
        if(toDrop == *(items[i]))
        {
            if(count==items[i]->count || count<0){
                location->itemsOnGround.push_back(items[i]);
                items.erase(items.begin()+i);     
                continue;
            }
            else
            {
                items[i]->count-=count;
                item * duplicate = (item *) malloc(sizeof(item));
                *duplicate = *(items[i]);
                duplicate->count=count;
                location->itemsOnGround.push_back(duplicate);
                continue;
            }
        }
    }
    person* assignedPerson = location->personHere;
    if (assignedPerson == NULL) return;

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