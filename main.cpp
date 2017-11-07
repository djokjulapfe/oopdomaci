#include <iostream>
#include "BigDecimal.h"

void test() {

	BigDecimal a("32.15");
	BigDecimal b("2.345");
	BigDecimal c("2.345");
	BigDecimal d("-2.345");
	//BigDecimal a(*b);

	std::cout << a << " - " << b << " = " << a.sub(&b) << std::endl;
	std::cout << b << " - " << a << " = " << b.sub(&a) << std::endl;
	std::cout << a << " + " << b << " = " << a.add(&b) << std::endl;
	std::cout << b << " + " << a << " = " << b.add(&a) << std::endl;
	std::cout << b << " > " << a << " = " << b.greater(&a) << std::endl;
	std::cout << b << " < " << a << " = " << b.less(&a) << std::endl;
	std::cout << b << " = " << a << " = " << b.equals(&a) << std::endl;
	std::cout << b << " > " << c << " = " << b.greater(&c) << std::endl;
	std::cout << b << " < " << c << " = " << b.less(&c) << std::endl;
	std::cout << b << " = " << c << " = " << b.equals(&c) << std::endl;
	std::cout << "|" << a << "| = " << a.abs() << std::endl;
	std::cout << "|" << d << "| = " << d.abs() << std::endl;
	std::cout << a << " * " << b << " = " << a.mul(&b) << std::endl;
	//res = 75.39175
	//std::cout << a << " * " << d << " = " << a.mul(&d) << std::endl;
}

int main() {
	test();
	/*
	BigDecimal* x = new BigDecimal("12.34");
	BigDecimal* y = new BigDecimal("2.345");
	//BigDecimal a(*y);
	//std::cout << a << std::endl;
	std::cout << *x << " - " << *y << " = " << x->sub(y) << std::endl;
	std::cout << *y << " - " << *x << " = " << y->sub(x) << std::endl;
	std::cout << *x << " + " << *y << " = " << x->add(y) << std::endl;
	std::cout << *y << " + " << *x << " = " << y->add(x) << std::endl;*/
	return 0;
}