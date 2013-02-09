// BasisVec.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include "BasisVec.h"
#include "Environment.h"

//=========================================================================================
GALuaBasisVec::GALuaBasisVec( const char* name )
{
	int nameLen = strlen( name ) + 1;
	this->name = new char[ nameLen ];
	strcpy_s( this->name, nameLen, name );
}

//=========================================================================================
/*virtual*/ GALuaBasisVec::~GALuaBasisVec( void )
{
	delete[] name;
	name = 0;
}

//=========================================================================================
/*virtual*/ Utilities::List::Item* GALuaBasisVec::MakeCopy( void ) const
{
	// This would be a bit faster if we ref-counted strings.
	return new GALuaBasisVec( name );
}

//=========================================================================================
// The bar functionality can be implemented using GA calculations, but we provide
// this implementation, because it is much, much faster!
/*virtual*/ GeometricAlgebra::Vector* GALuaBasisVec::MakeBar( ScalarAlgebra::Scalar& sign ) const
{
	// Which basis vector are we dealing with here?
	int i;
	if( !gaLuaEnv->LookupBasisVec( name, i ) )
		return 0;

	// Which basis vector do we map to?
	int j, sign_int;
	if( !gaLuaEnv->BarMapGet( i, j, sign_int ) )
		return 0;
	sign = double( sign_int );
	const char* basisVecName;
	if( !gaLuaEnv->LookupBasisVec( j, basisVecName ) )
		return 0;

	// Return the mapped basis vector.
	return new GALuaBasisVec( basisVecName );
}

//=========================================================================================
/*virtual*/ double GALuaBasisVec::InnerProduct( const Vector& right ) const
{
	// Which basis vectors are we dealing with here?
	int i, j;
	if( !gaLuaEnv->LookupBasisVec( name, i ) )
		return 0.0;
	if( !gaLuaEnv->LookupBasisVec( right.Name(), j ) )
		return 0.0;

	// Lookup their inner product in the environment's table.
	double scalar;
	if( !gaLuaEnv->GetBasisVecIpTableEntry( i, j, scalar ) )
		return 0.0;

	// Here you go!
	return scalar;
}

//=========================================================================================
/*virtual*/ const char* GALuaBasisVec::Name( void ) const
{
	return name;
}

//=========================================================================================
/*virtual*/ const char* GALuaBasisVec::LatexName( void ) const
{
	return 0;
}

// BasisVec.cpp