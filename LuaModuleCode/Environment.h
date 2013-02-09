// Environment.h

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#pragma once

//=========================================================================================
#include "Calculator/CalcLib.h"

//=========================================================================================
class GALuaEnv : public CalcLib::GeometricAlgebraEnvironment
{
	DECLARE_CALCLIB_CLASS( GALuaEnv );

public:

	GALuaEnv( void );
	virtual ~GALuaEnv( void );

	virtual void PrintEnvironmentInfo( void ) override;
	virtual CalcLib::FunctionEvaluator* CreateFunction( const char* functionName ) override;
	virtual CalcLib::Evaluator* CreateVariable( const char* variableName ) override;
	virtual CalcLib::Evaluator* CreateBinaryOperator( const char* operatorName, CalcLib::Evaluator* leftOperand, CalcLib::Evaluator* rightOperand, bool& isBinaryOperationEvaluator ) override;
	virtual CalcLib::Evaluator* CreateUnaryOperator( const char* operatorName, CalcLib::Evaluator* operand, bool& isUnaryOperationEvaluator ) override;
	virtual CalcLib::Number* CreateNumber( void ) override;

	void DeleteBasisVecMap( void );
	bool RegisterBasisVec( const char* basisVec );
	bool LookupBasisVec( const char* basisVec, int& index );
	bool LookupBasisVec( int index, const char*& basisVec );
	int BasisVecCount( void );

	void DeleteBasisVecIpTable( void );
	bool SetBasisVecIpTableEntry( int i, int j, double scalar );
	bool GetBasisVecIpTableEntry( int i, int j, double& scalar );

	void DeleteBarMap( void );
	bool BarMapSet( int i, int j, int sign );
	bool BarMapGet( int i, int& j, int& sign );

private:

	// Yes, we could combine these into a single integer, by using
	// the sign of the integer as the sign and the magnitude of the
	// integer as an index, but that requires us to use 1-based, not
	// 0-based indices.  To keep things simpler, I'm sticking to 0-based
	// indices in C++, while using 1-based indices in Lua.
	struct BarMapEntry
	{
		int index;
		int sign;
	};

	int basisVecCount;
	Utilities::Map< int > basisVecMapByName;
	char** basisVecMapByIndex;
	double** basisVecIpTable;
	BarMapEntry* barMap;
};

//=========================================================================================
extern GALuaEnv* gaLuaEnv;

// Environment.h