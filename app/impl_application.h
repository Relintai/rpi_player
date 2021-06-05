#ifndef IMPL_APPLICATION_H
#define IMPL_APPLICATION_H

#include "application.h"

#include "main_scene.h"
#include "vlc_scene.h"

class ImplApplication : public Application {
public:
    ImplApplication() : Application() {
        scene = new VLCScene();
    }
    ~ImplApplication() {
        delete scene;
    }
};

#endif