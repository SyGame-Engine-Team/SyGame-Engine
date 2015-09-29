/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/MeshFeatureDeclarations.h
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
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_MESHFEATUREDECLARATIONS_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_MESHFEATUREDECLARATIONS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// MeshFeature declarations
#define _MESHFEATURE_DECLARE_N( _name_one, _name_many, _size ) \
    inline virtual UInt              Get##_name_one##Capacity() const; \
           virtual UInt              Get##_name_one##Count   () const; \
    inline virtual Mesh##_name_one * Get##_name_one          ( UInt iFeat ) const; \
           virtual UInt              Find##_name_one         ( const Mesh##_name_one * pFeat ) const; \
    inline virtual Void              Replace##_name_one      ( UInt iFeat, Mesh##_name_one * pFeat ); \
           virtual UInt              Connect##_name_one      ( Mesh##_name_one * pFeat ); \
           virtual Void              Disconnect##_name_one   ( UInt iFeat ); \
    inline virtual Void              Disconnect##_name_many  (); \
    Mesh##_name_one * _name_many[_size];

#define _MESHFEATURE_DECLARE_ARRAY( _name_one, _name_many ) \
    inline virtual UInt              Get##_name_one##Capacity() const; \
           virtual UInt              Get##_name_one##Count   () const; \
    inline virtual Mesh##_name_one * Get##_name_one          ( UInt iFeat ) const; \
           virtual UInt              Find##_name_one         ( const Mesh##_name_one * pFeat ) const; \
    inline virtual Void              Replace##_name_one      ( UInt iFeat, Mesh##_name_one * pFeat ); \
           virtual UInt              Connect##_name_one      ( Mesh##_name_one * pFeat ); \
           virtual Void              Disconnect##_name_one   ( UInt iFeat ); \
    inline virtual Void              Disconnect##_name_many  (); \
    Mesh##_name_one ** _name_many;

#define _MESHFEATURE_DECLARE_DYNARRAY( _name_one, _name_many ) \
    inline virtual UInt              Get##_name_one##Capacity() const; \
    inline virtual UInt              Get##_name_one##Count   () const; \
    inline virtual Mesh##_name_one * Get##_name_one          ( UInt iFeat ) const; \
    inline virtual UInt              Find##_name_one         ( const Mesh##_name_one * pFeat ) const; \
    inline virtual Void              Replace##_name_one      ( UInt iFeat, Mesh##_name_one * pFeat ); \
           virtual UInt              Connect##_name_one      ( Mesh##_name_one * pFeat ); \
    inline virtual Void              Disconnect##_name_one   ( UInt iFeat ); \
    inline virtual Void              Disconnect##_name_many  (); \
    Array<Mesh##_name_one *> _name_many;

/////////////////////////////////////////////////////////////////////////////////
// MeshFeature Constructors / Destructors
#define _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( _name_many ) \
    _name_many[0] = NULL; \
    _name_many[1] = NULL;

#define _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( _name_many ) \
    _name_many[0] = NULL; \
    _name_many[1] = NULL; \
    _name_many[2] = NULL;

#define _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( _name_many ) \
    _name_many[0] = NULL; \
    _name_many[1] = NULL; \
    _name_many[2] = NULL; \
    _name_many[3] = NULL;

#define _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( _name_one, _name_many, _size ) \
    _name_many = New Mesh##_name_one*[_size]; \
    for( UInt i = 0; i < (_size); ++i ) \
        _name_many[i] = NULL;

#define _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( _name_many ) \
    DeleteA( _name_many );

#define _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( _name_many ) \
    _name_many.UseMemoryContext( MemoryFn->GetSelectedContextID(), MemoryFn->GetSelectedAllocatorName() ); \
    _name_many.Create();

#define _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( _name_many ) \
    _name_many.Destroy();

/////////////////////////////////////////////////////////////////////////////////
// MeshFeature inline implementations
#define _MESHFEATURE_IMPLEMENT_INLINES_2( _class, _name_one, _name_many ) \
    inline UInt _class::Get##_name_one##Capacity() const { \
        return 2; \
    } \
    inline Mesh##_name_one * _class::Get##_name_one( UInt iFeat ) const { \
        Assert( iFeat < 2 ); \
        return _name_many[iFeat]; \
    } \
    inline Void _class::Replace##_name_one( UInt iFeat, Mesh##_name_one * pFeat ) { \
        Assert( iFeat < 2 && _name_many[iFeat] != NULL ); \
        _name_many[iFeat] = pFeat; \
    } \
    inline Void _class::Disconnect##_name_many() { \
        _name_many[0] = NULL; _name_many[1] = NULL; \
    }

#define _MESHFEATURE_IMPLEMENT_INLINES_3( _class, _name_one, _name_many ) \
    inline UInt _class::Get##_name_one##Capacity() const { \
        return 3; \
    } \
    inline Mesh##_name_one * _class::Get##_name_one( UInt iFeat ) const { \
        Assert( iFeat < 3 ); \
        return _name_many[iFeat]; \
    } \
    inline Void _class::Replace##_name_one( UInt iFeat, Mesh##_name_one * pFeat ) { \
        Assert( iFeat < 3 && _name_many[iFeat] != NULL ); \
        _name_many[iFeat] = pFeat; \
    } \
    inline Void _class::Disconnect##_name_many() { \
        _name_many[0] = NULL; _name_many[1] = NULL; _name_many[2] = NULL; \
    }

#define _MESHFEATURE_IMPLEMENT_INLINES_4( _class, _name_one, _name_many ) \
    inline UInt _class::Get##_name_one##Capacity() const { \
        return 4; \
    } \
    inline Mesh##_name_one * _class::Get##_name_one( UInt iFeat ) const { \
        Assert( iFeat < 4 ); \
        return _name_many[iFeat]; \
    } \
    inline Void _class::Replace##_name_one( UInt iFeat, Mesh##_name_one * pFeat ) { \
        Assert( iFeat < 4 && _name_many[iFeat] != NULL ); \
        _name_many[iFeat] = pFeat; \
    } \
    inline Void _class::Disconnect##_name_many() { \
        _name_many[0] = NULL; _name_many[1] = NULL; _name_many[2] = NULL; _name_many[3] = NULL; \
    }

#define _MESHFEATURE_IMPLEMENT_INLINES_ARRAY( _class, _name_one, _name_many, _size ) \
    inline UInt _class::Get##_name_one##Capacity() const { \
        return _size; \
    } \
    inline Mesh##_name_one * _class::Get##_name_one( UInt iFeat ) const { \
        Assert( iFeat < (_size) ); \
        return _name_many[iFeat]; \
    } \
    inline Void _class::Replace##_name_one( UInt iFeat, Mesh##_name_one * pFeat ) { \
        Assert( iFeat < (_size) && _name_many[iFeat] != NULL ); \
        _name_many[iFeat] = pFeat; \
    } \
    inline Void _class::Disconnect##_name_many() { \
        for( UInt i = 0; i < (_size); ++i ) \
            _name_many[i] = NULL; \
    }

#define _MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( _class, _name_one, _name_many ) \
    inline UInt _class::Get##_name_one##Capacity() const { \
        return INVALID_OFFSET; \
    } \
    inline UInt _class::Get##_name_one##Count() const { \
        return _name_many.Count(); \
    } \
    inline Mesh##_name_one * _class::Get##_name_one( UInt iFeat ) const { \
        Assert( iFeat < _name_many.Count() ); \
        return _name_many[iFeat]; \
    } \
    inline UInt _class::Find##_name_one( const Mesh##_name_one * pFeat ) const { \
        return _name_many.Search( (Mesh##_name_one *)pFeat ); \
    } \
    inline Void _class::Replace##_name_one( UInt iFeat, Mesh##_name_one * pFeat ) { \
        Assert( iFeat < _name_many.Count() && _name_many[iFeat] != NULL ); \
        _name_many[iFeat] = pFeat; \
    } \
    inline Void _class::Disconnect##_name_one( UInt iFeat ) { \
        Assert( iFeat < _name_many.Count() ); \
        _name_many.Remove( iFeat, NULL, 1 ); \
    } \
    inline Void _class::Disconnect##_name_many() { \
        _name_many.Clear(); \
    }
    
/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshFeatureDeclarations.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_MESHFEATUREDECLARATIONS_H

