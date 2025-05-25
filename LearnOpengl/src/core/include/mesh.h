#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

  struct Material {
    glm::vec4 Ka; //环境光
    glm::vec4 Kd; //漫反射
    glm::vec4 Ks; //镜面反射
  };

  class Mesh {
  public:
    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
      std::vector<Texture> textures, Material mats)
      : vertices(std::move(vertices)), indices(std::move(indices)),
      textures(std::move(textures)), mats(std::move(mats)) {
      setupMesh();
    }

    Mesh(const Mesh& mesh)
      : vertices(mesh.vertices), indices(mesh.indices),
      textures(mesh.textures), mats(mesh.mats),
      VAO(mesh.VAO), VBO(mesh.VBO), EBO(mesh.EBO),
      uniformBlockIndex(mesh.uniformBlockIndex) {
    }

    Mesh(Mesh&& mesh) noexcept {
      swap(*this, mesh);
    }

    Mesh& operator=(Mesh mesh) {
      swap(*this, mesh);
      return *this;
    }

    //Mesh& operator=(Mesh&& mesh) noexcept {
    //  swap(*this, mesh);
    //  return *this;
    //}

    friend void swap(Mesh& lhs, Mesh& rhs) noexcept {
      using std::swap; // 启用 ADL（参数依赖查找）
      swap(lhs.vertices, rhs.vertices);
      swap(lhs.indices, rhs.indices);
      swap(lhs.textures, rhs.textures);
      swap(lhs.mats, rhs.mats);
      swap(lhs.VAO, rhs.VAO);
      swap(lhs.VBO, rhs.VBO);
      swap(lhs.EBO, rhs.EBO);
      swap(lhs.uniformBlockIndex, rhs.uniformBlockIndex);
    }


    void draw(Shader& shader);



  public:
    // 网格数据
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;
    Material mats;

  private:
    /*  渲染数据  */
    unsigned int VAO, VBO, EBO;
    unsigned int uniformBlockIndex;
    /*  函数  */
    void setupMesh();
  };

  class Model {
  public:
    Model() = default;
    Model(std::string_view path) { loadModel(path); };
    Model(const Model& model)
      : meshes(model.meshes)
      , directory(model.directory)
      , texture_loaded(model.texture_loaded) {}

    Model(Model&& model) noexcept {
      swap(*this, model);
    }

    Model& operator=(Model model) {
      swap(*this, model);
      return *this;
    }

    //Model& operator=(Model&& model) noexcept {
    //  swap(*this, model);
    //  return *this;
    //}

    friend void swap(Model& lhs, Model& rhs) noexcept {
      using std::swap; // 启用 ADL（参数依赖查找）
      swap(lhs.meshes, rhs.meshes);
      swap(lhs.directory, rhs.directory);
      swap(lhs.texture_loaded, rhs.texture_loaded);
    }

    void draw(Shader& shader);

  private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> texture_loaded;

    void loadModel(std::string_view path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
  };

}  // namespace MMesh

#endif