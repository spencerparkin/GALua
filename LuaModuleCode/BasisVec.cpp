// BasisVec.cpp

#include "BasisVec.h"
#include "Env.h"

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
/*virtual*/ GeometricAlgebra::Vector* GALuaBasisVec::MakeBar( ScalarAlgebra::Scalar& sign ) const
{
	return 0;
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