#include "gl/Window.h"
#include "gl/Input.h"
#include "gl/Renderer.h"

#include <GL/glew.h>
#include <ctime>
#include <stdio.h>

Input input;
bool gameOver {false};

void Window::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error (%d): %s\n", error, description);
}

void Window::key_callback(GLFWwindow* window, int key, __attribute__((unused))int scancode, int action, __attribute__((unused))int mods)
{
    static bool fullscreen {false};
    static int windowPosX, windowPosY;
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:  glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_LEFT_CONTROL: input.ctrlHeld = true;    break;
        case GLFW_KEY_SPACE:      input.spaceHeld = true;     break;
        case GLFW_KEY_LEFT_SHIFT: input.shiftHeld = true;     break;
        case GLFW_KEY_W:            input.wHeld   = true;     break;
        case GLFW_KEY_A:            input.aHeld   = true;     break;
        case GLFW_KEY_S:            input.sHeld   = true;     break;
        case GLFW_KEY_D:            input.dHeld   = true;     break;
        case GLFW_KEY_DOWN:        input.downHeld = true;     break;
        case GLFW_KEY_LEFT:        input.leftHeld = true;     break;
        case GLFW_KEY_RIGHT:      input.rightHeld = true;     break;
        case GLFW_KEY_UP:           input.upHeld  = true;     break;
        case GLFW_KEY_KP_2:         input.kp2Held = true;     break;
        case GLFW_KEY_KP_4:         input.kp4Held = true;     break;
        case GLFW_KEY_KP_5:         input.kp5Held = true;     break;
        case GLFW_KEY_KP_6:         input.kp6Held = true;     break;
        case GLFW_KEY_KP_8:         input.kp8Held = true;     break;
        case GLFW_KEY_F11:
            if (!fullscreen)
            {
                glfwGetWindowPos(window, &windowPosX, &windowPosY);
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_DONT_CARE);
                fullscreen = true;
            }
            else
            {
                glfwSetWindowMonitor(window, nullptr, windowPosX, windowPosY, 800, 800, GLFW_DONT_CARE);
                fullscreen = false;
            }
        }
    }
    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_LEFT_CONTROL: input.ctrlHeld = false;   break;
        case GLFW_KEY_SPACE:      input.spaceHeld = false;    break;
        case GLFW_KEY_LEFT_SHIFT: input.shiftHeld = false;    break;
        case GLFW_KEY_W:            input.wHeld   = false;    break;
        case GLFW_KEY_A:            input.aHeld   = false;    break;
        case GLFW_KEY_S:            input.sHeld   = false;    break;
        case GLFW_KEY_D:            input.dHeld   = false;    break;
        case GLFW_KEY_DOWN:        input.downHeld = false;    break;
        case GLFW_KEY_LEFT:        input.leftHeld = false;    break;
        case GLFW_KEY_RIGHT:      input.rightHeld = false;    break;
        case GLFW_KEY_UP:           input.upHeld  = false;    break;
        case GLFW_KEY_KP_2:         input.kp2Held = false;    break;
        case GLFW_KEY_KP_4:         input.kp4Held = false;    break;
        case GLFW_KEY_KP_5:         input.kp5Held = false;    break;
        case GLFW_KEY_KP_6:         input.kp6Held = false;    break;
        case GLFW_KEY_KP_8:         input.kp8Held = false;    break;
        }
    }
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        input.tileX = ceil((8*xpos) / 800);
        input.tileY = ceil(8 - (8*ypos) / 800);
    }
}

void Window::framebuffer_size_callback(__attribute__((unused))GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
GLFWwindow* window;

Window::Window()
    : prev_fps(0), fps(0), seconds(0), avg_fps(0), tn(0), ta(0), playerToMove(1)
{
    time(&ta);

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(-1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(800, 800, "Schach (Weiß am Zuge), FPS = 0", nullptr, nullptr);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glewExperimental = true;
    if (glewInit())
        fprintf(stderr, "Error during glewInit(), %d\n", glewInit());
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "Status: Using OpenGL %s\n", glGetString(GL_VERSION));

    glFrontFace(GL_CW);
}

bool Window::shouldRun()
{
    return !glfwWindowShouldClose(window);
}

void Window::updateWindow()
{
    if (!gameOver)
        glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::countfps()
{
    if (time(&tn) != ta)
    {
        ta++;
        if (playerToMove)
            glfwSetWindowTitle(window, ("Schach (Weiß am Zuge), FPS =" + std::to_string(fps)).c_str());
        else
            glfwSetWindowTitle(window, ("Schach (Schwarz am Zuge), FPS =" + std::to_string(fps)).c_str());
        prev_fps = fps;
        avg_fps += fps;
        seconds++;
        fps = 0;
    }
    fps++;
}

void Window::getSelectedTile(int& tileX, int& tileY)
{

    tileX = input.tileX;
    tileY = input.tileY;
}

void Window::removeSelection()
{
    input.tileX = 0;
    input.tileY = 0;
}

void Window::updateTitle()
{
    if (playerToMove)
        glfwSetWindowTitle(window, ("Schach (Weiß am Zuge), FPS =" + std::to_string(prev_fps)).c_str());
    else
        glfwSetWindowTitle(window, ("Schach (Schwarz am Zuge), FPS =" + std::to_string(prev_fps)).c_str());
}

void Window::setGameOver(bool winner)
{
    gameOver = true;
    if (winner)
        glfwSetWindowTitle(window, "Schachmatt, Schwarz hat gewonnen!");
    else
        glfwSetWindowTitle(window, "Schachmatt, Weiß hat gewonnen!");
}
Window::~Window()
{
    printf("avg. Fps = %f\n", avg_fps/seconds);
    glfwDestroyWindow(window);
    glfwTerminate();
}
