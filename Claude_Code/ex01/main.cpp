#include <iostream>
#include <string>
#include "iter.hpp"

/*
** Takes its argument by non-const reference: it modifies the element
** it's given. Only usable on a non-const array, thanks to iter's
** non-const overload.
*/
template <typename T>
void	increment( T & elem )
{
	++elem;
}

/*
** Takes its argument by const reference: only reads it. Usable on both
** const and non-const arrays.
*/
template <typename T>
void	print( T const & elem )
{
	std::cout << elem << " ";
}

/*
** A plain (non-template) function, to show iter also works with an
** ordinary function taking its argument by const reference.
*/
void	printString( std::string const & s )
{
	std::cout << "[" << s << "] ";
}

/*
** A function object (functor), to show iter also works with an
** instantiated callable that isn't a bare function.
*/
class Doubler
{
	public:
		void	operator()( int & elem ) const
		{
			elem *= 2;
		}
};
//Anything you declare outside any namespace lives in the global namespace. :: with nothing on its left names that namespace explicitly
int	main( void )
{
	int	numbers[] = { 1, 2, 3, 4, 5 };
	int const	len = sizeof( numbers ) / sizeof( numbers[0] );

	std::cout << "-- non-const int array --" << std::endl;
	std::cout << "before: ";
	::iter( numbers, len, print<int> );
	std::cout << std::endl;

	::iter( numbers, len, increment<int> );
	std::cout << "after increment: ";
	::iter( numbers, len, print<int> );
	std::cout << std::endl;

	::iter( numbers, len, Doubler() );
	std::cout << "after doubling: ";
	::iter( numbers, len, print<int> );
	std::cout << std::endl;

	std::cout << std::endl << "-- const int array --" << std::endl;
	int const	constNumbers[] = { 10, 20, 30 };
	int const	constLen = sizeof( constNumbers ) / sizeof( constNumbers[0] );

	std::cout << "values: ";
	::iter( constNumbers, constLen, print<int> );
	std::cout << std::endl;

	std::cout << std::endl << "-- std::string array --" << std::endl;
	std::string	words[] = { "chaine1", "chaine2", "chaine3" };
	int const	wordsLen = sizeof( words ) / sizeof( words[0] );

	::iter( words, wordsLen, printString );
	std::cout << std::endl;

	return ( 0 );
}
