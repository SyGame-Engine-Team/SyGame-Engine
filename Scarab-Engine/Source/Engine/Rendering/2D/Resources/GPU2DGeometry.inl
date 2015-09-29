/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DGeometry.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Geometry
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPU2DGeometry implementation
inline Bool GPU2DGeometry::IsBound() const {
    return m_pDevice2DGeometry->IsCreated();
}

inline Void GPU2DGeometry::UnBind() {
    Assert( IsBound() );
    m_pDevice2DGeometry->Destroy();
}

inline GPU2DGeometryType GPU2DGeometry::GetType() const {
    return (GPU2DGeometryType)( m_pDevice2DGeometry->GetType() );
}

inline Void GPU2DGeometry::GetAABRect( GPU2DRectangle * outBounds, const GPU2DMatrix32 * pWorldTransform ) const {
    Assert( IsBound() );
    m_pDevice2DGeometry->GetAABRect( (Device2DRectangle*)outBounds, (const Device2DMatrix32 *)pWorldTransform );
}
inline Void GPU2DGeometry::GetWidenedAABRect( GPU2DRectangle * outBounds, const GPU2DStrokeStyle * pStrokeStyle, Float fStrokeWidth, const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    Assert( pStrokeStyle->IsBound() );
    m_pDevice2DGeometry->GetWidenedAABRect( (Device2DRectangle*)outBounds, &(pStrokeStyle->m_hDevice2DStrokeStyle), fStrokeWidth, (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}

inline Bool GPU2DGeometry::FillContainsPoint( const GPU2DPoint * pPoint, const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    return m_pDevice2DGeometry->FillContainsPoint( (const Device2DPoint *)pPoint, (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}
inline Bool GPU2DGeometry::StrokeContainsPoint( const GPU2DPoint * pPoint, const GPU2DStrokeStyle * pStrokeStyle, Float fStrokeWidth, const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    Assert( pStrokeStyle->IsBound() );
    return m_pDevice2DGeometry->StrokeContainsPoint( (const Device2DPoint *)pPoint, &(pStrokeStyle->m_hDevice2DStrokeStyle), fStrokeWidth, (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}

inline Float GPU2DGeometry::ComputeLength( const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    return m_pDevice2DGeometry->ComputeLength( (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}
inline Float GPU2DGeometry::ComputeArea( const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    return m_pDevice2DGeometry->ComputeArea( (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}
inline Void GPU2DGeometry::ComputePointAtLength( GPU2DPoint * outPoint, GPU2DPoint * outUnitTangent, Float fLength, const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    m_pDevice2DGeometry->ComputePointAtLength( (Device2DPoint*)outPoint, (Device2DPoint*)outUnitTangent, fLength, (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}

inline Void GPU2DGeometry::CompareWithGeometry( GPU2DGeometryRelation * outRelation, const GPU2DGeometry * pInputGeometry, const GPU2DMatrix32 * pInputWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    Assert( pInputGeometry->IsBound() );
    m_pDevice2DGeometry->CompareWithGeometry( (Device2DGeometryRelation*)outRelation, pInputGeometry->m_pDevice2DGeometry, (const Device2DMatrix32 *)pInputWorldTransform, fFlatteningTolerance );
}
inline Void GPU2DGeometry::CombineWithGeometry( GPU2DPathGeometry * outGeometry, GPU2DGeometryCombineMode iMode, const GPU2DGeometry * pInputGeometry, const GPU2DMatrix32 * pInputWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    Assert( pInputGeometry->IsBound() );
    Assert( outGeometry->IsBound() );
    Assert( outGeometry->IsOpened() );
    m_pDevice2DGeometry->CombineWithGeometry( &(outGeometry->m_hDevice2DPathGeometry), (Device2DGeometryCombineMode)iMode, pInputGeometry->m_pDevice2DGeometry, (const Device2DMatrix32 *)pInputWorldTransform, fFlatteningTolerance );
}

inline Void GPU2DGeometry::Simplify( GPU2DPathGeometry * outGeometry, GPU2DGeometrySimplifyOption iOption, const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    Assert( outGeometry->IsBound() );
    Assert( outGeometry->IsOpened() );
    m_pDevice2DGeometry->Simplify( &(outGeometry->m_hDevice2DPathGeometry), (Device2DGeometrySimplifyOption)iOption, (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}
inline Void GPU2DGeometry::Outline( GPU2DPathGeometry * outGeometry, const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    Assert( outGeometry->IsBound() );
    Assert( outGeometry->IsOpened() );
    m_pDevice2DGeometry->Outline( &(outGeometry->m_hDevice2DPathGeometry), (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}
inline Void GPU2DGeometry::Widen( GPU2DPathGeometry * outGeometry, const GPU2DStrokeStyle * pStrokeStyle, Float fStrokeWidth, const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    Assert( pStrokeStyle->IsBound() );
    Assert( outGeometry->IsBound() );
    Assert( outGeometry->IsOpened() );
    m_pDevice2DGeometry->Widen( &(outGeometry->m_hDevice2DPathGeometry), &(pStrokeStyle->m_hDevice2DStrokeStyle), fStrokeWidth, (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}

inline Void GPU2DGeometry::Tessellate( GPU2DMesh * outMesh, const GPU2DMatrix32 * pWorldTransform, Float fFlatteningTolerance ) const {
    Assert( IsBound() );
    Assert( outMesh->IsBound() );
    Assert( outMesh->IsOpened() );
    m_pDevice2DGeometry->Tessellate( &(outMesh->m_hDevice2DMesh), (const Device2DMatrix32 *)pWorldTransform, fFlatteningTolerance );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DRectangleGeometry implementation
inline Void GPU2DRectangleGeometry::Bind( const GPU2DRectangle * pRectangle ) {
    Assert( !(IsBound()) );
    m_hDevice2DRectangleGeometry.Create( (const Device2DRectangle *)pRectangle );
}

inline Void GPU2DRectangleGeometry::GetRect( GPU2DRectangle * outRectangle ) const {
    Assert( IsBound() );
    m_hDevice2DRectangleGeometry.GetRect( (Device2DRectangle*)outRectangle );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DRoundedRectangleGeometry implementation
inline Void GPU2DRoundedRectangleGeometry::Bind( const GPU2DRoundedRectangle * pRoundedRectangle ) {
    Assert( !(IsBound()) );
    m_hDevice2DRoundedRectangleGeometry.Create( (const Device2DRoundedRectangle *)pRoundedRectangle );
}

inline Void GPU2DRoundedRectangleGeometry::GetRoundedRect( GPU2DRoundedRectangle * outRoundedRectangle ) const {
    Assert( IsBound() );
    m_hDevice2DRoundedRectangleGeometry.GetRoundedRect( (Device2DRoundedRectangle*)outRoundedRectangle );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DEllipseGeometry implementation
inline Void GPU2DEllipseGeometry::Bind( const GPU2DEllipse * pEllipse ) {
    Assert( !(IsBound()) );
    m_hDevice2DEllipseGeometry.Create( (const Device2DEllipse *)pEllipse );
}

inline Void GPU2DEllipseGeometry::GetEllipse( GPU2DEllipse * outEllipse ) const {
    Assert( IsBound() );
    m_hDevice2DEllipseGeometry.GetEllipse( (Device2DEllipse*)outEllipse );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DPathGeometry implementation
inline Void GPU2DPathGeometry::Bind() {
    Assert( !(IsBound()) );
    m_hDevice2DPathGeometry.Create();
}

inline Bool GPU2DPathGeometry::IsOpened() const {
    Assert( IsBound() );
    return m_hDevice2DPathGeometry.IsOpened();
}
inline Bool GPU2DPathGeometry::IsFigureInProgress() const {
    Assert( IsBound() );
    Assert( IsOpened() );
    return m_hDevice2DPathGeometry.IsFigureInProgress();
}

inline UInt GPU2DPathGeometry::GetFigureCount() const {
    Assert( IsBound() );
    return m_hDevice2DPathGeometry.GetFigureCount();
}
inline UInt GPU2DPathGeometry::GetSegmentCount() const {
    Assert( IsBound() );
    return m_hDevice2DPathGeometry.GetSegmentCount();
}

inline Void GPU2DPathGeometry::Open( GPU2DGeometryFillMode iFillMode, GPU2DGeometrySegmentFlag iSegmentFlags ) {
    Assert( IsBound() );
    Assert( !(IsOpened()) );
    m_hDevice2DPathGeometry.Open( (Device2DGeometryFillMode)iFillMode, (Device2DGeometrySegmentFlag)iSegmentFlags );
}
inline Void GPU2DPathGeometry::Close() {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( !(IsFigureInProgress()) );
    m_hDevice2DPathGeometry.Close();
}

inline Void GPU2DPathGeometry::SetFillMode( GPU2DGeometryFillMode iFillMode ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    m_hDevice2DPathGeometry.SetFillMode( (Device2DGeometryFillMode)iFillMode );
}
inline Void GPU2DPathGeometry::SetSegmentFlags( GPU2DGeometrySegmentFlag iSegmentFlags ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    m_hDevice2DPathGeometry.SetSegmentFlags( (Device2DGeometrySegmentFlag)iSegmentFlags );
}

inline Void GPU2DPathGeometry::BeginFigure( const GPU2DPoint * pStart, Bool bIsFilled ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( !(IsFigureInProgress()) );
    m_hDevice2DPathGeometry.BeginFigure( (const Device2DPoint *)pStart, bIsFilled );
}
inline Void GPU2DPathGeometry::EndFigure( Bool bClose ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( IsFigureInProgress() );
    m_hDevice2DPathGeometry.EndFigure( bClose );
}

inline Void GPU2DPathGeometry::AddLine( const GPU2DPoint * pPoint ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( IsFigureInProgress() );
    m_hDevice2DPathGeometry.AddLine( (const Device2DPoint *)pPoint );
}
inline Void GPU2DPathGeometry::AddLines( const GPU2DPoint * arrPoints, UInt iPointCount ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( IsFigureInProgress() );
    m_hDevice2DPathGeometry.AddLines( (const Device2DPoint *)arrPoints, iPointCount );
}
inline Void GPU2DPathGeometry::AddBezier( const GPU2DBezierSegment * pBezierSegment ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( IsFigureInProgress() );
    m_hDevice2DPathGeometry.AddBezier( (const Device2DBezierSegment *)pBezierSegment );
}
inline Void GPU2DPathGeometry::AddBeziers( const GPU2DBezierSegment * arrBezierSegments, UInt iSegmentCount ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( IsFigureInProgress() );
    m_hDevice2DPathGeometry.AddBeziers( (const Device2DBezierSegment *)arrBezierSegments, iSegmentCount );
}
inline Void GPU2DPathGeometry::AddQuadraticBezier( const GPU2DQuadraticBezierSegment * pQuadraticBezierSegment ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( IsFigureInProgress() );
    m_hDevice2DPathGeometry.AddQuadraticBezier( (const Device2DQuadraticBezierSegment *)pQuadraticBezierSegment );
}
inline Void GPU2DPathGeometry::AddQuadraticBeziers( const GPU2DQuadraticBezierSegment * arrQuadraticBezierSegments, UInt iSegmentCount ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( IsFigureInProgress() );
    m_hDevice2DPathGeometry.AddQuadraticBeziers( (const Device2DQuadraticBezierSegment *)arrQuadraticBezierSegments, iSegmentCount );
}
inline Void GPU2DPathGeometry::AddArc( const GPU2DArcSegment * pArcSegment ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    Assert( IsFigureInProgress() );
    m_hDevice2DPathGeometry.AddArc( (const Device2DArcSegment *)pArcSegment );
}

inline Void GPU2DPathGeometry::Copy( GPU2DPathGeometry * outGeometry ) {
    Assert( IsBound() );
    Assert( !(IsOpened()) );
    Assert( outGeometry->IsBound() );
    Assert( outGeometry->IsOpened() );
    m_hDevice2DPathGeometry.Copy( &(outGeometry->m_hDevice2DPathGeometry) );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DTransformedGeometry implementation
inline Void GPU2DTransformedGeometry::Bind( GPU2DGeometry * pSourceGeometry, const GPU2DMatrix32 * pTransform ) {
    Assert( !(IsBound()) );
    Assert( pSourceGeometry->IsBound() );
    m_hDevice2DTransformedGeometry.Create( pSourceGeometry->m_pDevice2DGeometry, (const Device2DMatrix32 *)pTransform );
    m_pSourceGeometry = pSourceGeometry;
}

inline GPU2DGeometry * GPU2DTransformedGeometry::GetSourceGeometry() const {
    Assert( IsBound() );
    return m_pSourceGeometry;
}
inline Void GPU2DTransformedGeometry::GetTransform( GPU2DMatrix32 * outTransform ) const {
    Assert( IsBound() );
    m_hDevice2DTransformedGeometry.GetTransform( (Device2DMatrix32*)outTransform );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DGeometryGroup implementation
inline Void GPU2DGeometryGroup::Add( GPU2DGeometry * pGeometry ) {
    Assert( !(IsBound()) );
    Assert( pGeometry->IsBound() );
    Assert( m_iSourceGeometryCount < m_iSourceGeometryCapacity );
    m_arrSourceGeometries[m_iSourceGeometryCount] = pGeometry;
    ++m_iSourceGeometryCount;
}

inline UInt GPU2DGeometryGroup::GetSourceGeometryCapacity() const {
    return m_iSourceGeometryCapacity;
}
inline UInt GPU2DGeometryGroup::GetSourceGeometryCount() const {
    return m_iSourceGeometryCount;
}
inline GPU2DGeometry * GPU2DGeometryGroup::GetSourceGeometry( UInt iGeometry ) const {
    Assert( iGeometry < m_iSourceGeometryCount );
    return m_arrSourceGeometries[iGeometry];
}
inline GPU2DGeometry ** GPU2DGeometryGroup::GetSourceGeometries() const {
    return m_arrSourceGeometries;
}

inline GPU2DGeometryFillMode GPU2DGeometryGroup::GetFillMode() const {
    Assert( IsBound() );
    return (GPU2DGeometryFillMode)( m_hDevice2DGeometryGroup.GetFillMode() );
}


