#pragma once

#include <glad/glad.h>
#include <GL/gl.h>
#include <string>
#include <stb_image.h>

class Texture {
public:
  Texture(
    unsigned char* pixels,
    int width,
    int height,
    GLenum format = GL_RGBA
  ) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      width,
      height,
      0,
      format,
      GL_UNSIGNED_BYTE,
      pixels
    );
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  Texture(std::string path, GLenum format = GL_RGBA) {
    int width, height, channels;
    auto pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      width,
      height,
      0,
      format,
      GL_UNSIGNED_BYTE,
      pixels
    );
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  Texture() {}

  inline void bind() {
    glBindTexture(GL_TEXTURE_2D, this->texture);
  }

  inline void free() {
    glDeleteTextures(1, &this->texture);
    this->texture = 0;
  }

  inline bool is_valid() {
    return this->texture != 0;
  }

  inline GLuint id() {
    return texture;
  }

private:
  GLuint texture = 0;
};
