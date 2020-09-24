#ifndef TEMPLATE_BASIC_DISPLAY_HPP_
#define TEMPLATE_BASIC_DISPLAY_HPP_

#include <driver/basic_display.hpp>

namespace embdrv
{
/** Documentation here
 */
class TemplateDisplayDriver final : public embvm::basicDisplay
{
  public:
	/// Address is 0x3D if DC pin is set to 1
	explicit TemplateDisplayDriver() : embvm::basicDisplay("TemplateDisplayDriver") {}

	void clear() noexcept final;
	void invert(enum invert inv) noexcept final;
	void contrast(uint8_t contrast) noexcept final;
	void cursor(coord_t x, coord_t y) noexcept final;
	void pixel(coord_t x, coord_t y, color c, mode m) noexcept final;
	void line(coord_t x0, coord_t y0, coord_t x1, coord_t y1, color c, mode m) noexcept final;
	void rect(coord_t x, coord_t y, uint8_t width, uint8_t height, color c, mode m) noexcept final;
	void rectFill(coord_t x, coord_t y, uint8_t width, uint8_t height, color c,
				  mode m) noexcept final;
	void circle(coord_t x, coord_t y, uint8_t radius, color c, mode m) noexcept final;
	void circleFill(coord_t x, coord_t y, uint8_t radius, color c, mode m) noexcept final;
	void drawChar(coord_t x, coord_t y, uint8_t character, color c, mode m) noexcept final;
	void drawBitmap(uint8_t* bitmap) noexcept final;
	uint8_t screenWidth() const noexcept final;
	uint8_t screenHeight() const noexcept final;

	void scrollRight(coord_t start, coord_t stop) noexcept final;
	void scrollLeft(coord_t start, coord_t stop) noexcept final;
	void scrollVertRight(coord_t start, coord_t stop) noexcept final;
	void scrollVertLeft(coord_t start, coord_t stop) noexcept final;
	void scrollStop() noexcept final;

	void flipVertical(bool flip) noexcept final;
	void flipHorizontal(bool flip) noexcept final;

	void display() noexcept final;

	void putchar(uint8_t c) noexcept final;

  private:
	void start_() noexcept final;
	void stop_() noexcept final;
};

} // namespace embdrv

#endif // TEMPLATE_BASIC_DISPLAY_HPP_
