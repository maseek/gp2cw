////////////////////////////////////////////////////
// human view
////////////////////////////////////////////////////

#include "ZionNoiseStd.h"
#include "ViewHuman.h"
#include "..\Graphics\CameraNode.h"
#include "..\Window\Message.h"
#include "MovementController.h"
#include "..\Window\Window.h"

namespace zn
{
    const float SCREEN_REFRESH_RATE( 1000.f / 60.f );

    ViewHuman::ViewHuman() : m_pKeyboardHandler( NULL ), m_pMouseHandler( NULL ), m_pFreeCameraController( NULL ),
        m_currentTime( 0.f )
    {

    }

    ViewHuman::~ViewHuman()
    {
        m_pMouseHandler = NULL;
        m_pKeyboardHandler = NULL;
        ZN_SAFE_DELETE( m_pFreeCameraController );
    }

    void ViewHuman::SetRenderer( shared_ptr< Renderer > pRenderer )
    {
        m_pRenderer = pRenderer;
    }

    void ViewHuman::SetCameraOffset( const fVec3& offset )
    {
        m_pCamera->SetOffset( offset );
    }

    void ViewHuman::VInit()
    {
        if( m_pRenderer )
        {
            m_pScene = shared_ptr< ScreenElementScene >( ZN_NEW ScreenElementScene( m_pRenderer ) );
            m_screenElements.push_front( m_pScene );

            Frustum frustum;
            Mat4x4 camPos = Mat4x4::identity;
            camPos.SetPosition( fVec3( 0.f, 1.7f, 0.f ) );
            m_pCamera.reset( ZN_NEW CameraNode( &Mat4x4::identity, frustum ) );
            m_pCamera->Init();
            m_pScene->SetCamera( m_pCamera );
            m_pScene->AddSceneNode( NO_GAME_OBJECT_ID, m_pCamera );

            m_pFreeCameraController = ZN_NEW MovementController( m_pCamera, 0, 0 );
            m_pKeyboardHandler = m_pFreeCameraController;
            m_pMouseHandler = m_pFreeCameraController;
            g_pGame->GetWindow()->CaptureMouse();
        }
    }

    void ViewHuman::VRender( const float deltaMs )
    {
        m_currentTime += deltaMs;
        if( m_currentTime >= SCREEN_REFRESH_RATE )
        {
            g_pGame->GetRenderer()->PreRender();

            m_screenElements.sort( SortBy_SharedPtr_Content< IScreenElement >() );

			for( ScreenElementList::iterator i = m_screenElements.begin(), end = m_screenElements.end(); 
                i != end; ++i )
			{
				if ( ( *i )->IsVisible() )
				{
					( *i )->VRender( m_currentTime );
				}
			}

            g_pGame->GetRenderer()->PostRender();
            g_pGame->GetRenderer()->Present();
            m_currentTime = 0.f;
        }
    }

    void ViewHuman::VUpdate( const float deltaMs )
    {
        if( m_pFreeCameraController )
        {
            m_pFreeCameraController->Update( deltaMs );
        }
        for( ScreenElementList::iterator i = m_screenElements.begin(), end = m_screenElements.end(); 
            i != end; ++i )
        {
            ( *i )->VUpdate( deltaMs );
        }
    }
    
    const bool ViewHuman::VProcessMessage( const Message& message )
    {
        // go through screen elements first

        // then input handlers
        bool result = false;

        switch( message.type )
        {
        case Message::KeyDown:
        case Message::KeyUp:
            if( m_pKeyboardHandler )
                result = m_pKeyboardHandler->VOnKeyMsg( message );
            break;
        case Message::MouseButtonDown:
        case Message::MouseButtonUp:
        case Message::MouseMove:
        case Message::MouseWheel:
            if( m_pMouseHandler )
                result = m_pMouseHandler->VOnMouseMsg( message );
            break;
        }

        return result;
    }

    void ViewHuman::VAddSceneNode( GameObjectId id, shared_ptr< SceneNode > pSceneNode )
    {
        m_pScene->AddSceneNode( id, pSceneNode );
    }
}