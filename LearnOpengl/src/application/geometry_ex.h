#include "node.h"


class GeometryEx : public Node  {
public:
  GeometryEx() = default;
  ~GeometryEx() = default;
  void loadData() override;
  void draw() override;

private:
  std::vector<float> points;  
  Shader m_shader;

  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;
};
