/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DMesh.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Meshes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPU2DMesh implementation
inline Bool GPU2DMesh::IsBound() const {
    return m_hDevice2DMesh.IsCreated();
}

inline Bool GPU2DMesh::IsOpened() const {
    Assert( IsBound() );
    return m_hDevice2DMesh.IsOpened();
}

inline Void GPU2DMesh::Open() {
    Assert( IsBound() );
    Assert( !(IsOpened()) );
    m_hDevice2DMesh.Open();
}
inline Void GPU2DMesh::Close() {
    Assert( IsBound() );
    Assert( IsOpened() );
    m_hDevice2DMesh.Close();
}

inline Void GPU2DMesh::AddTriangles( const GPU2DTriangle * arrTriangles, UInt iTriangleCount ) {
    Assert( IsBound() );
    Assert( IsOpened() );
    m_hDevice2DMesh.AddTriangles( (const Device2DTriangle *)arrTriangles, iTriangleCount );
}
