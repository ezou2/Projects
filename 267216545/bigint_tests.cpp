#include <stdexcept>
#include <sstream>
#include <iostream>
#include "bigint.h"
#include "tctest.h"

struct TestObjs {
  BigInt zero;
  BigInt one;
  BigInt two;
  BigInt u64_max;
  BigInt two_pow_64;
  BigInt negative_two_pow_64;
  BigInt three;
  BigInt negative_nine;
  BigInt negative_three;
  BigInt nine;
  BigInt negative_u64_max;

  TestObjs();
};

// prototypes for setup and cleanup functions
TestObjs *setup();
void cleanup(TestObjs *objs);

// Verify that a BigInt contains appropriate data by checking the
// contents of its internal vector of uint64_t values.
// This allows us to validate the contents of a BigInt object
// without needing to rely on member functions other than get_bit_vector().
// Throws std::runtime_error if the actual values don't exactly match
// the expected values.
void check_contents(const BigInt &bigint, std::initializer_list<uint64_t> expected_vals);

// prototypes of test functions
void test_default_ctor(TestObjs *objs);
void test_u64_ctor(TestObjs *objs);
void test_initlist_ctor(TestObjs *objs);
void test_copy_ctor(TestObjs *objs);
void test_get_bits(TestObjs *objs);
void test_add_1(TestObjs *objs);
void test_add_2(TestObjs *objs);
void test_add_3(TestObjs *objs);
void test_add_4(TestObjs *objs);
void test_sub_1(TestObjs *objs);
void test_sub_2(TestObjs *objs);
void test_sub_3(TestObjs *objs);
void test_sub_4(TestObjs *objs);
void test_is_bit_set_1(TestObjs *objs);
void test_is_bit_set_2(TestObjs *objs);
void test_lshift_1(TestObjs *objs);
void test_lshift_2(TestObjs *objs);
void test_mul_1(TestObjs *objs);
void test_mul_2(TestObjs *objs);
void test_compare_1(TestObjs *objs);
void test_compare_2(TestObjs *objs);
void test_div_1(TestObjs *objs);
void test_div_2(TestObjs *objs);
void test_to_hex_1(TestObjs *objs);
void test_to_hex_2(TestObjs *objs);
void test_to_dec_1(TestObjs *objs);
void test_to_dec_2(TestObjs *objs);

// our test functions
void test_add_edge_cases(TestObjs *objs);
void test_compare_edge_cases(TestObjs *objs);
void test_mul_edge_cases(TestObjs *objs);
void test_initlist_ctor_edge_cases(TestObjs *objs);
void test_sub_edge_cases(TestObjs *objs);
void test_is_bit_set_edge_cases(TestObjs *objs);
void test_lshift_edge_cases(TestObjs *objs);
void test_div_edge_cases(TestObjs *objs);
void test_mul_3(TestObjs *objs);
void test_mul_4(TestObjs *objs);
void test_compare_3(TestObjs *objs);
void test_compare_4(TestObjs *objs);
void test_to_hex_3(TestObjs *objs);
void test_to_hex_4(TestObjs *objs);
void test_to_dec_3(TestObjs *objs);
void test_to_dec_4(TestObjs *objs);
void test_multiple_funcs(TestObjs *objs);
void test_div_more_edge_cases(TestObjs *objs);
void test_to_dec_more_edge_cases(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_default_ctor);
  TEST(test_u64_ctor);
  TEST(test_initlist_ctor);
  TEST(test_copy_ctor);
  TEST(test_get_bits);
  TEST(test_add_1);
  TEST(test_add_2);
  TEST(test_add_3);
  TEST(test_add_4);
  TEST(test_sub_1);
  TEST(test_sub_2);
  TEST(test_sub_3);
  TEST(test_sub_4);
  TEST(test_is_bit_set_1);
  TEST(test_is_bit_set_2);
  TEST(test_lshift_1);
  TEST(test_lshift_2);
  TEST(test_mul_1);
  TEST(test_mul_2);
  TEST(test_compare_1);
  TEST(test_compare_2);
  TEST(test_div_1);
  TEST(test_div_2);
  TEST(test_to_hex_1);
  TEST(test_to_hex_2);
  TEST(test_to_dec_1);
  TEST(test_to_dec_2);

  // calls to TEST for our additional test functions
  TEST(test_initlist_ctor_edge_cases);
  TEST(test_add_edge_cases);
  TEST(test_compare_edge_cases);
  TEST(test_mul_edge_cases);
  TEST(test_sub_edge_cases);
  TEST(test_is_bit_set_edge_cases);
  TEST(test_lshift_edge_cases);
  TEST(test_div_edge_cases);
  TEST(test_mul_3);
  TEST(test_mul_4);
  TEST(test_compare_3); 
  TEST(test_compare_4);
  TEST(test_to_hex_3);
  TEST(test_to_hex_4);
  TEST(test_to_dec_3);
  TEST(test_multiple_funcs);
  TEST(test_div_more_edge_cases);
  TEST(test_to_dec_4);
  TEST(test_to_dec_more_edge_cases);

  TEST_FINI();
}

// All initialization of test fixture objects should be done
// in the TestObjs constructor.
TestObjs::TestObjs()
  : zero()
  , one(1UL)
  , two(2UL)
  , u64_max(0xFFFFFFFFFFFFFFFFUL)
  , two_pow_64({ 0UL, 1UL })
  , negative_two_pow_64({ 0UL, 1UL }, true)
  , three(3UL)
  , negative_nine(9UL, true)
  , negative_three(3UL, true)
  , nine(9UL)
  , negative_u64_max(0xFFFFFFFFFFFFFFFFUL, true)
{
}

TestObjs *setup() {
  return new TestObjs;
}

void cleanup(TestObjs *objs) {
  delete objs;
}

void check_contents(const BigInt &bigint, std::initializer_list<uint64_t> expected_vals) {
  const std::vector<uint64_t> &actual_vals = bigint.get_bit_vector();

  auto i = actual_vals.begin();
  auto j = expected_vals.begin();

  unsigned count = 0;

  while (i != actual_vals.end() && j != expected_vals.end()) {
    if (*i != *j) {
      std::stringstream msg;
      msg << "in word " << count
          << " actual value " << std::hex << *i
          << " does not match expected value " << std::hex << *j;
      throw std::runtime_error(msg.str());
    }
    ++i;
    ++j;
    ++count;
  }

  // at this point, if either the expected values or actual values
  // have additional data, all of the additional values need to be 0

  while (i != actual_vals.end()) {
    if (*i != 0UL) {
      std::stringstream msg;
      msg << "in word " << count
          << " actual value " << std::hex << *i
          << " does not match expected value 0";
      throw std::runtime_error(msg.str());
    }
    ++i;
    ++count;
  }

  while (j != expected_vals.end()) {
    if (*j != 0UL) {
      std::stringstream msg;
      msg << "in word " << count
          << " actual value 0 "
          << " does not match expected value " << std::hex << *j;
      throw std::runtime_error(msg.str());
    }
    ++j;
    ++count;
  }
}

void test_default_ctor(TestObjs *objs) {
  check_contents(objs->zero, { 0UL });
  ASSERT(!objs->zero.is_negative());
}

void test_u64_ctor(TestObjs *objs) {
  check_contents(objs->one, { 1UL });
  ASSERT(!objs->one.is_negative());

  check_contents(objs->two, { 2UL });
  ASSERT(!objs->two.is_negative());

  check_contents(objs->u64_max, { 0xFFFFFFFFFFFFFFFFUL });
  ASSERT(!objs->u64_max.is_negative());

  check_contents(objs->three, { 3UL });
  ASSERT(!objs->three.is_negative());

  check_contents(objs->negative_nine, { 9UL });
  ASSERT(objs->negative_nine.is_negative());

  check_contents(objs->negative_three, { 3UL });
  ASSERT(objs->negative_three.is_negative());

  check_contents(objs->nine, { 9UL });
  ASSERT(!objs->nine.is_negative());
}

void test_initlist_ctor(TestObjs *objs) {
  check_contents(objs->two_pow_64, { 0UL, 1UL });
  ASSERT(!objs->two_pow_64.is_negative());

  check_contents(objs->negative_two_pow_64, { 0UL, 1UL });
  ASSERT(objs->negative_two_pow_64.is_negative());
}

void test_initlist_ctor_edge_cases(TestObjs *objs) {
  BigInt result1({});
  check_contents(result1, { 0UL });
  ASSERT(!result1.is_negative());

  BigInt result2({0}, true);
  check_contents(result2, { 0UL });
  ASSERT(!result2.is_negative());

  BigInt result3({1, 0, 0});
  check_contents(result3, { 1UL });
  ASSERT(!result3.is_negative());

  BigInt result4({0, 0, 0});
  check_contents(result4, { 0UL });
  ASSERT(!result4.is_negative());
}

void test_copy_ctor(TestObjs *objs) {
  BigInt copy(objs->two_pow_64);

  check_contents(copy, { 0UL, 1UL });
  ASSERT(!copy.is_negative());
}

void test_get_bits(TestObjs *objs) {
  ASSERT(0UL == objs->zero.get_bits(0));
  ASSERT(0UL == objs->zero.get_bits(1));

  ASSERT(1UL == objs->one.get_bits(0));
  ASSERT(0UL == objs->one.get_bits(1));

  ASSERT(0xFFFFFFFFFFFFFFFFUL == objs->u64_max.get_bits(0));
  ASSERT(0UL == objs->u64_max.get_bits(1));

  ASSERT(0UL == objs->two_pow_64.get_bits(0));
  ASSERT(1UL == objs->two_pow_64.get_bits(1));
}

void test_add_1(TestObjs *objs) {
  // very basic tests for addition

  BigInt result1 = objs->zero + objs->zero;
  check_contents(result1, { 0UL });
  ASSERT(!result1.is_negative());

  BigInt result2 = objs->zero + objs->one;
  check_contents(result2, { 1UL });
  ASSERT(!result2.is_negative());

  BigInt result3 = objs->one + objs->zero;
  check_contents(result3, { 1UL });
  ASSERT(!result3.is_negative());

  BigInt result4 = objs->u64_max + objs->one;
  check_contents(result4, { 0UL, 1UL });
  ASSERT(!result4.is_negative());
}

void test_add_2(TestObjs *) {
  // Harder addition test(s)
  {
    BigInt left({0xc26e6d5f53ab28a0UL, 0xfb12a615f71867UL, 0x1f12c444bbc13UL});
    BigInt right({0x68adaac8bc0c460aUL, 0x2b287106b393UL});
    BigInt result = left + right;
    check_contents(result, {0x2b1c18280fb76eaaUL, 0xfb3dce86fdcbfbUL, 0x1f12c444bbc13UL});
    ASSERT(!result.is_negative());
  }
}

void test_add_3(TestObjs *objs) {
  // Test additions with mixed signs

  BigInt result1 = objs->three + objs->negative_nine;
  check_contents(result1, { 6UL });
  ASSERT(result1.is_negative());

  BigInt result2 = objs->negative_nine + objs->three;
  check_contents(result2, { 6UL });
  ASSERT(result2.is_negative());
}

void test_add_4(TestObjs *) {
  // Harder addition test(s) with negative and nonnegative operands
  {
    BigInt left({0x9f33ca9e0290d102UL, 0xd331662e19562498UL, 0x2347e72885d6e83UL, 0x529bac52c46b047UL, 0x69b3f801ea78f778UL, 0xfdba8845fc024708UL, 0x2UL});
    BigInt right({0x3efacbd8f95c7bbUL, 0x974f58eddfc56dfeUL, 0xa844e13b7c33ebedUL, 0xb4dd001fbdf5f260UL, 0x755c8aee33f48fecUL, 0x60UL}, true);
    BigInt result = left + right;
    check_contents(result, {0x9b441de072fb0947UL, 0x3be20d403990b69aUL, 0x59ef9d370c298296UL, 0x504cbaa56e50bde6UL, 0xf4576d13b684678bUL, 0xfdba8845fc0246a7UL, 0x2UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xd2ec154f6f077e95UL, 0x1bb578fcbf5bec60UL, 0x36735d365fe0f69eUL, 0xa803e03015c610e8UL, 0xca14aac782de50a2UL, 0x2191456abd619da2UL, 0xd9a345c56ba9a659UL, 0x1de31271dc3UL}, true);
    BigInt right({0xe2b272a002612fe3UL, 0xd1ec655e5e2e3d38UL, 0x21bc7b47bae7b899UL, 0xb211826c4e5c346fUL, 0x39d7cf09bfdfe04dUL, 0x1253ec5a1e5b8f9aUL, 0xbabcbf088154c6eeUL, 0x3e676f5f2637b2edUL, 0xcc7edfb347fcd8b4UL, 0x5978260d5ecf00a7UL, 0xe242586be49c40c1UL, 0xf734798ec1dd4ddaUL, 0x76394dUL}, true);
    BigInt result = left + right;
    check_contents(result, {0xb59e87ef7168ae78UL, 0xeda1de5b1d8a2999UL, 0x582fd87e1ac8af37UL, 0x5a15629c64224557UL, 0x3ec79d142be30f0UL, 0x33e531c4dbbd2d3dUL, 0x946004cdecfe6d47UL, 0x3e67713d575ed0b1UL, 0xcc7edfb347fcd8b4UL, 0x5978260d5ecf00a7UL, 0xe242586be49c40c1UL, 0xf734798ec1dd4ddaUL, 0x76394dUL});
    ASSERT(result.is_negative());
  }

}

void test_add_edge_cases(TestObjs *) {
  // edge cases of add including zero/nonzero, negative/nonnegative and large numbers
  // testing lots of carrying the one!

  {
    BigInt left({0}, false);
    BigInt right({0}, false);
    BigInt result = left + right;
    check_contents(result, {0UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0}, false);
    BigInt right({1}, false);
    BigInt result = left + right;
    check_contents(result, {1UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({1}, false);
    BigInt right({0}, false);
    BigInt result = left + right;
    check_contents(result, {1UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({1}, false);
    BigInt right({1}, false);
    BigInt result = left + right;
    check_contents(result, {2UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({1}, false);
    BigInt right({1}, true);
    BigInt result = left + right;
    check_contents(result, {0UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0}, false);
    BigInt right({1}, true);
    BigInt result = left + right;
    check_contents(result, {1UL});
    ASSERT(result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt right({1}, false);
    BigInt result = left + right;
    check_contents(result, {0, 1});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt right({1}, false);
    BigInt result = left + right;
    check_contents(result, {0, 0, 1});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt right({1}, false);
    BigInt result = left + right;
    check_contents(result, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt right({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt result = left + right;
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt right({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt result = left + right;
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt right({1}, false);
    BigInt result = left + right;
    check_contents(result, {0xFFFFFFFFFFFFFFFEUL});
    ASSERT(result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt right({1}, false);
    BigInt result = left + right;
    check_contents(result, {0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL});
    ASSERT(result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt right({1}, false);
    BigInt result = left + right;
    check_contents(result, {0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL});
    ASSERT(result.is_negative());
  }
}

void test_sub_1(TestObjs *objs) {
  // very basic tests for subtraction

  BigInt result1 = objs->zero - objs->zero;
  check_contents(result1, { 0UL });
  ASSERT(!result1.is_negative());

  BigInt result2 = objs->one - objs->zero;
  check_contents(result2, { 1UL });
  ASSERT(!result2.is_negative());

  // 0 - 1 should be -1
  BigInt result3 = objs->zero - objs->one;
  check_contents(result3, { 1UL });
  ASSERT(result3.is_negative());
}

void test_sub_2(TestObjs *) {
  // Harder test(s) for subtraction (with only nonnegative operands)
  {
    BigInt left({0x6eda4eff94cfbae7UL, 0x7c8c43da635c4344UL, 0x20cdb213d5541982UL, 0xbabfefb2192bd25fUL, 0x101fc2c8776f1e13UL, 0x91ecffb0a16ed1dfUL, 0x6f8e57b78c2b4126UL, 0x129a78f1cd7e3ce3UL, 0x64cb2c9c277276bUL, 0xd3e06abb451c73b8UL, 0xe9cc5fd7751987e1UL, 0x1a2d858e65ab0f23UL, 0x5cdd7cbf9bbb3853UL, 0xaac5e94bUL});
    BigInt right({0x555ecb5be6a2352UL, 0x6ac021fcd678e7bdUL, 0xb2c36b0123d8adefUL, 0xda6c10c4b0UL});
    BigInt result = left - right;
    check_contents(result, {0x69846249d6659795UL, 0x11cc21dd8ce35b87UL, 0x6e0a4712b17b6b93UL, 0xbabfeed7ad1b0daeUL, 0x101fc2c8776f1e13UL, 0x91ecffb0a16ed1dfUL, 0x6f8e57b78c2b4126UL, 0x129a78f1cd7e3ce3UL, 0x64cb2c9c277276bUL, 0xd3e06abb451c73b8UL, 0xe9cc5fd7751987e1UL, 0x1a2d858e65ab0f23UL, 0x5cdd7cbf9bbb3853UL, 0xaac5e94bUL});
    ASSERT(!result.is_negative());
  }
}

void test_sub_3(TestObjs *objs) {
  // Some mixed-sign subtraction tests

  BigInt result1 = objs->negative_three - objs->one;
  check_contents(result1, { 4UL });
  ASSERT(result1.is_negative());

  BigInt result2 = objs->one - objs->negative_three;
  check_contents(result2, { 4UL });
  ASSERT(!result2.is_negative());
}

void test_sub_4(TestObjs *) {
  // harder subtraction test(s) involving various
  // combinations of non-negative and negative operands

  {
    BigInt left({0xda069f1394210a58UL, 0x17aeeda0ba999cc9UL, 0x76d8613d4dfa6239UL, 0x2c20a77d76bb2b7dUL, 0xa852e8c7965c99cbUL, 0x2da25d942abf02ddUL, 0x8c7166619290106eUL, 0x72115b9db363aa85UL, 0xc3dcfa489c903019UL, 0x7f69b50321b7f3d5UL, 0x72ba467bb329ff89UL, 0x20a41c2UL}, true);
    BigInt right({0x25e9179995f954a7UL, 0x803da91345e6d0f3UL, 0x509d92c5fd3b8895UL, 0x2d61fc2da4c3962dUL, 0x1fe8e2e2223b1717UL, 0xe7e8724f74e51f4aUL, 0xe138b44b919a3926UL, 0xab9971d99b12be24UL, 0x4f39add59d016768UL, 0xe16b13e35c0e108cUL, 0xa6ef8ef9ec844222UL, 0xe78df73060cc35eeUL, 0x14fUL}, true);
    BigInt result = left - right;
    check_contents(result, {0x4be2788601d84a4fUL, 0x688ebb728b4d3429UL, 0xd9c53188af41265cUL, 0x14154b02e086aafUL, 0x7795fa1a8bde7d4cUL, 0xba4614bb4a261c6cUL, 0x54c74de9ff0a28b8UL, 0x3988163be7af139fUL, 0x8b5cb38d0071374fUL, 0x62015ee03a561cb6UL, 0x3435487e395a4299UL, 0xe78df7305ec1f42cUL, 0x14fUL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0x2731e5530045218aUL, 0x1c29aaUL});
    BigInt right({0xc2baed5ea8cf3dd0UL, 0x81fe5f9adde938d6UL, 0x56ec433719861074UL, 0x7f6d01dbc70c1274UL, 0xd02fd7f2403ee362UL, 0x24c5400e94347fb3UL, 0x764521caebba2c9UL, 0x51a61e0aa376ff6dUL, 0x103c5aa88eb6e90UL}, true);
    BigInt result = left - right;
    check_contents(result, {0xe9ecd2b1a9145f5aUL, 0x81fe5f9ade056280UL, 0x56ec433719861074UL, 0x7f6d01dbc70c1274UL, 0xd02fd7f2403ee362UL, 0x24c5400e94347fb3UL, 0x764521caebba2c9UL, 0x51a61e0aa376ff6dUL, 0x103c5aa88eb6e90UL});
    ASSERT(!result.is_negative());
  }
}

void test_sub_edge_cases(TestObjs *) {
  // edge cases of add including zero/nonzero, negative/nonnegative and large numbers
  // testing lots of carrying the one!

  {
    BigInt left({0}, false);
    BigInt right({0}, true);
    BigInt result = left - right;
    check_contents(result, {0UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0}, false);
    BigInt right({1}, true);
    BigInt result = left - right;
    check_contents(result, {1UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({1}, false);
    BigInt right({0}, true);
    BigInt result = left - right;
    check_contents(result, {1UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({1}, false);
    BigInt right({1}, true);
    BigInt result = left - right;
    check_contents(result, {2UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({1}, false);
    BigInt right({1}, false);
    BigInt result = left - right;
    check_contents(result, {0UL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0}, false);
    BigInt right({1}, false);
    BigInt result = left - right;
    check_contents(result, {1UL});
    ASSERT(result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt right({1}, true);
    BigInt result = left - right;
    check_contents(result, {0, 1});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt right({1}, true);
    BigInt result = left - right;
    check_contents(result, {0, 0, 1});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt right({1}, true);
    BigInt result = left - right;
    check_contents(result, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt right({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, false);
    BigInt result = left - right;
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt right({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt result = left - right;
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt right({1}, true);
    BigInt result = left - right;
    check_contents(result, {0xFFFFFFFFFFFFFFFEUL});
    ASSERT(result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt right({1}, true);
    BigInt result = left - right;
    check_contents(result, {0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL});
    ASSERT(result.is_negative());
  }

  {
    BigInt left({0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL}, true);
    BigInt right({1}, true);
    BigInt result = left - right;
    check_contents(result, {0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL});
    ASSERT(result.is_negative());
  }
}

void test_is_bit_set_1(TestObjs *objs) {
  // some basic tests for is_bit_set

  ASSERT(!objs->zero.is_bit_set(0));
  ASSERT(!objs->zero.is_bit_set(1));

  // it should be ok to inquire about any bit position, even if it's beyond
  // the extent of the internal bitstring representation
  ASSERT(!objs->zero.is_bit_set(1000000));

  ASSERT(objs->one.is_bit_set(0));
  ASSERT(!objs->one.is_bit_set(1));

  ASSERT(!objs->two_pow_64.is_bit_set(63));
  ASSERT(objs->two_pow_64.is_bit_set(64));
  ASSERT(!objs->two_pow_64.is_bit_set(65));
}

void test_is_bit_set_2(TestObjs *) {
  // test(s) for is_bit_set: these test a random
  // sampling of bits in various large-ish BigInt values

  {
    BigInt val({0xad77cbed0273e33UL, 0xe151b7c18231a194UL, 0x7b2beb2888d66093UL, 0xeab20633a9a9595aUL});
    ASSERT(val.is_bit_set(0));
    ASSERT(val.is_bit_set(4));
    ASSERT(val.is_bit_set(9));
    ASSERT(!val.is_bit_set(15));
    ASSERT(!val.is_bit_set(24));
    ASSERT(val.is_bit_set(30));
    ASSERT(val.is_bit_set(42));
    ASSERT(val.is_bit_set(52));
    ASSERT(!val.is_bit_set(58));
    ASSERT(val.is_bit_set(89));
    ASSERT(val.is_bit_set(109));
    ASSERT(!val.is_bit_set(158));
    ASSERT(val.is_bit_set(177));
    ASSERT(val.is_bit_set(184));
    ASSERT(!val.is_bit_set(202));
    ASSERT(val.is_bit_set(241));
  }
}

void test_is_bit_set_edge_cases(TestObjs *) {
  // testing if bits are set in zero, negative or small or large values

  {
    BigInt val({});
    ASSERT(!val.is_bit_set(-1));
    ASSERT(!val.is_bit_set(-2));
    ASSERT(!val.is_bit_set(0));
    ASSERT(!val.is_bit_set(3));
    ASSERT(!val.is_bit_set(2));
    ASSERT(!val.is_bit_set(65));
    ASSERT(!val.is_bit_set(64));
    ASSERT(!val.is_bit_set(63));
    ASSERT(!val.is_bit_set(100));
    ASSERT(!val.is_bit_set(200));
  }
  {
    BigInt val(1);
    ASSERT(!val.is_bit_set(-1));
    ASSERT(!val.is_bit_set(-2));
    ASSERT(val.is_bit_set(0));
    ASSERT(!val.is_bit_set(3));
    ASSERT(!val.is_bit_set(2));
    ASSERT(!val.is_bit_set(65));
    ASSERT(!val.is_bit_set(64));
    ASSERT(!val.is_bit_set(63));
    ASSERT(!val.is_bit_set(100));
    ASSERT(!val.is_bit_set(200));
  }
  
  {
    BigInt val(0xFFFFFFFFFFFFFFFFUL);
    ASSERT(!val.is_bit_set(-1));
    ASSERT(!val.is_bit_set(-2));
    ASSERT(val.is_bit_set(0));
    ASSERT(val.is_bit_set(3));
    ASSERT(val.is_bit_set(2));
    ASSERT(!val.is_bit_set(65));
    ASSERT(!val.is_bit_set(64));
    ASSERT(val.is_bit_set(63));
    ASSERT(!val.is_bit_set(100));
    ASSERT(!val.is_bit_set(200));
  }
}

void test_lshift_1(TestObjs *objs) {
  // Some very basic left shift tests

  BigInt result1 = objs->one << 1;
  check_contents(result1, { 2UL });
  ASSERT(!result1.is_negative());

  BigInt result2 = objs->one << 63;
  check_contents(result2, { 0x8000000000000000UL });
  ASSERT(!result2.is_negative());

  BigInt result3 = objs->one << 64;
  check_contents(result3, { 0UL, 1UL });
  ASSERT(!result3.is_negative());

  BigInt result4 = objs->three << 63;
  check_contents(result4, { 0x8000000000000000UL, 1UL });
  ASSERT(!result4.is_negative());

  // make sure that a std::invalid_argument exception is
  // thrown if there is an attempt to left shift a
  // negative value
  try {
    objs->negative_nine << 42;
    FAIL("left shifting a negative value should throw an exception");
  } catch (std::invalid_argument &ex) {
    // good
  }
}

void test_lshift_2(TestObjs *) {
  // left shift test(s) on large-ish values

  {
    BigInt val({0xbcc523fa26450fc2UL, 0x5490bb4c35ae6c03UL, 0x310a4f3349801bbeUL});
    BigInt result = val << 390;
    check_contents(result, {0x0UL, 0x0UL, 0x0UL, 0x0UL, 0x0UL, 0x0UL, 0x3148fe899143f080UL, 0x242ed30d6b9b00efUL, 0x4293ccd26006ef95UL, 0xcUL});
    ASSERT(!result.is_negative());
  }

  {
    BigInt val({0x7ca1c82cd5678c64UL, 0x24c995d549d6cbe8UL, 0x55df71ecab97c375UL, 0x9523341dc8fd0196UL, 0xf9c1dd3486f16b31UL, 0x7fe83598f38b19d1UL, 0x3b77ae13ce121dUL});
    BigInt result = val << 444;
    check_contents(result, {0x0UL, 0x0UL, 0x0UL, 0x0UL, 0x0UL, 0x0UL, 0x4000000000000000UL, 0x87ca1c82cd5678c6UL, 0x524c995d549d6cbeUL, 0x655df71ecab97c37UL, 0x19523341dc8fd019UL, 0x1f9c1dd3486f16b3UL, 0xd7fe83598f38b19dUL, 0x3b77ae13ce121UL});
    ASSERT(!result.is_negative());
  }
}

void test_lshift_edge_cases(TestObjs *objs) {
  // test left shift on edge cases with negative/nonnegative, zero/nonzero and small/big values
  {
    BigInt val({0});
    BigInt result = val << 0;
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  // Some more basic left shift tests
  BigInt result1 = objs->one << 65;
  check_contents(result1, {0UL, 2UL});
  ASSERT(!result1.is_negative());

  BigInt result2 = objs->one << 0;
  check_contents(result2, { 1UL });
  ASSERT(!result2.is_negative());

  BigInt result3 = objs->nine << 64;
  check_contents(result3, { 0UL, 9 });
  ASSERT(!result3.is_negative());

  BigInt result4 = objs->u64_max << 64;
  check_contents(result4, { 0UL, 0xFFFFFFFFFFFFFFFFUL });
  ASSERT(!result4.is_negative());

  // test on largish value
  {
    BigInt val({0x0000000000000159});
    BigInt result = val << 10;
    check_contents(result, {0x0000000000056400});
    ASSERT(!result.is_negative());
  }
}

void test_mul_1(TestObjs *objs) {
  // some very basic multiplication tests

  BigInt result1 = objs->three * objs->three;
  check_contents(result1, { 9UL });
  ASSERT(!result1.is_negative());

  BigInt result2 = objs->two_pow_64 * objs->two;
  check_contents(result2, { 0UL, 2UL });
  ASSERT(!result2.is_negative());

  BigInt result3 = objs->u64_max * objs->u64_max;
  check_contents(result3, { 0x0000000000000001UL, 0xFFFFFFFFFFFFFFFEUL });
  ASSERT(!result3.is_negative());
}

void test_mul_2(TestObjs *) {
  // multiplication test(s) with larger values

  {
    BigInt left({0x63a2caa5324b539dUL, 0x176fde8a24cUL});
    BigInt right({0x7fd944a0481a66c6UL});
    BigInt result = left * right;
    check_contents(result, {0x2bf1cf198f85396eUL, 0x92c5b43447ed673fUL, 0xbb463828efUL});
    ASSERT(!result.is_negative());
  }
}

void test_mul_edge_cases(TestObjs *) {
  // edge cases of add including zero/nonzero, negative/nonnegative and large numbers
  BigInt result1 = BigInt({1}, true) * BigInt({1}, false);
  check_contents(result1, {1});
  ASSERT(result1.is_negative());

  BigInt result2 = BigInt({1}, true) * BigInt({1}, true);
  check_contents(result2, {1});
  ASSERT(!result2.is_negative());

  BigInt result3 = BigInt({1}, true) * BigInt({0}, false);
  check_contents(result3, {0});
  ASSERT(!result3.is_negative());

  BigInt result4 = BigInt({0}, false) * BigInt({1}, true);
  check_contents(result4, {0});
  ASSERT(!result4.is_negative());

  {
    BigInt left({0, 1, 0, 1}, true);
    BigInt right({1, 1}, true);
    BigInt result = (left * right) - BigInt({0, 1, 1, 1, 1});
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }
}

void test_mul_3(TestObjs *objs) {
  // more basic multiplication tests

  BigInt result1 = objs->two * objs->one; //multiply by 1
  check_contents(result1, { 2 });
  ASSERT(!result1.is_negative());

  BigInt result2 = objs->negative_two_pow_64 * objs->two;
  check_contents(result2, { 0UL, 2UL });
  ASSERT(result2.is_negative());

  BigInt result3 = objs->u64_max * objs->negative_u64_max; //positive*negative
  check_contents(result3, { 0x0000000000000001UL, 0xFFFFFFFFFFFFFFFEUL });
  ASSERT(result3.is_negative());

  BigInt result4 = objs->u64_max * objs->zero; //positive*0
  check_contents(result4, { 0UL });
  ASSERT(!result4.is_negative());

  BigInt result5 = objs->negative_u64_max * objs->zero; //negative * 0
  check_contents(result5, { 0UL });
  ASSERT(!result5.is_negative());

  BigInt result6 = objs->negative_u64_max * objs->negative_u64_max; //negative * negative
  check_contents(result6, { 0x0000000000000001UL, 0xFFFFFFFFFFFFFFFEUL });
  ASSERT(!result6.is_negative());
}

void test_compare_1(TestObjs *objs) {
  // some basic tests for compare
  ASSERT(objs->zero.compare(objs->zero) == 0);
  ASSERT(objs->zero.compare(objs->one) < 0);
  ASSERT(objs->one.compare(objs->zero) > 0);
  ASSERT(objs->negative_nine.compare(objs->three) < 0);
  ASSERT(objs->three.compare(objs->negative_three) > 0);
}

void test_compare_2(TestObjs *) {
  // test(s) for compare with larger values (both negative and
  // non-negative)

  {
    BigInt left({0xcedf54d4ce62e836UL, 0x80bcc908ac2470e5UL, 0x7f5530530656407UL, 0xd708e6b15caaffe6UL, 0xea4413128f8d85dfUL, 0x8761bb0238eef891UL, 0x112046064d41670bUL, 0x52e17af98118272fUL, 0x2bcb5a93df6b9791UL, 0xe4UL}, true);
    BigInt right({0xa4771be9296f10c9UL, 0x99652558ce608a70UL, 0x72a5a8d90da8a461UL, 0xae81eebacbc14061UL, 0xbe8ee1d7d75975c9UL, 0xef670eb972588d3aUL, 0x73c02c5138c02430UL, 0x6ed794ae35fd9f68UL, 0x4dd6b9ebe076ac72UL, 0x42b6daa3d05d11c7UL, 0x2cfe1d5965b49e99UL, 0x927becUL});
    int result = left.compare(right);
    ASSERT(result < 0);
  }
}

void test_compare_edge_cases(TestObjs *objs) {
  // test if both have large magnitude
  {
    BigInt left_1({~0UL, ~0UL}, false);
    BigInt right_1({~0UL, 1UL}, false);
    int result_1 = left_1.compare(right_1);
    ASSERT(result_1 > 0);

    BigInt left_2({0UL, ~0UL, ~0UL}, false);
    BigInt right_2({1UL, ~0UL, ~0UL}, false);
    int result_2 = left_2.compare(right_2);
    ASSERT(result_2 < 0);

    {
      BigInt left({0, 1, 1, 1, 1}, false);
      BigInt right({1, 1, 1, 1, 1}, false);
      ASSERT(left.compare(right) < 0);
    }

    // special case where overflow in lhs.back() - rhs.back() does not reflect true comparison
    {
      BigInt left({0, 1}, false);
      BigInt right({0, 2}, false);
      ASSERT(left.compare(right) < 0);
    }
  }
}

void test_div_1(TestObjs *objs) {
  // Some relatively basic division tests

  BigInt result1 = objs->nine / objs->three;
  check_contents(result1, { 3UL });
  ASSERT(!result1.is_negative());

  BigInt result2 = objs->nine / objs->two;
  check_contents(result2, { 4UL });
  ASSERT(!result2.is_negative());

  BigInt result3 = objs->negative_nine / objs->three;
  check_contents(result3, { 3UL });
  ASSERT(result3.is_negative());

  BigInt result4 = objs->negative_nine / objs->two;
  check_contents(result4, { 4UL });
  ASSERT(result4.is_negative());
}

void test_div_2(TestObjs *) {
  // harder division test(s) with larger values

  {
    BigInt left({0x5a1f7b06e95d205bUL, 0x16bef383084c9bf5UL, 0x6bfd5cb9a0cfa403UL, 0xbb47e519c0ffc392UL, 0xc8c47a8ab9cc20afUL, 0x30302fb07ef81d25UL, 0x8b8bcb6df3f72911UL, 0x3de679169dc89703UL, 0x48f52b428f255e1dUL, 0xd623c2e8a460f5beUL, 0xae2df81a84808054UL, 0xcfb038910d158d63UL, 0xcf97bc9UL});
    BigInt right({0xe1d191b09fd571e7UL, 0xd6e34973337d88fdUL, 0x7235628c33211b03UL, 0xe0bbc74b5d7fe26aUL, 0xf6242ed96eb2c8d9UL, 0x3b0cad8e5dd18f5UL, 0x558c283a839910c0UL, 0xbb4df9de72952652UL, 0xed8b519e3c63ce56UL, 0xe96f9c8454bde1c4UL, 0x76b62db592951f97UL, 0x577341UL});
    BigInt result = left / right;
    check_contents(result, {0xfb3e6b02be39b6ceUL, 0x25UL});
    ASSERT(!result.is_negative());
  }
}

void test_div_edge_cases(TestObjs *) {
  // make sure division by zero throws invalid argument exception
  try {
    BigInt(1) / BigInt(0);
    FAIL("dividing by zero should throw an exception");
  } catch (std::invalid_argument &ex) {
    // good
  }

  // make sure division by zero throws invalid argument exception
  try {
    BigInt(0) / BigInt(0);
    FAIL("dividing by zero should throw an exception");
  } catch (std::invalid_argument &ex) {
    // good
  }

  {
    BigInt left(0);
    BigInt right(1);
    BigInt result = left / right;
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left(1);
    BigInt right(1);
    BigInt result = left / right;
    check_contents(result, {1});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left(1, true);
    BigInt right(1);
    BigInt result = left / right;
    check_contents(result, {1});
    ASSERT(result.is_negative());
  }

  {
    BigInt left(1);
    BigInt right(1, true);
    BigInt result = left / right;
    check_contents(result, {1});
    ASSERT(result.is_negative());
  }

  {
    BigInt left(1, true);
    BigInt right(1, true);
    BigInt result = left / right;
    check_contents(result, {1});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left(2, true);
    BigInt right(1, true);
    BigInt result = left / right;
    check_contents(result, {2});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left(100, true);
    BigInt right(1, true);
    BigInt result = left / right;
    check_contents(result, {100});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left(100, true);
    BigInt right(10, true);
    BigInt result = left / right;
    check_contents(result, {10});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left(9, true);
    BigInt right(10, true);
    BigInt result = left / right;
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0, 1}, true);
    BigInt right({0, 1}, true);
    BigInt result = left / right;
    check_contents(result, {1});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0}, true);
    BigInt right({0, 1}, true);
    BigInt result = left / right;
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0, 1, 1, 1, 1}, true);
    BigInt right({1, 1}, true);
    BigInt result = (left / right) - BigInt({0, 1, 0, 1});
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  {
    BigInt left({0, 1, 1, 1, 1}, true);
    BigInt right({1, 1}, true);
    BigInt result = (left / right) - BigInt({0, 1, 0, 1});
    check_contents(result, {0});
    ASSERT(!result.is_negative());
  }

  // test division related to to_dec
  u_int64_t ten_pow_19 = 0x8AC7230489E80000UL;

  {
    BigInt val({0x00000000022e1abcUL, 0x0000000884b280daUL, 0x2ff8fb5c4117322aUL});
    BigInt quotient = val / ten_pow_19;
    BigInt remainder = val - quotient * ten_pow_19;
    ASSERT(val == quotient * ten_pow_19 + remainder);
  }
}

void test_div_more_edge_cases(TestObjs *) {
  {
    BigInt dividend({0x87104D1121E33A11, 0x587E69B81086D57A});
    BigInt base({0x8AC7230489E80000UL});
    BigInt quotient = dividend / base;
    BigInt remainder = dividend - quotient * base;
    ASSERT(dividend == quotient * base + remainder);
  }
}

void test_to_dec_more_edge_cases(TestObjs *) {
  {
    BigInt val({1});
    ASSERT("1" == val.to_dec());
  }

  {
    BigInt val({1}, true);
    ASSERT("-1" == val.to_dec());
  }

  {
    BigInt val({1, 1});
    ASSERT("18446744073709551617" == val.to_dec());
  }

  {
    BigInt val({1, 1}, true);
    ASSERT("-18446744073709551617" == val.to_dec());
  }

  {
    BigInt val({1, 1, 1});
    ASSERT("340282366920938463481821351505477763073" == val.to_dec());
  }

  {
    BigInt val({1, 1, 1, 1});
    ASSERT("6277101735386680764176071790128604879584176795969512275969" == val.to_dec());
  }

  {
    BigInt val({1, 1, 1, 1, 1});
    ASSERT("115792089237316195429848086744074588617446056455769168919041760803882641915905" == val.to_dec());
  }

  {
    BigInt val({1, 1, 1, 1, 1, 1});
    ASSERT("2135987035920910082510813795406868310032552609100727358564487664277988894699682310826844728852481" == val.to_dec());
  }
}

void test_to_hex_1(TestObjs *objs) {
  // some basic tests for to_hex()

  std::string result1 = objs->zero.to_hex();
  ASSERT("0" == result1);

  std::string result2 = objs->negative_nine.to_hex();
  ASSERT("-9" == result2);

  std::string result3 = objs->u64_max.to_hex();
  ASSERT("ffffffffffffffff" == result3);

  std::string result4 = objs->two_pow_64.to_hex();
  ASSERT("10000000000000000" == result4);
}

void test_to_hex_2(TestObjs *) {
  // test(s) for to_hex() on larger values

  {
    BigInt val({0xd8b5422df2c7e5d4UL, 0x2186595636ed41d7UL, 0xcf498dc4c634eb41UL, 0xa6579a3f9d2aab0cUL, 0xb5cbefaf0e63a6e3UL, 0xf419b0aadf4d14f1UL, 0xcec650d523acc64eUL, 0x14318cf757a58UL});
    std::string result = val.to_hex();
    ASSERT("14318cf757a58cec650d523acc64ef419b0aadf4d14f1b5cbefaf0e63a6e3a6579a3f9d2aab0ccf498dc4c634eb412186595636ed41d7d8b5422df2c7e5d4" == result);
  }

  {
    BigInt val({0xd8b5422df2c7e5d4UL, 0x2186595636ed41d7UL, 0xcf498dc4c634eb41UL, 0xa6579a3f9d2aab0cUL, 0xb5cbefaf0e63a6e3UL, 0xf419b0aadf4d14f1UL, 0xcec650d523acc64eUL, 0x14318cf757a58UL}, true);
    std::string result = val.to_hex();
    ASSERT("-14318cf757a58cec650d523acc64ef419b0aadf4d14f1b5cbefaf0e63a6e3a6579a3f9d2aab0ccf498dc4c634eb412186595636ed41d7d8b5422df2c7e5d4" == result);
  }

}

void test_to_dec_1(TestObjs *objs) {
  // some basic tests for to_dec()

  std::string result0 = objs->one.to_dec();
  ASSERT("1" == result0);

  std::string result1 = objs->zero.to_dec();
  ASSERT("0" == result1);

  std::string result2 = objs->negative_nine.to_dec();
  ASSERT("-9" == result2);

  std::string result3 = objs->u64_max.to_dec();
  ASSERT("18446744073709551615" == result3);
}

void test_to_dec_2(TestObjs *) {
  // harder test(s) for to_dec()

  {
    BigInt val({0x361adeb15b6962c7UL, 0x31a5b3c012d2a685UL, 0x7b3b4839UL});
    std::string result = val.to_dec();
    ASSERT("703527900324720116021349050368162523567079645895" == result);
  }
}

void test_mul_4(TestObjs *) {
  // multiplication test(s) with larger values
  {
    //(+)*(-)
    BigInt left({0x0000000000039447, 0x0000043C9DD84E41, 0x0000000002125928});
    BigInt right({0x0000D55AFADFE0C6, 0x00000324D8BB8F3F}, true);
    BigInt result = left * right;
    check_contents(result, {0xFBA4B5E4A45BCEEA, 0x69135B6836F68CC1, 0xFBD3D531130076BA, 0x83610169A2969C87, 6});
    ASSERT(result.is_negative());
    
  }
  {
    //(-)*(-)
    BigInt left({0x0000000000039447, 0x0000043C9DD84E41, 0x0000000002125928}, true);
    BigInt right({0x0000D55AFADFE0C6, 0x00000324D8BB8F3F}, true);
    BigInt result = left * right;
    check_contents(result, {0xFBA4B5E4A45BCEEA, 0x69135B6836F68CC1, 0xFBD3D531130076BA, 0x83610169A2969C87, 6});
    ASSERT(!result.is_negative());
    
  }
  {
    //(-)*(+)
    BigInt left({0x0000000000039447, 0x0000043C9DD84E41, 0x0000000002125928}, true);
    BigInt right({0x0000D55AFADFE0C6, 0x00000324D8BB8F3F});
    BigInt result = left * right;
    check_contents(result, {0xFBA4B5E4A45BCEEA, 0x69135B6836F68CC1, 0xFBD3D531130076BA, 0x83610169A2969C87, 6});
    ASSERT(result.is_negative());
    
  }

  {
    //(-)*(0)
    BigInt left({0x0000000000039447, 0x0000043C9DD84E41, 0x0000000002125928}, true);
    BigInt right({0UL});
    BigInt result = left * right;
    check_contents(result, {0});
    ASSERT(!result.is_negative());
    
  }
  
}

void test_compare_3(TestObjs *objs) {
  // more basic tests for compare
  ASSERT(objs->u64_max.compare(objs->u64_max) == 0);
  ASSERT(objs->negative_u64_max.compare(objs->u64_max) < 0); //(-)<(+)
  ASSERT(objs->negative_three.compare(objs->negative_nine) > 0); //(-)>(-)
  ASSERT(objs->two.compare(objs->two_pow_64) < 0); //(+)<(+)
}

void test_compare_4(TestObjs *) {
  {
    //(+)vs(+)
    BigInt left({0x0000000000039447, 0x0000043C9DD84E41, 0x0000000002125928});
    BigInt right({0x0000D55AFADFE0C6, 0x00000324D8BB8F3F});
     int result = left.compare(right);
    ASSERT(result > 0);
  }
  {
    //(-)vs(-)
    BigInt left({0x0000000000039447, 0x0000043C9DD84E41, 0x0000000002125928}, true);
    BigInt right({0x0000D55AFADFE0C6, 0x00000324D8BB8F3F}, true);
     int result = left.compare(right);
    ASSERT(result < 0);
  }
    {
    //(+)vs(-)
    BigInt left({0x0000000000039447, 0x0000043C9DD84E41, 0x0000000002125928});
    BigInt right({0x0000D55AFADFE0C6, 0x00000324D8BB8F3F}, true);
     int result = left.compare(right);
    ASSERT(result > 0);
  }
  
}

void test_to_hex_3(TestObjs *objs){
  //more simple numbers
  {
    std::string result1 = objs -> one.to_hex();
    ASSERT("1" == result1);
  }
   {
    std::string result2 = objs -> two.to_hex();
    ASSERT("2" == result2);
    
  }
  {
    std::string result3 = objs -> three.to_hex();
    ASSERT("3" == result3);
  }
  {
    std::string result4 = objs -> negative_three.to_hex();
    ASSERT("-3" == result4);
  }
  {
    std::string result5 = objs -> nine.to_hex();
    ASSERT("9" == result5);
  }
  {
    std::string result6 = objs -> negative_two_pow_64.to_hex();
    ASSERT("-10000000000000000" == result6);
  }
}

void test_to_hex_4(TestObjs *) {
  //longer to hex
  {
    BigInt val({1, 2, 3, 4});
    std::string result = val.to_hex();
    ASSERT("4000000000000000300000000000000020000000000000001" == result);
  }

  {
    BigInt val({32, 38, 17}, true);
    std::string result = val.to_hex();
    ASSERT("-1100000000000000260000000000000020" == result);
  }

  {
    BigInt val({0, 0, 0, 0}, true);
    std::string result = val.to_hex();
    ASSERT("0" == result);
  }

}

void test_to_dec_3(TestObjs *objs) {
  // more basic tests for to_dec()

  std::string result1 = objs->one.to_dec();
  ASSERT("1" == result1);

  std::string result2 = objs->two.to_dec();
  ASSERT("2" == result2);

  std::string result3 = objs->three.to_dec();
  ASSERT("3" == result3);

  std::string result4 = objs->negative_three.to_dec();
  ASSERT("-3" == result4);

  std::string result5 = objs->nine.to_dec();
  ASSERT("9" == result5);
}

void test_to_dec_4(TestObjs *) {
  // harder test(s) for to_dec()

  {
    BigInt val({0x00000000022e1abc, 0x0000000884b280da, 0x2ff8fb5c4117322a});
    std::string result = val.to_dec();
    ASSERT("1176284372693530354880283686823528514731703051961139862204" == result);
  }
  {
    BigInt val({0x00000000022e1abc, 0x0000000884b280da, 0x2ff8fb5c4117322a}, true);
    std::string result = val.to_dec();
    ASSERT("-1176284372693530354880283686823528514731703051961139862204" == result);
  }
}

void test_multiple_funcs(TestObjs *objs){
  {
    //addition and division
    BigInt result1 = (objs->nine + objs->three)/(objs->one + objs->two);
    check_contents(result1, {4});
    ASSERT(!result1.is_negative());
  }

  {
    //negative addition and division
    BigInt result2 = (objs->negative_nine + objs->three)/(objs->one + objs->two);
    check_contents(result2, {2});
    ASSERT(result2.is_negative());
  }

  {
    //multiplication and division
    BigInt result3 = (objs->nine * objs->two)/(objs->one * objs->two);
    check_contents(result3, {9});
    ASSERT(!result3.is_negative());
  }
  {
    //negative multiplication and division
    BigInt result4 = (objs->negative_nine * objs->two)/(objs->one * objs->two);
    check_contents(result4, {9});
    ASSERT(result4.is_negative());
  }

  {
    //multiple multiplication
    BigInt result4 = (objs->three * objs->two)*(objs->one * objs->two);
    check_contents(result4, {12});
    ASSERT(!result4.is_negative());
  }
  {
    //negative multiple multiplication
    BigInt result5 = (objs->negative_three * objs->two)*(objs->one * objs->two);
    check_contents(result5, {12});
    ASSERT(result5.is_negative());
  }
  {
    //multiplication divide by one
    BigInt result6 = (objs->three * objs->two)/(objs->one);
    check_contents(result6, {6});
    ASSERT(!result6.is_negative());
  }
  {
    //multiplication, divide, shift
    BigInt result7 = (objs->three * objs->two)/(objs->one);
    result7 = result7 << 1;
    check_contents(result7, {12});
    ASSERT(!result7.is_negative());
  }
  //multiplication, divide, compare
  //   lhs = (objs->three * objs->two)/(objs->negative_three - objs->two);
  //   rhs = (objs->three * objs->two)/(objs->one);
   {
    ASSERT(((objs->three * objs->two)/(objs->negative_three - objs->two)).compare((objs->three * objs->two)/(objs->one))<0);
  }

}
