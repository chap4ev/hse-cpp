#include "biginteger.h"

BigInteger::BigInteger() : _is_negative(false), _digits( {0}) {};

BigInteger::BigInteger(int num) {
  *this = BigInteger((int64_t)num);
};

BigInteger::BigInteger(int64_t num) {
  _is_negative = num < 0;

  if (num == 0) {
    _digits.push_back(0);
    return;
  }

  num = std::abs(num);

  while (num > 0) {
    _digits.push_back(num % NODE_MOD);
    num /= NODE_MOD;
  }
};

BigInteger::BigInteger(int64_t num, const int& shift) {

  _is_negative = num < 0;

  if (num == 0) {
    _digits.push_back(0);
    return;
  }

  _digits = std::vector<int32_t> (shift, 0);

  num = std::abs(num);

  while (num > 0) {
    _digits.push_back(num % NODE_MOD);
    num /= NODE_MOD;
  }
};

BigInteger::BigInteger(std::string str) {
  int is_negative_to_int = (str[0] == '-');

  _is_negative = is_negative_to_int;

  int remain_size = str.size();

  while (remain_size > is_negative_to_int) {
    int str_iter = remain_size - 9;
    int bitsize = 9;
    if (remain_size < 10) {
      str_iter = is_negative_to_int;
      bitsize = remain_size - str_iter;
    }
    _digits.push_back(std::stoi(str.substr(str_iter, bitsize)));
    remain_size -= 9;
  }
}


BigInteger::BigInteger(const BigInteger& other)
  : _is_negative(other._is_negative), _digits(other._digits) {};


BigInteger& BigInteger::operator=(const BigInteger& other) {
  _is_negative = other._is_negative;
  _digits = other._digits;
  return *this;
};


BigInteger BigInteger::operator+(const BigInteger& other) const {
  return BigInteger(*this) += other;
};

BigInteger BigInteger::operator-(const BigInteger& other) const {
  return BigInteger(*this) -= other;
};

BigInteger BigInteger::operator*(const BigInteger& other) const {
  return BigInteger(*this) *= other;
};

BigInteger BigInteger::operator*(const int32_t& other) const {
  return BigInteger(*this) *= (int64_t)other;
};

BigInteger BigInteger::operator*(const int64_t& other) const {
  return BigInteger(*this) *= other;
};

BigInteger BigInteger::operator/(const BigInteger& other) const {
  return BigInteger(*this) /= other;
};

BigInteger BigInteger::operator/(const int32_t& other) const {
  return BigInteger(*this) /= other;
};

BigInteger BigInteger::operator%(const BigInteger& other) const {
  return BigInteger(*this) %= other;
};

BigInteger BigInteger::operator-() const {
  BigInteger copy(*this);
  copy._is_negative ^= true;
  return copy;
};


BigInteger& BigInteger::operator--() {
  return *this -= BigInteger(1);
};

const BigInteger BigInteger::operator--(int) {
  BigInteger copy(*this);
  *this -= BigInteger(1);
  return copy;
};

BigInteger& BigInteger::operator++() {
  return *this += BigInteger(1);
};

const BigInteger BigInteger::operator++(int) {
  BigInteger copy(*this);
  *this += BigInteger(1);
  return copy;
};


BigInteger& BigInteger::_addAbsoluteValues(const BigInteger& other) {
  // adding abs value of other to abs value of *this

  _digits.resize(std::max(_digits.size(), other._digits.size()) + 1);

  for (int i = 0; i < _digits.size() - 1; ++i) {

    if (i < other._digits.size()) {
      _digits[i] += other._digits[i];
    } else if (_digits[i] < NODE_MOD) {
      // nothing was added and no need to pass remainder
      break;
    }

    if (_digits[i] >= NODE_MOD) {
      _digits[i + 1] += 1;
      _digits[i] -= NODE_MOD;
    }
  }

  _trim_zeros();
  return *this;
};

BigInteger& BigInteger::_minusAbsoluteValue(const BigInteger& other) {
  // substracting abs value of *this by abs value of other
  // (*this >= other)

  for (int i = 0; i < _digits.size() - 1; ++i) {
    if (i < other._digits.size()) {
      _digits[i] -= other._digits[i];
    } else if (_digits[i] >= 0) {
      // nothing was substracted and no need to pass remainder
      break;
    }

    if (_digits[i] < 0) {
      _digits[i + 1] -= 1;
      _digits[i] += NODE_MOD;
    }
  }

  // calculate last node
  if (_digits.size() == other._digits.size()) {
    _digits.back() -= other._digits.back();
  }

  _trim_zeros();
  return *this;
}


BigInteger& BigInteger::operator+=(const BigInteger& other) {
  if (this == &other) {
    return _addAbsoluteValues(BigInteger(*this));
  }

  if (_is_negative != other._is_negative) {
    *this -= -other;

  } else { // (_is_negative == other._is_negative)
    _addAbsoluteValues(other);
  }
  return *this;
};

BigInteger& BigInteger::operator-=(const BigInteger& other) {
  if (this == &other) {
    return _clear();
  }

  if (_is_negative != other._is_negative) {
    return _addAbsoluteValues(other);

  } else { // (_is_negative == other._is_negative)
    if (*this == other) {
      return _clear();
    }

    if (abs(*this) > abs(other)) {
      _minusAbsoluteValue(other);

    } else {
      *this = BigInteger(other)._minusAbsoluteValue(*this);
      _is_negative = !_is_negative;
    }
    return *this;
  }

};


BigInteger& BigInteger::operator*=(const int32_t& other) {
  return *this *= (int64_t)other;
};

BigInteger& BigInteger::operator*=(const int64_t& other) {
  BigInteger result;

  int64_t other_abs = abs(other);

  for (int node_i = 0; node_i < _digits.size(); ++node_i) {
    result += BigInteger(other_abs * _digits[node_i], node_i);
  }

  result._is_negative = _is_negative ^ (other < 0);

  return *this = result;
};

BigInteger& BigInteger::operator*=(const BigInteger& other) {
  BigInteger result;

  BigInteger other_abs(other);
  other_abs._is_negative = false;

  for (int node_i = 0; node_i < _digits.size(); ++node_i) {
    BigInteger tmp(other_abs * (int64_t)_digits[node_i]);
    tmp._digits_shift(node_i);
    result += tmp;
  }

  result._is_negative = _is_negative ^ other._is_negative;

  return *this = result;
};

BigInteger& BigInteger::operator/=(const int32_t& other) {
  if (other == 0)
    return *this;

  BigInteger result;

  int32_t other_abs = abs(other);

  int64_t remainer = 0;

  for (int node_i = _digits.size() - 1; node_i >= 0; --node_i) {
    int64_t tmp_res = (remainer * NODE_MOD + _digits[node_i]) / other_abs;
    remainer = (remainer * NODE_MOD + _digits[node_i]) % other_abs;
    result += BigInteger(tmp_res, node_i);
  }
  result._trim_zeros();
  result._is_negative = _is_negative ^ (other < 0);

  return *this = result;
};

BigInteger& BigInteger::operator/=(const BigInteger& other) {
  if (other == BigInteger(0))
    return *this;

  // binsearch

  BigInteger this_abs = abs(*this);
  BigInteger other_abs = abs(other);

  BigInteger left(0);
  BigInteger right(++abs(*this));

  BigInteger bi1(1);
  while (right - left > bi1) {
    BigInteger mid((left + right) / 2);

    if (other_abs * mid > this_abs) {
      right = mid;
    } else {
      left = mid;
    }
  }

  left._is_negative = _is_negative ^ other._is_negative;

  return *this = left;
};

BigInteger& BigInteger::operator%=(const BigInteger& other) {
  return *this -= *this / other * other;
};


bool BigInteger::operator==(const BigInteger& other) const {
  if (_is_negative != other._is_negative ||
      _digits.size() != other._digits.size()) {
    return false;
  }

  for (int i = _digits.size() - 1; i >= 0; --i) {
    if (_digits[i] != other._digits[i]) {
      return false;
    }
  }
  return true;
};

bool BigInteger::operator!=(const BigInteger& other) const {
  return !(*this == other);
};


bool BigInteger::operator>(const BigInteger& other) const {
  if (_is_negative == true && other._is_negative == false) {
    return false;
  }
  if (_is_negative == false && other._is_negative == true) {
    return true;
  }

  if (_digits.size() != other._digits.size()) {
    return (_digits.size() > other._digits.size()) ^ _is_negative;
  }

  if (*this == other) {
    return false;
  }

  // compairing by absolute value
  bool grather = false;

  for (int i = _digits.size() - 1; i >= 0; --i) {
    auto res = _digits[i] <=> other._digits[i];
    if (res != 0) {
      grather = res > 0;
      break;
    }
  }

  return grather ^ _is_negative;
};

bool BigInteger::operator>=(const BigInteger& other) const {
  return !(*this < other);
};

bool BigInteger::operator<(const BigInteger& other) const {
  return other > *this;
};

bool BigInteger::operator<=(const BigInteger& other) const {
  return !(*this > other);
};


std::string BigInteger::toString() const {
  std::string str_representation = _is_negative ? "-" : "";

  str_representation += std::to_string(_digits.back());

  for (int i = _digits.size() - 2; i >= 0; --i) {
    std::string digits_node = std::to_string(_digits[i]);
    std::string added_zeros(NODE_STR_SIZE - digits_node.size(), '0');
    str_representation += added_zeros += digits_node;
  }

  return str_representation;
};


BigInteger::operator bool() const {
  return *this != BigInteger(0);
};


BigInteger& BigInteger::_digits_shift(const int32_t& amount) {
  std::vector<int32_t> paste_vec(amount, 0);
  _digits.insert(_digits.begin(), paste_vec.begin(), paste_vec.end());
  return *this;
}

BigInteger& BigInteger::_clear() {
  _digits = {0};
  _is_negative = false;
  return *this;
}

void BigInteger::_trim_zeros() {
  while (_digits.size() > 1 && _digits.back() == 0)
    _digits.pop_back();
}


BigInteger abs(const BigInteger& num) {
  BigInteger copy(num);
  copy._is_negative = false;
  return copy;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
  out << num.toString();
  return out;
};

std::istream& operator>>(std::istream& in, BigInteger& num) {
  std::string str;
  in >> str;
  num = BigInteger(str);
  return in;
};