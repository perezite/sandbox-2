#include "DrawTarget.h"
#include <iostream>

namespace sb
{
	void DrawTarget::draw(Drawable* drawable, const DrawStates& states)
	{
		drawable->draw(*this, states);
	}

	void DrawTarget::draw(Drawable& drawable, const DrawStates& states)
	{
		drawable.draw(*this, states);
	}
}