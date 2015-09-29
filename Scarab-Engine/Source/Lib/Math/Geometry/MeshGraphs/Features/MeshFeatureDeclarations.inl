/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/MeshFeatureDeclarations.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Declaration helper macros for MeshFeatures links
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshFeature implementations
#define _MESHFEATURE_IMPLEMENT_2( _class, _name_one, _name_many ) \
    UInt _class::Get##_name_one##Count() const { \
        if ( _name_many[0] == NULL ) \
            return 0; \
        if ( _name_many[1] == NULL ) \
            return 1; \
        return 2; \
    } \
    UInt _class::Find##_name_one( const Mesh##_name_one * pFeat ) const { \
        if ( _name_many[0] == NULL ) \
            return INVALID_OFFSET; \
        if ( _name_many[0] == pFeat ) \
            return 0; \
        if ( _name_many[1] == NULL ) \
            return INVALID_OFFSET; \
        if ( _name_many[1] == pFeat ) \
            return 1; \
        return INVALID_OFFSET; \
    } \
    UInt _class::Connect##_name_one( Mesh##_name_one * pFeat ) { \
        if ( _name_many[0] == NULL ) { \
            _name_many[0] = pFeat; \
            return 0; \
        } \
        if ( _name_many[0] == pFeat ) \
            return 0; \
        if ( _name_many[1] == NULL ) { \
            _name_many[1] = pFeat; \
            return 1; \
        } \
        if ( _name_many[1] == pFeat ) \
            return 1; \
        return INVALID_OFFSET; \
    } \
    Void _class::Disconnect##_name_one( UInt iFeat ) { \
        Assert( iFeat < 2 ); \
        if ( iFeat == 1 ) { \
            _name_many[1] = NULL; \
            return; \
        } \
        _name_many[0] = _name_many[1]; \
        _name_many[1] = NULL; \
    }

#define _MESHFEATURE_IMPLEMENT_3( _class, _name_one, _name_many ) \
    UInt _class::Get##_name_one##Count() const { \
        if ( _name_many[0] == NULL ) \
            return 0; \
        if ( _name_many[1] == NULL ) \
            return 1; \
        if ( _name_many[2] == NULL ) \
            return 2; \
        return 3; \
    } \
    UInt _class::Find##_name_one( const Mesh##_name_one * pFeat ) const { \
        if ( _name_many[0] == NULL ) \
            return INVALID_OFFSET; \
        if ( _name_many[0] == pFeat ) \
            return 0; \
        if ( _name_many[1] == NULL ) \
            return INVALID_OFFSET; \
        if ( _name_many[1] == pFeat ) \
            return 1; \
        if ( _name_many[2] == NULL ) \
            return INVALID_OFFSET; \
        if ( _name_many[2] == pFeat ) \
            return 2; \
        return INVALID_OFFSET; \
    } \
    UInt _class::Connect##_name_one( Mesh##_name_one * pFeat ) { \
        if ( _name_many[0] == NULL ) { \
            _name_many[0] = pFeat; \
            return 0; \
        } \
        if ( _name_many[0] == pFeat ) \
            return 0; \
        if ( _name_many[1] == NULL ) { \
            _name_many[1] = pFeat; \
            return 1; \
        } \
        if ( _name_many[1] == pFeat ) \
            return 1; \
        if ( _name_many[2] == NULL ) { \
            _name_many[2] = pFeat; \
            return 2; \
        } \
        if ( _name_many[2] == pFeat ) \
            return 2; \
        return INVALID_OFFSET; \
    } \
    Void _class::Disconnect##_name_one( UInt iFeat ) { \
        Assert( iFeat < 3 ); \
        if ( iFeat == 2 ) { \
            _name_many[2] = NULL; \
            return; \
        } \
        if ( iFeat == 1 ) { \
            _name_many[1] = _name_many[2]; \
            _name_many[2] = NULL; \
            return; \
        } \
        _name_many[0] = _name_many[1]; \
        _name_many[1] = _name_many[2]; \
        _name_many[2] = NULL; \
    }

#define _MESHFEATURE_IMPLEMENT_4( _class, _name_one, _name_many ) \
    UInt _class::Get##_name_one##Count() const { \
        if ( _name_many[0] == NULL ) \
            return 0; \
        if ( _name_many[1] == NULL ) \
            return 1; \
        if ( _name_many[2] == NULL ) \
            return 2; \
        if ( _name_many[3] == NULL ) \
            return 3; \
        return 4; \
    } \
    UInt _class::Find##_name_one( const Mesh##_name_one * pFeat ) const { \
        if ( _name_many[0] == NULL ) \
            return INVALID_OFFSET; \
        if ( _name_many[0] == pFeat ) \
            return 0; \
        if ( _name_many[1] == NULL ) \
            return INVALID_OFFSET; \
        if ( _name_many[1] == pFeat ) \
            return 1; \
        if ( _name_many[2] == NULL ) \
            return INVALID_OFFSET; \
        if ( _name_many[2] == pFeat ) \
            return 2; \
        if ( _name_many[3] == NULL ) \
            return INVALID_OFFSET; \
        if ( _name_many[3] == pFeat ) \
            return 3; \
        return INVALID_OFFSET; \
    } \
    UInt _class::Connect##_name_one( Mesh##_name_one * pFeat ) { \
        if ( _name_many[0] == NULL ) { \
            _name_many[0] = pFeat; \
            return 0; \
        } \
        if ( _name_many[0] == pFeat ) \
            return 0; \
        if ( _name_many[1] == NULL ) { \
            _name_many[1] = pFeat; \
            return 1; \
        } \
        if ( _name_many[1] == pFeat ) \
            return 1; \
        if ( _name_many[2] == NULL ) { \
            _name_many[2] = pFeat; \
            return 2; \
        } \
        if ( _name_many[2] == pFeat ) \
            return 2; \
        if ( _name_many[3] == NULL ) { \
            _name_many[3] = pFeat; \
            return 3; \
        } \
        if ( _name_many[3] == pFeat ) \
            return 3; \
        return INVALID_OFFSET; \
    } \
    Void _class::Disconnect##_name_one( UInt iFeat ) { \
        Assert( iFeat < 4 ); \
        if ( iFeat == 3 ) { \
            _name_many[3] = NULL; \
            return; \
        } \
        if ( iFeat == 2 ) { \
            _name_many[2] = _name_many[3]; \
            _name_many[3] = NULL; \
            return; \
        } \
        if ( iFeat == 1 ) { \
            _name_many[1] = _name_many[2]; \
            _name_many[2] = _name_many[3]; \
            _name_many[3] = NULL; \
            return; \
        } \
        _name_many[0] = _name_many[1]; \
        _name_many[1] = _name_many[2]; \
        _name_many[2] = _name_many[3]; \
        _name_many[3] = NULL; \
    }

#define _MESHFEATURE_IMPLEMENT_ARRAY( _class, _name_one, _name_many, _size ) \
    UInt _class::Get##_name_one##Count() const { \
        for( UInt i = 0; i < (_size); ++i ) { \
            if ( _name_many[i] == NULL ) \
                return i; \
        } \
        return (_size); \
    } \
    UInt _class::Find##_name_one( const Mesh##_name_one * pFeat ) const { \
        for( UInt i = 0; i < (_size); ++i ) { \
            if ( _name_many[i] == NULL ) \
                return INVALID_OFFSET; \
            if ( _name_many[i] == pFeat ) \
                return i; \
        } \
        return INVALID_OFFSET; \
    } \
    UInt _class::Connect##_name_one( Mesh##_name_one * pFeat ) { \
        for( UInt i = 0; i < (_size); ++i ) { \
            if ( _name_many[i] == NULL ) { \
                _name_many[i] = pFeat; \
                return i; \
            } \
            if ( _name_many[i] == pFeat ) \
                return i; \
        } \
        return INVALID_OFFSET; \
    } \
    Void _class::Disconnect##_name_one( UInt iFeat ) { \
        Assert( iFeat < (_size) ); \
        for( UInt i = iFeat; i < ((_size) - 1); ++i ) { \
            _name_many[i] = _name_many[i+1]; \
            if ( _name_many[i] == NULL ) \
                return; \
        } \
        _name_many[(_size) - 1] = NULL; \
    }

#define _MESHFEATURE_IMPLEMENT_DYNARRAY( _class, _name_one, _name_many ) \
    UInt _class::Connect##_name_one( Mesh##_name_one * pFeat ) { \
        UInt iCount = _name_many.Count(); \
        for( UInt i = 0; i < iCount; ++i ) { \
            if ( _name_many[i] == pFeat ) \
                return i; \
        } \
        _name_many.Push( pFeat ); \
        return iCount; \
    }


