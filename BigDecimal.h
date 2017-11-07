#ifndef DZ1_BIGDECIMAL_H
#define DZ1_BIGDECIMAL_H

#include <iostream>

class BigDecimal {

public:
	BigDecimal(char *);

	BigDecimal(int *, int *);

	~BigDecimal();

	BigDecimal add(BigDecimal *);

	BigDecimal sub(BigDecimal *);

	BigDecimal abs(BigDecimal *);

	//BigDecimal mul(BigDecimal*);
	//BigDecimal div(BigDecimal*);
	bool greater(BigDecimal *);
	//bool less(BigDecimal*);
	//bool equals(BigDecimal*);

	// TODO: move this to private
	BigDecimal neg();

private:

	BigDecimal(const BigDecimal &);

	int strlen(const char *s) const {
		int ret = -1;
		while (s[++ret] != 0);
		return ret;
	}

	void remove_zeros() {
		// check if all zeros:
		int mantis_len = strlen(mantis);
		bool zero = true;
		for (int i = 0; i < mantis_len; ++i) {
			if (mantis[i] != '0') {
				zero = false;
				break;
			}
		}
		if (zero) {
			delete mantis;
			mantis = new char[1];
			*mantis = '0';
			exp = 0;
			sign = 1;
		} else {
			int rm_front = 0, rm_back = mantis_len - 1;
			for (; rm_front < mantis_len && mantis[rm_front] == '0'; ++rm_front) exp--;
			for (; rm_back >= 0 && mantis[rm_back] == '0'; --rm_back);
			rm_back = mantis_len - 1 - rm_back;
			char *new_mantis = new char[mantis_len + 1 - rm_back - rm_front];
			char *it = new_mantis;
			for (int i = rm_front; i < mantis_len - rm_back; ++i) {
				*it++ = mantis[i];
			}
			*it = 0;
			delete mantis;
			mantis = new_mantis;
		}
	}

	int numAt(int idx) const {
		int char_idx = exp - idx - 1;
		if (char_idx < 0 || char_idx >= strlen(mantis)) return 0;
		else return mantis[char_idx] - '0';
	}

	int maxIdx() const {
		return exp - 1;
	}

	int minIdx() const {
		return exp - strlen(mantis);
	}

	int sign;
	int exp;
	char *mantis;

	friend std::ostream &operator<<(std::ostream &, const BigDecimal &);
};


#endif //DZ1_BIGDECIMAL_H
