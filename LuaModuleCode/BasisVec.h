// BasisVec.h

#pragma once

//=========================================================================================
#include "Calculator/CalcLib.h"

//=========================================================================================
class GALuaBasisVec : public GeometricAlgebra::Vector
{
public:

	GALuaBasisVec( const char* name );
	virtual ~GALuaBasisVec( void );

	virtual Item* MakeCopy( void ) const override;
	virtual Vector* MakeBar( ScalarAlgebra::Scalar& sign ) const override;
	virtual double InnerProduct( const Vector& right ) const override;
	virtual const char* Name( void ) const override;
	virtual const char* LatexName( void ) const override;

	char* name;
};

// BasisVec.h