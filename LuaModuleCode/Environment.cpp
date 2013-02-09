// Environment.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

//=========================================================================================
#include "Environment.h"
#include "BasisVec.h"

//=========================================================================================
IMPLEMENT_CALCLIB_CLASS1( GALuaEnv, GeometricAlgebraEnvironment );

//=========================================================================================
GALuaEnv* gaLuaEnv = 0;

//=========================================================================================
GALuaEnv::GALuaEnv( void )
{
	basisVecCount = 0;
	basisVecMapByIndex = 0;
	basisVecIpTable = 0;
	barMap = 0;
}

//=========================================================================================
/*virtual*/ GALuaEnv::~GALuaEnv( void )
{
	DeleteBasisVecMap();
	DeleteBasisVecIpTable();
	DeleteBarMap();
}

//=========================================================================================
/*virtual*/ void GALuaEnv::PrintEnvironmentInfo( void )
{
}

//=========================================================================================
/*virtual*/ CalcLib::FunctionEvaluator* GALuaEnv::CreateFunction( const char* functionName )
{
	return 0;
}

//=========================================================================================
/*virtual*/ CalcLib::Evaluator* GALuaEnv::CreateVariable( const char* variableName )
{
	if( !basisVecMapByName.Lookup( variableName ) )
		return 0;

	GeometricAlgebra::Vector* basisVec = new GALuaBasisVec( variableName );
	CalcLib::MultivectorNumber* multivectorNumber = new CalcLib::MultivectorNumber( *basisVec );
	CalcLib::Evaluator* evaluator = new CalcLib::ConstantEvaluator( multivectorNumber );
	return evaluator;
}

//=========================================================================================
/*virtual*/ CalcLib::Evaluator* GALuaEnv::CreateBinaryOperator( const char* operatorName, CalcLib::Evaluator* leftOperand, CalcLib::Evaluator* rightOperand, bool& isBinaryOperationEvaluator )
{
	return GeometricAlgebraEnvironment::CreateBinaryOperator( operatorName, leftOperand, rightOperand, isBinaryOperationEvaluator );
}

//=========================================================================================
/*virtual*/ CalcLib::Evaluator* GALuaEnv::CreateUnaryOperator( const char* operatorName, CalcLib::Evaluator* operand, bool& isUnaryOperationEvaluator )
{
	return GeometricAlgebraEnvironment::CreateUnaryOperator( operatorName, operand, isUnaryOperationEvaluator );
}

//=========================================================================================
/*virtual*/ CalcLib::Number* GALuaEnv::CreateNumber( void )
{
	return GeometricAlgebraEnvironment::CreateNumber();
}

//=========================================================================================
void GALuaEnv::DeleteBasisVecMap( void )
{
	basisVecMapByName.RemoveAll();
	if( basisVecMapByIndex )
	{
		for( int index = 0; index < basisVecCount; index++ )
			delete[] basisVecMapByIndex[ index ];
		delete[] basisVecMapByIndex;
		basisVecMapByIndex = 0;
	}
}

//=========================================================================================
bool GALuaEnv::RegisterBasisVec( const char* basisVec )
{
	if( basisVecMapByName.Lookup( basisVec ) )
		return false;

	int index = basisVecCount++;
	if( !basisVecMapByName.Insert( basisVec, index ) )
		return false;

	return true;
}

//=========================================================================================
bool GALuaEnv::LookupBasisVec( const char* basisVec, int& index )
{
	return basisVecMapByName.Lookup( basisVec, &index );
}

//=========================================================================================
bool GALuaEnv::LookupBasisVec( int index, const char*& basisVec )
{
	if( index < 0 || index > basisVecCount )
		return false;

	if( !basisVecMapByIndex )
	{
		basisVecMapByIndex = new char*[ basisVecCount ];
		for( int i = 0; i < basisVecCount; i++ )
			basisVecMapByIndex[i] = 0;
		Utilities::Map< int >::Iterator iterator( &basisVecMapByName );
		while( !iterator.Finished() )
		{
			const char* basisVecNameSrc;
			int index = iterator.CurrentEntry( &basisVecNameSrc );
			int nameLen = strlen( basisVecNameSrc ) + 1;
			char* basisVecNameDst = new char[ nameLen ];
			strcpy_s( basisVecNameDst, nameLen, basisVecNameSrc );
			basisVecMapByIndex[ index ] = basisVecNameDst;
			iterator.Next();
		}
	}

	basisVec = basisVecMapByIndex[ index ];
	return true;
}

//=========================================================================================
int GALuaEnv::BasisVecCount( void )
{
	return basisVecCount;
}

//=========================================================================================
void GALuaEnv::DeleteBasisVecIpTable( void )
{
	if( basisVecIpTable )
	{
		for( int i = 0; i < basisVecCount; i++ )
			delete[] basisVecIpTable[i];
		delete[] basisVecIpTable;
		basisVecIpTable = 0;
	}
}

//=========================================================================================
bool GALuaEnv::SetBasisVecIpTableEntry( int i, int j, double scalar )
{
	if( basisVecCount == 0 )
		return false;
	if( i < 0 || i > basisVecCount - 1 )
		return false;
	if( j < 0 || j > basisVecCount - 1 )
		return false;
	
	if( !basisVecIpTable )
	{
		basisVecIpTable = new double*[ basisVecCount ];
		int i, j;
		for( i = 0; i < basisVecCount; i++ )
		{
			basisVecIpTable[i] = new double[ basisVecCount ];
			for( j = 0; j < basisVecCount; j++ )
				basisVecIpTable[i][j] = 0.0;
		}
	}

	basisVecIpTable[i][j] = scalar;
	return true;
}

//=========================================================================================
bool GALuaEnv::GetBasisVecIpTableEntry( int i, int j, double& scalar )
{
	if( basisVecCount == 0 || !basisVecIpTable )
		return false;
	if( i < 0 || i > basisVecCount - 1 )
		return false;
	if( j < 0 || j > basisVecCount - 1 )
		return false;

	scalar = basisVecIpTable[i][j];
	return true;
}

//=========================================================================================
void GALuaEnv::DeleteBarMap( void )
{
	delete[] barMap;
	barMap = 0;
}

//=========================================================================================
bool GALuaEnv::BarMapSet( int i, int j, int sign )
{
	if( basisVecCount == 0 )
		return false;
	if( i < 0 || i > basisVecCount - 1 )
		return false;
	if( j < 0 || j > basisVecCount - 1 )
		return false;
	if( sign != 1 && sign != -1 )
		return false;

	if( !barMap )
	{
		barMap = new BarMapEntry[ basisVecCount ];
		for( int k = 0; k < basisVecCount; k++ )
		{
			BarMapEntry* entry = &barMap[k];
			entry->index = -1;
			entry->sign = 0;
		}
	}

	barMap[i].index = j;
	barMap[i].sign = sign;
	return true;
}

//=========================================================================================
bool GALuaEnv::BarMapGet( int i, int& j, int& sign )
{
	if( i < 0 || i > basisVecCount - 1 )
		return false;

	BarMapEntry* entry = &barMap[i];
	j = entry->index;
	sign = entry->sign;
	return true;
}

// Environment.cpp