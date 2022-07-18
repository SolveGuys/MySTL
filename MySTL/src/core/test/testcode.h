#pragma once
#include <iostream>
#include <cassert>

#define TEST_LOG_FOMAT(pred) std::string(__FILE__) + std::string(" line: ") + std::to_string(__LINE__) + std::string(" [") + std::string(#pred) + std::string("] ")

// pred �� ���� ������ �׽�Ʈ�Ͽ��� �� ����� ��ȯ���ش�.
#define TEST_CODE(pred) if(pred == false) \
							std::cout << TEST_LOG_FOMAT(pred) << std::string("failed") << std::endl; \
						else \
							std::cout << TEST_LOG_FOMAT(pred) << std::string("succeeded") << std::endl; \



// exp�� false�� ��� ��Ÿ�� ������ �߻������ش�.
// ����: assertm([���� ����], "�����޼���")
#define assertm(exp, msg) assert(((void)msg, exp))