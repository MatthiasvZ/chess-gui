#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


class Window
{
    public:
        Window();
        bool shouldRun();
        void updateWindow();
        void countfps();
        void updateTitle();
        void getSelectedTile(int& tileX, int& tileY);
        void removeSelection();
        inline bool getPlayer() { return playerToMove; }
        inline void setplayer(const bool& player)
        {
            playerToMove = player;
        }
        void setGameOver(bool winner);
        ~Window();

    protected:

    private:
        static void error_callback(int error, const char* description);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        unsigned int prev_fps, fps, seconds;
        float avg_fps;
        long tn, ta;
        bool playerToMove;
};

#endif // WINDOW_H
