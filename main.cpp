#include <iostream>
#include "any.hpp"

int main() {
  bigblind::any var = 1;
  double d = 1.5;
  var = d;
  var = 'c';
  // var.reset();
  std::cout << var.type().name() << "\n";
}