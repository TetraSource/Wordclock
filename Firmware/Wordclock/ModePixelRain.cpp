
#include "ModePixelRain.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	ModePixelRainBase::PixelDrop* ModePixelRainBase::pixels =
		new ModePixelRainBase::PixelDrop[0];

	uint8_t ModePixelRainBase::count = 0;

	ModePixelRainBase::ModePixelRainBase()
		: ModeBase()
	{}

	void ModePixelRainBase::internalSelect(const uint8_t &newCount)
	{
		this->count = 0;
		this->pixels = new PixelDrop[newCount];
		Wordclock::startTimer(this, 1, 0);
	}

	void ModePixelRainBase::deselect()
	{
		if (isInTransition()) {
			ModeBase::deselect();
		}
		else {
			delete[] pixels;
			Wordclock::cancelTimer(this, 0);
		}
	}

	void ModePixelRainBase::movePixels()
	{
		// Move pixels alongside axis and remove them if they leave the screen.
		for (uint8_t i = 0; i < this->count; i++) {
			if ((this->pixels[i].dir == Top || this->pixels[i].dir == Left)
				&& ++this->pixels[i].pos - this->pixels[i].len >=
				(this->pixels[i].dir == Left ? Painter::width : Painter::height)
				||
				(this->pixels[i].dir == Bottom || this->pixels[i].dir == Right)
				&& --this->pixels[i].pos + this->pixels[i].len < 0)
			{
				for (uint8_t j = i + 1; j < this->count; j++) {
					this->pixels[j - 1] = this->pixels[j];
				}
				this->count--;
			}
		}
	}

	void ModePixelRainBase::genPixel(const uint8_t &directions,
		const uint8_t &minLength, const uint8_t &maxLength, const CRGB &color)
	{
		ModePixelRainBase::PixelDrop* pixel = &pixels[count++];
		pixel->color = color;

		// set direction
		uint8_t dirCount = 0;
		for (uint8_t i = 0; i <= Right; i++) {
			if (directions & (1 << i))
				dirCount++;
		}
		if (dirCount > 0) {
			uint8_t dir = random(dirCount);
			for (uint8_t i = 0; i <= dir; i++) {
				if (!(directions & (1 << i)))
					dir++;
			}

			// set anything else
			pixel->dir = dir;
			pixel->len = random(minLength, maxLength + 1);
			pixel->axisPos = random(
				dir <= Bottom ? Painter::width : Painter::height);

			if (dir == Bottom)
				pixel->pos = Painter::height - 1;
			else if (dir == Right)
				pixel->pos = Painter::width - 1;
			else
				pixel->pos = 0;
		}
	}

#define DRAW_FLYING_PIXEL(x, y) \
for (uint8_t j = 0; j < length; j++) { \
	uint8_t factor = length - j; \
	Painter::setColor(CRGB( \
		(color.r * factor) / length, \
		(color.g * factor) / length, \
		(color.b * factor) / length)); \
	Painter::paint(x, y); \
}

	void ModePixelRainBase::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
			return;
		}
		for (uint8_t i = 0; i < count; i++) {
			CRGB color = pixels[i].color;
			uint8_t length = pixels[i].len;

			switch (pixels[i].dir)
			{
			case Top:
				DRAW_FLYING_PIXEL(pixels[i].axisPos, pixels[i].pos - j)
				break;
			case Bottom:
				DRAW_FLYING_PIXEL(pixels[i].axisPos, pixels[i].pos + j)
				break;
			case Left:
				DRAW_FLYING_PIXEL(pixels[i].pos - j, pixels[i].axisPos)
				break;
			// case Right:
			default:
				DRAW_FLYING_PIXEL(pixels[i].pos + j, pixels[i].axisPos)
			}
		}
	}
}