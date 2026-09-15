#include <iostream>
#include <string>
#include "Array.hpp"

// Prints every element of a const Array<int> alongside its size, proving
// operator[] const and size() are both usable on a const reference.
static void	printIntArray( std::string const & label, Array<int> const & arr )
{
	std::cout << label << " (size " << arr.size() << "): ";
	for ( unsigned int i = 0; i < arr.size(); ++i )
		std::cout << arr[i] << " ";
	std::cout << std::endl;
}

int	main( void )
{
	std::cout << "-- default construction (empty array) --" << std::endl;
	Array<int>	empty;
	std::cout << "empty.size() = " << empty.size() << std::endl;

	std::cout << std::endl << "-- construction with n, default-initialized --" << std::endl;
	Array<int>	ints( 5 );
	printIntArray( "ints", ints );

	Array<std::string>	strings( 3 );
	std::cout << "strings (size " << strings.size() << "): ";
	for ( unsigned int i = 0; i < strings.size(); ++i )
		std::cout << "[" << strings[i] << "] ";
	std::cout << std::endl;

	std::cout << std::endl << "-- filling ints, then copy-constructing --" << std::endl;
	for ( unsigned int i = 0; i < ints.size(); ++i )
		ints[i] = static_cast<int>( i ) * 10;
	printIntArray( "ints before copy", ints );

	Array<int>	copyOfInts( ints );
	printIntArray( "copyOfInts (fresh copy)", copyOfInts );

	copyOfInts[0] = 999;
	ints[1] = -1;
	std::cout << "-- after mutating one element in each --" << std::endl;
	printIntArray( "ints", ints );
	printIntArray( "copyOfInts", copyOfInts );

	std::cout << std::endl << "-- assignment operator --" << std::endl;
	Array<int>	assigned( 2 );
	assigned = ints;
	printIntArray( "assigned (after = ints)", assigned );

	assigned[2] = 4242;
	ints[2] = -2;
	std::cout << "-- after mutating one element in each --" << std::endl;
	printIntArray( "ints", ints );
	printIntArray( "assigned", assigned );

	std::cout << std::endl << "-- self-assignment --" << std::endl;
	Array<int> *	selfPtr = &assigned;
	assigned = *selfPtr;
	printIntArray( "assigned (after self-assignment)", assigned );

	std::cout << std::endl << "-- out-of-bounds access (positive index) --" << std::endl;
	try
	{
		std::cout << ints[100] << std::endl;
	}
	catch ( std::exception & e )
	{
		std::cout << "caught exception: " << e.what() << std::endl;
	}

	std::cout << std::endl << "-- out-of-bounds access (negative index) --" << std::endl;
	try
	{
		std::cout << ints[-1] << std::endl;
	}
	catch ( std::exception & e )
	{
		std::cout << "caught exception: " << e.what() << std::endl;
	}

	std::cout << std::endl << "-- out-of-bounds access on an empty array --" << std::endl;
	try
	{
		std::cout << empty[0] << std::endl;
	}
	catch ( std::exception & e )
	{
		std::cout << "caught exception: " << e.what() << std::endl;
	}

	std::cout << std::endl << "-- const array access through const &, size() untouched --" << std::endl;
	printIntArray( "ints (via const ref)", ints );

	return ( 0 );
}
