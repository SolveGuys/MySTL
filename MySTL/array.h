#pragma once
#include <array> //todo:: 구현 완료후 삭제
#include "coreType.h"
#include "iterator.h"
#include "compressed_pair.h"

namespace my 
{
	template<class T, std::size_t _size>
	class array
	{
		struct array_iterator;

		//Member types
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using iterator = array_iterator;
		using const_iterator = const array_iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;//my::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;//my::reverse_iterator<const_iterator>;

	//Member functions
	public:
		
		// Constructor
		array();

		// Destructor
		virtual ~array();

		operator=();

		// at()
		reference at(size_type pos);
		constexpr reference at(size_type pos);

		const_reference at(size_type pos) const;
		constexpr const_reference at(size_type pos) const;

		T _elems[size];
	};
}
