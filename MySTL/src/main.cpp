#include "vector.h"
#include "compressed_pair.h"
#include "array.h"
#include "test/testcode.h"
#include <string>
#include <vector>
#include <array>

void my_vector_test();
void compressed_pair_test();
int my_array_test();

int main()
{
	//std::vector<int> vec1(5,3);

	my_vector_test();
	compressed_pair_test();
	my_array_test();
	return 0;
}

void my_vector_test()
{
	struct Data {

		Data() :num(0)
		{
			std::cout << "Data(): " << num << std::endl;
		}
		Data(int _num) : num(_num)
		{
			std::cout << "Data(int num): " << num << std::endl;
		}
		Data(const Data& other)
		{
			num = other.num;
			std::cout << "Data(const Data& other): copy" << std::endl;
		}
		Data(const Data&& other) noexcept
		{
			num = other.num;
			std::cout << "Data(const Data&& other): move" << std::endl;
		}
		~Data()
		{
			std::cout << "~Data(): " << num << std::endl;
		}
		int num;
	};

	my::vector<Data> vec1;	// fine
	vec1.emplace_back(1);
	vec1.emplace_back(2);
	vec1.emplace_back(3);
	vec1.emplace_back(4);
	vec1.emplace_back(5);
	std::cout << "vec1.resize(20)" << std::endl;
	vec1.resize(20); 
	//my::vector<Data> vec2(vec1);

	//my::vector<Data> vec3;
	//vec3 = vec1;

	//my::vector<Data> vec1;	// fine
	//my::vector<Data> vec2(5); // must error 2512

	//my::vector<Data> vec2(1, 10);
	//std::vector<Data> vec3(1, 11); //fine

	//std::cout << "my vector================" << std::endl;
	//my::vector<Data> my_vec(3);

	//Data data(3);
	//for (int i = 0; i < 10; ++i)
	//{
	//	my_vec.push_back(data);
	//	std::cout << "push_back result - size: " << my_vec.size() << " cap: " << my_vec.capacity() << std::endl;;
	//}

	//std::cout << std::endl;

	//std::cout << "stl vector===========" << std::endl;
	//std::vector<Data> stl_vec(3);
	//for (int i = 0; i < 10; ++i)
	//{
	//	stl_vec.push_back(data);
	//	std::cout << "push_back result - size: " << stl_vec.size() << " cap: " << stl_vec.capacity() << std::endl;;
	//}

	// 테스트 필요한 기능
	// 사용자 생성 할당자 테스트
	// 1~4번 생성자 테스트
	std::cout << "aaaa\n";
	my::array<int, 5> a{ 1,2,3,4,5 };
	//
	for (auto it = a.rbegin(); it != a.rend(); it++)
		std::cout << *it << ' ';
	std::cout << "aaaa\n";
}

int my_array_test()
{
	int nErrorCount = 0;

#ifdef my_array_completed
	{
		my::array<int, 5> a = { { 0, 1, 2, 3, 4 } };
		my::array<int, 5> b = { { 0, 1, 2, 3    } };
		my::array<int, 5> c = { { 4, 3, 2, 1, 0 } };
		my::array<int, 0> d = { { 0 } };

		assert(!a.empty());
		assert(a.size() == 5);
		assert(a[0] == 0);
		assert(a[4] == 4);

		assert(!b.empty());
		assert(b.size() == 5);
		assert(b[0] == 0);
		assert(b[3] == 3);

		assert(d.empty());
		assert(d.size() == 0);

		// swap
		a.swap(c);
		assert(a[0] == 4);
		assert(c[0] == 0);

		// begin, end
		my::array<int, 5>::iterator it = a.begin();
		//assert((a.validate_iterator(it) & (isf_valid | isf_can_dereference)) != 0);
		assert(*it == 4);

		++it;
		assert(*it == 3);

		++it;
		assert(*it == 2);

		--it;
		assert(*it == 3);

		it += 3;
		//assert((a.validate_iterator(it) & (isf_valid | isf_can_dereference)) != 0);
		assert(*it == 0);

		++it;
		assert(it == a.end());
		//assert((a.validate_iterator(it) & isf_valid) != 0);
		//assert(a.validate());

		// rbegin, rend
		my::array<int, 5>::reverse_iterator itr = a.rbegin();
		//assert((a.validate_iterator(itr.base()) & (isf_valid | isf_can_dereference)) != 0);
		assert(*itr == 0);

		itr++;
		assert(*itr == 1);

		// data
		int* pArray = a.data();
		assert(pArray == a._elems);

		// front
		int& nFront = a.front();
		assert(nFront == 4);

		// back
		int& nBack = a.back();
		assert(nBack == 0);

		// at
		assert(a[0] == a.at(0));

		// global operators
		a[0] = 0; a[1] = 1; a[2] = 2; a[3] = 3; a[4] = 4;  // 01234
		b[0] = 0; b[1] = 1; b[2] = 2; b[3] = 3; b[4] = 4;  // 01234
		c[0] = 0; c[1] = 1; c[2] = 2; c[3] = 3; c[4] = 9;  // 01239

		assert((a == b));
		assert(!(a != b));
		assert(!(a < b));
		assert((a <= b));
		assert((a >= b));
		assert(!(a > b));

		assert(!(a == c));
		assert((a != c));
		assert((a < c));
		assert((a <= c));
		assert(!(a >= c));
		assert(!(a > c));

		// deduction guides
#ifdef __cpp_deduction_guides
		//C++20 내용이므로 구현하지 않음
		/*
		my::array deduced{ 1,2,3,4,5 };

		//static_assert(my::is_same_v<decltype(deduced)::value_type, int>, "deduced array value_type mismatch");
		assert(deduced.size() == 5);
		*/
#endif
		// TODO : structured binding
		// structured binding
		
		{
			my::array<int, 5> aCopy = a;
			auto&& [a0, a1, a2, a3, a4] = aCopy;

			assert(a0 == aCopy[0]);
			assert(a1 == aCopy[1]);
			assert(a2 == aCopy[2]);
			assert(a3 == aCopy[3]);
			assert(a4 == aCopy[4]);

			a0 = 100;
			assert(aCopy[0] == 100);

			a4 = 0;
			assert(aCopy[4] == 0);

			// The deduced type may or may not be a reference type; it is an aliased type,
			//  as per https://en.cppreference.com/w/cpp/language/structured_binding:
			//  > Like a reference, a structured binding is an alias to an existing object. Unlike a reference,
			//    the type of a structured binding does not have to be a reference type.
			// Any reference specifier is thus removed to check only the type & its const qualifier
			
			//static_assert(my::is_same_v<my::remove_reference_t<decltype(a0)>, int>);

			const my::array<int, 5> aConstCopy = a;
			auto&& [aConst0, aConst1, aConst2, aConst3, aConst4] = aConstCopy;

			///static_assert(my::is_same_v<my::remove_reference_t<decltype(aConst0)>, const int>);
		}
		
	}

	// constexpr tests
	{
#if 0
		constexpr my::array<int, 4> a = { { 0, 1, 2, 3 } };

		static_assert(a == my::array<int, 4>{ { 0, 1, 2, 3 }}, "array constexpr failure");

		static_assert(a[0] == 0, "array constexpr failure");
		static_assert(a[1] == 1, "array constexpr failure");
		static_assert(a[2] == 2, "array constexpr failure");
		static_assert(a[3] == 3, "array constexpr failure");

		static_assert(a.at(0) == 0, "array constexpr failure");
		static_assert(a.at(1) == 1, "array constexpr failure");
		static_assert(a.at(2) == 2, "array constexpr failure");
		static_assert(a.at(3) == 3, "array constexpr failure");

		static_assert(a.data()[0] == 0, "array constexpr failure");
		static_assert(a.data()[1] == 1, "array constexpr failure");
		static_assert(a.data()[2] == 2, "array constexpr failure");
		static_assert(a.data()[3] == 3, "array constexpr failure");

		static_assert(a.empty() == false, "array constexpr failure");
		static_assert(a.size() == 4, "array constexpr failure");
		static_assert(a.max_size() == 4, "array constexpr failure");

		static_assert(a.front() == 0, "array constexpr failure");
		static_assert(a.back() == 3, "array constexpr failure");

		static_assert(a.begin()[0] == 0, "array constexpr failure");
		static_assert(a.begin()[1] == 1, "array constexpr failure");
		static_assert(a.begin()[2] == 2, "array constexpr failure");
		static_assert(a.begin()[3] == 3, "array constexpr failure");

		static_assert(a.cbegin()[0] == 0, "array constexpr failure");
		static_assert(a.cbegin()[1] == 1, "array constexpr failure");
		static_assert(a.cbegin()[2] == 2, "array constexpr failure");
		static_assert(a.cbegin()[3] == 3, "array constexpr failure");

		static_assert(a.crbegin()[0] == 3, "array constexpr failure");
		static_assert(a.crbegin()[1] == 2, "array constexpr failure");
		static_assert(a.crbegin()[2] == 1, "array constexpr failure");
		static_assert(a.crbegin()[3] == 0, "array constexpr failure");

		static_assert(a.end()[-1] == 3, "array constexpr failure");
		static_assert(a.end()[-2] == 2, "array constexpr failure");
		static_assert(a.end()[-3] == 1, "array constexpr failure");
		static_assert(a.end()[-4] == 0, "array constexpr failure");

		static_assert(a.cend()[-1] == 3, "array constexpr failure");
		static_assert(a.cend()[-2] == 2, "array constexpr failure");
		static_assert(a.cend()[-3] == 1, "array constexpr failure");
		static_assert(a.cend()[-4] == 0, "array constexpr failure");

		static_assert(a.crend()[-1] == 0, "array constexpr failure");
		static_assert(a.crend()[-2] == 1, "array constexpr failure");
		static_assert(a.crend()[-3] == 2, "array constexpr failure");
		static_assert(a.crend()[-4] == 3, "array constexpr failure");
#endif
	}

	// to_array
#if 0
	{
		{
			constexpr int c_array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			constexpr auto arr = to_array(c_array);

			static_assert(is_same_v<remove_cv_t<decltype(arr)>, my::array<int, 10>>, "unexpected return type");

			static_assert(arr[0] == 0, "unexpected array value");
			static_assert(arr[1] == 1, "unexpected array value");
			static_assert(arr[2] == 2, "unexpected array value");
			static_assert(arr[3] == 3, "unexpected array value");
			static_assert(arr[4] == 4, "unexpected array value");
			static_assert(arr[5] == 5, "unexpected array value");
			static_assert(arr[6] == 6, "unexpected array value");
			static_assert(arr[7] == 7, "unexpected array value");
			static_assert(arr[8] == 8, "unexpected array value");
			static_assert(arr[9] == 9, "unexpected array value");
		}

		{
			constexpr auto arr = to_array({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

			static_assert(is_same_v<remove_cv_t<decltype(arr)>, my::array<int, 10>>, "unexpected return type");

			static_assert(arr[0] == 0, "unexpected array value");
			static_assert(arr[1] == 1, "unexpected array value");
			static_assert(arr[2] == 2, "unexpected array value");
			static_assert(arr[3] == 3, "unexpected array value");
			static_assert(arr[4] == 4, "unexpected array value");
			static_assert(arr[5] == 5, "unexpected array value");
			static_assert(arr[6] == 6, "unexpected array value");
			static_assert(arr[7] == 7, "unexpected array value");
			static_assert(arr[8] == 8, "unexpected array value");
			static_assert(arr[9] == 9, "unexpected array value");
		}

		{
			constexpr auto arr = to_array<long>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

			static_assert(is_same_v<remove_cv_t<decltype(arr)>, my::array<long, 10>>, "unexpected return type");

			static_assert(arr[0] == 0l, "unexpected array value");
			static_assert(arr[1] == 1l, "unexpected array value");
			static_assert(arr[2] == 2l, "unexpected array value");
			static_assert(arr[3] == 3l, "unexpected array value");
			static_assert(arr[4] == 4l, "unexpected array value");
			static_assert(arr[5] == 5l, "unexpected array value");
			static_assert(arr[6] == 6l, "unexpected array value");
			static_assert(arr[7] == 7l, "unexpected array value");
			static_assert(arr[8] == 8l, "unexpected array value");
			static_assert(arr[9] == 9l, "unexpected array value");
		}

		{
			constexpr auto arr = to_array<unsigned long>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

			static_assert(is_same_v<remove_cv_t<decltype(arr)>, my::array<unsigned long, 10>>, "unexpected return type");

			static_assert(arr[0] == 0ul, "unexpected array value");
			static_assert(arr[1] == 1ul, "unexpected array value");
			static_assert(arr[2] == 2ul, "unexpected array value");
			static_assert(arr[3] == 3ul, "unexpected array value");
			static_assert(arr[4] == 4ul, "unexpected array value");
			static_assert(arr[5] == 5ul, "unexpected array value");
			static_assert(arr[6] == 6ul, "unexpected array value");
			static_assert(arr[7] == 7ul, "unexpected array value");
			static_assert(arr[8] == 8ul, "unexpected array value");
			static_assert(arr[9] == 9ul, "unexpected array value");
		}

		{
			constexpr auto arr = to_array("my");

			static_assert(is_same_v<remove_cv_t<decltype(arr)>, my::array<char, 6>>, "unexpected return type");

			static_assert(arr[0] == 'E', "unexpected value in array");
			static_assert(arr[1] == 'A', "unexpected value in array");
			static_assert(arr[2] == 'S', "unexpected value in array");
			static_assert(arr[3] == 'T', "unexpected value in array");
			static_assert(arr[4] == 'L', "unexpected value in array");
		}

	}
#endif

#endif
	return nErrorCount;

}



void compressed_pair_test()
{
	class Empty {};
	compressed_pair<int, int> cp1(one_and_variadic_arg_t{}, 1, 1);
	compressed_pair<Empty, int> cp2(zero_and_variadic_arg_t{}, 1);
	compressed_pair<Empty, int> cp3(zero_and_variadic_arg_t{});

	TEST_CODE(sizeof(cp1) == 8);
	TEST_CODE(sizeof(cp2) == 4);
}
