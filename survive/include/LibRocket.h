
#ifndef CILIBROCKET
#define CILIBROCKET


#include "LibRocketRenderInterface.h"
#include "LibRocketSystemInterface.h"
#include "Rocket/Core.h"
#include "Rocket/Debugger.h"
#include "Rocket/Controls.h"


class LibRocket : public Rocket::Core::Plugin {
    
public:
	static LibRocket* getInstance();

	void setup();
    
	Rocket::Core::ElementDocument* loadDocument(ci::fs::path filePath, bool showImmediately = true);

    void render() { m_context->Render(); }
    
	void update() { m_context->Update(); }
    
	void registerCallbacks();

	void unregisterCallbacks();
    
    void toggleDebugger()   { Rocket::Debugger::SetVisible( !Rocket::Debugger::IsVisible() ); }
    void hideDebugger()     { Rocket::Debugger::SetVisible(false); }
    void showDebugger()     { Rocket::Debugger::SetVisible(true); }


private:
	LibRocket() {};
	~LibRocket();

	LibRocket(const LibRocket& p);
	LibRocket& operator=(LibRocket const&);

    bool mouseDown( ci::app::MouseEvent event );
    bool mouseUp( ci::app::MouseEvent event );
    bool mouseMove( ci::app::MouseEvent event );
    bool mouseDrag( ci::app::MouseEvent event );
    bool mouseWheel( ci::app::MouseEvent event );
    bool keyDown( ci::app::KeyEvent event );
    bool keyUp( ci::app::KeyEvent event );
    void resize();
    
    int getMouseButton( ci::app::MouseEvent event );
    void initKeyMap();
    Rocket::Core::word getCharacterCode(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state);
  
    LibRocketRenderInterface*  m_renderer;
    LibRocketSystemInterface*  m_systemInterface;
    
    Rocket::Core::Context* m_context;

    ci::signals::scoped_connection  m_cbMouseDown, m_cbMouseDrag, m_cbMouseUp, m_cbMouseMove, m_cbMouseWheel;
    ci::signals::scoped_connection  m_cbKeyDown, m_cbKeyUp;
    ci::signals::scoped_connection  m_cbResize;
  
    std::map<int, Rocket::Core::Input::KeyIdentifier> m_cinderKeyToRocket;

};


#endif
