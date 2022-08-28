#pragma once
#include <iostream>
#include <cassert>

#define TEST_LOG_FOMAT(pred) std::string(__FILE__) + std::string(" line: ") + std::to_string(__LINE__) + std::string(" [") + std::string(#pred) + std::string("] ")

// pred 에 대한 구문을 테스트하여서 그 결과를 반환해준다.
#define TEST_CODE(pred) if(pred == false) \
							std::cout << TEST_LOG_FOMAT(pred) << std::string("failed") << std::endl; \
						else \
							std::cout << TEST_LOG_FOMAT(pred) << std::string("succeeded") << std::endl; \


