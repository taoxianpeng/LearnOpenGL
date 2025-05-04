#pragma once

#include "keyboard.h"

class Application
{
public:
    Application() = default;
    virtual ~Application() = default;
    
    virtual void initializeGL();
    virtual void uninitializeGL();

    virtual void onProjectLoad();
    virtual void onUpdate();
    virtual void onKeyInputEvent(const KeyEvent& keyEvent, const KeyActionEvent& keyActionEvent);
};