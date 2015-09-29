/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Outdoor/WorldOutdoor.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Outdoor
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : This is a seamless unbounded field, you must manage world limits
//              at the game-logic level.
//              You must make the initialization yourself in your derived
//              class's constructor by calling _Initialize() before doing
//              anything else.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_OUTDOOR_WORLDOUTDOOR_H
#define SCARAB_ENGINE_WORLD_OUTDOOR_WORLDOUTDOOR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "WorldOutdoorArea.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The WorldOutdoor class
class WorldOutdoor : public WorldNode
{
public:
    WorldOutdoor( const Vertex2 & vOrigin, const Vector2 & vAreaSize, UInt iViewDistance, const GChar * strName = NULL );
    virtual ~WorldOutdoor();

    // Type
    inline virtual WorldNodeType GetType() const;

    // Getters
    inline const Vertex2 & GetOrigin() const;
    inline const Vector2 & GetAreaSize() const;

    inline UInt GetViewDistance() const;
    inline UInt GetVisibleAreasPitch() const;

    inline Void GetEyeArea( Point2 * outAreaCoords ) const;

    // Landscape queries
    WorldOutdoorArea * GetArea( const Point2 & vAreaCoords, UInt * outVisibleX = NULL, UInt * outVisibleY = NULL ) const;
    WorldOutdoorArea * GetVisibleArea( UInt iVisibleX, UInt iVisibleY, Point2 * outAreaCoords = NULL ) const;

    WorldOutdoorArea * GetArea( const Vertex2 & vPosition, Point2 * outAreaCoords = NULL,
                                                           UInt * outVisibleX = NULL, UInt * outVisibleY = NULL ) const;

    Void SetViewDistance( UInt iViewDistance );

protected:
    // Custom areas support
    Void _Initialize();
    virtual WorldOutdoorArea * _CreateArea( const Vertex2 & vOrigin, const Vector2 & vSize, const GChar * strName = NULL ) = 0;

    // Culling support
    virtual Void _OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull );
    virtual Void _OnEyeUpdate( Vertex2 * outEye ) const; // Use this to override the culler's default camera eye

private:
    // Helpers
    inline UInt _GetVisibleAreaIndex( UInt iVisibleX, UInt iVisibleY ) const;
    inline WorldOutdoorArea * _GetVisibleArea( UInt iVisibleX, UInt iVisibleY ) const;
    inline Void _UpdateArea( UInt iVisibleX, UInt iVisibleY, const Point2 & vAreaCoords );

	inline Void _GetAreaCoords( Point2 * outAreaCoords, const Vertex2 & vPosition ) const;
    inline Void _GetVisibleAreaCoords( Point2 * outAreaCoords, UInt iVisibleX, UInt iVisibleY, const Point2 & vEyeArea ) const;
    Bool _GetVisibleCoords( UInt * outVisibleX, UInt * outVisibleY, const Point2 & vAreaCoords, const Point2 & vEyeArea ) const;

    Void _UpdateVisibleAreas( const Vertex2 & vEyePosition );
    Void _VisibleAreas_Warp( const Point2 & vNewEyeArea );
    Void _VisibleAreas_TranslateX( const Point2 & vNewEyeArea, Int iDX );
    Void _VisibleAreas_TranslateY( const Point2 & vNewEyeArea, Int iDY );
    Void _VisibleAreas_TranslateXY( const Point2 & vNewEyeArea, Int iDX, Int iDY );

    Void _PathFindingGraph_UpdateAdjacency_Pass1( UInt iVisibleX, UInt iVisibleY, Bool bAreaUpdate );
    Void _PathFindingGraph_UpdateAdjacency_Pass2( UInt iVisibleX, UInt iVisibleY );

    // Area data
    Vertex2 m_vOrigin;
    Vector2 m_vAreaSize;

    Point2 m_vEyeArea;

    UInt m_iViewDistance;
    UInt m_iVisibleAreasPitch;
    WorldOutdoorArea ** m_arrVisibleAreas;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldOutdoor.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_OUTDOOR_WORLDOUTDOOR_H

