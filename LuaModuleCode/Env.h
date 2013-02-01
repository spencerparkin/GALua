// Env.h

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
	int BasisVecCount( void );

	void DeleteBasisVecIpTable( void );
	bool SetBasisVecIpTableEntry( int i, int j, double scalar );
	bool GetBasisVecIpTableEntry( int i, int j, double& scalar );

private:

	int basisVecCount;
	Utilities::Map< int > basisVecMap;
	double** basisVecIpTable;
};

//=========================================================================================
extern GALuaEnv* gaLuaEnv;

// Env.h