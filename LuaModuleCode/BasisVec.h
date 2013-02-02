// BasisVec.h

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