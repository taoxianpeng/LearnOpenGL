#pragma once

#include "mesh.h"
#include "common.h"
#include "log.h"

using namespace MMesh;

static unsigned int TextureFromFile(std::string_view path) {
	unsigned int textureID;
	CheckCall(glGenTextures(1, &textureID));
	int texWidth, texHeight, texChannal;

	unsigned char* data =
		stbi_load(path.data(), &texWidth, &texHeight, &texChannal, 0);
	if (data) {
		GLenum format = GL_RGB;
		if (texChannal == 1)
			format = GL_RED;
		else if (texChannal == 3)
			format = GL_RGB;
		else if (texChannal == 4)
			format = GL_RGBA;

		CheckCall(glBindTexture(GL_TEXTURE_2D, textureID));
		CheckCall(glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format,
			GL_UNSIGNED_BYTE, data));
		CheckCall(glGenerateMipmap(GL_TEXTURE_2D));

		CheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		CheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		CheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR));
		CheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}
	else {
		LOGE("Texture failed to load at path: {}", path);
	}
	stbi_image_free(data);
	return textureID;
}

void Mesh::draw(Shader& shader) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (auto i = 0; i < textures.size(); i++) {
		CheckCall(glActiveTexture(GL_TEXTURE0 + i));
		std::string number;
		std::string name = textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.setInt(("material." + name + number).c_str(), i);
		CheckCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	}
	CheckCall(glActiveTexture(GL_TEXTURE0));

	//spdlog::info("******* mesh color:{},{},{}", mats.Kd.r, mats.Kd.g, mats.Kd.b);
	CheckCall(glBindVertexArray(VAO));
	CheckCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBlockIndex, 0, sizeof(Material)));
	CheckCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
	CheckCall(glBindVertexArray(0));
}

void Mesh::setupMesh() {
	CheckCall(glGenVertexArrays(1, &VAO));
	CheckCall(glGenBuffers(1, &VBO));
	CheckCall(glGenBuffers(1, &EBO));
	CheckCall(glGenBuffers(1, &uniformBlockIndex));

	CheckCall(glBindVertexArray(VAO));
	CheckCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	CheckCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
		GL_STATIC_DRAW));

	CheckCall(glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIndex));
	CheckCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(mats), (void*)(&mats), GL_STATIC_DRAW));


	CheckCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	CheckCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW));

	// 顶点位置
	CheckCall(glEnableVertexAttribArray(0));
	CheckCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	// 法线位置
	CheckCall(glEnableVertexAttribArray(1));
	CheckCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, normal))));
	// 顶点纹理坐标
	CheckCall(glEnableVertexAttribArray(2));
	CheckCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, texCoords))));

	CheckCall(glBindVertexArray(0));
}

void Model::draw(Shader& shader) {
	for (auto mesh : meshes) {
		mesh.draw(shader);
	}
}

void Model::loadModel(std::string_view path) {
	Assimp::Importer import;
	const aiScene* scene =
import.ReadFile(path.data(), aiProcess_FlipUVs | aiProcess_Triangulate);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
			!scene->mRootNode) {
			spdlog::error("ERORR::ASSIMP::{}", import.GetErrorString());
			return;
		}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
	spdlog::info("node NumMeshes:{}, NumChildren:{}", scene->mRootNode->mNumMeshes, scene->mRootNode->mNumChildren);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (auto i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		auto item = processMesh(mesh, scene);
		meshes.emplace_back(item);
		spdlog::info("Kd color:{}, {}, {}", item.mats.Kd.r, item.mats.Kd.g, item.mats.Kd.b);
	}
	for (auto i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
	spdlog::info("meshes count:{}", meshes.size());
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (auto i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;

		// 处理顶点位置信息
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.position = vector;
		// LOGD("vertex position {} {} {}", vector.x, vector.y, vector.z);

		if (mesh->HasNormals()) {
			// 处理法线信息
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;

			vertex.normal = vector;
		}

		// 处理纹理信息
		if (mesh->mTextureCoords[0]) {
			glm::vec2 uv{};
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;

			vertex.texCoords = uv;

			// // tangent
			// vector.x = mesh->mTangents[i].x;
			// vector.y = mesh->mTangents[i].y;
			// vector.z = mesh->mTangents[i].z;
			// vertex.Tangent = vector;
			// // bitangent
			// vector.x = mesh->mBitangents[i].x;
			// vector.y = mesh->mBitangents[i].y;
			// vector.z = mesh->mBitangents[i].z;
			// vertex.Bitangent = vector;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.emplace_back(vertex);
	}
	LOGI("Mesh vertices size: {}", vertices.size());
	//spdlog::info("vertices Texture:{}")

	// 处理索引
	for (auto i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (auto j = 0; j < face.mNumIndices; j++) {
			indices.emplace_back(face.mIndices[j]);
		}
	}

	Material mat;

	// 加载材质
	if (mesh->mMaterialIndex >= 0) {

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//加载纯色材质
		aiColor3D color;
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mat.Ka = glm::vec4(color.r, color.g, color.b, 1.0f);
		spdlog::info("ambient :{},{},{} | {}", mat.Ka.r, mat.Ka.g, mat.Ka.b, mat.Ka.a);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mat.Kd = glm::vec4(color.r, color.g, color.b, 1.0f);
		spdlog::info("diffuse :{},{},{} | {}", mat.Kd.r, mat.Kd.g, mat.Kd.b, mat.Kd.a);
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mat.Ks = glm::vec4(color.r, color.g, color.b, 1.0f);
		spdlog::info("specular :{},{},{} | {}", mat.Ks.r, mat.Ks.g, mat.Ks.b, mat.Ks.a);


		// 将漫反射纹理材质 和 镜面纹理材质都加载到一个数组中
		std::vector<Texture> diffuse_texture = loadMaterialTextures(
			material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_texture.begin(),
			diffuse_texture.end());
		std::vector<Texture> specular_texture = loadMaterialTextures(
			material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_texture.begin(),
			specular_texture.end());
	}
	spdlog::info("mesh texture count:{}", textures.size());
	auto mMesh = Mesh(vertices, indices, textures, mat);
	spdlog::info("Kd color:{}, {}, {}", mMesh.mats.Kd.r, mMesh.mats.Kd.g, mMesh.mats.Kd.b);

	return mMesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat,
	aiTextureType type,
	std::string typeName) {
	std::vector<Texture> textures;
	for (auto i = 0; i < mat->GetTextureCount(type); i++) {
		aiString aiStr;
		mat->GetTexture(type, i, &aiStr);

		std::string path = std::string(this->directory + "/" + aiStr.C_Str());
		LOGI("Load Resource path: {}", path);

		auto it = std::find_if(
			texture_loaded.cbegin(), texture_loaded.cend(),
			[&](const Texture& tex) {
				return (std::strcmp(tex.path.data(), aiStr.C_Str()) == 0);
			});

		if (it == texture_loaded.cend()) {
			Texture texture;
			// 将Mesh中的纹理提取到 数组中保存
			texture.id = TextureFromFile(path);
			texture.type = typeName;
			textures.emplace_back(texture);
			texture_loaded.emplace_back(texture);
			LOGI("id:{}, type:{}", texture.id, texture.type);
		}
	}
	return textures;
}
