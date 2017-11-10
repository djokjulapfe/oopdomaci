#include <iostream>
#include "BigDecimal.h"

#define PROF_TEST

#ifdef PROF_TEST

using namespace std;

void test(){
	BigDecimal a("2315.434\0");// a = 2315.434
	BigDecimal b("-135\0"); // b = -135
	BigDecimal c = a.shr(4); // c = 23154340
	BigDecimal d = b.shl(1); // d = -13.5
	int n;
	BigDecimal e = a.rmd(&n); // e = 2315434, n = 3
	BigDecimal f = d.add(&e); // f = 2315420.5
	BigDecimal g = a.sub(&c); // g = -23152024.566
	if(e.greater(&g))
		cout << "TACNO" << endl;
	else
		cout << "NETACNO" << endl;

	if(e.less(&g))
		cout << "NETACNO" << endl;
	else
		cout << "TACNO" << endl;

	if(!g.equals(&g)) cout << "GRESKA";
	BigDecimal h = b.abs();

	cout << "a = " << a << endl << "b = " << b << " c = " << c << endl;
	cout << "d = " << d << " e = " << e << " f = " << f << " g = " << g << endl;
	cout << "h = " << h << endl;
	cout << "n = " << n << endl;
}

#else

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
	//std::cout << a << " * " << b << " = " << a.mul(&b) << std::endl;
	//res = 75.39175
	//std::cout << a << " * " << d << " = " << a.mul(&d) << std::endl;
}

#endif

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