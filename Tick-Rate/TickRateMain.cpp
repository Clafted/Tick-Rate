#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <thread>
#include <chrono>

#include "Headers/Map.h"
#include "Headers/MapReader.h"
#include "Headers/Object.h"
#include "Headers/AnimatedObject.h"
#include "Headers/MovingObject.h"
#include "Headers/SolidAnimatedObject.h"
#include "Headers/Atlas.h"
#include "Headers/Spritesheet.h"
#include "Headers/Platform.h"
#include "Headers/Entity.h"
#include "Headers/Player.h"
#include "Headers/Enemy.h"
#include "Headers/Shader.hpp"

GLFWwindow* window;
int windowWidth = 800, windowHeight = 600;

void processInput(GLFWwindow* window);
void gameLoop();

void updateTextureCoordinates(unsigned int &VBO, float * bufferData);
void addAnimatedObject(Map::TILETYPE type, std::string texturePath, glm::vec2 position = glm::vec2(0.0f), int width = 1, int height = 1, float animationRate = 0.5f);
void createVAOandVBO(unsigned int& VAO, unsigned int& VBO, Object * object);
void mouseClick(GLFWwindow* window, int button, int action, int mods);
void mouseMove(GLFWwindow* window, double xPos, double yPos);
void windowResize(GLFWwindow* window, int width, int height);

//Game loop functions
void update(float deltaTime);
void checkMovingObjectsCollisions(float deltaTime);
void checkMapCollisions(float deltaTime);

glm::vec2 blockPosition(int index, int tileValue);

bool mouseLeftHeldDown;

float deltaTime;

std::vector<MovingObject*> movingObjects{};
std::vector<Entity*> entities{};
Player* player;
Enemy* taizer;

Object* tile;
AnimatedObject* platform;
Map* map;
int accessedTile;

glm::mat4 view;
glm::mat4 model;


int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Try to create a window
    window = glfwCreateWindow(windowWidth, windowHeight, "IDK", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Initialize GLAD (Callback linker)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, windowWidth, windowHeight);

    //Set callbacks
    glfwSetFramebufferSizeCallback(window, windowResize);
    glfwSetMouseButtonCallback(window, mouseClick);
    glfwSetCursorPosCallback(window, mouseMove);

    //Create MapReader Instance
    MapReader::getInstance();
    MapReader::loadMap("Maps/Map.txt");

    //Start game
    gameLoop();

    //Exit
    glfwTerminate();
    return 0;
}

void gameLoop()
{
    //LOAD ALL OF THE DATA

    //VAOs and VBOs
    unsigned int bgVAO;
    unsigned int bgVBO;

    //Shaders
    Shader textureShader("Shaders/textureShader.vs", "Shaders/textureShader.fs");
    textureShader.use();
    textureShader.setInt("txtr", 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Background
    //Vertices
    float bgVertices[30];

    glGenVertexArrays(1, &bgVAO);
    glBindVertexArray(bgVAO);
    glGenBuffers(1, &bgVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bgVertices), bgVertices, GL_STATIC_DRAW);
    //Set attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Texture backgroundT("Textures/CitySkyline_Bg.png");
    float bgHalfWidth = backgroundT.Width() / (float)backgroundT.Height();
    float bgHalfHeight = 1.0f;
    
    float newbgVertices[] = 
    {
        //Position                         Texture
        -bgHalfWidth, -bgHalfHeight, 0.0f, 0.0f, 0.0f,
        -bgHalfWidth, bgHalfHeight, 0.0f, 0.0f, 1.0f,
        bgHalfWidth, bgHalfHeight, 0.0f, 1.0f, 1.0f,
        bgHalfWidth, bgHalfHeight, 0.0f, 1.0f, 1.0f,
        bgHalfWidth, -bgHalfHeight, 0.0f, 1.0f, 0.0f,
        -bgHalfWidth, -bgHalfHeight, 0.0f, 0.0f, 0.0f 
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(newbgVertices), newbgVertices, GL_STATIC_DRAW);

    glBindTexture(GL_TEXTURE_2D, backgroundT.TextureID());
    glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "model"), 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
    glBindVertexArray(0);

    //REGULAR OBJECTS
    //Tiles
    tile = new Object(new Atlas("Textures/Map_Atlas.png", 5, 3, 20, 20), glm::vec2(0.0f, 0.0f), 20, 20);
    createVAOandVBO(tile->VAO, tile->VBO, tile);

    //ANIMATED OBJECTS
    //Platforms
    platform = new SolidAnimatedObject(new Spritesheet("Textures/PlatformAtlas.png", 1, 2, 60, 5), glm::vec2(60.0f, 140.0f), 60, 5);
    platform->addState(1, 5);
    platform->setState(0);
    platform->setXVelocity(0.5f);
    createVAOandVBO(platform->VAO, platform->VBO, platform);
    movingObjects.push_back(platform);

    //ENTITIES
    //Enemy
    taizer = new Enemy(new Spritesheet("Textures/Taizer_SS.png", 0 ,0, 10, 20), glm::vec2(400.0f, tile->Height() * 15), 10, 20);
    createVAOandVBO(taizer->VAO, taizer->VBO, taizer);
    movingObjects.push_back(taizer);

    //Player
    player = new Player(new Spritesheet("Textures/ParkourMan_SS.png", 4, 1, 20, 30), glm::vec2(40.0f, tile->Height() * 12), 10, 30);
    player->addState(12, 30);
    player->addState(1, 60);
    createVAOandVBO(player->VAO, player->VBO, player);
    movingObjects.push_back(player);

    map = MapReader::CurrentMap();

    float previousElapsedTime = (float)glfwGetTime();

    //Start actual game loop
    while (!glfwWindowShouldClose(window) && player->numLives() > 0)
    {
        // Update deltaTime
        deltaTime = ((float)glfwGetTime() - previousElapsedTime);
        previousElapsedTime = (float)glfwGetTime();

        // Add limit to deltaTime
        if (deltaTime > 0.01f) deltaTime = 0.01f;

        processInput(window);
        update(deltaTime);

        //Clear buffers
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Reset view matrix for background
        view = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "view"), 1, GL_FALSE, &view[0][0]);

        //Draw background
        glBindVertexArray(bgVAO);
        glBindTexture(GL_TEXTURE_2D, backgroundT.TextureID());
        glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "model"), 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        view = glm::scale(view, glm::vec3(0.009f, 0.009f, 1.0f));
        view = glm::translate(view, -glm::vec3(player->Position().x, player->Position().y * 0.2f + 140, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "view"), 1, GL_FALSE, &view[0][0]);

        //Render every tile in the map data
        for (int i = 0; i < map->Size(); i++)
        {
            accessedTile = map->data[i];
            if (map == nullptr) break;
            if (accessedTile == Map::AIR) continue;

            //Update model matrices to appropriate tile position
            glm::mat4 objectModel = glm::mat4(1.0f);
            objectModel = glm::translate(objectModel, glm::vec3(((i % map->width) * map->tileWidth) + (map->tileWidth / 2.0f), (map->height - 1 -(i / map->width)) * tile->Height() , 0.0f));

            glm::vec2 relativeAtlasPosition = blockPosition(i, accessedTile);

            tile->setAtlasPositionByIndex((int)(accessedTile * 5 + relativeAtlasPosition.x), (int)(accessedTile * 3 + relativeAtlasPosition.y));

            //Update object and texture coordinates, then render
            updateTextureCoordinates(tile->VBO, tile->ArrayData());
            glBindVertexArray(tile->VAO);
            glBindTexture(GL_TEXTURE_2D, tile->TextureID());
            glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "model"), 1, GL_FALSE, &objectModel[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        //Render all AnimatedObjects
        for (MovingObject* aObj : movingObjects)
        {
            if (aObj == nullptr || aObj->IsVisible() == false) continue;

            updateTextureCoordinates(aObj->VBO, aObj->ArrayData());
            glBindVertexArray(aObj->VAO);
            glBindTexture(GL_TEXTURE_2D, aObj->TextureID());
            glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "model"), 1, GL_FALSE, &(aObj->ModelMat4()[0][0]));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        //Render all Entities
        for (Entity* entity : entities)
        {
            if (entity == nullptr || entity->IsVisible() == false || entity->numLives() <= 0) continue;

            updateTextureCoordinates(entity->VBO, entity->ArrayData());
            glBindVertexArray(entity->VAO);
            glBindTexture(GL_TEXTURE_2D, entity->TextureID());
            glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "model"), 1, GL_FALSE, &(entity->ModelMat4()[0][0]));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Free data
    glDeleteBuffers(5, new unsigned int[] {bgVBO, tile->VBO, platform->VBO, player->VBO, taizer->VBO});
    glDeleteVertexArrays(4, new unsigned int[] {bgVAO, player->VAO, tile->VAO, taizer->VAO});

    delete[] new Object* [] {tile, platform, player, taizer};
    delete map;
}

void update(float deltaTime)
{
    AnimatedObject::updateTickRateClock(deltaTime);

    for (MovingObject* obj : movingObjects)
    {
        obj->update(deltaTime);
    }

    //Update AnimatedObjects
    checkMapCollisions(deltaTime);

    //Check collisions between AnimatedObjects
    checkMovingObjectsCollisions(deltaTime);
}

void checkMovingObjectsCollisions(float deltaTime)
{
    MovingObject* current, * other;
    for (int i = 0; i < movingObjects.size() - 1; i++)
    {
        current = movingObjects[i];

        for (int j = i + 1; j < movingObjects.size(); j++)
        {
            other = movingObjects[j];

            if (current->isCollidingWith(*other))
            {
                current->handleCollisionWith(other);
                other->handleCollisionWith(current);
            }
        }
    }
}

void checkMapCollisions(float deltaTime)
{
    for (MovingObject* aObj : movingObjects)
    {
        if (aObj->checkMapCollisionY(map) == true) aObj->correctMapCollisionY();
        if (aObj->checkMapCollisionX(map) == true) aObj->correctMapCollisionX();
    }
}

glm::vec2 blockPosition(int index, int tileValue)
{
    // Default to middle-level
    glm::vec2 currentPosition = glm::vec2(1.0f);

    bool aboveIsSame = (map->GetSide(index, Map::ABOVE) == tileValue),
         belowIsSame = (map->GetSide(index, Map::BELOW) == tileValue),
         leftIsSame = (map->GetSide(index, Map::LEFT) == tileValue),
         rightIsSame = (map->GetSide(index, Map::RIGHT) == tileValue);

    if (aboveIsSame) currentPosition.y++; // y becomes 1 or 2
    if (belowIsSame) currentPosition.y--; // y becomes 0 or 1

    if (leftIsSame) currentPosition.x++; // x becomes 1 or 2
    if (rightIsSame) currentPosition.x--; // x becomes 0 or 1

    if (!aboveIsSame && !belowIsSame && !leftIsSame && !rightIsSame)
    {
        currentPosition = glm::vec2(4, 1);
    }
    else if (!leftIsSame && !rightIsSame)
    {
        currentPosition.x = 3;
    }
    else if (!aboveIsSame && !belowIsSame)
    {
        if (leftIsSame)
        {
            currentPosition = glm::vec2(4, 0);
        }else if (rightIsSame) 
        {
            currentPosition = glm::vec2(4, 2);
        }
    }

    return currentPosition;
}

void updateTextureCoordinates(unsigned int & VBO, float * bufferData)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float), 2 * sizeof(float), &bufferData[3]);
    glBufferSubData(GL_ARRAY_BUFFER, 8 * sizeof(float), 2 * sizeof(float), &bufferData[8]);
    glBufferSubData(GL_ARRAY_BUFFER, 13 * sizeof(float), 2 * sizeof(float), &bufferData[13]);
    glBufferSubData(GL_ARRAY_BUFFER, 18 * sizeof(float), 2 * sizeof(float), &bufferData[18]);
    glBufferSubData(GL_ARRAY_BUFFER, 23 * sizeof(float), 2 * sizeof(float), &bufferData[23]);
    glBufferSubData(GL_ARRAY_BUFFER, 28 * sizeof(float), 2 * sizeof(float), &bufferData[28]);

    glBindVertexArray(0);
}

//Creates VAO's and VBO's with the given references and Objects
void createVAOandVBO(unsigned int& VAO, unsigned int& VBO, Object * object)
{
    //Create VBOs and VAOs
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    //Bind VBO and assign data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, object->ArraySize(), object->ArrayData(), GL_DYNAMIC_DRAW);
    //Set attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void windowResize(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) AnimatedObject::setTickRate(0.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) AnimatedObject::setTickRate(1.0f);

    if (glfwGetKey(window, GLFW_KEY_A) == glfwGetKey(window, GLFW_KEY_D))
    {
        player->slowDownX();
    }

    bool leftDown = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS),
            rightDown = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player->wallBoost();
        else if (leftDown) player->wallJump(SIDE::LEFT);
        else if (rightDown) player->wallJump(SIDE::RIGHT);
            
        player->jump();
    }
    else
    {
        if (leftDown)
        {
            player->ledgeGrab(SIDE::LEFT, map);
            player->accelerate(glm::vec2(-200.0f, 0.0f), deltaTime);
        }
        else if (rightDown)
        {
            player->ledgeGrab(SIDE::RIGHT, map);
            player->accelerate(glm::vec2(200.0f, 0.0f), deltaTime);
        }

        if(leftDown != rightDown) player->wallSlide();
    }   
}

void mouseClick(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        //Set true if mosue is held down, false otherwise.
        mouseLeftHeldDown = (action == GLFW_PRESS);
    }
}

void mouseMove(GLFWwindow* window, double xPos, double yPos)
{
    xPos = (xPos - windowWidth / 2) / windowWidth / 2;
    yPos -= windowHeight / 2;
    glm::vec2 cursorWorldPos = glm::vec2(view * glm::vec4((float)xPos, (float)yPos, 1.0f, 1.0f));


    /*if (mouseLeftHeldDown && xPos < (player->Position().x + (player->texture()->Width() / 2)) && xPos >(player->Position().x - (player->textureWidth() / 2))
        && yPos < (player->Position().y + (player->textureHeight() / 2.0)) && yPos >(player->Position().y - (player->textureHeight() / 2.0)))
    {
        player->setPosition(xPos, yPos);
        std::cout << "Dragging player";
    }*/
}

