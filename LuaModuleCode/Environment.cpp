// Environment.cpp

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
	basisVecIpTable = 0;
}

//=========================================================================================
/*virtual*/ GALuaEnv::~GALuaEnv( void )
{
	DeleteBasisVecMap();
	DeleteBasisVecIpTable();
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
	if( !basisVecMap.Lookup( variableName ) )
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
	basisVecMap.RemoveAll();
}

//=========================================================================================
bool GALuaEnv::RegisterBasisVec( const char* basisVec )
{
	if( basisVecMap.Lookup( basisVec ) )
		return false;
	return basisVecMap.Insert( basisVec, basisVecCount++ );
}

//=========================================================================================
bool GALuaEnv::LookupBasisVec( const char* basisVec, int& index )
{
	return basisVecMap.Lookup( basisVec, &index );
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

// Environment.cpp