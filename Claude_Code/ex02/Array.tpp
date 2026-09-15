#ifndef ARRAY_TPP
# define ARRAY_TPP

// Default constructor: empty array. Still goes through operator new[]
// (with 0 elements) so _elements is always a valid, delete[]-able pointer.
template <typename T>
Array<T>::Array( void ) : _elements( new T[0] ), _size( 0 )
{
}

// Constructs an array of n elements. new T[n] already default-constructs
// class-type elements, but leaves built-in types (int, char, ...) with an
// indeterminate value, so every element is explicitly assigned T() to give
// it a defined default either way.
template <typename T>
Array<T>::Array( unsigned int n ) : _elements( new T[n] ), _size( n )
{
	for ( unsigned int i = 0; i < _size; ++i )
		_elements[i] = T();
}

// Copy constructor: allocates its own array and copies each element
// individually, so the copy owns independent storage from src.
template <typename T>
Array<T>::Array( Array const & src ) : _elements( new T[src._size] ), _size( src._size )
{
	for ( unsigned int i = 0; i < _size; ++i )
		_elements[i] = src._elements[i];
}

// Copy assignment: builds the replacement array fully before touching
// this object's own storage, so a failed allocation never leaves *this
// half-modified; the self-assignment guard skips all of that entirely.
template <typename T>
Array<T> &	Array<T>::operator=( Array const & rhs )
{
	if ( this != &rhs )
	{
		T *	newElements = new T[rhs._size];

		for ( unsigned int i = 0; i < rhs._size; ++i )
			newElements[i] = rhs._elements[i];
		delete [] _elements;
		_elements = newElements;
		_size = rhs._size;
	}
	return ( *this );
}

// Destructor: releases the array allocated by whichever constructor or
// operator= built this instance's current storage.
template <typename T>
Array<T>::~Array( void )
{
	delete [] _elements;
}

// Non-const subscript: throws if index is out of bounds (this also
// catches a negative int argument, since it implicitly converts to a huge
// unsigned int before reaching this check), otherwise returns a mutable
// reference to the element.
template <typename T>
T &	Array<T>::operator[]( unsigned int index )
{
	if ( index >= _size )
		throw ( Array<T>::OutOfBoundsException() );
	return ( _elements[index] );
}

// Const subscript: same bounds check as the non-const overload, but
// returns a const reference so it's usable on a const Array and can't
// modify the element.
template <typename T>
T const &	Array<T>::operator[]( unsigned int index ) const
{
	if ( index >= _size )
		throw ( Array<T>::OutOfBoundsException() );
	return ( _elements[index] );
}

// Returns the number of elements. const and parameterless per the
// subject: it only reads _size, never modifies the instance.
template <typename T>
unsigned int	Array<T>::size( void ) const
{
	return ( _size );
}

// OutOfBoundsException::what(): the only member this exception needs,
// since it holds no data of its own (exempt from Orthodox Canonical Form).
template <typename T>
const char *	Array<T>::OutOfBoundsException::what( void ) const throw()
{
	return ( "Array: index out of bounds" );
}

#endif
