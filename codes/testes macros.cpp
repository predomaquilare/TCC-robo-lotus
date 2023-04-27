#include <iostream>

#define teste

#ifdef teste
  #define p 45
#endif


int main() {
  std::cout << p;
}
