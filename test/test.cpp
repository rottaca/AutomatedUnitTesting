// AutomatedUnitTesting.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include <assert.h>
#include <type_traits>

#include <gtest/gtest.h>

#include "constraints.hpp"
#include "constraint_combiner.hpp"
#include "evaluation.hpp"
#include "testgenerator.hpp"
#include "helper.hpp"


#include <vector>

aut::greater<0, int> fib(aut::greater<0, int> n) {
	if (n <= 0) return 0;
	if (n == 1) return 1;

	int f1 = 1;
	int f2 = 1;
	for (int i = 2; i < n; ++i)
	{
		int fneu = f2 + f1;
		f1 = f2;
		f2 = fneu;
	}
	return f2;
}

aut::greater<0.f, float> myFunc(aut::greater<0.f, float> n, aut::in_range<-10.f, 10.f, float> m) {
	std::cout << "n: " << n << std::endl;
	std::cout << "m: " << m << std::endl;

	return n * m;
}

aut::greater<0.f, float> myFunc2(
	aut::greater<0.f, float> n,
	aut::in_range<-10.f, 10.f, float> m,
	aut::one_of<1, 2, -1, 3> o) {
	return n * m * static_cast<float>(o);
}

float myFunc2_unconstrained(float n, float m, float o) {
	return n * m * static_cast<float>(o);
}

void BubbleSort(std::vector<aut::in_range<0, 100, int>>& arr)
{
	const size_t n = arr.size();
	for (int i = 0; i < n - 1; ++i)
	{
		bool swapped = false;
		for (int j = 0; j < n - i - 1; ++j)
		{
			if (arr[j] > arr[j + 1]) //check if adjacent element is
						 //not in order
			{
				std::swap(arr[j], arr[j + 1]);
				swapped = true;
			}
		}
		// Value at n-i-1 will be maximum of all the values below this index.
		if (!swapped)
			break;
	}
}


TEST(Constraints, BaseClass) {
	aut::constraint_proxy s0{ 10.0 };
}

TEST(Constraints, BasicsInt) {
	aut::in_range<0, 10> a{ 11 };
	aut::less<10> b{ 2 };
	aut::greater<10> c{ 2 };
	aut::less_eq<10> d{ 2 };
	aut::greater_eq<10> e{ 2 };
	aut::one_of<0, 1, 54, 2> f{ 1 };

	EXPECT_FALSE(a.is_valid());
	EXPECT_TRUE(b.is_valid());
	EXPECT_FALSE(c.is_valid());
	EXPECT_TRUE(d.is_valid());
	EXPECT_FALSE(e.is_valid());
	EXPECT_TRUE(f.is_valid());
}

TEST(Constraints, Vector) {
	std::vector<aut::in_range<0, 100, int>> vec = { 0, 10,4, 123 };

	EXPECT_TRUE(vec[0].is_valid());
	EXPECT_TRUE(vec[1].is_valid());
	EXPECT_TRUE(vec[2].is_valid());
	EXPECT_FALSE(vec[3].is_valid());
}


TEST(Constraints, Arithmetic) {

	aut::less<100> a{ 11 };
	aut::in_range<0, 10> b{ 2 };

	auto c1 = a + b;
	EXPECT_EQ(c1, 13);
	auto c2 = 1 + a;
	EXPECT_EQ(c2, 12);
	auto c3 = a - b;
	EXPECT_EQ(c3, 9);
	auto c4 = 1 - a;
	EXPECT_EQ(c4, -10);
	auto c5 = a * b;
	EXPECT_EQ(c5, 22);
	auto c6 = 1 * a;
	EXPECT_EQ(c6, 11);
	auto c7 = a / b;
	EXPECT_EQ(c7, 5);
	auto c8 = 1 / a;
	EXPECT_EQ(c8, 0);

	c8++;
	EXPECT_EQ(c8, 1);
	c8--;
	EXPECT_EQ(c8, 0);

	assert(c8 < 5);
	assert(c8 != 5);

	c7 += b;
	EXPECT_EQ(c7, 7);
	c8 += 1;
	EXPECT_EQ(c8, 1);
	c8 -= b;
	EXPECT_EQ(c8, -1);
	c8 -= 1;
	EXPECT_EQ(c8, -2);
	c8 *= b;
	EXPECT_EQ(c8, -4);
	c8 *= 1;
	EXPECT_EQ(c8, -4);
	c8 /= b;
	EXPECT_EQ(c8, -2);
	c8 /= 1;
	EXPECT_EQ(c8, -2);
}

TEST(Constraints, Arithmetic2) {
	aut::less<100> a{ 11 };
	aut::in_range<0, 10> b{ 2 };

	auto c = a + b;
	EXPECT_EQ(c, 13);
}

TEST(Constraints, FunctionCall) {
	auto res = fib(10);

	EXPECT_TRUE(res.is_valid());
}

TEST(Constraints, FunctionCall2) {
	std::vector<aut::in_range<0, 100, int>> vec = { 0, 10,4, 123 };
	std::vector<aut::in_range<0, 100, int>> exp = { 0, 4, 10, 123 };

	BubbleSort(vec);

	EXPECT_EQ(vec, exp);
}
TEST(Constraints, FunctionCall3) {
	aut::in_range<0.f, 10.f> b{ 16.f };

	auto res = sqrt(b);
	EXPECT_FLOAT_EQ(res, 4.f);
}

TEST(ConstraintCombiner, And) {
	aut::_and<aut::in_range<0.f, 10.f>, aut::one_of<1.f, 2.f, 10.f>> b{ 10.f };

	EXPECT_TRUE(b.is_valid());
}

TEST(ConstraintCombiner, Or) {
	aut::_or<aut::in_range<0.f, 10.f>, aut::one_of<100.f, 200.f>> c{ 100.f };

	EXPECT_TRUE(c.is_valid());
}

TEST(ConstraintCombiner, Not) {
	aut::_or<aut::in_range<0.f, 10.f>, aut::one_of<100.f, 200.f>> c{ 100.f };
	aut::_not<decltype(c)> d{ 10.f };

	EXPECT_TRUE(c.is_valid());
	EXPECT_FALSE(d.is_valid());
}

TEST(ConstraintCombiner, FunctionCall) {
	aut::_or<aut::in_range<0.f, 10.f>, aut::one_of<100.f, 200.f>> c{ 100.f };
	auto res = sqrt(c);
	EXPECT_FLOAT_EQ(res, 10.f);
}


TEST(Evaluation, Basic) {
	{
		aut::less<1> a{ 16 };
		constexpr auto bv = aut::evaluate<decltype(a)>::valid_border_values;
		std::array<int, 1> exp = { 0 };
		EXPECT_EQ(bv, exp);
	}

	{
		aut::greater<1> b{ 16 };
		constexpr auto bv = aut::evaluate<decltype(b)>::valid_border_values;
		std::array<int, 1> exp = { 2 };
		EXPECT_EQ(bv, exp);
	}

	{
		aut::one_of<1, 5, 3> b{ 16 };
		constexpr auto bv = aut::evaluate<decltype(b)>::valid_border_values;
		std::array<int, 3> exp = { 1, 5, 3 };
		EXPECT_EQ(bv, exp);
	}
}

TEST(Evaluation, Combiner) {
	{
		aut::_and<aut::greater<1>, aut::one_of<-1, 1, 5, 3>> b{ 16 };
		constexpr auto bv = aut::evaluate<decltype(b)>::valid_border_values;
		std::array<int, 2> exp = { 5, 3 };
		EXPECT_EQ(bv, exp);
	}

	{
		aut::_or<aut::greater<1>, aut::one_of<-1, 1, 2, 5, 3>> b{ 16 };
		constexpr auto bv = aut::evaluate<decltype(b)>::valid_border_values;
		std::array<int, 5> exp = { 2, -1, 1, 5, 3 };
		EXPECT_EQ(bv, exp);
	}
}


TEST(TestGenerator, GlobalFunction) {
	aut::test_func{myFunc};
}

TEST(TestGenerator, GlobalFunction2) {
	aut::test_func{myFunc2, true};
}

TEST(TestGenerator, LambdaFunction) {
	const auto lambda_func = [](aut::in_range<0, 10> a) -> aut::greater_eq<0> {
		return (int)a;
	};
	aut::test_func{ lambda_func};
}

class TestClass {
public:
	static aut::greater_eq<0> static_member_func(aut::in_range<0, 10> a) {
		return (int)a;
	}
	aut::greater_eq<0> my_member_func(aut::in_range<0, 10> a) {
		return (int)a;
	}
};

TEST(TestGenerator, MemberFunctionWrappedInLambda) {
	TestClass tc{};
	auto l = [&tc](aut::in_range<0, 10> a) {return tc.my_member_func(a); };
	aut::test_func{ l};
}
TEST(TestGenerator, StaticMemberFunction) {
	aut::test_func{ TestClass::static_member_func};
}

//TEST(TestGenerator, Runtime) {
//	aut::measure_runtime([]() {return myFunc2(1, 2, 3); });
//	aut::measure_runtime([]() {return myFunc2_unconstrained(1, 2, 3); });
//}