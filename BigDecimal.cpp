#include "BigDecimal.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

BigDecimal::BigDecimal(char *num) : mantis(num) {
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
	if (texp < 1) {
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
		BigDecimal r = BigDecimal(ret);
		delete ret;
		return r;
	} else {
		if (a->sign == -1) {
			BigDecimal aneg = a->neg();
			return b->sub(&aneg);
		} else {
			BigDecimal bneg = b->neg();
			return a->sub(&bneg);
		}
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
		BigDecimal r = BigDecimal(ret);
		delete ret;
		return r;
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

BigDecimal BigDecimal::abs() {
	BigDecimal x(*this);
	x.sign = 1;
	return x;
}

BigDecimal BigDecimal::neg() {
	BigDecimal x(*this);
	x.sign *= -1;
	return x;
}

bool BigDecimal::greater(BigDecimal *b) {
	BigDecimal *a = this;
	if (a->sign == b->sign) {
		int min = MIN(a->minIdx(), b->minIdx());
		int max = MAX(a->maxIdx(), b->maxIdx());
		for (int i = max; i >= min; --i) {
			if (a->numAt(i) > b->numAt(i)) return true;
			else if (a->numAt(i) < b->numAt(i)) return false;
		}
		return false;
	} else {
		return a->sign == 1;
	}
}

bool BigDecimal::less(BigDecimal *b) {
	return b->greater(this);
}

bool BigDecimal::equals(BigDecimal *b) {
	BigDecimal *a = this;
	if (a->sign == b->sign) {
		int min = MIN(a->minIdx(), b->minIdx());
		int max = MAX(a->maxIdx(), b->maxIdx());
		for (int i = max; i >= min; --i) {
			if (a->numAt(i) != b->numAt(i)) return false;
		}
		return true;
	} else {
		return false;
	}
}

BigDecimal BigDecimal::shl(int n) {
	BigDecimal ret(*this);
	ret.exp -= n;
	return ret;
}

BigDecimal BigDecimal::shr(int n) {
	BigDecimal ret(*this);
	ret.exp += n;
	return ret;
}

BigDecimal BigDecimal::rmd(int *n) {
	int mantis_len = strlen(mantis);
	*n = mantis_len - this->exp;
	BigDecimal ret(*this);
	ret.exp = mantis_len;
	return ret;
}