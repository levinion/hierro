#include <hierro/error.hpp>

HierroResult<void> ok() {
  return HierroResult<void>::ok();
}

HierroResult<void> err(HierroError error) {
  return HierroResult<void>::err(error);
}
