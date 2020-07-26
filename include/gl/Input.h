#ifndef INPUT_H
#define INPUT_H


struct Input
{
        bool ctrlHeld {false};
        bool spaceHeld {false};
        bool shiftHeld {false};
        bool wHeld {false};
        bool aHeld {false};
        bool sHeld {false};
        bool dHeld {false};
        bool downHeld {false};
        bool leftHeld {false};
        bool rightHeld {false};
        bool upHeld {false};
        bool kp2Held {false};
        bool kp4Held {false};
        bool kp5Held {false};
        bool kp6Held {false};
        bool kp8Held {false};
        bool fullscreen {false};

        int tileX {0};
        int tileY {0};
};

#endif // INPUT_H
