#ifndef DZ1_BIGDECIMAL_H
#define DZ1_BIGDECIMAL_H

#include <iostream>

/**
 * @brief Class can store large decimal numbers.
 */

class BigDecimal {

public:
	/**
	 * @brief Main constructor class
	 * @param num a string containing a decimal number
	 */
	BigDecimal(char *num);

	/**
	 * @brief Helper constructor for easy assignment
	 * @param whole array of numbers before the decimal point
	 * @param fraction array of numbers before the decimal point
	 */
	BigDecimal(int *whole, int *fraction);

	/**
	 * @brief Standard destructor that deletes the internal char array
	 */
	~BigDecimal();

	/**
	 * @brief Adds two numbers
	 * @param b number to be added
	 * @return the sum of b and this
	 */
	BigDecimal add(BigDecimal *b);

	/**
	 * @brief Subtracts two numbers
	 * @param b number to be subtracted
	 * @return the difference this - b
	 */
	BigDecimal sub(BigDecimal *b);

	/**
	 * @brief Calculates the absolute value of a number
	 * @return the absolute number
	 */
	BigDecimal abs();

	/**
	 * @brief Checks if this > b
	 * @param b number to be checked against
	 * @return true iff this > b
	 */
	bool greater(BigDecimal *b);

	/**
	 * @brief Checks if this < b
	 * @param b number to be checked agaianst
	 * @return true iff this < b
	 */
	bool less(BigDecimal *b);

	/**
	 * @brief Checks if two numbers are equal
	 * @param b number to be checked against
	 * @return true iff this == b
	 */
	bool equals(BigDecimal *b);

	/**
	 * @brief Shifts the number left n times
	 * @param n number of times to be shifted left
	 * @return the number shifted n times to the left
	 */
	BigDecimal shl(int n);

	/**
	 * @brief Shifts the number right n times
	 * @param n number of times to be shifted right
	 * @return the number shifted n times to the right
	 */
	BigDecimal shr(int n);

	/**
	 * @brief Shifts the number so that the decimal point gets erased
	 * @param n the number of times the number is shifted
	 * @return the number without the decimal point
	 */
	BigDecimal rmd(int *n);

	/**
	 * @brief Copy constructor that safely copies a BigDecimal
	 * @param bigDecimal the number to be copied
	 */
	BigDecimal(const BigDecimal &bigDecimal);


private:

	/**
	 * @brief default constructor
	 */
	BigDecimal();

	/**
	 * @brief Calculates the negative of a number
	 * @return -this
	 */
	BigDecimal neg();

	/**
	 * @brief Helper function for calculating length of a string
	 * @param s string to be checked
	 * @return length of a strin
	 */
	int strlen(const char *s) const {
		// find the index of '\0' character
		int ret = -1;
		while (s[++ret] != 0);
		return ret;
	}

	/**
	 * @brief Safely removes zeros from start and end
	 */
	void remove_zeros() {

		// Check if all zeros
		bool zero = true;

		// loop and find first zero if exists
		for (int i = 0; i < m_len; ++i) {
			if (mantis[i] != '0') {
				zero = false;
				break;
			}
		}
		if (zero) {
			// initialize to zero
			delete mantis;
			mantis = new char[1];
			*mantis = '0';
			exp = 0;
			sign = 1;
		} else {
			// initial values
			int rm_front = 0, rm_back = m_len - 1;

			// find the first nonzero value from the left
			for (; rm_front < m_len && mantis[rm_front] == '0'; ++rm_front)
				exp--;

			// find the first nonzero value from the right
			for (; rm_back >= 0 && mantis[rm_back] == '0'; --rm_back);

			// flip to store number of zeros from the back
			rm_back = m_len - 1 - rm_back;

			// make a new mantis with less data
			char *new_mantis = new char[m_len + 1 - rm_back - rm_front];
			char *it = new_mantis;

			// copy the mantis to new mantis
			for (int i = rm_front; i < m_len - rm_back; ++i) {
				*it++ = mantis[i];
			}
			*it = 0;

			// change the mantis
			delete mantis;
			mantis = new_mantis;
		}
	}

	/**
	 * @brief Helper function for transforming logical indices to memory
	 * @param idx logical index
	 * @return number at logical index
	 */
	int numAt(int idx) const {
		// convert logical index to memory index
		int char_idx = exp - idx - 1;

		// check if memory index exceeds string indices
		if (char_idx < 0 || char_idx >= strlen(mantis)) return 0;
		else return mantis[char_idx] - '0';
	}

	/**
	 * @return logical index of the first digit
	 */
	int maxIdx() const {
		return exp - 1;
	}

	/**
	 * @return logical index of the last digit
	 */
	int minIdx() const {
		return exp - strlen(mantis);
	}

	int sign; /* Sign of the big decimal. Can take values of -1 and 1. */
	int exp; /* Exponent of the big decimal. */
	int m_len; /* Length (in bytes) of the mantis */
	char *mantis; /* Mantis of the big number */

	/**
	 * @brief function for std::ostream operator<< overloading
	 * @return std::ostream that can print a BigDecimal
	 */
	friend std::ostream &operator<<(std::ostream &, const BigDecimal &);
};


#endif //DZ1_BIGDECIMAL_H
