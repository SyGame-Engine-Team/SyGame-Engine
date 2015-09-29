/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Offset3.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 3D displacement
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Offset3.h"

/////////////////////////////////////////////////////////////////////////////////
// Offset3 implementation
const Offset3 Offset3::Null = Offset3(0, 0, 0);
const Offset3 Offset3::eI = Offset3(1, 0, 0);
const Offset3 Offset3::eJ = Offset3(0, 1, 0);
const Offset3 Offset3::eK = Offset3(0, 0, 1);

Offset3::Offset3()
{
    // nothing to do
}
Offset3::Offset3(Int x, Int y, Int z)
{
    X = x;
    Y = y;
    Z = z;
}
Offset3::Offset3(const Int vArr[3])
{
    X = vArr[0];
    Y = vArr[1];
    Z = vArr[2];
}
Offset3::Offset3(const Offset3 & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
}
Offset3::~Offset3()
{
    // nothing to do
}

Void Offset3::OrthoNormalize( Offset3 & vI, Offset3 & vJ, Offset3 & vK )
{
    // Gram-Schmidt OrthoNormalization
    vI.Normalize();

    Int iDotI = (vI * vJ);
    vJ -= (vI * iDotI);
    vJ.Normalize();

    Int iDotJ = (vJ * vK);
    iDotI = (vI * vK);
    vK -= ( (vI * iDotI) + (vJ * iDotJ) );
    vK.Normalize();
}
Void Offset3::MakeComplementBasis( Offset3 & vI, Offset3 & vJ, const Offset3 & vK )
{
    Scalar fInvNorm;
    Int iAbsX = ( vK.X < 0 ) ? -(vK.X) : vK.X;
    Int iAbsY = ( vK.Y < 0 ) ? -(vK.Y) : vK.Y;
    if ( iAbsX >= iAbsY ) {
        fInvNorm = MathFn->InvSqrt( (Scalar)( (vK.X * vK.X) + (vK.Z * vK.Z) ) );
        vI.X = -MathFn->Floor(fInvNorm * (Scalar)vK.Z);
        vI.Y = 0;
        vI.Z = +MathFn->Floor(fInvNorm * (Scalar)vK.X);
        vJ.X = (vK.Y * vI.Z);
        vJ.Y = ( (vK.Z * vI.X) - (vK.X * vI.Z) );
        vJ.Z = -(vK.Y * vI.X);
    } else {
        fInvNorm = MathFn->InvSqrt( (Scalar)( (vK.Y * vK.Y) + (vK.Z * vK.Z) ) );
        vI.X = 0;
        vI.Y = +MathFn->Floor(fInvNorm * (Scalar)vK.Z);
        vI.Z = -MathFn->Floor(fInvNorm * (Scalar)vK.Y);
        vJ.X = ( (vK.Y * vI.Z) - (vK.Z * vI.Y) );
        vJ.Y = -(vK.X * vI.Z);
        vJ.Z = (vK.X * vI.Y);
    }
}

