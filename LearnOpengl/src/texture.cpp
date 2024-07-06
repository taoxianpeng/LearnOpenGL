#pragma once

#include "texture.h"

#include <iostream>
#include <string>

#include <glad/glad.h>

#include "stb_image.h"

Texture::Texture()
    : data(nullptr),
    texture(0),
    textureWrapS(GL_REPEAT),
    textureWrapT(GL_REPEAT),
    textureMinFilter(GL_LINE),
    textureMaxFilter(GL_LINE),
    texWidth(0),
    texHeight(0),
    texChannal(0){}

Texture::Texture(Texture&& sourceTex) noexcept{
  data = sourceTex.data;
  texture = sourceTex.texture;
  textureWrapS = sourceTex.textureWrapS;
  textureWrapT = sourceTex.textureWrapT;
  textureMinFilter = sourceTex.textureMinFilter;
  textureMaxFilter = sourceTex.textureMaxFilter;
  texWidth = sourceTex.texWidth;
  texHeight = sourceTex.texHeight;
  texChannal = sourceTex.texChannal;
}

Texture::~Texture() {
    freeTexture();
}

void Texture::create(std::string_view image_path) {
  genTexture();
  stbi_set_flip_vertically_on_load(true);
  texture = loadTextureFromFile(image_path);
}

void Texture::setTextureMode(int wrapS, int wrapT, int minFilter,
                             int maxFilter) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
}

// 在渲染循环中使用
void Texture::useTexture(int textureUnitID) {
  glActiveTexture(textureUnitID);
  glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned int Texture::loadTextureFromFile(std::string_view path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  unsigned char* data =
      stbi_load(path.data(), &texWidth, &texHeight, &texChannal, 0);
  if (data) {
    GLenum format;
    if (texChannal == 1)
      format = GL_RED;
    else if (texChannal == 3)
      format = GL_RGB;
    else if (texChannal == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    setTextureMode(textureWrapS, textureWrapT, textureMinFilter,
                   textureMaxFilter);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
  }
  stbi_image_free(data);
  return textureID;
}

void Texture::genTexture() {
  glGenTextures(1, (GLuint*)&texture);
  glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::freeTexture() const { glDeleteTextures(1, (GLuint*)&texture); };
