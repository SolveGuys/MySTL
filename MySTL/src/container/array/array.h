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
		
		array_iterator& operator+=(int amount) { super::ptr += amount; return *this; }
		array_iterator operator+=(int amount) const { super::ptr += amount; return *super::ptr; }

		array_iterator& operator-=(int amount) { super::ptr -= amount; return *this; }
		array_iterator operator-=(int amount) const { super::ptr -= amount; return *super::ptr; }

		bool operator==(array_iterator other) const { return super::ptr == other.super::ptr; }
		bool operator!=(array_iterator other) const { return !(this->operator==(other)); }
		reference operator*() const { return *super::ptr; };
		pointer operator->() const { return super::->; }

	private:
		pointer _ptr; // start position of array
		size_t _idx; // offset into array
	};
	
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
		iterator begin() noexcept
		{
			iterator begin = iterator(_elems, 0);
			return begin;
		}

		const_iterator begin() const noexcept
		{
			const_iterator begin = const_iterator(_elems, 0);
			return begin;
		}

		iterator end() noexcept
		{
			iterator end = iterator(_elems, _size);
			return end;
		}
		
		const_iterator end() const noexcept 
		{
			return const_iterator(_elems, _size);
		}
		

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(begin());
		}
				
		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		reverse_iterator rend() noexcept
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(end());
		}
		

	//Operations
	public:
		void fill(const T& value) 
		{
			std::fill_n(_elems, size, value);
		}

		void swap(array& other) noexcept(std::is_nothrow_swappable<T>::value) 
		{
			std::_Swap_ranges_unchecked(_elems, _elems + _size, other._elems);
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



		const bool empty() noexcept
		{
			return _size == 0;
		}

		constexpr size_type size() const noexcept
		{
			return _size;
		}


		// at()
		
		reference at(size_type pos) 
		{
			assert(((void) "invalid my::array<T, N> subscript", _size > pos));

			if (_size <= pos) 
			{
				throw std::out_of_range("invalid my::array<T, N> subscript");
			}

			return _elems[pos];
		}

		constexpr reference at(size_type pos) const
		{
			assert(((void) "invalid my::array<T, N> subscript", _size > pos));

			if (_size <= pos)
			{
				throw std::out_of_range("invalid my::array<T, N> subscript");
			}

			return _elems[pos];
		}

		//const_reference at(size_type pos) const;
		//constexpr const_reference at(size_type pos) const;
		
		reference front() noexcept 
		{
			return _elems[0];
		}
		
		constexpr const_reference front() const noexcept
		{
			return _elems[0];
		}

		reference back() noexcept
		{
			return _elems[_size - 1];
		}
		
		constexpr reference back() const noexcept
		{
			return _elems[_size - 1];
		}

		constexpr T* data() noexcept 
		{
			return _elems;
		}

		const T* data() const noexcept 
		{
			return _elems;
		}

		T _elems[_size];
	};

	template<class T, std::size_t _size>
	bool operator==(const my::array<T, _size>& left, const my::array<T, _size>& right)
	{
		return std::equal(left.begin(), left.end(), right.begin());
	}
	template<class T, std::size_t _size>
	bool operator!=(const my::array<T, _size>& left, const my::array<T, _size>& right)
	{
		return !(left == right);
	}
	template<class T, std::size_t _size>
	bool operator<(const my::array<T, _size>& left, const my::array<T, _size>& right)
	{
		return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
	}
	template<class T, std::size_t _size>
	bool operator>(const my::array<T, _size>& left, const my::array<T, _size>& right)
	{
		return right < left;
	}
	template<class T, std::size_t _size>
	bool operator<=(const my::array<T, _size>& left, const my::array<T, _size>& right)
	{
		return !(right < left);
	}
	template<class T, std::size_t _size>
	bool operator>=(const my::array<T, _size>& left, const my::array<T, _size>& right)
	{
		return !(left < right);
	}

}
