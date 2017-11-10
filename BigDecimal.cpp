#include "BigDecimal.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

BigDecimal::BigDecimal(char *num) : mantis(num) {

	// initialize mantis length
	m_len = strlen(mantis);
	int num_len = m_len;

	// if the first character is '-', the sign is negative
	if (mantis[0] == '-') {
		sign = -1;
		m_len--;
	} else {
		sign = 1;
	}

	// check wether the decimal point exists
	for (int i = 0; i < m_len; ++i) {
		if (num[i + (1 - sign) / 2] == '.') {
			m_len--;
			break;
		}
	}

	// create a new char[] to store the mantis
	mantis = new char[m_len + 1];

	// an iterator for assignment
	char *m_idx = mantis;

	// skip first character if it is '-'
	int idx = sign == -1 ? 1 : 0;

	// copy mantis until decimal point
	for (; idx < num_len && num[idx] != '.'; ++idx) {
		*m_idx++ = num[idx];
	}

	// calculate the power of the first digit
	exp = idx - (sign == -1);

	// skip copying the decimal point
	idx++;

	// copy the rest of the mantis
	for (; idx < num_len; ++idx) {
		*m_idx++ = num[idx];
	}

	// set the last character to '\0'
	*m_idx = 0;

	// remove leading and trailing zeros
	remove_zeros();
}

BigDecimal::~BigDecimal() {
	// the only deletable data outside of the object is the mantis array
	delete[] mantis;
}

BigDecimal::BigDecimal(int *, int *) {
	// Not implemented because it is not needed
}

std::ostream &operator<<(std::ostream &out, const BigDecimal &bigDecimal) {

	// print the '-' sign if the number is negative
	if (bigDecimal.sign == -1) out << '-';

	// temporary exp value for changing
	int texp = bigDecimal.exp;

	if (texp < 1) {
		// if the ones number is a zero, print "0."
		out << "0.";

		// print all other zeros
		while (texp++) out << '0';

		// when zeros are printed, continue with other digits
		for (int i = bigDecimal.maxIdx(); i >= bigDecimal.minIdx(); --i) {
			out << bigDecimal.numAt(i);
		}

	} else {

		// print numbers until the decimal point
		while (texp) out << bigDecimal.numAt(--texp);

		// check if there are nonzero digits after decimal point
		int left = bigDecimal.m_len - bigDecimal.exp;

		// print the decimal point if needed
		if (left > 0) out << '.';

		// print the digts after the decimal point
		for (int i = -1; i >= -left; --i) {
			out << bigDecimal.numAt(i);
		}
	}
	return out;
}

BigDecimal BigDecimal::add(BigDecimal *b) {

	// helper pointer
	BigDecimal *a = this;

	// if signs are equal add else, subtract
	if (a->sign == b->sign) {

		// find first and last digit to add
		int min = MIN(a->minIdx(), b->minIdx());
		int max = MAX(a->maxIdx(), b->maxIdx());

		// new temporary string for storing the value
		char *ret = new char[max - min + 4];

		// initialize the string to zeros
		for (int i = 0; i < max - min + 3; ++i) {
			ret[i] = '0';
		}

		// set the last character to '\0'
		ret[max - min + 3] = 0;

		// add '-' character at the begining if the number is negative
		if (a->sign == -1) ret[0] = '-';

		// carry number for summing
		int carry = 0;

		// main adding loop for after the decimal point
		for (int idx = min; idx < 0; ++idx) {

			// add two numbers ant the carry
			int sum = a->numAt(idx) + b->numAt(idx) + carry;

			// set a digit
			ret[max - idx + 2] = (char)('0' + (sum % 10));

			// calculate a new carry
			carry = sum / 10;
		}

		// add the decimal point
		ret[max + 2] = '.';

		// main adding loop for after the decimal point
		for (int idx = 0; idx <= max; ++idx) {

			// add two numbers ant the carry
			int sum = a->numAt(idx) + b->numAt(idx) + carry;

			// set a digit
			ret[max - idx + 1] = (char)('0' + (sum % 10));

			// calculate a new carry
			carry = sum / 10;
		}

		// temporary storing location
		BigDecimal r = BigDecimal(ret);

		// removes possible memory leak
		delete ret;

		// returns the temporary location
		return r;

	} else {

		if (a->sign == -1) {

			// if the a is negative, subtract it from b
			BigDecimal aneg = a->neg();
			return b->sub(&aneg);

		} else {

			// if the b is negative, subtract it from b
			BigDecimal bneg = b->neg();
			return a->sub(&bneg);

		}
	}
}

BigDecimal BigDecimal::sub(BigDecimal *b) {

	// helper pointer
	BigDecimal *a = this;

	// if signs are equal subtract, else add
	if (a->sign == b->sign) {

		// negative constant for swapping b and a
		int neg = 1;
		if (b->greater(a)) neg = -1;

		// find first and last digit to subtract
		int min = MIN(a->minIdx(), b->minIdx());
		int max = MAX(a->maxIdx(), b->maxIdx());

		// new temporary string for storing the value
		char *ret = new char[max - min + 4];

		// initialize the string to zeros
		for (int i = 0; i < max - min + 3; ++i) {
			ret[i] = '0';
		}

		// set the last character to '\0'
		ret[max - min + 3] = 0;

		// add '-' character at the begining if the number is negative
		if (neg == -1) ret[0] = '-';

		// carry number for summing
		int carry = 0;

		// main subtract loop for after the decimal point
		for (int idx = min; idx < 0; ++idx) {

			// add two numbers ant the carry
			int sum = neg * a->numAt(idx) - neg * b->numAt(idx) + carry;

			// set a digit
			ret[max - idx + 2] = (char)('0' + ((sum + 10) % 10));

			// calculate a new carry
			carry = sum < 0 ? -1 : 0;

		}

		// add the decimal point
		ret[max + 2] = '.';

		// main adding loop for after the decimal point
		for (int idx = 0; idx <= max; ++idx) {

			// add two numbers ant the carry
			int sum = neg * a->numAt(idx) - neg * b->numAt(idx) + carry;

			// set a digit
			ret[max - idx + 1] = (char)('0' + ((sum + 10) % 10));

			// calculate a new carry
			carry = sum < 0 ? -1 : 0;

		}

		// temporary storing location
		BigDecimal r = BigDecimal(ret);

		// removes possible memory leak
		delete ret;

		// returns the temporary location
		return r;
	} else {
		// negate the second argument and add
		BigDecimal bneg = b->neg();
		return a->add(&bneg);
	}
}

BigDecimal::BigDecimal(const BigDecimal &obj) {

	// tew mantis character array
	mantis = new char[obj.m_len + 1];

	// copy all data from obj to the nem mantis
	for (int i = 0; i < obj.m_len; ++i) {
		mantis[i] = obj.mantis[i];
	}

	// set the last character to '\0'
	mantis[obj.m_len] = 0;

	// copy the rest of the fields
	exp = obj.exp;
	sign = obj.sign;
	m_len = obj.m_len;
}

BigDecimal BigDecimal::abs() {

	// temporary storing variable
	BigDecimal x(*this);

	// calculate the absolute value and return
	x.sign = 1;
	return x;
}

BigDecimal BigDecimal::neg() {

	// temporary storing variable
	BigDecimal x(*this);

	// calculate the absolute value and return
	x.sign *= -1;
	return x;
}

bool BigDecimal::greater(BigDecimal *b) {

	// helper pointer
	BigDecimal *a = this;

	// if signs are not equal, there is no sence to compare values
	if (a->sign == b->sign) {

		// find first and last digit to compare
		int min = MIN(a->minIdx(), b->minIdx());
		int max = MAX(a->maxIdx(), b->maxIdx());

		// main loop to detect greater than values
		for (int i = max; i >= min; --i) {

			// first time two numbers differ, the solution is known
			if (a->numAt(i) > b->numAt(i)) return true;
			else if (a->numAt(i) < b->numAt(i)) return false;
		}

		// if there are no differences, the numbers are equal
		return false;

	} else {

		// if a is positive then it is greater then b
		return a->sign == 1;
	}
}

bool BigDecimal::less(BigDecimal *b) {

	// a < b => b > a
	return b->greater(this);
}

bool BigDecimal::equals(BigDecimal *b) {

	// helper pointer
	BigDecimal *a = this;

	// numbers can only be equal if their signs and mantis lengths are equal
	if (a->sign == b->sign && a->m_len == b->m_len) {

		// find first and last digit to compare
		int min = MIN(a->minIdx(), b->minIdx());
		int max = MAX(a->maxIdx(), b->maxIdx());

		// find if there is a difference
		for (int i = max; i >= min; --i) {
			if (a->numAt(i) != b->numAt(i)) return false;
		}

		// if there are no differences the numbers are same
		return true;

	} else {

		// if the numbers have different signs, they are not the same
		return false;

	}
}

BigDecimal BigDecimal::shl(int n) {

	// temporary storing variable
	BigDecimal ret(*this);

	// move the exponent
	ret.exp -= n;
	return ret;
}

BigDecimal BigDecimal::shr(int n) {

	// temporary storing variable
	BigDecimal ret(*this);

	// move the exponent
	ret.exp += n;
	return ret;
}

BigDecimal BigDecimal::rmd(int *n) {

	// temporary storing variable
	BigDecimal ret(*this);

	if (m_len < exp) {

		// if there are zeros between the mantis and decimal point don't do
		// anything
		*n = 0;
		return ret;

	} else {

		// if there are nonzero digits after decimal point move the decimal
		// point to the end of the mantis
		*n = m_len - this->exp;

		// set the exponent to the number of digits
		ret.exp = m_len;
		return ret;
	}
}