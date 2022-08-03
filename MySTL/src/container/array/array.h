#pragma once
#include <array> //todo:: 구현 완료후 삭제
#include <algorithm> //using std::copy
#include "coreType.h"
#include "iterator.h"
#include "compressed_pair.h"

namespace my 
{

	/*
	template<class value_type, std::size_t _size>
	struct const_array_iterator : public base_iterator<random_access_iterator_tag, value_type>
	{
		using super = base_iterator<random_access_iterator_tag, value_type>;

		//Member types
		//using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type;

	public:
		array_iterator() : super(), _idx(0) {}
		explicit array_iterator(pointer inPtr, size_t offset = 0) : super(inPtr), _idx(offset) {}
		
		array_iterator& operator++() { super::ptr++; return *this; }
		array_iterator operator++(int amount) { super::ptr += amount; return *super::ptr; }
		array_iterator& operator--() { super::ptr--; return *this; }
		array_iterator operator--(int amount) { super::ptr -= amount; return *super::ptr; }
		bool operator==(array_iterator other) const { return super::ptr == other.super::ptr; }
		bool operator!=(array_iterator other) const { return !(this->operator==(other)); }
		reference operator*() const { return *operator->(); };
		pointer operator->() const { 
			assert(_idx < _size, "cannot dereference out of range array iterator");
			return _ptr + _idx; 
		}

	private:
		pointer _ptr;
		size_t _idx;
	};
	*/

	template<class value_type, std::size_t _size>
	struct array_iterator : public base_iterator<random_access_iterator_tag, value_type>
	{
		using super = base_iterator<random_access_iterator_tag, value_type>;
		
		//Member types
		//using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type;

	public:
		explicit array_iterator(pointer inPtr, size_t offset = 0) : super(inPtr), _idx(offset) {}
		array_iterator() : super(nullptr) {}
		array_iterator& operator++() { super::ptr++; return *this; }
		array_iterator operator++(int amount) { super::ptr += amount; return *super::ptr; }
		array_iterator& operator--() { super::ptr--; return *this; }
		array_iterator operator--(int amount) { super::ptr -= amount; return *super::ptr; }
		bool operator==(array_iterator other) const { return super::ptr == other.super::ptr; }
		bool operator!=(array_iterator other) const { return !(this->operator==(other)); }
		reference operator*() const { return *super::ptr; };
		pointer operator->() const { return super::->; }

	private:
		pointer _ptr;
		size_t _idx;
	};
	/*
	*/

	template<class T, std::size_t _size>
	class array
	{
	public:
		//struct array_iterator;// <T, size_type>;

		//Member types
		using value_type		= T;
		using size_type			= std::size_t;
		using difference_type	= std::ptrdiff_t;
		using pointer			= value_type*;
		using const_pointer		= const value_type*;
		using reference			= value_type&;
		using const_reference	= const value_type;

		using iterator = array_iterator<T, _size>;
		using const_iterator = const array_iterator<T, _size>;

		using reverse_iterator			= std::reverse_iterator<iterator>;//my::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;//my::reverse_iterator<const_iterator>;

		
		
	//Iterators
	public:
		iterator begin() 
		{
			return iterator(_elems, 0);
		}

		iterator end() 
		{
			return iterator(_elems, _size);
		}
	//Operations
	public:
		void fill(const T& value) 
		{
			std::fill_n(_elems, size, value);
		}

		void swap(array& other) noexcept(std::is_nothrow_swappable_v<T>::value) 
		{
			std::_Swap_ranges_unchecked(_elems, _elems+size, other._elems);
		}
	//Operators
	public:
		reference operator[](size_type pos) noexcept
		{
			//static_assert(pos < _size && pos >= 0);

			return _elems[pos];
		}
		
	//Member functions
	public:
		
		// Constructor, Destructor
		// my::array is an aggregate type, it dose NOT have a constructor/destructor.

		// at()
		
		reference at(size_type pos) 
		{
			assert(_size > pos, "invalid my::array<T, N> subscript");

			if (_size <= pos) 
			{
				throw std::out_of_range("invalid my::array<T, N> subscript");
			}

			return _elems[pos];
		}

		constexpr reference at(size_type pos) const
		{
			assert(_size > pos, "invalid my::array<T, N> subscript");

			if (_size <= pos)
			{
				throw std::out_of_range("invalid my::array<T, N> subscript");
			}

			return _elems[pos];
		}

		//const_reference at(size_type pos) const;
		//constexpr const_reference at(size_type pos) const;
		
		T _elems[_size];
	};

}
