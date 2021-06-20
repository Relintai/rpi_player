#ifndef VLC_SCENE_H
#define VLC_SCENE_H

#include "scene.h"

#include <SDL.h>

#include "vlc/vlc.h"

class VLCScene : public Scene {
public:
	void event(const SDL_Event &ev);
	void update(float delta);
	void render();

	VLCScene();
	~VLCScene();

	SDL_Texture *texture;
	SDL_mutex *mutex;
	int n;

	libvlc_instance_t *libvlc;
	libvlc_media_t *m;
	libvlc_media_player_t *mp;

	static VLCScene *scene;

	char *chroma;

	bool r;
	int done;
	int action;
	int pause;
};

#endif