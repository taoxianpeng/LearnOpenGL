#ifndef TEXTURE_H
#define TEXTURE_H

#include "string_view"

class Texture {
 public:
  explicit Texture();
  Texture(const Texture&) = delete;
  Texture(Texture&&) noexcept;

  Texture& operator=(const Texture&) = delete;
  Texture& operator=(Texture&&) = delete;
  ~Texture();

  void create(std::string_view image_path);
  void setTextureMode(int wrapS, int wrapT, int minFilter, int maxFilter);
  void useTextureUnit(int textureUnitID);

  const unsigned int getTextureID() const {
    return textureID;
  }

  int getWidth() { return texWidth; };
  int getHeight() { return texHeight; };
  int getChannal() { return texChannal; };

 private:
  void loadTextureFromFile(std::string_view path);
  void freeTexture() const;

 private:
  unsigned char* data;
  unsigned int textureID;
  int textureWrapS;
  int textureWrapT;
  int textureMinFilter;
  int textureMaxFilter;

  int texWidth;
  int texHeight;
  int texChannal;
};

#endif