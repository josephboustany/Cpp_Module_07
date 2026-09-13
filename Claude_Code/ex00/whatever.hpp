#ifndef WHATEVER_HPP
# define WHATEVER_HPP

/*
** swap: swaps the values of two given parameters. Returns nothing.
*/
template <typename T>
void	swap( T & a, T & b )
{
	T	tmp;

	tmp = a;
	a = b;
	b = tmp;
}

/*
** min: returns the smallest of the two parameters.
** If they are equal, returns the second one.
*/
template <typename T>
T const &	min( T const & a, T const & b )
{
	if ( a < b )
		return ( a );
	return ( b );
}

/*
** max: returns the greatest of the two parameters.
** If they are equal, returns the second one.
*/
template <typename T>
T const &	max( T const & a, T const & b )
{
	if ( b < a )
		return ( a );
	return ( b );
}

#endif
