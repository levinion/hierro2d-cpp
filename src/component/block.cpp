#include <glad/glad.h>
#include "hierro/component/block.h"
#include "hierro/shader/block/vertex.h"
#include "hierro/shader/block/fragment.h"
#include "hierro/shader.h"

Block::Block() {
  // init vao
  glGenVertexArrays(1, &this->vao);
  // init vbo
  glGenBuffers(1, &this->vbo);
  // init ebo
  glGenBuffers(1, &this->ebo);

  this->init_shader();

  // init vertices and indices
  this->update_vertices();
  this->update_indices();

  // update layout
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    1,
    4,
    GL_FLOAT,
    GL_FALSE,
    7 * sizeof(float),
    (void*)(3 * sizeof(float))
  );
  glEnableVertexAttribArray(1);
}

void Block::draw() {
  glBindVertexArray(this->vao);
  this->shader.use();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Block::update_vertices() {
  float x = (this->x - 0.5) * 2;
  float y = (this->y - 0.5) * 2;
  float height = this->height * 2;
  float width = this->width * 2;
  this->vertices = { // left down
                     x,
                     y - height,
                     0.0f,
                     this->color.r,
                     this->color.g,
                     this->color.b,
                     this->color.a,
                     // right down
                     x + width,
                     y - height,
                     0.0f,
                     this->color.r,
                     this->color.g,
                     this->color.b,
                     this->color.a,
                     // left up
                     x,
                     y,
                     0.0f,
                     this->color.r,
                     this->color.g,
                     this->color.b,
                     this->color.a,
                     // right up
                     x + width,
                     y,
                     0.0f,
                     this->color.r,
                     this->color.g,
                     this->color.b,
                     this->color.a
  };
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(
    GL_ARRAY_BUFFER,
    this->vertices.size() * sizeof(float),
    this->vertices.data(),
    GL_STATIC_DRAW
  );
}

void Block::update_indices() {
  this->indices = {
    0, 1, 3, //
    0, 2, 3,
  };
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    this->indices.size() * sizeof(unsigned int),
    this->indices.data(),
    GL_STATIC_DRAW
  );
}

void Block::update(std::function<void(Block*)>&& f) {
  f(this);
  this->update_vertices();
}

void Block::set_position(float x, float y) {
  this->x = x;
  this->y = y;
}

void Block::center() {
  this->set_position(0.5 - width / 2, 0.5 + height / 2);
}

void Block::init_shader() {
  auto vertex_shader_code = (const char*)_block_vertex_shader_code;
  auto fragment_shader_code = (const char*)_block_fragment_shader_code;

  this->shader = Shader(vertex_shader_code, fragment_shader_code);
}
