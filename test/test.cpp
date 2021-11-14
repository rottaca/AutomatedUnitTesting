// AutomatedUnitTesting.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include <assert.h>
#include <type_traits>

#include "constraints.hpp"
#include "constraint_combiner.hpp"
#include "evaluation.hpp"
#include "testgenerator.hpp"
#include "helper.hpp"


#include <vector>

aut::greater<0, int> fib(aut::greater<0, int> n) {
	if (n <= 0) return 0;
	if (n == 1) return 1;

	int f1 = 1; // Bem.: f0 = 0
	int f2 = 1;
	for (int i = 2; i < n; ++i)
	{
		int fneu = f2 + f1;
		f1 = f2;
		f2 = fneu;
	}
	return f2;
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

int main()
{
	{
		aut::constraint_proxy s0{ 10.0 };
	}

	{
		aut::in_range<0.f, 10.f> s1 = 5.f;
	}

	{
		aut::in_range<0, 10> s2 = 1;
		aut::in_range<0, 10> s3 = 3;
	
		auto res = s3 + s2;
		assert(res == 4);
	}

	{
		aut::in_range<0., 10.> s4{ 11.0 };
	}

	{
		auto res = fib(10);
	}

	{
		std::vector<aut::in_range<0, 100, int>> vec = { 0, 10,4, 123 };
		std::vector<aut::in_range<0, 100, int>> exp = { 0, 4, 10, 123 };
		BubbleSort(vec);
		assert(vec == exp);
	}

	{
		aut::in_range<0, 10> a{ 11 };
		aut::less<10> b{ 2 };
		aut::greater<10> c{ 2 };
		aut::less_eq<10> d{ 2 };
		aut::greater_eq<10> e{ 2 };
		aut::one_of<0, 1, 54, 2> f{ 1 };
	}

	{
		aut::less<100> a{ 11 };
		aut::in_range<0, 10> b{ 2 };

		auto c = a + b;
		assert(c == 13);
	}

	{
		aut::less<100> a{ 11 };
		aut::in_range<0, 10> b{ 2 };

		auto c1 = a + b;
		assert(c1 == 13);
		auto c2 = 1 + a;
		assert(c2 == 12);
		auto c3 = a - b;
		assert(c3 == 9);
		auto c4 = 1 - a;
		assert(c4 == -10);
		auto c5 = a * b;
		assert(c5 == 22);
		auto c6 = 1 * a;
		assert(c6 == 11);
		auto c7 = a / b;
		assert(c7 == 5);
		auto c8 = 1 / a;
		assert(c8 == 0);

		c8++;
		assert(c8 == 1);
		c8--;
		assert(c8 == 0);

		assert(c8 < 5);
		assert(c8 != 5);

		c7 += b;
		assert(c7 == 7);
		c8 += 1;
		assert(c8 == 1);
		c8 -= b;
		assert(c8 == -1);
		c8 -= 1;
		assert(c8 == -2);
		c8 *= b;
		assert(c8 == -4);
		c8 *= 1;
		assert(c8 == -4);
		c8 /= b;
		assert(c8 == -2);
		c8 /= 1;
		assert(c8 == -2);

	}

	{
		aut::in_range<0.f, 10.f> b{ 16.f };

		auto res = sqrt(b);
		assert(aut::float_equal(res, 4.f));
	}

	{
		aut::_and<aut::in_range<0.f, 10.f>, aut::one_of<1.f, 2.f, 10.f>> b{ 16.f };
		aut::_or<aut::in_range<0.f, 10.f>, aut::one_of<100.f, 200.f>> c{ 20 };
		aut::_not<decltype(c)> d{ 10.f };

		auto res = sqrt(b);
		assert(aut::float_equal(res, 4.f));
	}

	{
		aut::less<1> b{ 16 };
		constexpr auto bw = aut::evaluate<decltype(b)>::valid_border_values;
		std::array<int, 1> exp = { 0 };
		assert(bw == exp);
	}

	{
		aut::greater<1> b{ 16 };
		constexpr auto bw = aut::evaluate<decltype(b)>::valid_border_values;
		std::array<int, 1> exp = { 2 };
		assert(bw == exp);
	}

	{
		aut::one_of<1, 5, 3> b{ 16 };
		constexpr auto bw = aut::evaluate<decltype(b)>::valid_border_values;
		std::array<int, 3> exp = { 1, 5, 3 };
		assert(bw == exp);
	}

	{
		aut::_and<aut::greater<1>, aut::one_of<-1, 1, 5, 3>> b{ 16 };
		constexpr auto bw = aut::evaluate<decltype(b)>::valid_border_values;
		std::array<int, 2> exp = { 5, 3 };
		assert(bw == exp);
	}

	{
		aut::_or<aut::greater<1>, aut::one_of<-1, 1, 2, 5, 3>> b{ 16 };
		constexpr auto bw = aut::evaluate<decltype(b)>::valid_border_values;
		std::array<int, 5> exp = {2, -1, 1, 5, 3 };
		assert(bw == exp);
	}

	{
		//aut::error_print<decltype(fib)>{};
		//aut::error_print_val<fib>{};

		aut::generate_tests<fib>();
	}
	return 0;
}
