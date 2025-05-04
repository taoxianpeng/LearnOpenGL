#pragma once

#include "application.h"

class MyApplication : public Application
{
public:
    MyApplication() = default;
    ~MyApplication() = default;

    void onProjectLoad() override;
    void onUpdate() override;
    void onKeyInputEvent(const KeyEvent& keyEvent, const KeyActionEvent& keyActionEvent) override;
};

Application* createApplication();