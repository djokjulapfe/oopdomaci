#include "BigDecimal.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

BigDecimal::BigDecimal(char *num) : mantis(num) {
	std::cout << "DEBUG\n";
	int mantis_len = strlen(mantis);
	int num_len = mantis_len;
	if (mantis[0] == '-') {
		sign = -1;
		mantis_len--;
	} else {
		sign = 1;
	}
	for (int i = 0; i < mantis_len; ++i) {
		if (num[i + (1 - sign) / 2] == '.') {
			mantis_len--;
			break;
		}
	}
	mantis = new char[mantis_len + 1];
	char *m_idx = mantis;
	int idx = sign == -1 ? 1 : 0;
	for (; idx < num_len && num[idx] != '.'; ++idx) {
		*m_idx++ = num[idx];
	}
	exp = idx - (sign == -1);
	idx++;
	for (; idx < num_len; ++idx) {
		*m_idx++ = num[idx];
	}
	*m_idx = 0;
	remove_zeros();
}

BigDecimal::~BigDecimal() {
	delete[] mantis;
}

BigDecimal::BigDecimal(int *whole, int *fraction) {

}

std::ostream &operator<<(std::ostream &out, const BigDecimal &bigDecimal) {
	if (bigDecimal.sign == -1) out << '-';
	int texp = bigDecimal.exp;
	if (texp < 0) {
		out << "0.";
		while (texp++) out << '0';
		for (int i = bigDecimal.maxIdx(); i >= bigDecimal.minIdx(); --i) {
			out << bigDecimal.numAt(i);
		}
	} else {
		while (texp) out << bigDecimal.numAt(--texp);
		int mantis_len = bigDecimal.strlen(bigDecimal.mantis);
		int left = mantis_len - bigDecimal.exp;
		if (left > 0) out << '.';
		for (int i = -1; i >= -left; --i) {
			out << bigDecimal.numAt(i);
		}
	}
	return out;
}

BigDecimal BigDecimal::add(BigDecimal *b) {
	BigDecimal *a = this;
	if (a->sign == b->sign) {
		int min = MIN(a->minIdx(), b->minIdx());
		int max = MAX(a->maxIdx(), b->maxIdx());
		char *ret = new char[max - min + 4];
		for (int i = 0; i < max - min + 3; ++i) {
			ret[i] = '0';
		}
		ret[max - min + 3] = 0;
		if (a->sign == -1) ret[0] = '-';
		int carry = 0;
		for (int idx = min; idx < 0; ++idx) {
			int sum = a->numAt(idx) + b->numAt(idx) + carry;
			ret[max - idx + 2] = '0' + (sum % 10);
			carry = sum / 10;
		}
		ret[max + 2] = '.';
		for (int idx = 0; idx <= max; ++idx) {
			int sum = a->numAt(idx) + b->numAt(idx) + carry;
			ret[max - idx + 1] = '0' + (sum % 10);
			carry = sum / 10;
		}
		return BigDecimal(ret);
	} else {
		if (a->sign == -1) return b->sub(a);
		else return a->sub(b);
	}
}

BigDecimal BigDecimal::sub(BigDecimal *b) {
	BigDecimal *a = this;
	if (a->sign == b->sign) {
		int neg = 1;
		if (b->greater(a)) neg = -1;
		int min = MIN(a->minIdx(), b->minIdx());
		int max = MAX(a->maxIdx(), b->maxIdx());
		char *ret = new char[max - min + 4];
		for (int i = 0; i < max - min + 3; ++i) {
			ret[i] = '0';
		}
		ret[max - min + 3] = 0;
		if (neg == -1) ret[0] = '-';
		int carry = 0;
		for (int idx = min; idx < 0; ++idx) {
			int sum = neg * a->numAt(idx) - neg * b->numAt(idx) + carry;
			ret[max - idx + 2] = '0' + ((sum + 10) % 10);
			carry = sum < 0 ? -1 : 0;
		}
		ret[max + 2] = '.';
		for (int idx = 0; idx <= max; ++idx) {
			int sum = neg * a->numAt(idx) - neg * b->numAt(idx) + carry;
			ret[max - idx + 1] = '0' + ((sum + 10) % 10);
			carry = sum < 0 ? -1 : 0;
		}
		return BigDecimal(ret);
	} else {
		BigDecimal bneg = b->neg();
		return a->add(&bneg);
	}
}

BigDecimal::BigDecimal(const BigDecimal &obj) {
	int mantis_len = strlen(obj.mantis);
	mantis = new char[mantis_len + 1];
	for (int i = 0; i < mantis_len; ++i) {
		mantis[i] = obj.mantis[i];
	}
	mantis[mantis_len] = 0;
	exp = obj.exp;
	sign = obj.sign;
}

BigDecimal BigDecimal::neg() {
	BigDecimal x("0");
	int mantis_len = strlen(this->mantis);
	x.mantis = new char[mantis_len + 1];
	for (int i = 0; i < mantis_len; ++i) {
		x.mantis[i] = this->mantis[i];
	}
	x.mantis[mantis_len] = 0;
	x.exp = this->exp;
	x.sign = -this->sign;
	std::cout << x << std::endl;
	return x;
}

bool BigDecimal::greater(BigDecimal *b) {
	BigDecimal *a = this;
	if (a->sign == b->sign) {
		int min = MIN(a->minIdx(), b->minIdx());
		int max = MAX(a->maxIdx(), b->maxIdx());
		for (int i = maxIdx(); i >= minIdx(); --i) {
			if (a->numAt(i) > b->numAt(i)) return true;
			else if (a->numAt(i) < b->numAt(i)) return false;
		}
		return false;
	} else {
		return a->sign == -1;
	}
}

BigDecimal BigDecimal::abs(BigDecimal *) {
	BigDecimal x(*this);
	x.sign = 1;
	return x;
}
