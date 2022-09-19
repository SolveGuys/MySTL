#pragma once
#include <cstddef>

namespace my
{
	// iterator is the base class provided to simplify definitions of the required types for iterators.
	template<
		class Category,
		class T,
		class Distance = std::ptrdiff_t,
		class Pointer = T* ,
		class Reference = T&
	> struct base_iterator 
	{
		using iterator_category = Category;
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
		
		//포인터를 위한 iterator_traits
		// random_access_iterator_tag iterator_category;
		// _Ty value_type;
		// ptrdiff_t difference_type;
		// ptrdiff_t distance_type;    
		// _Ty* pointer;
		// _Ty& reference;
		
		base_iterator() 
			:ptr(nullptr)
		{} // default is nullptr

		base_iterator(pointer inPtr)
			:ptr(inPtr)
		{}
	protected:
		pointer ptr;
	};


	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };
}

/* 이후 
//input_iterator_tag ??
template< class IteratorType >
void Advance( IteratorType _iterator, int _offset, input_iterator_tag )
{
	while( --_offset )
		++_iterator;
}
//random_access_iterator_tag
template< class IteratorType >
void Advance( IteratorType _iterator, int _offset, random_access_iterator_tag )
{
	_iterator += _offset;
}
//bidirectional_iterator_tag
template< class IteratorType >
void Advance( IteratorType _iterator, int _offset, bidirectional_iterator_tag)
{
	if(_offset>=0){
	while
}
template< class IteratorType >
void Advance( IteratorType _iterator, int _offset )
{
	Advance( _iterator, _offset, typename IteratorType::iterator_category() );
}

*/
