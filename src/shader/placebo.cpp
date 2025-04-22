#include <libplacebo/log.h>
#include <libplacebo/shaders.h>
#include <libplacebo/opengl.h>
#include <libplacebo/shaders/custom.h>
#include "hierro/utils/log.hpp"
#include "hierro/shader/placebo.hpp"

namespace hierro {

const char* parse_libplacebo_shader(const char* shader_source) {
  pl_log_params params = { .log_cb = pl_log_color, .log_level = PL_LOG_INFO };
  pl_log log = pl_log_create(PL_API_VER, &params);

  pl_shader_params sp = { .id = 1, .gpu = NULL, .glsl = { .version = 330 } };

  pl_shader sh = pl_shader_alloc(log, &sp);

  pl_custom_shader cs = {
    .header =
      "uniform sampler2D inputTex;\n"
      "in vec2 vTexCoord;\n",
    .body = shader_source,
    .input = PL_SHADER_SIG_NONE,
    .output = PL_SHADER_SIG_COLOR,
  };

  bool ok = pl_shader_custom(sh, &cs);

  if (!ok) {
    hierro::LOG("pl shader failed");
    exit(1);
  }

  const pl_shader_res* res = pl_shader_finalize(sh);

  if (!res) {
    hierro::LOG("Shader finalize failed");
    exit(1);
  }

  pl_log_destroy(&log);

  return res->glsl;
}
} // namespace hierro
