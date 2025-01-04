#include <cassert>
#include <sstream>
#include <iomanip>
#include "bigint.h"

BigInt::BigInt() : vector({0}), negative_val(false) {
  // initializes private fields
}

BigInt::BigInt(uint64_t val, bool negative) : vector({val}), negative_val(negative) {
  // initializes private fields
}

BigInt::BigInt(std::initializer_list<uint64_t> vals, bool negative) : vector(vals), negative_val(negative) {
  fix_edge_cases();
}

BigInt::BigInt(const BigInt &other) : vector(other.vector), negative_val(other.negative_val) {
  // initializes private fields
}

BigInt::~BigInt() {
  // do nothing
}

BigInt &BigInt::operator=(const BigInt &rhs) {
  // initialize private fields
  vector = rhs.vector;
  negative_val = rhs.negative_val;
  return *this;
}

bool BigInt::is_negative() const {
 return negative_val;
}

void BigInt::fix_edge_cases() {
  // deal with empty initializer list edge case
  if (vector.size() == 0) {
    vector = {0};
  }

  // deal with leading zeros edge case
  while (vector.back() == 0 && vector.size() > 1) {
    vector.pop_back();
  }

  // deal with negative zero edge case
  if (vector.size() == 1 && vector[0] == 0) {
    negative_val = false;
  }
}

const std::vector<uint64_t> &BigInt::get_bit_vector() const {
 // return field v
 return vector;
}

uint64_t BigInt::get_bits(unsigned index) const {
  // check index out of bounds
  if (index < 0 || vector.size() <= index) {
    return 0;
  }

  // return value at index
  return vector[index];
}

BigInt BigInt::operator+(const BigInt &rhs) const {
  BigInt sum;
  if (this->is_negative() == rhs.is_negative()) { // both positive or both negative
    sum.vector = sum_magnitudes(vector, rhs.vector);
    sum.negative_val = is_negative();
  } else { // one positive, one negative
    sum.vector = diff_magnitudes(vector, rhs.vector);

    // calculate sign
    if (this->is_negative() && (compare_magnitudes(this->vector, rhs.vector) > 0)) {
      sum.negative_val = true;
    } else if (rhs.is_negative() && (compare_magnitudes(this->vector, rhs.vector) < 0)) {
      sum.negative_val = true;
    } else {
      sum.negative_val = false;
    }
  }
  return sum;
}

BigInt BigInt::operator-(const BigInt &rhs) const {
 return *this + (-rhs); // call addition with unary minus (negation)
}

BigInt BigInt::operator-() const {
  // make copy
  BigInt negation = BigInt(*this);

  // change sign if nonzero
  if (!is_zero()) {
    negation.negative_val = !negation.negative_val;
  }
 
  return negation;
}

bool BigInt::is_bit_set(unsigned n) const {
  // returns the n % 64th bit of the n / 64th element
  return get_bits(n / 64) & (1UL << (n % 64));
}

BigInt BigInt::operator<<(unsigned n) const {
  // shifting a negative throws an exception
  if (is_negative()) {
    throw std::invalid_argument("received negative");
  }

  // initialize new BigInt
  BigInt shifted = BigInt({0});

  // shift bits n % 64 places to the right
  for (auto i = vector.begin(); i != vector.end(); i++) {
    shifted.vector.back() += *i << n % 64;
    if (n % 64 == 0) {
      shifted.vector.push_back(0);
    } else {
      shifted.vector.push_back(*i >> (64 - n % 64));
    }
  }

  // shift elements n / 64 places to the left
  for (unsigned i = 0; i < n / 64; i++) {
    shifted.vector.insert(shifted.vector.begin(), 0);
  }

  // remove leading zeros
  while (shifted.vector.back() == 0 && shifted.vector.size() > 1) {
    shifted.vector.pop_back();
  }

  return shifted;
}

BigInt BigInt::operator*(const BigInt &rhs) const {
  // initialize product and calculate the absolute value of the right hand side
  BigInt product = BigInt();
  BigInt absolute_value_rhs = rhs.abs();

  // iterate through each bit of the left hand side and multiple that power of two by the rhs
  for (unsigned i = 0; i < 64 * vector.size(); i++) {
    if (this->is_bit_set(i)) {
      product.vector = sum_magnitudes(product.vector, (absolute_value_rhs << i).vector);
    }
  }

  // calculate sign and deal with edge case
  product.negative_val = this->is_negative() != rhs.is_negative();
  product.fix_edge_cases();
  return product;
}

BigInt BigInt::operator/(const BigInt &rhs) const {
  // dividing by zero throws and exception
  if (rhs.is_zero()) {
    throw std::invalid_argument("division by zero");
  }

  // call recursive helper function
  BigInt quotient = divide(this->abs(), rhs.abs(), BigInt(0), this->abs());

  // return answer
  quotient.negative_val = this->is_negative() != rhs.is_negative();
  quotient.fix_edge_cases();
  return quotient;
}

BigInt BigInt::divide(const BigInt &dividend, const BigInt &divisor, const BigInt &lower, const BigInt &upper) { // bounds are inclusive
  // base case
  if (lower == upper) {
    return lower;
  }

  // recursive case
  BigInt center = (upper + lower).div_by_2();
  BigInt prod = center * divisor;
  if (prod > dividend) {
    // then we know the answer is less than center
    return divide(dividend, divisor, lower, center - BigInt(1));
  } else if (prod < dividend) {
    // then answer could be center so we check the next highest
    BigInt prod_next_highest = prod + divisor;

    if (prod_next_highest < dividend) {
      return divide(dividend, divisor, center + BigInt(1), upper);
    } else if (prod_next_highest == dividend) {
      return center + BigInt(1);
    } else {
      return center;
    }

  } else {
    // else center is the quotient
    return center;
  }
}

BigInt BigInt::div_by_2() const {
  // initialize with first element from vector
  BigInt quotient(vector[0] >> 1);

  // right shift by one
  for (auto i = vector.begin() + 1; i != vector.end(); i++) {
    quotient.vector.back() += *i << 63;
    quotient.vector.push_back(*i >> 1);
  }

  return quotient;
}

int BigInt::compare(const BigInt &rhs) const {
  if (this->is_negative() == rhs.is_negative()) { // both positive or both negative
    return this->is_negative() ? -compare_magnitudes(vector, rhs.vector) : compare_magnitudes(vector, rhs.vector);
  } else {
    return this->is_negative() ? -1 : 1;
  }
}

std::string BigInt::to_hex() const {
 std::stringstream s;

 // if the integer is negative, we start with a minus
 if (negative_val) {
   s << '-';
 }

 // iterate through every item in vector and print
 for (auto i = vector.rbegin(); i != vector.rend(); i++) {
   s << std::hex << *i << std::setfill('0') << std::setw(16);
 }

 return s.str();
}

std::string BigInt::to_dec() const {
  std::stringstream s;

  // if the integer is negative, we start with a minus
  if (negative_val) {
    s << '-';
  }

  // if the integer is zero, we return the string with a 0 digit
  if (is_zero()) {
    return "0";
  }

  // divide by 10 ^ 19 and print remainder to string stream until the quotient is zero
  print_remainder(s, this->abs(), BigInt(0x8AC7230489E80000UL));

  return s.str();
}

void BigInt::print_remainder(std::stringstream &s, const BigInt &dividend, const BigInt &base) {
  BigInt quotient = dividend / base;
  BigInt remainder = (dividend - (quotient * base));
  if (!quotient.is_zero()) {
    print_remainder(s, quotient, base);
  }
  s << std::dec << remainder.vector[0] << std::setfill('0') << std::setw(19);
}

bool BigInt::is_zero() const {
 return vector.size() == 1 && vector[0] == 0;
}

std::vector<uint64_t> BigInt::sum_magnitudes(const std::vector<uint64_t> &lhs, const std::vector<uint64_t> &rhs) {
 // initialize sum to longer vector
 std::vector<uint64_t> sum = lhs.size() < rhs.size() ? rhs : lhs;
 std::vector<uint64_t> other = lhs.size() < rhs.size() ? lhs : rhs;

 // loop through each element of the shorter vector
 for (auto i = sum.begin(), j = other.begin(); j != other.end(); i++, j++) {
   // compute sum
   *i += *j;

   // check for overflow
   if (*i < *j) {
     carry_the_one(sum, i + 1);
   }
 }

 return sum;
}

std::vector<uint64_t> BigInt::diff_magnitudes(const std::vector<uint64_t> &lhs, const std::vector<uint64_t> &rhs) {
 // initialize diff to larger magnitude
 std::vector<uint64_t> diff = compare_magnitudes(lhs, rhs) < 0 ? rhs : lhs;
 std::vector<uint64_t> other = compare_magnitudes(lhs, rhs) < 0 ? lhs : rhs;

// loop through each element of the shorter vector
 for (auto i = diff.begin(), j = other.begin(); j != other.end(); i++, j++) {
   if (*i < *j) {
     borrow_the_one(diff, i + 1);
   }
   *i -= *j;
 }

// remove leading zeros
 while (diff.back() == 0 && diff.size() > 1) {
   diff.pop_back();
 }

 return diff;
}

int BigInt::compare_magnitudes(const std::vector<uint64_t> &lhs, const std::vector<uint64_t> &rhs){
  if (lhs.size() != rhs.size()) {
    return lhs.size() < rhs.size() ? -1 : 1;
  } else {
    auto i = lhs.rbegin();
    auto j = rhs.rbegin();

    while (i != lhs.rend()) {
      if (*i != *j) {
        return *i < *j ? -1 : 1;
      }
      i++;
      j++;
    }
    return 0;
  }
}

void BigInt::carry_the_one(std::vector<uint64_t> &v, std::vector<uint64_t>::iterator i) {
 if (i != v.end()) {
   *i += 1;

   // check for overflow
   if (*i < 1) {
     carry_the_one(v, i + 1);
   }
 } else {
   v.push_back(1);
 }
}

void BigInt::borrow_the_one(std::vector<uint64_t> &v, std::vector<uint64_t>::iterator i) {
  if (*i == 0) {
    *i = 0xFFFFFFFFFFFFFFFF;
    borrow_the_one(v, i + 1);
  }
  *i -= 1;
}

void BigInt::right_shift_by_one(std::vector<uint64_t> &v, std::vector<uint64_t>::iterator i) {
  if (i != v.end()) {
    *i = (*i >> 1) + (*(i + 1) << 63);
    right_shift_by_one(v, i + 1);
  }
}

BigInt BigInt::abs() const { // takes absolute value
  BigInt absolute_value = *this;
  absolute_value.negative_val = false;
  return absolute_value;
}