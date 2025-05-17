#pragma once
#include <GLFW/glfw3.h>

class Editor {
public:
  Editor();
  ~Editor();

  void init(GLFWwindow *window);
  void startFrameRender();
  void updateEditorGUI();
  void endFrameRender();
  void destory();
};