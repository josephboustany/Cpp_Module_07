#ifndef ARRAY_HPP
# define ARRAY_HPP

# include <exception>

/*
** Array<T>: a fixed-size, dynamically-allocated array of T.
**
** Every constructor allocates through operator new[] exactly once, for
** exactly as many elements as are needed right then (no preventive
** allocation). The parameterized constructor default-initializes every
** element by assigning it T() right after allocation: new T[n] alone
** already default-constructs class-type elements, but leaves built-in
** types (int, char, ...) with an indeterminate value, so the explicit
** T() assignment is what actually gives every element a defined default
** for both kinds of T.
*/
template <typename T>
class Array
{
	public:
		Array( void );
		Array( unsigned int n );
		Array( Array const & src );
		Array &	operator=( Array const & rhs );
		~Array( void );

		T &			operator[]( unsigned int index );
		T const &	operator[]( unsigned int index ) const;
		unsigned int	size( void ) const;

		class OutOfBoundsException : public std::exception
		{
			public:
				virtual const char *	what( void ) const throw();
		};

	private:
		T *				_elements;
		unsigned int	_size;
};

# include "Array.tpp"

#endif
