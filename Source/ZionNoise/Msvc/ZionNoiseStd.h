#pragma once
////////////////////////////////////////////////////
// precompiled header file
////////////////////////////////////////////////////

#include <stdlib.h>
#include <string>
#include <queue>
#include <list>
#include <map>
#include <vector>
#include <math.h>

// luabind warnings
#pragma warning( disable : 4251 )

// overload get_pointer to use std::shared_ptr within luabind
namespace boost { 
  template<class T> const T* get_pointer(const std::shared_ptr<T>& ptr) 
  {
    return ptr.get();
  }

  template<class T> T* get_pointer(std::shared_ptr<T>& ptr)
  {
    return ptr.get();
  }
}

extern "C"
{
    #include "lua.h"
}

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

#define FBXSDK_NEW_API

using luabind::module;
using luabind::class_;
using luabind::constructor;
using luabind::call_function;
using luabind::def;

using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::static_pointer_cast;

using std::vector;
using std::map;
using std::list;
using std::queue;
using std::string;
using std::wstring;
using std::pair;
using std::make_pair;

template< class T >
shared_ptr< T > MakeStrongPtr( weak_ptr< T > pWeakPtr )
{
    if ( !pWeakPtr.expired() )
        return shared_ptr< T >( pWeakPtr );
    else
        return shared_ptr< T >();
}

#if defined( _WIN32 ) || defined( __WIN32__ )
    // Windows
#   define ZN_PLATFORM_WIN32
    // avoid min/max problems
#   ifndef NOMINMAX
#       define NOMINMAX
#   endif
    // exclude rarely-used services from Windows headers
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#else
#   error This operation system is not supported
#endif

// redefine new for debugging purposes
#if defined( _DEBUG ) || defined( DEBUG )
#	define ZN_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#   define ZN_DEBUG
#else
#	define ZN_NEW new
#endif

// safe delete pointer
#ifndef ZN_SAFE_DELETE
#   define ZN_SAFE_DELETE( x ) if( x ) delete x; x = NULL;
#endif
// safe delete array
#ifndef ZN_SAFE_DELETE_ARRAY
#   define ZN_SAFE_DELETE_ARRAY( x ) if( x ) delete [] x; x = NULL;
#endif
// safe release
#ifndef ZN_SAFE_RELEASE
#   define ZN_SAFE_RELEASE( x ) if( x ) x->Release(); x = NULL;
#endif

// typedefs
namespace zn
{
    typedef signed char int8;
    typedef unsigned char uint8;
    typedef signed short int16;
    typedef unsigned short uint16;
    typedef signed int int32;
    typedef unsigned int uint32;
#   ifdef _MSC_VER
        typedef signed __int64 int64;
        typedef unsigned __int64 uint64;
#   else
        typedef signed long long int64;
        typedef unsigned long long uint64;
#   endif

    typedef uint32 GameObjectId;
    typedef uint32 Index;

    template< class T >
    struct SortBy_SharedPtr_Content
    {
        bool operator()( const shared_ptr< T > &ptr1, const shared_ptr< T > &ptr2 ) const
        {
            return *ptr1 < *ptr2;
        }
    };

    extern const GameObjectId NO_GAME_OBJECT_ID;
    extern const float ZN_PI;
    extern const float ZN_2PI;
}

/// HAVOK includes
#include <Common/Base/hkBase.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>
#include <Common/Base/Fwd/hkcstdio.h>
// Physics
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Utilities/CharacterControl/hkpCharacterControl.h>
#include <Physics/Utilities/CharacterControl/FirstPersonCharacter/hkpFirstPersonCharacter.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBodyListener.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpCharacterContext.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpCharacterStateManager.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesConnectivityUtil.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
// Platform specific initialization
#include <Common/Base/System/Init/PlatformInit.cxx>
///end HAVOK includes

#include <fmod.hpp>

#include "..\Utilities\Math.h"
#include "..\Geometry\Geometry.h"
#include "..\Graphics\RenderPass.h"

#include "..\Game\Game.h"