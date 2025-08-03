#pragma once
#include "node.h"
#include <rttr/registration.h>
#include <rttr/type.h>


class SkyBox : public Node{
public:
  SkyBox() = default;
  ~SkyBox() = default;

  //void setCubmapUrl(const std::vector<std::string>& cubmapFileUrl);
  void loadData() override;
  void draw() override;

  RTTR_ENABLE();

private:
  unsigned int loadCubeMap(const std::vector<std::string>& faces);

private:
  std::vector<std::string> imagePath;
  GLuint m_VAO = 0;
  GLuint m_VBO = 0;
  Shader m_shader;
};