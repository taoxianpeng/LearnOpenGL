#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <string_view>
#include <vector>

#include "Shader.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

namespace MMesh {

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh {
 public:
  explicit Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
                std::vector<Texture> textures);
  void draw(Shader &shader);

  // 网格数据
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<unsigned int> indices;

 private:
  /*  渲染数据  */
  unsigned int VAO, VBO, EBO;
  /*  函数  */
  void setupMesh();
};

class Model {
 public:
  explicit Model(std::string_view path) { loadModel(path); };
  void draw(Shader &shader);

 private:
  std::vector<Mesh> meshes;
  std::string directory;
  std::vector<Texture> texture_loaded;

  void loadModel(std::string_view path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);

  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);
};

}  // namespace MMesh

#endif