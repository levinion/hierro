#include <hierro/error.hpp>

namespace hierro {

HierroResult<void> ok() {
  return HierroResult<void>::ok();
}

HierroResult<void> err(HierroError error) {
  return HierroResult<void>::err(error);
}

} // namespace hierro
