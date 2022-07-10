#include "vector.h"
#include "compressed_pair.h"
#include "test/testcode.h"
#include <string>
#include <vector>



void my_vector_test();
void compressed_pair_test();

int main()
{
	my_vector_test();
	compressed_pair_test();

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
			std::cout << "Data(const Data& other)" << std::endl;
		}
		~Data()
		{
			std::cout << "~Data(): " << num << std::endl;
		}
		int num;
	};

	//my::vector<Data> vec1;	// fine
	//my::vector<Data> vec2(5); // must error 2512

	//my::vector<Data> vec2(1, 10);
	//std::vector<Data> vec3(1, 11); //fine

	my::vector<Data> vec4(3);
	std::vector<Data> vec5(3);


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
