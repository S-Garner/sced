#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>

#include "../Renderer/Shapes/IShape2D.hpp"
#include "../Renderer/Shapes/RectangleShape.hpp"
#include "../Renderer/Shapes/CircleShape.hpp"
#include "../Renderer/Shapes/EllipseShape.hpp"
#include "../Renderer/Shapes/RegularPolygonShape.hpp"

#include "../Renderer/Renderer2D.hpp"
#include "../Renderer/Shader/Shader.hpp"
#include "../Renderer/Transform.hpp"
#include "../Renderer/Vertex2D.hpp"

#include "../objects/SCObject.hpp"
#include "../core/Window.h"
#include "../input/Input.h"
#include "../color/SColor.hpp"


int main() {
    // --- Initialize GLFW + OpenGL ---
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "SCObject Shape Test", nullptr, nullptr);
    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    Renderer2D renderer;
    Shader shader = Shader::fromFiles("Shader/config/flat.vert", "Shader/config/flat.frag");

    // ************ SUN SHAPE
    SCObject sun(&renderer);

    CircleShape sunShape({0.0f, 0.0f}, 0.2f, 64, {1.0f, 0.9f, 0.0f});

    sun.addShape(sunShape);

    // 2. Create one base rectangle (ray)
    //RectangleShape ray({-0.02f, 0.25f}, {0.02f, 0.45f}, {1.0f, 0.9f, 0.0f});
    RectangleShape ray({-0.02f, 0.25f}, {0.02f, 0.45f}, SColor::normalizeColor(255, 200, 0));

    // 3. Add six rays around the circle
    const int rayCount = 6;
    for (int i = 0; i < rayCount; ++i) {
       float angle = glm::radians(360.0f / rayCount * i);

       // Get base vertices from ray
       auto verts = ray.generateVertices();

        // Rotate each vertex around the circle center
       glm::mat4 rotMat = Transform::rotateZ_about(Transform::setIdentity(), angle, {0.0f, 0.0f});
       for (auto& v : verts) {
           glm::vec4 p = rotMat * glm::vec4(v.pos, 0.0f, 1.0f);
           v.pos = glm::vec2(p);
       }

        // Add to the SCObject
       sun.addShape(verts);
    }

    sun.setPosition({0.0f, 0.0f});
    sun.setScale({1.0f, 1.0f});

    //***** SUN SHAPE *********

    // **** Sky shape ****

    SCObject sky(&renderer);

    RectangleShape fullScreenRect(
    { -1.333f, -1.0f },
    {  1.333f,  1.0f },
    { 0.f, 0.8f, 0.9f }
    );

    sky.addShape(fullScreenRect);

    // ************ CLOUD SHAPE
    
    SCObject cloud(&renderer);

    RectangleShape cloudBase({0.0f, -0.1f}, {0.5f, 0.1f}, {0.95f, 0.95f, 0.95f});
    cloud.addShape(cloudBase);

    CircleShape cloudPart({0.0f, 0.0f}, 0.15f, 64, {0.95f, 0.95f, 0.95f});

    cloud.addShape(cloudPart);

    auto cloudVerts1 = cloudPart.generateVertices();
    auto cloudVerts2 = cloudPart.generateVertices();
    auto cloudVerts3 = cloudPart.generateVertices();

    glm::mat4 cloudTransform1 = Transform::translate(Transform::setIdentity(), {0.2f, 0.2f});
    cloudTransform1 = Transform::scale(cloudTransform1, {1.5f, 1.5f});

    glm::mat4 cloudTransform2 = Transform::translate(Transform::setIdentity(), {0.4f, 0.05f});
    cloudTransform2 = Transform::scale(cloudTransform2, {1.1f, 1.1f});

    glm::mat4 cloudTransform3 = Transform::translate(Transform::setIdentity(), {0.55f, -0.03f});
    cloudTransform3 = Transform::scale(cloudTransform3, {0.8f, 0.8f});

    auto puff1 = cloud.addShape(cloudVerts1);
    auto puff2 = cloud.addShape(cloudVerts2);
    auto puff3 = cloud.addShape(cloudVerts3);

    //auto index1 = cloud.findShapeIndex(cloud.addShape(const *cloudVerts1));
    
    //cloud.setShapeColor(puff1, {1.0f, 0.2f, 0.2f});
    cloud.setShapeModel(puff1, cloudTransform1);
    cloud.setShapeModel(puff2, cloudTransform2);
    cloud.setShapeModel(puff3, cloudTransform3);

    int width, height;
    float rotation = 0.0f;

    float firstVal = -2.0f;

    cloud.setPosition({firstVal, 0.2f});

    SCObject cloud2 = cloud.clone();
    cloud2.setPosition({firstVal - 1.0f, 0.2f});
    cloud2.setScale({.4f, .4f});

    // --- Main Loop ---
    while (!glfwWindowShouldClose(window)) {
        firstVal += 0.002f;

        if (firstVal > 2.0f)
            firstVal = -2.0f;
        
        glfwPollEvents();
        glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (float)width / (float)height;

        rotation += 0.01f; // radians per frame
        if (rotation > glm::two_pi<float>())
        rotation -= glm::two_pi<float>();

        sun.setRotation(rotation);

        glm::mat4 vp = glm::ortho(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);
        //entity.draw(shader, vp);

        cloud.setPosition({(firstVal), 0.2f});
        //cloud2.setRotation(-rotation * 0.7f);
        cloud2.setPosition({firstVal + 1.0f, 0.2f - .5f});

        float t = glfwGetTime();

        float r = 0.5f + 0.5f * sin(t * 0.5f);
        float g = 0.5f + 0.5f * sin(t * 0.7f + 1.5f);
        float b = 0.5f + 0.5f * sin(t * 1.3f + 3.1f);

        glm::vec3 newColor(r, g, b);

        for (ShapeHandle h : cloud.getShapeHandles())
            cloud.setShapeColor(h, newColor);
            
        for (ShapeHandle h : cloud2.getShapeHandles())
            cloud2.setShapeColor(h, newColor);

        sky.draw(shader, vp);

        cloud.draw(shader, vp);
        cloud2.draw(shader, vp);

        sun.draw(shader, vp);

        if (firstVal < 1.8f && firstVal > 1.6f)
            sun.setVisible(false);
        else
            sun.setVisible(true);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
