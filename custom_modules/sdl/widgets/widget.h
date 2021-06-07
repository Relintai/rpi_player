
#ifndef WIDGET_H
#define WIDGET_H

#include <SDL.h>

#include "core/rect2.h"
#include "core/vector.h"

class Widget {
public:
	enum StretchFlags {
		STRETCH_HORIZONTAL = 1 << 0,
		STRETCH_VERTICAL = 1 << 1,

		STRETCH_OFF = 0,
		STRETCH_BOTH = STRETCH_HORIZONTAL | STRETCH_VERTICAL,
	};

	void event(const SDL_Event &ev);
	void update(float delta);
	void render();

	virtual void _event(const SDL_Event &ev);
	virtual void _update(float delta);
	virtual void _render();

	Widget();
	virtual ~Widget();

	float min_size;
	int stretch_flags;
	float stretch_ratio;

	Rect2 rect;

private:
	Vector<Widget *> _children;
};

#endif
