#ifndef MAIN_H
#define MAIN_H

#include "Camera.hpp" // camera object
#include "Model.h" // model object
#include "Textures/Texture.h" // texture functions
#include "UnitCube.h" // 1x1 textured cube

#include <GLFW/glfw3.h> // util lib for window, io, and context management

#include <glm/glm.hpp> // math lib
#include <glm/gtc/matrix_transform.hpp> // provides functions to create common transformation matrices
#include <glm/gtc/type_ptr.hpp> // provides functions for handling pointers to vector and matrix types

#include <map> // used in blending transparent textures to render in order by distance

#endif
