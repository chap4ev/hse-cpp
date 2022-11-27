#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <iostream>
#include <string>
#include <vector>


class BigInteger {
 public:

  const int32_t NODE_MOD = 1e9;
  const size_t NODE_STR_SIZE = 9;

  BigInteger();
  BigInteger(int);
  BigInteger(int64_t);
  BigInteger(std::string);
  BigInteger(int64_t num, const int& shift);

  BigInteger(const BigInteger&);
  ~BigInteger() = default;

  BigInteger& operator=(const BigInteger&);

  BigInteger operator+(const BigInteger&) const;
  BigInteger operator-(const BigInteger&) const;
  BigInteger operator*(const int32_t&) const;
  BigInteger operator*(const int64_t&) const;
  BigInteger operator*(const BigInteger&) const;
  BigInteger operator/(const int32_t&) const;
  BigInteger operator/(const BigInteger&) const;
  BigInteger operator%(const BigInteger&) const;

  BigInteger operator-() const;

  BigInteger& operator--();
  const BigInteger operator--(int);
  BigInteger& operator++();
  const BigInteger operator++(int);

  BigInteger& operator+=(const BigInteger&);
  BigInteger& operator-=(const BigInteger&);
  BigInteger& operator*=(const int32_t&);
  BigInteger& operator*=(const int64_t&);
  BigInteger& operator*=(const BigInteger&);
  BigInteger& operator/=(const int32_t&);
  BigInteger& operator/=(const BigInteger&);
  BigInteger& operator%=(const BigInteger&);

  bool operator==(const BigInteger&) const;
  bool operator!=(const BigInteger&) const;
  bool operator>(const BigInteger&) const;
  bool operator>=(const BigInteger&) const;
  bool operator<(const BigInteger&) const;
  bool operator<=(const BigInteger&) const;

  std::string toString() const;

  operator bool() const;

  friend std::ostream& operator<<(std::ostream&, const BigInteger&);
  friend std::istream& operator>>(std::istream&, BigInteger&);

  friend BigInteger abs(const BigInteger&);


 private:

  bool _is_negative;
  std::vector<int32_t> _digits;

  BigInteger& _digits_shift(const int32_t&);

  BigInteger& _addAbsoluteValues(const BigInteger&);
  BigInteger& _minusAbsoluteValue(const BigInteger&);

  BigInteger& _clear();
  void _trim_zeros();
};

BigInteger abs(const BigInteger&);

std::ostream& operator<<(std::ostream&, const BigInteger&);
std::istream& operator>>(std::istream&, BigInteger&);


#endif  // BIGINTEGER_H