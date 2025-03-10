#include "hierro/error.h"

#include <ostream>
#include <iostream>

std::ostream& operator<<(std::ostream& out, const HierroError err) {
  switch (err) {
    case GLADERROR:
      out << "GLAD ERROR";
      return out;
    default:
      out << "UNDEFINED ERROR";
      return out;
  }
};

void HierroResult::unwrap() {
  if (this->value) {
    std::cout << "[hierro error]{" << this->value.value() << "}" << std::endl;
    std::exit(1);
  }
}
