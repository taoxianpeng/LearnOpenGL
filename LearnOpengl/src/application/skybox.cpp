#include "skybox.h"
#include "direction.h"
#include "log.h"

void SkyBox::loadData() {
  float skyboxVertices[] = {
      // positions
      -100.0f, 100.0f,  -100.0f, -100.0f, -100.0f, -100.0f,
      100.0f,  -100.0f, -100.0f, 100.0f,  -100.0f, -100.0f,
      100.0f,  100.0f,  -100.0f, -100.0f, 100.0f,  -100.0f,

      -100.0f, -100.0f, 100.0f,  -100.0f, -100.0f, -100.0f,
      -100.0f, 100.0f,  -100.0f, -100.0f, 100.0f,  -100.0f,
      -100.0f, 100.0f,  100.0f,  -100.0f, -100.0f, 100.0f,

      100.0f,  -100.0f, -100.0f, 100.0f,  -100.0f, 100.0f,
      100.0f,  100.0f,  100.0f,  100.0f,  100.0f,  100.0f,
      100.0f,  100.0f,  -100.0f, 100.0f,  -100.0f, -100.0f,

      -100.0f, -100.0f, 100.0f,  -100.0f, 100.0f,  100.0f,
      100.0f,  100.0f,  100.0f,  100.0f,  100.0f,  100.0f,
      100.0f,  -100.0f, 100.0f,  -100.0f, -100.0f, 100.0f,

      -100.0f, 100.0f,  -100.0f, 100.0f,  100.0f,  -100.0f,
      100.0f,  100.0f,  100.0f,  100.0f,  100.0f,  100.0f,
      -100.0f, 100.0f,  100.0f,  -100.0f, 100.0f,  -100.0f,

      -100.0f, -100.0f, -100.0f, -100.0f, -100.0f, 100.0f,
      100.0f,  -100.0f, -100.0f, 100.0f,  -100.0f, -100.0f,
      -100.0f, -100.0f, 100.0f,  100.0f,  -100.0f, 100.0f};

  CheckCall(glGenVertexArrays(1, &m_VAO));
  CheckCall(glGenBuffers(1, &m_VBO));

  CheckCall(glBindVertexArray(m_VAO));
  CheckCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
  CheckCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),
                         skyboxVertices, GL_STATIC_DRAW));

  CheckCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                  (void *)(0)));
  CheckCall(glEnableVertexAttribArray(0));

  CheckCall(glBindVertexArray(0));

  const std::string resourcePath =
      Direction::getInstance()->getProjectRootPath().string();
  const std::vector<std::string> &faces = {
      resourcePath + "/resource/skybox/right.jpg",
      resourcePath + "/resource/skybox/left.jpg",
      resourcePath + "/resource/skybox/top.jpg",
      resourcePath + "/resource/skybox/bottom.jpg",
      resourcePath + "/resource/skybox/front.jpg",
      resourcePath + "/resource/skybox/back.jpg"};

  GLuint id = loadCubeMap(faces);
  setCubmapTextureID(id);

  if (getCubmapTextureID() != 0) {
    LOGD("skybox texture load ok!");
  } else {
    LOGE("faild to load skybox texture");
  }

  std::string vertPath = resourcePath + "/shader/skybox.vert";
  std::string fragPath = resourcePath + "/shader/skybox.frag";

  m_shader = Shader(vertPath, fragPath);
}

void SkyBox::draw() {
  CheckCall(glDepthMask(GL_FALSE));
  GLuint textureID = getCubmapTextureID();
  CheckCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));
  CheckCall(glBindVertexArray(m_VAO));
  m_shader.use();
  m_shader.setMat4("view", m_view);
  m_shader.setMat4("projection", m_projection);
  m_shader.setMat4("model", m_model);
  CheckCall(glDrawArrays(GL_TRIANGLES, 0, 36));
  CheckCall(glDepthMask(GL_TRUE));
}

unsigned int SkyBox::loadCubeMap(const std::vector<std::string> &faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (auto i = 0; i < faces.size(); ++i) {
    unsigned char *data =
        stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (nullptr != data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
      LOGE("face id: {}, texture data is empty!", i);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}
