#pragma once

#include <expected>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "freetype2/freetype/freetype.h"
#include "hierro/utils/error.hpp"
#include "hierro/utils/shader.hpp"
#include "hierro/utils/data.hpp"

namespace hierro {

class Character {
public:
  unsigned int texture_id;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

enum class VerticalAlign { Left, Right, Center };
enum class HorizontalAlign { Top, Bottom, Center };

class TextGenerater {
public:
  static TextGenerater* get_instance();
  std::expected<void, std::string> init(std::string font, Size size);
  std::expected<void, std::string> draw_text(
    std::wstring text,
    Position position,
    Size size,
    bool wrap,
    bool overflow,
    float spacing,
    float line_spacing,
    float scale,
    Color color,
    VerticalAlign vertical_align,
    HorizontalAlign horizontal_align
  );
  void destroy();
  void viewport(float x, float y);
  float line_height();

private:
  static TextGenerater* instance;
  std::unordered_map<char32_t, Character> character_table;
  Shader shader;
  HierroResult<void> init_freetype(std::string font);
  HierroResult<void> init_shader();
  HierroResult<void> init_buffer();
  HierroResult<void> add_character(char32_t c);
  unsigned int vao;
  unsigned int vbo;
  FT_Library ft;
  FT_Face face;
  // @warn: height is must, and width can be 0.0
  Size font_size = { 0.0, 48.0 };
};

} // namespace hierro
