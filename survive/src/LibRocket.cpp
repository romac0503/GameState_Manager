#include "StdAfx.h"

#include "LibRocket.h"
#include <functional>



using namespace ci;
using namespace ci::app;
using namespace std;

template <typename T> int getKeyModifier(T event)
{
  int ret = 0;
  if (event.isControlDown())
    ret |= Rocket::Core::Input::KM_CTRL;
  if (event.isShiftDown())
    ret |= Rocket::Core::Input::KM_SHIFT;
  if (event.isAltDown())
    ret |= Rocket::Core::Input::KM_ALT;
  if (event.isMetaDown())
    ret |= Rocket::Core::Input::KM_META;
  // TODO: Caps, Num, and Scroll lock
  return ret;
}

LibRocket* LibRocket::getInstance()
{
	static LibRocket instance;
	return &instance;
}

LibRocket::~LibRocket()
{
    unregisterCallbacks();

	Rocket::Core::Shutdown();

	m_systemInterface->Release();
	m_renderer->Release();

}


class Processor:
	public Rocket::Core::EventListener
{
	virtual void ProcessEvent(Rocket::Core::Event& event)
	{
		auto element = event.GetCurrentElement();
		console() << "EVENT" << endl;
	}
};



void LibRocket::setup()
{
	m_renderer = new LibRocketRenderInterface();
	m_systemInterface = new LibRocketSystemInterface();
	
	Rocket::Core::SetRenderInterface( m_renderer );
	Rocket::Core::SetSystemInterface( m_systemInterface );
    
    Rocket::Core::Initialise();
    Rocket::Controls::Initialise();
    
  
    m_context = NULL;
    m_context = Rocket::Core::CreateContext( "main", Rocket::Core::Vector2i( 1024, 768 ) );
	
	

    if ( m_context == NULL )
    {
		Rocket::Core::Shutdown();
        console() << "no context" << endl;
        exit(-1);
    }
    
	Rocket::Debugger::Initialise( m_context );
    
    Rocket::Core::FontDatabase::LoadFontFace( getAssetPath( "gui/Delicious-Roman.otf" ).generic_string().c_str() );
	Rocket::Core::FontDatabase::LoadFontFace(getAssetPath("gui/Delicious-Bold.otf").generic_string().c_str());
	Rocket::Core::FontDatabase::LoadFontFace(getAssetPath("gui/editundo.ttf").generic_string().c_str());

   // loadDocument( getAssetPath( "test01.rml" ) );
	
	Processor* processor = new Processor();
	//m_document->AddEventListener("click", processor);
    
    Rocket::Core::RegisterPlugin(this);
  
	registerCallbacks();
  initKeyMap();
}


void LibRocket::registerCallbacks()
{
    ci::app::WindowRef window = app::App::get()->getWindow();
	m_cbMouseDown    = window->getSignalMouseDown().connect( std::bind( &LibRocket::mouseDown, this, std::placeholders::_1 ) );
	m_cbMouseUp      = window->getSignalMouseUp().connect( std::bind( &LibRocket::mouseUp, this, std::placeholders::_1 ) );
	m_cbMouseDrag    = window->getSignalMouseDrag().connect( std::bind( &LibRocket::mouseDrag, this, std::placeholders::_1 ) );
	m_cbMouseMove    = window->getSignalMouseMove().connect( std::bind( &LibRocket::mouseMove, this, std::placeholders::_1 ) );
	m_cbMouseWheel   = window->getSignalMouseWheel().connect( std::bind( &LibRocket::mouseWheel, this, std::placeholders::_1 ) );
	m_cbKeyDown      = window->getSignalKeyDown().connect( std::bind( &LibRocket::keyDown, this, std::placeholders::_1 ) );
	m_cbKeyUp        = window->getSignalKeyUp().connect( std::bind( &LibRocket::keyUp, this, std::placeholders::_1 ) );
    m_cbResize       = window->getSignalResize().connect( std::bind( &LibRocket::resize, this ) );
}


void LibRocket::unregisterCallbacks()
{
    m_cbMouseDown.disconnect();
    m_cbMouseUp.disconnect();
    m_cbMouseDrag.disconnect();
    m_cbMouseMove.disconnect();
    m_cbMouseWheel.disconnect();
    m_cbKeyDown.disconnect();
    m_cbKeyUp.disconnect();
    m_cbResize.disconnect();
}


Rocket::Core::ElementDocument* LibRocket::loadDocument(fs::path filePath, bool showImmediately)
{
	Rocket::Core::ElementDocument* document = m_context->LoadDocument(filePath.generic_string().c_str());

	if (showImmediately)
	{
		document->Show();
	}

	return document;
}


bool LibRocket::mouseDown(MouseEvent event)
{        
 	m_context->ProcessMouseButtonDown( getMouseButton( event ), getKeyModifier( event ) );
    return false;
}



bool LibRocket::mouseUp(MouseEvent event)
{
	m_context->ProcessMouseButtonUp( getMouseButton( event ), getKeyModifier( event ) );
    return false;
}


bool LibRocket::mouseMove(MouseEvent event)
{
    Vec2i pos = event.getPos();
	m_context->ProcessMouseMove( pos.x, pos.y, getKeyModifier( event ) );
    return false;
}


bool LibRocket::mouseDrag(MouseEvent event)
{
    Vec2i pos = event.getPos();
	m_context->ProcessMouseMove( pos.x, pos.y, getKeyModifier( event ) );
    return false;
}


bool LibRocket::mouseWheel(MouseEvent event)
{
    m_context->ProcessMouseWheel( (int) event.getWheelIncrement(), getKeyModifier( event ) );
    return false;
}

bool LibRocket::keyDown(KeyEvent event)
{
    auto i = m_cinderKeyToRocket.find(event.getCode());
    if (i != m_cinderKeyToRocket.end())
    {
        //    printf("keydown: cinder: %d, rocket: %d\n", i->first, i->second);
        int modifierState = getKeyModifier(event);
        m_context->ProcessKeyDown(i->second, modifierState);
        Rocket::Core::word character = getCharacterCode(i->second, modifierState);
        if (character > 0)
            m_context->ProcessTextInput(character);
    }
    return false;
}

bool LibRocket::keyUp(KeyEvent event)
{
    auto i = m_cinderKeyToRocket.find(event.getCode());
    if (i != m_cinderKeyToRocket.end())
    {
        m_context->ProcessKeyUp(i->second, getKeyModifier(event));
    }
    return false;
}


void LibRocket::resize()
{
    m_context->SetDimensions( Rocket::Core::Vector2i( getWindowWidth(), getWindowHeight() ) );
}


int LibRocket::getMouseButton(MouseEvent event)
{
	if (event.isLeft())
	{
		return 0;
	}
	else if (event.isRight())
	{
		return 1;
	}
    
	return 2;
}

void LibRocket::initKeyMap()
{
  m_cinderKeyToRocket[KeyEvent::KEY_BACKSPACE] = Rocket::Core::Input::KI_BACK;
  m_cinderKeyToRocket[KeyEvent::KEY_TAB] = Rocket::Core::Input::KI_TAB;
  m_cinderKeyToRocket[KeyEvent::KEY_CLEAR] = Rocket::Core::Input::KI_CLEAR;
  m_cinderKeyToRocket[KeyEvent::KEY_RETURN] = Rocket::Core::Input::KI_RETURN;
  m_cinderKeyToRocket[KeyEvent::KEY_PAUSE] = Rocket::Core::Input::KI_PAUSE;
  m_cinderKeyToRocket[KeyEvent::KEY_ESCAPE] = Rocket::Core::Input::KI_ESCAPE;
  m_cinderKeyToRocket[KeyEvent::KEY_SPACE] = Rocket::Core::Input::KI_SPACE;
  m_cinderKeyToRocket[KeyEvent::KEY_QUOTE] = Rocket::Core::Input::KI_OEM_7;
  m_cinderKeyToRocket[KeyEvent::KEY_PLUS] = Rocket::Core::Input::KI_OEM_PLUS;
  m_cinderKeyToRocket[KeyEvent::KEY_COMMA] = Rocket::Core::Input::KI_OEM_COMMA;
  m_cinderKeyToRocket[KeyEvent::KEY_MINUS] = Rocket::Core::Input::KI_OEM_MINUS;
  m_cinderKeyToRocket[KeyEvent::KEY_PERIOD] = Rocket::Core::Input::KI_OEM_PERIOD;
  m_cinderKeyToRocket[KeyEvent::KEY_SLASH] = Rocket::Core::Input::KI_OEM_2;
  m_cinderKeyToRocket[KeyEvent::KEY_0] = Rocket::Core::Input::KI_0;
  m_cinderKeyToRocket[KeyEvent::KEY_1] = Rocket::Core::Input::KI_1;
  m_cinderKeyToRocket[KeyEvent::KEY_2] = Rocket::Core::Input::KI_2;
  m_cinderKeyToRocket[KeyEvent::KEY_3] = Rocket::Core::Input::KI_3;
  m_cinderKeyToRocket[KeyEvent::KEY_4] = Rocket::Core::Input::KI_4;
  m_cinderKeyToRocket[KeyEvent::KEY_5] = Rocket::Core::Input::KI_5;
  m_cinderKeyToRocket[KeyEvent::KEY_6] = Rocket::Core::Input::KI_6;
  m_cinderKeyToRocket[KeyEvent::KEY_7] = Rocket::Core::Input::KI_7;
  m_cinderKeyToRocket[KeyEvent::KEY_8] = Rocket::Core::Input::KI_8;
  m_cinderKeyToRocket[KeyEvent::KEY_9] = Rocket::Core::Input::KI_9;
  m_cinderKeyToRocket[KeyEvent::KEY_SEMICOLON] = Rocket::Core::Input::KI_OEM_1;
  m_cinderKeyToRocket[KeyEvent::KEY_EQUALS] = Rocket::Core::Input::KI_OEM_PLUS;
  m_cinderKeyToRocket[KeyEvent::KEY_LEFTBRACKET] = Rocket::Core::Input::KI_OEM_4;
  m_cinderKeyToRocket[KeyEvent::KEY_BACKSLASH] = Rocket::Core::Input::KI_OEM_5;
  m_cinderKeyToRocket[KeyEvent::KEY_RIGHTBRACKET] = Rocket::Core::Input::KI_OEM_6;
  m_cinderKeyToRocket[KeyEvent::KEY_BACKQUOTE] = Rocket::Core::Input::KI_OEM_3;
  m_cinderKeyToRocket[KeyEvent::KEY_a] = Rocket::Core::Input::KI_A;
  m_cinderKeyToRocket[KeyEvent::KEY_b] = Rocket::Core::Input::KI_B;
  m_cinderKeyToRocket[KeyEvent::KEY_c] = Rocket::Core::Input::KI_C;
  m_cinderKeyToRocket[KeyEvent::KEY_d] = Rocket::Core::Input::KI_D;
  m_cinderKeyToRocket[KeyEvent::KEY_e] = Rocket::Core::Input::KI_E;
  m_cinderKeyToRocket[KeyEvent::KEY_f] = Rocket::Core::Input::KI_F;
  m_cinderKeyToRocket[KeyEvent::KEY_g] = Rocket::Core::Input::KI_G;
  m_cinderKeyToRocket[KeyEvent::KEY_h] = Rocket::Core::Input::KI_H;
  m_cinderKeyToRocket[KeyEvent::KEY_i] = Rocket::Core::Input::KI_I;
  m_cinderKeyToRocket[KeyEvent::KEY_j] = Rocket::Core::Input::KI_J;
  m_cinderKeyToRocket[KeyEvent::KEY_k] = Rocket::Core::Input::KI_K;
  m_cinderKeyToRocket[KeyEvent::KEY_l] = Rocket::Core::Input::KI_L;
  m_cinderKeyToRocket[KeyEvent::KEY_m] = Rocket::Core::Input::KI_M;
  m_cinderKeyToRocket[KeyEvent::KEY_n] = Rocket::Core::Input::KI_N;
  m_cinderKeyToRocket[KeyEvent::KEY_o] = Rocket::Core::Input::KI_O;
  m_cinderKeyToRocket[KeyEvent::KEY_p] = Rocket::Core::Input::KI_P;
  m_cinderKeyToRocket[KeyEvent::KEY_q] = Rocket::Core::Input::KI_Q;
  m_cinderKeyToRocket[KeyEvent::KEY_r] = Rocket::Core::Input::KI_R;
  m_cinderKeyToRocket[KeyEvent::KEY_s] = Rocket::Core::Input::KI_S;
  m_cinderKeyToRocket[KeyEvent::KEY_t] = Rocket::Core::Input::KI_T;
  m_cinderKeyToRocket[KeyEvent::KEY_u] = Rocket::Core::Input::KI_U;
  m_cinderKeyToRocket[KeyEvent::KEY_v] = Rocket::Core::Input::KI_V;
  m_cinderKeyToRocket[KeyEvent::KEY_w] = Rocket::Core::Input::KI_W;
  m_cinderKeyToRocket[KeyEvent::KEY_x] = Rocket::Core::Input::KI_X;
  m_cinderKeyToRocket[KeyEvent::KEY_y] = Rocket::Core::Input::KI_Y;
  m_cinderKeyToRocket[KeyEvent::KEY_z] = Rocket::Core::Input::KI_Z;
  m_cinderKeyToRocket[KeyEvent::KEY_DELETE] = Rocket::Core::Input::KI_DELETE;
  
  m_cinderKeyToRocket[KeyEvent::KEY_KP0] = Rocket::Core::Input::KI_NUMPAD0;
  m_cinderKeyToRocket[KeyEvent::KEY_KP1] = Rocket::Core::Input::KI_NUMPAD1;
  m_cinderKeyToRocket[KeyEvent::KEY_KP2] = Rocket::Core::Input::KI_NUMPAD2;
  m_cinderKeyToRocket[KeyEvent::KEY_KP3] = Rocket::Core::Input::KI_NUMPAD3;
  m_cinderKeyToRocket[KeyEvent::KEY_KP4] = Rocket::Core::Input::KI_NUMPAD4;
  m_cinderKeyToRocket[KeyEvent::KEY_KP5] = Rocket::Core::Input::KI_NUMPAD5;
  m_cinderKeyToRocket[KeyEvent::KEY_KP6] = Rocket::Core::Input::KI_NUMPAD6;
  m_cinderKeyToRocket[KeyEvent::KEY_KP7] = Rocket::Core::Input::KI_NUMPAD7;
  m_cinderKeyToRocket[KeyEvent::KEY_KP8] = Rocket::Core::Input::KI_NUMPAD8;
  m_cinderKeyToRocket[KeyEvent::KEY_KP9] = Rocket::Core::Input::KI_NUMPAD9;
  
  m_cinderKeyToRocket[KeyEvent::KEY_KP_PERIOD] = Rocket::Core::Input::KI_DECIMAL;
  m_cinderKeyToRocket[KeyEvent::KEY_KP_DIVIDE] = Rocket::Core::Input::KI_DIVIDE;
  m_cinderKeyToRocket[KeyEvent::KEY_KP_MULTIPLY] = Rocket::Core::Input::KI_MULTIPLY;
  m_cinderKeyToRocket[KeyEvent::KEY_KP_MINUS] = Rocket::Core::Input::KI_OEM_MINUS;
  m_cinderKeyToRocket[KeyEvent::KEY_KP_PLUS] = Rocket::Core::Input::KI_ADD;
  m_cinderKeyToRocket[KeyEvent::KEY_KP_ENTER] = Rocket::Core::Input::KI_NUMPADENTER;
  m_cinderKeyToRocket[KeyEvent::KEY_KP_EQUALS] = Rocket::Core::Input::KI_OEM_NEC_EQUAL;
  
  m_cinderKeyToRocket[KeyEvent::KEY_UP] = Rocket::Core::Input::KI_UP;
  m_cinderKeyToRocket[KeyEvent::KEY_DOWN] = Rocket::Core::Input::KI_DOWN;
  m_cinderKeyToRocket[KeyEvent::KEY_RIGHT] = Rocket::Core::Input::KI_RIGHT;
  m_cinderKeyToRocket[KeyEvent::KEY_LEFT] = Rocket::Core::Input::KI_LEFT;
  m_cinderKeyToRocket[KeyEvent::KEY_INSERT] = Rocket::Core::Input::KI_INSERT;
  m_cinderKeyToRocket[KeyEvent::KEY_HOME] = Rocket::Core::Input::KI_HOME;
  m_cinderKeyToRocket[KeyEvent::KEY_END] = Rocket::Core::Input::KI_END;
  m_cinderKeyToRocket[KeyEvent::KEY_PAGEUP] = Rocket::Core::Input::KI_PRIOR;
  m_cinderKeyToRocket[KeyEvent::KEY_PAGEDOWN] = Rocket::Core::Input::KI_NEXT;
  
  m_cinderKeyToRocket[KeyEvent::KEY_F1] = Rocket::Core::Input::KI_F1;
  m_cinderKeyToRocket[KeyEvent::KEY_F2] = Rocket::Core::Input::KI_F2;
  m_cinderKeyToRocket[KeyEvent::KEY_F3] = Rocket::Core::Input::KI_F3;
  m_cinderKeyToRocket[KeyEvent::KEY_F4] = Rocket::Core::Input::KI_F4;
  m_cinderKeyToRocket[KeyEvent::KEY_F5] = Rocket::Core::Input::KI_F5;
  m_cinderKeyToRocket[KeyEvent::KEY_F6] = Rocket::Core::Input::KI_F6;
  m_cinderKeyToRocket[KeyEvent::KEY_F7] = Rocket::Core::Input::KI_F7;
  m_cinderKeyToRocket[KeyEvent::KEY_F8] = Rocket::Core::Input::KI_F8;
  m_cinderKeyToRocket[KeyEvent::KEY_F9] = Rocket::Core::Input::KI_F9;
  m_cinderKeyToRocket[KeyEvent::KEY_F10] = Rocket::Core::Input::KI_F10;
  m_cinderKeyToRocket[KeyEvent::KEY_F11] = Rocket::Core::Input::KI_F11;
  m_cinderKeyToRocket[KeyEvent::KEY_F12] = Rocket::Core::Input::KI_F12;
  m_cinderKeyToRocket[KeyEvent::KEY_F13] = Rocket::Core::Input::KI_F13;
  m_cinderKeyToRocket[KeyEvent::KEY_F14] = Rocket::Core::Input::KI_F14;
  m_cinderKeyToRocket[KeyEvent::KEY_F15] = Rocket::Core::Input::KI_F15;
  
  m_cinderKeyToRocket[KeyEvent::KEY_NUMLOCK] = Rocket::Core::Input::KI_NUMLOCK;
  m_cinderKeyToRocket[KeyEvent::KEY_CAPSLOCK] = Rocket::Core::Input::KI_CAPITAL;
  m_cinderKeyToRocket[KeyEvent::KEY_SCROLLOCK] = Rocket::Core::Input::KI_SCROLL;
  m_cinderKeyToRocket[KeyEvent::KEY_RSHIFT] = Rocket::Core::Input::KI_RSHIFT;
  m_cinderKeyToRocket[KeyEvent::KEY_LSHIFT] = Rocket::Core::Input::KI_LSHIFT;
  m_cinderKeyToRocket[KeyEvent::KEY_RCTRL] = Rocket::Core::Input::KI_RCONTROL;
  m_cinderKeyToRocket[KeyEvent::KEY_LCTRL] = Rocket::Core::Input::KI_LCONTROL;
  
  m_cinderKeyToRocket[KeyEvent::KEY_RMETA] = Rocket::Core::Input::KI_RMETA;
  m_cinderKeyToRocket[KeyEvent::KEY_LMETA] = Rocket::Core::Input::KI_LMETA;
  
  m_cinderKeyToRocket[KeyEvent::KEY_HELP] = Rocket::Core::Input::KI_HELP;
  m_cinderKeyToRocket[KeyEvent::KEY_PRINT] = Rocket::Core::Input::KI_SNAPSHOT;
  m_cinderKeyToRocket[KeyEvent::KEY_MENU] = Rocket::Core::Input::KI_LMENU;
  m_cinderKeyToRocket[KeyEvent::KEY_POWER] = Rocket::Core::Input::KI_POWER;
}

char ascii_map[4][51] =
{
  // shift off and capslock off
  {
		0,
		' ',
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'i',
		'j',
		'k',
		'l',
		'm',
		'n',
		'o',
		'p',
		'q',
		'r',
		's',
		't',
		'u',
		'v',
		'w',
		'x',
		'y',
		'z',
		';',
		'=',
		',',
		'-',
		'.',
		'/',
		'`',
		'[',
		'\\',
		']',
		'\'',
		0,
		0
	},
  
	// shift on and capslock off
  {
		0,
		' ',
		')',
		'!',
		'@',
		'#',
		'$',
		'%',
		'^',
		'&',
		'*',
		'(',
		'A',
		'B',
		'C',
		'D',
		'E',
		'F',
		'G',
		'H',
		'I',
		'J',
		'K',
		'L',
		'M',
		'N',
		'O',
		'P',
		'Q',
		'R',
		'S',
		'T',
		'U',
		'V',
		'W',
		'X',
		'Y',
		'Z',
		':',
		'+',
		'<',
		'_',
		'>',
		'?',
		'~',
		'{',
		'|',
		'}',
		'"',
		0,
		0
	},
  
	// shift on and capslock on
  {
		0,
		' ',
		')',
		'!',
		'@',
		'#',
		'$',
		'%',
		'^',
		'&',
		'*',
		'(',
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'i',
		'j',
		'k',
		'l',
		'm',
		'n',
		'o',
		'p',
		'q',
		'r',
		's',
		't',
		'u',
		'v',
		'w',
		'x',
		'y',
		'z',
		':',
		'+',
		'<',
		'_',
		'>',
		'?',
		'~',
		'{',
		'|',
		'}',
		'"',
		0,
		0
	},
  
	// shift off and capslock on
  {
		0,
		' ',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'0',
		'A',
		'B',
		'C',
		'D',
		'E',
		'F',
		'G',
		'H',
		'I',
		'J',
		'K',
		'L',
		'M',
		'N',
		'O',
		'P',
		'Q',
		'R',
		'S',
		'T',
		'U',
		'V',
		'W',
		'X',
		'Y',
		'Z',
		';',
		'=',
		',',
		'-',
		'.',
		'/',
		'`',
		'[',
		'\\',
		']',
		'\'',
		0,
		0
	}
};

char keypad_map[2][18] =
{
	{
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'\n',
		'*',
		'+',
		0,
		'-',
		'.',
		'/',
		'='
	},
  
	{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		'\n',
		'*',
		'+',
		0,
		'-',
		0,
		'/',
		'='
	}
};

// Returns the character code for a key identifer / key modifier combination.
Rocket::Core::word LibRocket::getCharacterCode(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state)
{
	// Check if we have a keycode capable of generating characters on the main keyboard (ie, not on the numeric
	// keypad; that is dealt with below).
	if (key_identifier <= Rocket::Core::Input::KI_OEM_102)
	{
		// Get modifier states
		bool shift = (key_modifier_state & Rocket::Core::Input::KM_SHIFT) > 0;
		bool capslock = (key_modifier_state & Rocket::Core::Input::KM_CAPSLOCK) > 0;
    
		// Return character code based on identifier and modifiers
		if (shift && !capslock)
			return ascii_map[1][key_identifier];
    
		if (shift && capslock)
			return ascii_map[2][key_identifier];
    
		if (!shift && capslock)
			return ascii_map[3][key_identifier];
    
		return ascii_map[0][key_identifier];
	}
  
	// Check if we have a keycode from the numeric keypad.
	else if (key_identifier <= Rocket::Core::Input::KI_OEM_NEC_EQUAL)
	{
		if (key_modifier_state & Rocket::Core::Input::KM_NUMLOCK)
			return keypad_map[0][key_identifier - Rocket::Core::Input::KI_NUMPAD0];
		else
			return keypad_map[1][key_identifier - Rocket::Core::Input::KI_NUMPAD0];
	}
  
	else if (key_identifier == Rocket::Core::Input::KI_RETURN)
		return '\n';
  
	return 0;
}