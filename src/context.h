#ifndef context_h
#define context_h

#include <SFML/Graphics.hpp>
#include "scenes.h"

struct globalContext
{
    sf::RenderWindow * window;    
    Scene * scene;
};

struct globalContext context;

#endif