#include <iostream>
#include "BigDecimal.h"

int main() {
	//BigDecimal* x = new BigDecimal("21.38");
	BigDecimal* y = new BigDecimal("-45.95");
	//BigDecimal a(*y);
	//std::cout << a << std::endl;
	std::cout << y->neg() << std::endl;
	//std::cout << x->sub(y) << std::endl;
	//std::cout << y->sub(x) << std::endl;
	return 0;
}