#include "vlc_scene.h"

#include "renderer/renderer.h"

#include <stdio.h>

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define VIDEOWIDTH 320
#define VIDEOHEIGHT 240

// VLC prepares to render a video frame.
static void *lock(void *data, void **p_pixels) {

	VLCScene *c = (VLCScene *)data;

	int pitch;
	SDL_LockMutex(c->mutex);
	SDL_LockTexture(c->texture, NULL, p_pixels, &pitch);

	return NULL; // Picture identifier, not needed here.
}

// VLC just rendered a video frame.
static void unlock(void *data, void *id, void *const *p_pixels) {

	VLCScene *c = (VLCScene *)data;

	uint16_t *pixels = (uint16_t *)*p_pixels;

	// We can also render stuff.
	int x, y;
	for (y = 10; y < 40; y++) {
		for (x = 10; x < 40; x++) {
			if (x < 13 || y < 13 || x > 36 || y > 36) {
				pixels[y * VIDEOWIDTH + x] = 0xffff;
			} else {
				// RV16 = 5+6+5 pixels per color, BGR.
				pixels[y * VIDEOWIDTH + x] = 0x02ff;
			}
		}
	}

	SDL_UnlockTexture(c->texture);
	SDL_UnlockMutex(c->mutex);
}

// VLC wants to display a video frame.
static void display(void *data, void *id) {

	VLCScene *c = (VLCScene *)data;

	int w = Renderer::get_singleton()->get_window_size_w();
	int h = Renderer::get_singleton()->get_window_size_h();

	SDL_Rect rect;
	rect.w = VIDEOWIDTH;
	rect.h = VIDEOHEIGHT;
	rect.x = (int)((1. + .5 * sin(0.03 * c->n)) * (w - VIDEOWIDTH) / 2);
	rect.y = (int)((1. + .5 * cos(0.03 * c->n)) * (h - VIDEOHEIGHT) / 2);

	Renderer::get_singleton()->set_draw_color(0, 80, 0, 255);
	Renderer::get_singleton()->clear();
	SDL_RenderCopy(Renderer::get_singleton()->get_renderer(), c->texture, NULL, &rect);
	//Renderer::get_singleton()->present();

	//SDL_SetRenderDrawColor(Renderer::get_singleton()->get_renderer(), 0, 80, 0, 255);
	//SDL_RenderClear(Renderer::get_singleton()->get_renderer());
	//SDL_RenderCopy(Renderer::get_singleton()->get_renderer(), c->texture, NULL, &rect);
	//SDL_RenderPresent(Renderer::get_singleton()->get_renderer());
}

void VLCScene::event(const SDL_Event &ev) {

	action = 0;

	switch (ev.type) {
		case SDL_KEYDOWN:
			action = ev.key.keysym.sym;
			break;
	}

	switch (action) {
		case ' ':
			printf("Pause toggle.\n");
			pause = !pause;
			break;
	}
}

void VLCScene::update(float delta) {
	if (!r) {
		r = true;
		printf("play\n");
		libvlc_media_player_play(mp);
	}

	if (!pause) {
		n++;
	}
}

void VLCScene::render() {
	Renderer::get_singleton()->present();
}

VLCScene::VLCScene() {
	r = false;
	done = 0;
	action = 0;
	pause = 0;

	texture = SDL_CreateTexture(
			Renderer::get_singleton()->get_renderer(),
			SDL_PIXELFORMAT_BGR565, SDL_TEXTUREACCESS_STREAMING,
			VIDEOWIDTH, VIDEOHEIGHT);

	if (!texture) {
		fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
		return;
	}

	mutex = SDL_CreateMutex();

	char const *vlc_argv[] = {
		//"--no-audio", // Don't play audio.
		"--no-xlib", // Don't use Xlib.

		// Apply a video filter.
		//"--video-filter", "sepia",
		//"--sepia-intensity=200"
	};
	int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

	if (getenv("VLC_PLUGIN_PATH") == nullptr) {
		setenv("VLC_PLUGIN_PATH", "/usr/lib/vlc/plugins", 1);
	}

	// If you don't have this variable set you must have plugins directory
	// with the executable or libvlc_new() will not work!
	printf("VLC_PLUGIN_PATH=%s\n", getenv("VLC_PLUGIN_PATH"));

	// Initialise libVLC.
	libvlc = libvlc_new(vlc_argc, vlc_argv);
	if (NULL == libvlc) {
		printf("LibVLC initialization failure.\n");
		return;
	}

	m = libvlc_media_new_path(libvlc, "./test.mp4");

	mp = libvlc_media_player_new_from_media(m);
	libvlc_media_release(m);

	libvlc_video_set_callbacks(mp, lock, unlock, display, this);
	libvlc_video_set_format(mp, "RV16", VIDEOWIDTH, VIDEOHEIGHT, VIDEOWIDTH * 2);
}

VLCScene::~VLCScene() {

	// Stop stream and clean up libVLC.
	libvlc_media_player_stop(mp);
	libvlc_media_player_release(mp);
	libvlc_release(libvlc);

	// Close window and clean up libSDL.
	SDL_DestroyMutex(mutex);
}
