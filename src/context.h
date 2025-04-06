#pragma once
#ifndef context_h
#define context_h

#include <SFML/Graphics.hpp>
#include "scenes.h"
#include "util.h"

struct globalContext
{
    sf::RenderWindow * window;    
    Scene * scene;
    bool inBattle;

    //
    struct Pos mainCityPos;
};


#endif