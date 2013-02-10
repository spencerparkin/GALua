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
Utilities::Map< GALuaBasisVec::BasisVecNameData* > GALuaBasisVec::basisVecNameDataCache;
bool GALuaBasisVec::deleteNameDataOnZeroRefCount = true;

//=========================================================================================
/*static*/ void GALuaBasisVec::WipeBasisVecNameCache( bool requireZeroRefCount /*= true*/ )
{
	// Requiring a zero count is obviously important, because if we don't
	// require such a count and there are active referencers out there,
	// they could cause a crash once we've deleted all the name data.
	if( !requireZeroRefCount )
		basisVecNameDataCache.DeleteAndRemoveAll();
	else
	{
		while( basisVecNameDataCache.Count() > 0 )
		{
			// Find the first unreferenced cache entry.
			BasisVecNameData* foundBasisVecNameData = 0;
			Utilities::Map< BasisVecNameData* >::Iterator iterator( &basisVecNameDataCache );
			while( !iterator.Finished() && !foundBasisVecNameData )
			{
				const char* name;
				BasisVecNameData* basisVecNameData = iterator.CurrentEntry( &name );
				if( basisVecNameData->refCount == 0 )
					foundBasisVecNameData = basisVecNameData;
				else
					iterator.Next();
			}
			
			// If there wasn't one, we're done.
			if( !foundBasisVecNameData )
				break;

			// Remove and delete the entry.
			basisVecNameDataCache.Remove( foundBasisVecNameData->name );
			delete foundBasisVecNameData;
		}
	}
}

//=========================================================================================
GALuaBasisVec::BasisVecNameData::BasisVecNameData( const char* name )
{
	// Nobody references us yet.
	refCount = 0;

	// Copy the name.
	int nameLen = strlen( name ) + 1;
	this->name = new char[ nameLen ];
	strcpy_s( this->name, nameLen, name );

	// We should probably arrange to have the user tell us what
	// the corresponding latex name should be, but for now,
	// let's just formulate it in terms of the given name.
	bool bar = ( name[ nameLen - 2 ] == 'b' );
	char buffer0[128], buffer1[128];
	if( bar )
		strncpy_s( buffer0, sizeof( buffer0 ), name, nameLen - 2 );
	else
		strcpy_s( buffer0, sizeof( buffer0 ), name );
	const char* nullVec = ( 0 == strcmp( buffer0, "no" ) ) ? "o" : 0;
	if( !nullVec )
		nullVec = ( 0 == strcmp( buffer0, "ni" ) ) ? "\\infty" : 0;
	nameLen = 128;
	latexName = new char[ nameLen ];
	if( nullVec )
	{
		if( bar )
			sprintf_s( latexName, nameLen, "\\overbar{%s}", nullVec );
		else
			strcpy_s( latexName, nameLen, nullVec );
	}
	else
	{
		char firstChar = buffer0[0];
		strcpy_s( buffer1, sizeof( buffer1 ), buffer0 + 1 );	
		if( bar )
			sprintf_s( latexName, nameLen, "\\overbar{%c}_{%s}", firstChar, buffer1 );
		else
			sprintf_s( latexName, nameLen, "%c_{%s}", firstChar, buffer1 );
	}
}

//=========================================================================================
/*virtual*/ GALuaBasisVec::BasisVecNameData::~BasisVecNameData( void )
{
	delete[] name;
	name = 0;
	delete[] latexName;
	latexName = 0;
}

//=========================================================================================
GALuaBasisVec::GALuaBasisVec( const char* name )
{
	// Share pointers to name data so that we're not thrashing the memory manager.
	if( !basisVecNameDataCache.Lookup( name, &basisVecNameData ) )
	{
		basisVecNameData = new BasisVecNameData( name );
		basisVecNameDataCache.Insert( name, basisVecNameData );
	}
	basisVecNameData->refCount++;
}

//=========================================================================================
/*virtual*/ GALuaBasisVec::~GALuaBasisVec( void )
{
	if( basisVecNameData )
	{
		basisVecNameData->refCount--;

		if( deleteNameDataOnZeroRefCount && basisVecNameData->refCount == 0 )
		{
			// TODO: We never get in here.  Where's the leak???
			basisVecNameDataCache.Remove( basisVecNameData->name );
			delete basisVecNameData;
		}

		basisVecNameData = 0;
	}
}

//=========================================================================================
/*virtual*/ Utilities::List::Item* GALuaBasisVec::MakeCopy( void ) const
{
	// This would be a bit faster if we ref-counted strings.
	return new GALuaBasisVec( Name() );
}

//=========================================================================================
// The bar functionality can be implemented using GA calculations, but we provide
// this implementation, because it is much, much faster!
/*virtual*/ GeometricAlgebra::Vector* GALuaBasisVec::MakeBar( ScalarAlgebra::Scalar& sign ) const
{
	// Which basis vector are we dealing with here?
	int i;
	if( !gaLuaEnv->LookupBasisVec( Name(), i ) )
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
	if( !gaLuaEnv->LookupBasisVec( Name(), i ) )
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
	return basisVecNameData->name;
}

//=========================================================================================
/*virtual*/ const char* GALuaBasisVec::LatexName( void ) const
{
	return basisVecNameData->latexName;
}

// BasisVec.cpp