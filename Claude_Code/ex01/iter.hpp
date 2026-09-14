#ifndef ITER_HPP
# define ITER_HPP

# include <cstddef>

/*
** iter: calls the function f on every element of the array, in order.
**
** Two overloads are provided so both const and non-const arrays are
** supported:
**   - the non-const overload deduces T as the array's real (non-const)
**     type, so f can take its argument by non-const reference and
**     modify the elements in place.
**   - the const overload is the only one that can match a
**     const-qualified array, and forces f to receive its argument by
**     value or by const reference, since the elements can't be modified.
** Overload resolution picks whichever matches the actual constness of
** the array that is passed in.
*/
template <typename T, typename F>
void	iter( T * array, std::size_t const len, F f )
{
	for ( std::size_t i = 0; i < len; ++i )
		f( array[i] );
}

template <typename T, typename F>
void	iter( T const * array, std::size_t const len, F f )
{
	for ( std::size_t i = 0; i < len; ++i )
		f( array[i] );
}

#endif
