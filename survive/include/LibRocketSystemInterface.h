
#ifndef LIBROCKET_SYSTEMINTERFACE
#define LIBROCKET_SYSTEMINTERFACE

#include <cinder/app/App.h>

#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/Log.h>
#include <Rocket/Core/Types.h>

class LibRocketSystemInterface: public Rocket::Core::SystemInterface {

public:
    
	LibRocketSystemInterface() {}
	
    ~LibRocketSystemInterface() {}

    float GetElapsedTime() { return (float) ci::app::getElapsedSeconds(); }

};

#endif
