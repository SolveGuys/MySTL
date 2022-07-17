#pragma once

#include <cstddef>
#include <memory>
#include <initializer_list>
//#include <vector> //todo:: 구현 완료후 삭제
#include "coreType.h"
#include "iterator.h"
#include "compressed_pair.h"


//std::vector<int> a;
namespace my {



	template<class T, typename Allocator = std::allocator<T>>
	class vector
	{
		static constexpr std::size_t INIT_CAP = 10;
		static constexpr float RESIZE_FACTOR = 1.5;


		struct vector_iterator;

		using value_type = T;
		using allocator_type = typename Allocator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const reference;
		using pointer = typename std::allocator_traits<Allocator>::pointer;
		using const_pointer = const pointer;
		using iterator = vector_iterator;
		using const_iterator = const iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;



		struct vector_compressed_pair : public compressed_pair<allocator_type, value_type*>
		{
			using super = compressed_pair<allocator_type, value_type*>;
			vector_compressed_pair(const allocator_type& alloc)
				:super(one_and_variadic_arg_t(), alloc)
			{}
			const allocator_type& get_allocator() const noexcept { return super::get_first(); }
			allocator_type& get_allocator() noexcept { return super::get_first(); }

			const value_type*& get_data() const noexcept { return super::get_second(); }
			value_type*& get_data() { return super::get_second(); }
		};

		struct vector_iterator : public base_iterator<random_access_iterator_tag, value_type>
		{
			using super = base_iterator<random_access_iterator_tag, value_type>;

		public:
			explicit vector_iterator(pointer inPtr) : super(inPtr) {}
			vector_iterator() : super(nullptr) {}
			vector_iterator& operator++() { super::ptr++; return *this; }
			vector_iterator operator++(int amount) { super::ptr += amount; return *super::ptr; }
			bool operator==(vector_iterator other) const { return super::ptr == other.super::ptr; }
			bool operator!=(vector_iterator other) const { return !(this->operator==(other)); }
			reference operator*() const { return *super::ptr; };

		};


	public:
		// Constructor
		vector() // 1
			:_alloc_data_pair(allocator_type())
		{
			_init_allocate();
		}
		explicit vector(const Allocator& alloc) // 2
			:_alloc_data_pair(alloc)
		{
			_init_allocate();
		}
		vector(size_type count, const value_type& value, const Allocator& alloc = Allocator())	// 3
			:_alloc_data_pair(alloc)
		{
			_init_copy_construct(count, value);
		}
		explicit vector(size_type count, const Allocator& alloc = Allocator())// 4
			:_alloc_data_pair(alloc)
		{
			_init_default_construct(count);
		}
		//template< class InputIt > // 정수형 매개변수를 전달할 경우에도 자동으로 여기로 들어옴. 
		//vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())	// 5
		//{

		//}
		vector(const vector& other);																			// 6
		vector(const vector& other, const Allocator& alloc);													// 7
		vector(vector&& other) noexcept;																		// 8
		vector(vector&& other, const Allocator& alloc);														// 9 
		vector(std::initializer_list<T> init, const Allocator& alloc = Allocator());							// 10

		// Destructor
		virtual ~vector()
		{
			for (size_type idx = 0; idx < _size; ++idx)
			{
				std::allocator_traits<allocator_type>::destroy(_alloc_data_pair.get_allocator(), _alloc_data_pair.get_data() + idx);
			}
			_relase_all();
		}

		// operator=
		vector& operator=(const vector& other);		// 1
		vector& operator=(const vector&& other);	// 2
		vector& operator=(std::initializer_list<T> ilist); // 3

		// assign()
		void assign(size_type count, const T& value);	// 1
		template< class InputIt >
		void assign(InputIt first, InputIt last);		// 2
		void assign(std::initializer_list<T> ilist);	// 3

		// at()
		reference at(size_type pos)				// 1
		{
			return *(_alloc_data_pair.get_data() + pos);
		}
		const_reference at(size_type pos) const	// 2
		{
			return *(_alloc_data_pair.get_data() + pos);
		}

		// operator[]
		reference operator[](size_type pos)				// 1
		{
			return at(pos);
		}
		const_reference operator[](size_type pos) const	// 2
		{
			return at(pos);
		}

		// front()
		reference front()				// 1
		{
			return at(0);
		}
		const_reference front() const	// 2
		{
			return at(0);
		}

		// back()
		reference back()				// 1
		{
			return at(_size - 1);
		}
		const_reference back() const	// 2
		{
			return at(_size - 1);
		}

		// data()
		T* data() noexcept				// 1
		{
			return _alloc_data_pair.get_data();
		}
		const T* data() const noexcept // 2
		{
			return _alloc_data_pair.get_data();
		}

		// Iterators
		// begin, cbegin
		iterator begin() noexcept				// 1
		{
			return iterator(data());
		}
		const_iterator begin() const noexcept	// 2
		{
			return iterator(data());
		}
		const_iterator cbegin() const noexcept; // 3

		// end, cend
		iterator end() noexcept				// 1
		{
			return iterator(at(_size));
		}
		const_iterator end() const noexcept	// 2
		{
			return iterator(at(_size));
		}
		const_iterator cend() const noexcept;	// 3

		// Capacity
		bool empty() const noexcept
		{
			return _size == 0;
		}
		size_type size() const noexcept
		{
			return _size;
		}
		size_type max_size() const noexcept;
		void reserve(size_type new_cap);
		size_type capacity() const noexcept;
		void shrink_to_fit();

		// Modifiers
		void clear() noexcept;
		iterator insert(const_iterator pos, const T& value);					// 1
		iterator insert(const_iterator pos, T&& value);							// 2
		iterator insert(const_iterator pos, size_type count, const T& value);	// 3
		template< class InputIt >
		iterator insert(const_iterator pos,	InputIt first, InputIt last);		// 4
		iterator insert(const_iterator pos, std::initializer_list<T> ilist);	// 5

		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args);

		iterator erase(const_iterator pos);										// 1
		iterator erase(const_iterator first, const_iterator last);				// 2

		void push_back(const T& value)
		{
			if (_size < _capacity)
			{
				_create_with_copy_constructor(1, value);
			}
			else
			{

			}
		}
		void push_back(T&& value);

		template< class... Args >
		void emplace_back(Args&&... args);

		void pop_back();

		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		void swap(vector& other) noexcept;


	private:
		vector_compressed_pair _alloc_data_pair;
		size_type _size{};
		size_type _capacity{};

		void _init_allocate(size_type capacity = INIT_CAP)
		{
			_capacity = capacity;
			_alloc_data_pair.get_data() = std::allocator_traits<allocator_type>::allocate(_alloc_data_pair.get_allocator(), _capacity);
		}
		void _expand_capacity()
		{
			_capacity *= RESIZE_FACTOR;
			value_type* new_data = std::allocator_traits<allocator_type>::allocate(_alloc_data_pair.get_allocator(), _capacity);
			for (size_type idx = 0; idx < _size; ++idx)
			{
				// copy construct 호출
				std::allocator_traits<allocator_type>::construct(_alloc_data_pair.get_allocator(), (new_data + idx), this->at(idx));
			}
		}
		void _create_with_default_constructor(size_type count)
		{
			size_type idx = _size;
			try
			{
				_size += count;
				for (; idx < _size; ++idx)
				{
					// call default constructor
					std::allocator_traits< allocator_type>::construct(_alloc_data_pair.get_allocator(), _alloc_data_pair.get_data() + idx);
				}
			}
			// TODO: 생성 예외 발생시 정책은?
			// 1: 모두 소멸, 메모리 해제?
			// 2: 생성한 객체만 소멸? - 그럼 메모리는??
			catch (...)
			{
				idx--; // n번째 생성중 예외 발생했기 때문에 n-1 번째 부터 소멸
				_size -= count;
				for (; idx >= _size; --idx)
				{
					std::allocator_traits<allocator_type>::destroy(_alloc_data_pair.get_allocator(), _alloc_data_pair.get_data() + idx);
				}
				throw;
			}
		}
		void _create_with_copy_constructor(size_type count, const value_type& other)
		{
			size_type idx = _size;
			try
			{
				_size += count;
				for (; idx < _size; ++idx)
				{
					// call copy constructor
					std::allocator_traits<allocator_type>::construct(_alloc_data_pair.get_allocator(), (_alloc_data_pair.get_data() + idx), other);
				}
			}
			// TODO: 생성 예외 발생시 정책은?
			// 1: 모두 소멸, 메모리 해제?
			// 2: 생성한 객체만 소멸? - 그럼 메모리는??
			catch (...)
			{
				idx--; // n번째 생성중 예외 발생했기 때문에 n-1 번째 부터 소멸
				_size -= count;
				for (; idx >= _size; --idx)
				{
					std::allocator_traits<allocator_type>::destroy(_alloc_data_pair.get_allocator(), _alloc_data_pair.get_data() + idx);
				}
				throw;
			}
		}

		void _init_copy_construct(size_type count, const value_type& other)
		{
			_init_allocate(_cal_new_capacity(count));
			try
			{
				_create_with_copy_constructor(count, other);
			}
			catch (...)
			{
				_relase_all()
				throw;
			}
		}

		void _init_default_construct(size_type count)
		{
			_init_allocate(_cal_new_capacity(count));
			try
			{
				_create_with_default_constructor(count);
			}
			catch (...)
			{
				_relase_all();
				throw;
			}
		}
		void _relase_all()
		{
			_alloc_data_pair.get_allocator().deallocate(_alloc_data_pair.get_data(), _capacity);
			_capacity = 0;
		}
		
		size_type _cal_new_capacity(size_type count)
		{
			size_type new_capacity = INIT_CAP;

			size_type resize_capacity = static_cast<size_type>(count * RESIZE_FACTOR);

			if (resize_capacity > new_capacity)
			{
				new_capacity = resize_capacity;
			}
			return new_capacity;
		}
		// 추후 구현해야할 기능들
		
		// assing()


		// get_allocator
		// allocator_type get_allocator() const noexcept;

		// rbegin, rcbegin
		// reverse_iterator rbegin() noexcept;
		// const_reverse_iterator rbegin() const noexcept;
		// const_reverse_iterator crbegin() const noexcept;

		// rend, rcend
		// reverse_iterator rend() noexcept;
		// const_reverse_iterator rend() const noexcept;
		// const_reverse_iterator crend() const noexcept;


	};


	//vector<bool>	space - efficient dynamic bitset

	//template< class T, class Alloc >
	//bool operator==(const std::vector<T, Alloc>& lhs, const std::vector<T, Alloc>& rhs);

	//template< class T, class Alloc >
	//bool operator!=(const std::vector<T, Alloc>& lhs, const std::vector<T, Alloc>& rhs);

	//template< class T, class Alloc >
	//bool operator<(const std::vector<T, Alloc>& lhs, const std::vector<T, Alloc>& rhs);

	//template< class T, class Alloc >
	//bool operator<=(const std::vector<T, Alloc>& lhs, const std::vector<T, Alloc>& rhs);

	//template< class T, class Alloc >
	//bool operator>(const std::vector<T, Alloc>& lhs, const std::vector<T, Alloc>& rhs);

	//template< class T, class Alloc >
	//bool operator>=(const std::vector<T, Alloc>& lhs, const std::vector<T, Alloc>& rhs);

	//template< class T, class Alloc >
	//void swap(std::vector<T, Alloc>& lhs, std::vector<T, Alloc>& rhs);



}

