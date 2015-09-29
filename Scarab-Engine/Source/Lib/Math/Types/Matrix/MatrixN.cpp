/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/MatrixN.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic NxM-dimensional matrix
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
#include "MatrixN.h"

/////////////////////////////////////////////////////////////////////////////////
// TMatrixN implementation

//template <typename Real>
//bool LinearSystem<Real>::Solve (const GMatrix<Real>& A, const Real* B,
//    Real* X)
//{
//    // Computations are performed in-place.
//    int size = A.GetColumns();
//    GMatrix<Real> invA = A;
//    memcpy(X, B, size*sizeof(Real));
//
//    int* colIndex = new1<int>(size);
//    int* rowIndex = new1<int>(size);
//    bool* pivoted = new1<bool>(size);
//    memset(pivoted, 0, size*sizeof(bool));
//
//    int i1, i2, row = 0, col = 0;
//    Real save;
//
//    // Elimination by full pivoting.
//    for (int i0 = 0; i0 < size; i0++)
//    {
//        // Search matrix (excluding pivoted rows) for maximum absolute entry.
//        Real maxValue = (Real)0;
//        for (i1 = 0; i1 < size; ++i1)
//        {
//            if (!pivoted[i1])
//            {
//                for (i2 = 0; i2 < size; ++i2)
//                {
//                    if (!pivoted[i2])
//                    {
//                        Real absValue = Math<Real>::FAbs(invA[i1][i2]);
//                        if (absValue > maxValue)
//                        {
//                            maxValue = absValue;
//                            row = i1;
//                            col = i2;
//                        }
//                    }
//                }
//            }
//        }
//
//        if (maxValue == (Real)0)
//        {
//            // The matrix is not invertible.
//            delete1(colIndex);
//            delete1(rowIndex);
//            delete1(pivoted);
//            return false;
//        }
//
//        pivoted[col] = true;
//
//        // Swap rows so that A[col][col] contains the pivot entry.
//        if (row != col)
//        {
//            invA.SwapRows(row,col);
//
//            save = X[row];
//            X[row] = X[col];
//            X[col] = save;
//        }
//
//        // Keep track of the permutations of the rows.
//        rowIndex[i0] = row;
//        colIndex[i0] = col;
//
//        // Scale the row so that the pivot entry is 1.
//        Real inv = ((Real)1.0)/invA[col][col];
//        invA[col][col] = (Real)1;
//        for (i2 = 0; i2 < size; ++i2)
//        {
//            invA[col][i2] *= inv;
//        }
//        X[col] *= inv;
//
//        // Zero out the pivot column locations in the other rows.
//        for (i1 = 0; i1 < size; ++i1)
//        {
//            if (i1 != col)
//            {
//                save = invA[i1][col];
//                invA[i1][col] = (Real)0;
//                for (i2 = 0; i2 < size; i2++)
//                {
//                    invA[i1][i2] -= invA[col][i2]*save;
//                }
//                X[i1] -= X[col]*save;
//            }
//        }
//    }
//
//    // Reorder rows so that A[][] stores the inverse of the original matrix.
//    for (i1 = size-1; i1 >= 0; --i1)
//    {
//        if (rowIndex[i1] != colIndex[i1])
//        {
//            for (i2 = 0; i2 < size; ++i2)
//            {
//                save = invA[i2][rowIndex[i1]];
//                invA[i2][rowIndex[i1]] = invA[i2][colIndex[i1]];
//                invA[i2][colIndex[i1]] = save;
//            }
//        }
//    }
//
//    delete1(colIndex);
//    delete1(rowIndex);
//    delete1(pivoted);
//    return true;
//}
