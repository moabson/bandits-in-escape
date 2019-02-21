#ifndef TEXTURE_H
#define TEXTURE_H


#include <GL/glut.h>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using std::string;
using std::cout;
using std::endl;

class Texture
{
private:
    static sf::Image image;
    static bool loadImage(string name);

public:
    Texture() {}
    static bool loadTexture(string name, GLuint index);
    static GLuint textures[20];
    static bool sphere;
};

#endif // TEXTURE_H
