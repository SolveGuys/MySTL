#pragma once

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <type_traits>

//#include <vector> //todo:: 구현 완료후 삭제
#include "coreType.h"
#include "iterator.h"
#include "compressed_pair.h"
#include "test/testcode.h"

 
//std::vector<int> a;
namespace my {

	template<class T, typename Allocator = std::allocator<T>>
	class vector
	{
		static constexpr std::size_t INIT_CAP = 4;
		static constexpr float RESIZE_FACTOR = 1.5;

		 
		struct vector_iterator;

		using value_type = T;
		using allocator_type = typename Allocator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
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

			const value_type* get_data() const noexcept { return super::get_second(); }
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
		vector() noexcept(noexcept(Allocator())) // 1
			:_alloc_data_pair(allocator_type())
		{
			_init_allocate();
		}
		explicit vector(const Allocator& alloc) noexcept // 2
			:_alloc_data_pair(alloc)
		{
			_init_allocate();
		}
		vector(size_type count, const value_type& value, const Allocator& alloc = Allocator())	// 3
			:_alloc_data_pair(alloc)
		{
			_init_copy_value(count, value);
		}
		explicit vector(size_type count, const Allocator& alloc = Allocator())// 4
			:_alloc_data_pair(alloc)
		{
			_init_default_construct(count);
		}
		//template< typename InputIt > // 정수형 매개변수를 전달할 경우에도 자동으로 여기로 들어옴. 
		//vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())	// 5
		//{

		//}
		vector(const vector& other)																			// 6
			:vector(other, Allocator())
		{
		}
		vector(const vector& other, const Allocator& alloc)													// 7
			:_alloc_data_pair(alloc)
		{
			// todo: 로그 삭제할것
			std::cout << "vector(const vector& other) copy" << std::endl;
			_init_allocate(other.size());
			_copy_data_in_unused_space(other);
		}
		vector(vector&& other) noexcept																		// 8
			:_alloc_data_pair(Allocator())
		{
			std::cout << "vector(vector&& other) move" << std::endl;
			this->_swap_all(other);
		}
		vector(vector&& other, const Allocator& alloc)														// 9 
			:_alloc_data_pair(alloc)
		{
			std::cout << "vector(vector&& other, Allocator& alloc) move" << std::endl;
			this->_swap_all(other);
		}
		vector(std::initializer_list<T> init, const Allocator& alloc = Allocator());							// 10

		// Destructor
		virtual ~vector()
		{
			for (; 0 < _size; --_size)
			{
				std::allocator_traits<allocator_type>::destroy(get_allocator(), data() + _size - 1);
			}
			_relase_all();
		}

		// operator=
		vector& operator=(const vector& other)		// 1
		{
			for (; 0 < _size; --_size)
			{
				std::allocator_traits<allocator_type>::destroy(get_allocator(), data() + _size - 1);
			}

			if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value 
				&& get_allocator() != other.get_allocator())
			{
				// 기존 allocator로 할당받은 메모리 반납
				_relase_all();
				get_allocator() = other.get_allocator();
			}

			assertm(_size == 0, "size must be 0 before reallocation. ");

			_realloacate(other._capacity);
			_copy_data_in_unused_space(other);
			return (*this);
		}
		vector& operator=(const vector&& other);	// 2
		vector& operator=(std::initializer_list<T> ilist); // 3

		// assign()
		void assign(size_type count, const T& value);	// 1
		template< class InputIt >
		void assign(InputIt first, InputIt last);		// 2
		void assign(std::initializer_list<T> ilist);	// 3

		// get_allocator()
		allocator_type get_allocator() const noexcept
		{
			return _alloc_data_pair.get_allocator();
		}

		// at()
		reference at(size_type pos)				// 1
		{
			if (!(pos < size()))
			{
				throw std::out_of_range("invalid vector<T> subscript");
			}
			return *(data() + pos);
		}
		const_reference at(size_type pos) const	// 2
		{
			if (!(pos < size()))
			{
				throw std::out_of_range("invalid vector<T> subscript");
			}
			return *(data() + pos);
		}

		// operator[]
		reference operator[](size_type pos)				// 1
		{
			return *(data() + pos);
		}
		const_reference operator[](size_type pos) const	// 2
		{
			return *(data() + pos);
		}

		// front()
		reference front()				// 1
		{
			return *data();
		}
		const_reference front() const	// 2
		{
			return *data();
		}

		// back()
		reference back()				// 1
		{
			return *(_end_pos() - 1);
		}
		const_reference back() const	// 2
		{
			return *(_end_pos() - 1);
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
		size_type max_size() const noexcept
		{
			return std::allocator_traits<allocator_type>::max_size(get_allocator());
		}
		void reserve(size_type new_cap)
		{
			if (new_cap <= _capacity) return;

			_realloacate(new_cap);
		}
		size_type capacity() const noexcept
		{
			return _capacity;
		}
		void shrink_to_fit()
		{
			if (_size == _capacity) return;

			_realloacate(_size);
		}

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

		void push_back(const value_type& value)
		{
			emplace_back(value);
		}
		void push_back(value_type&& value)
		{
			emplace_back(std::move(value));
		}

		template< class... Args >
		reference emplace_back(Args&&... args)
		{
			if (!_has_unused_capacity())
			{
				// capacity 확장.
				_realloacate(static_cast<size_type>(_capacity * RESIZE_FACTOR));
			}
			_emplace_back_with_unused_capacity(std::forward<Args>(args)...);
			return back();
		}

		void pop_back()
		{
			if (_size <= 0) return;
			_destroy_elements_from_back(1);
		}

		void resize(size_type count)
		{
			auto create_func = [count] {
				_create_with_default_constructor(count);
			};
			_resize_elements(count, create_func);
		}

		void resize(size_type count, const value_type& value)
		{
			auto create_func = [&value, count] {
				_create_with_copy_constructor(count, value);
			};
			_resize_elements(count, create_func);
		}

		void swap(vector& other) noexcept
		{
			if (data() == other.data()) return;

			this->_swap_all(other);
		}


	private:
		vector_compressed_pair _alloc_data_pair;
		size_type _size{};
		size_type _capacity{};

		void _init_allocate(size_type capacity = INIT_CAP)
		{
			assertm(_capacity == 0, "_init_allocate can only be called when capacity is 0.");
			 
			_capacity = capacity;
			_alloc_data_pair.get_data() = std::allocator_traits<allocator_type>::allocate(get_allocator(), _capacity);
			//std::cout << _msize(_alloc_data_pair.get_data()) / sizeof(value_type) << std::endl;
		}
		
		void _realloacate(size_type new_capacity)
		{
			//std::cout << "_realloacate() -  old datd():" << data() << std::endl;

			assertm(new_capacity >= _size, "invalid capacity.");
			if (new_capacity == _capacity) return;

			if (_capacity == 0)
			{
				_init_allocate(new_capacity);
				return;
			}

			const size_type old_capacity = _capacity;
			value_type* new_data = std::allocator_traits<allocator_type>::allocate(get_allocator(), new_capacity);

			if (std::is_nothrow_move_constructible_v<value_type>)	// move에서 예외를 발생시키 지않는다면 move
			{
				for (size_type idx = 0; idx < _size; ++idx)
				{
					std::allocator_traits<allocator_type>::construct(get_allocator(), (new_data + idx), std::move(data()[idx]));
				}
			}
			else // copy construct
			{
				size_type idx = 0;
				try
				{
					for (; idx < _size; ++idx)
					{
						std::allocator_traits<allocator_type>::construct(get_allocator(), (new_data + idx), data()[idx]);
					}
				}
				catch (...)
				{
					idx--; // idx번째 생성중 예외 발생했기 때문에 idx-1 번째 부터 소멸
					for (; idx >= 0; --idx)
					{
						std::allocator_traits<allocator_type>::destroy(get_allocator(), (new_data + idx));
					}
					// 새로할당한 메모리 해제.
					std::allocator_traits<allocator_type>::deallocate(get_allocator(), new_data, new_capacity);
					throw;
				}

				for (size_type idx = 0; idx < _size; ++idx)
				{
					// 기존 값 소멸
					std::allocator_traits<allocator_type>::destroy(get_allocator(), data() + idx);
				}

			}

			// 기존 메모리 해제
			std::allocator_traits<allocator_type>::deallocate(get_allocator(), data(), old_capacity);

			_capacity = new_capacity;
			_alloc_data_pair.get_data() = new_data;
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
					std::allocator_traits< allocator_type>::construct(_allocator(), data() + idx);
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
					std::allocator_traits<allocator_type>::destroy(_allocator(), data() + idx);
				}
				throw;
			}
		}
		void _create_with_copy_constructor(size_type count, const value_type& other)
		{
			assertm(_size < _capacity, "capacity must be greater than size.");

			size_type idx = _size;
			try
			{
				_size += count;
				for (; idx < _size; ++idx)
				{
					// call copy constructor
					std::allocator_traits<allocator_type>::construct(_allocator(), (data() + idx), other);
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
					std::allocator_traits<allocator_type>::destroy(_allocator(), data() + idx);
				}
				throw;
			}
		}
		
		void _destroy_elements_from_back(size_type count)
		{
			for (size_type i = count; i > 0; --i)
			{
				std::allocator_traits<allocator_type>::destroy(_allocator(), data() - i);
			}
			_size -= count;
		}
		void _init_copy_value(size_type count, const value_type& other)
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
			//std::cout << "_relase_all() -  datd():" << data() << " count: " << _msize(data()) / sizeof(value_type) << std::endl;
			std::allocator_traits<allocator_type>::deallocate(get_allocator(), data(), _capacity);
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

		bool _has_unused_capacity()
		{
			return _capacity > _size;
		}
		template< class... Args >
		void _emplace_back_with_unused_capacity(Args&& ... args)
		{
			std::allocator_traits<allocator_type>::construct(get_allocator(), (_end_pos()), std::forward<Args>(args)...);
			_size++;
		}

		T* _end_pos() noexcept				
		{
			return (_alloc_data_pair.get_data() + _size);
		}
		const T* _end_pos() const noexcept 
		{
			return (_alloc_data_pair.get_data() + _size);
		}

		template<class Lamda>
		void _resize_elements(size_type count, Lamda create_func)
		{
			if (count > _size)
			{
				if (count > _capacity)
				{
					_realloacate(count * RESIZE_FACTOR);
				}
				// todo: create_func 에서 예외 발생할경우 어떻게 할지.
				create_func();
			}
			else if (count < _size)
			{
				_destroy_elements_from_back(_size - count);
			}
		}
		void _swap_all(vector& other)
		{
			pointer temp_data = other._alloc_data_pair.get_data();
			other._alloc_data_pair.get_data() = this->_alloc_data_pair.get_data();
			this->_alloc_data_pair.get_data() = temp_data;

			allocator_type temp_allocator = other._alloc_data_pair.get_allocator();
			other._alloc_data_pair.get_allocator() = this->_alloc_data_pair.get_allocator();
			this->_alloc_data_pair.get_allocator() = temp_allocator;

			size_type temp_size = other._size;
			other._size = this->_size;
			this->_size = temp_size;

			size_type temp_capacity = other._capacity;
			other._capacity = this->_capacity;
			this->_capacity = temp_capacity;
		}
		void _copy_data_in_unused_space(const vector& other)
		{
			size_type idx = 0;
			try
			{
				_size = other._size;
				for (; idx < _size; ++idx)
				{
					std::allocator_traits< allocator_type>::construct(get_allocator(), data() + idx, other[idx]);
				}
			}
			catch (...)
			{
				idx--; // n번째 생성중 예외 발생했기 때문에 n-1 번째 부터 소멸
				_size = 0;
				for (; idx >= 0; --idx)
				{
					std::allocator_traits<allocator_type>::destroy(get_allocator(), data() + idx);
				}
			}
		}


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

