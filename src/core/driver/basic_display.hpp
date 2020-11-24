// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef BASIC_DISPLAY_HPP_
#define BASIC_DISPLAY_HPP_

#include "driver.hpp"
#include <cassert>
#include <cstring>

namespace embvm
{
/** This class provides an interface for a basic 2-color display device.
 *
 * The basicDisplay class defines common interfaces for basic 2-color display drivers, which can be
 * LED or OLED.
 *
 * # Define an basic display driver
 *
 * To create a basic display driver, derive from the basicDisplay object:
 *
 * @code
 * class ssd1306 final : public basicDisplay
 * { ... };
 * @endcode
 *
 * Derived classes must implement the following functions:
 * - pixel(coord_t x, coord_t y, color c, mode m), which enables the other draw functions
 * - display()
 * - clear()
 * - invert()
 * - contrast()
 * - cursor()
 * - screenWidth()
 * - screenHeight()
 * - scrollRight()
 * - scrollLeft()
 * - scrollVertRight()
 * - scrollVertLeft()
 * - scrollStop()
 * - flipVertical()
 * - flipHorizontal()
 *
 *
 * Derived classes must also implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 *
 * Turning on a dispay should be handled in the start_() function, and turning off the display
 * should be handled in the stop_() function.
 *
 * # Using a basic display
 *
 * The display is turned on by calling start(), and turned off by calling stop().
 *
 * Most display functions update the screen buffer, which is the representation of the display
 * buffer contents in local memory.
 *
 * @code
 * snprintf(tof_string_, 32, "ToF Range: %u mm\n", v);
 * screen0.printString(0, 0, tof_string_);
 * screen0.printString(0, 32, tof_mode_string_);
 * @endcode
 *
 * Changes to the screen buffer are not written to the display's hardware buffer until the
 * display() function is called.
 *
 * @code
 * screen0.display();
 * @endcode
 *
 * The screen buffer can be updated incrementally. If clearing the entire buffer is desired,
 * call the clear() function.
 *
 * @code
 * screen0.clear();
 * @endcode
 *
 * @ingroup FrameworkDriver
 */
class basicDisplay : public embvm::DriverBase
{
  public:
	/// Display color inversion options.
	enum class invert : uint8_t
	{
		/// Use the default display color setting.
		normal = 0,
		/// Invert the display colors.
		invert
	};

	/// Set the drawing mode, if supported by the driver.
	enum class mode : uint8_t
	{
		/// Either sets or clears a pixel as requested, overwriting the value set in the
		/// screen buffer.
		normal = 0,
		/// The pixel output in the screen buffer will be an XOR of the current value and the
		/// input value.
		XOR
	};

	/// Basic displays have two pixel colors: black and white.
	enum class color : uint8_t
	{
		black = 0,
		white
	};

	/// Coordinate representation type.
	using coord_t = uint8_t;

  protected:
	/** Default constructor.
	 *
	 * Initializes the DriverBase class with a BASIC_DISPLAY type ID
	 */
	basicDisplay() noexcept : embvm::DriverBase(embvm::DriverType::BASIC_DISPLAY) {}

  public:
	/** Basic Display Driver Type ID
	 *
	 * @returns Basic Display type ID.
	 */
	static constexpr embvm::DriverType type() noexcept
	{
		return embvm::DriverType::BASIC_DISPLAY;
	}

	/// Clear the screen contents.
	virtual void clear() noexcept = 0;

	/// Flush the screen buffer contents to the hardware display buffer.
	virtual void display() noexcept = 0;

	/** Invert the display color.
	 * The WHITE color of the display will turn to BLACK and the BLACK will turn to WHITE.
	 *
	 * @param inv invert::invert uses color inversion, invert::normal uses the display standard.
	 */
	virtual void invert(invert inv) noexcept = 0;

	/** Set contrast.
	 *
	 * @param contrast Target display contrast value, ranging from 0 to 255.
	 */
	virtual void contrast(uint8_t contrast) noexcept = 0;

	/** Set the cusor within the screen buffer.
	 *
	 * Contents in the screen buffer are not modified by setting the cursor position.
	 *
	 * @param x X-coordinate of the cursor.
	 * @param y Y-coordinate of the cursor.
	 */
	virtual void cursor(coord_t x, coord_t y) noexcept = 0;

	/** Draw a pixel in the screen buffer.
	 *
	 * Draw pixel using the configured color setting and draw mode in the screen buffer's x,y
	 * position.
	 *
	 * @param x X-coordinate of the pixel.
	 * @param y Y-coordinate of the pixel.
	 */
	void pixel(coord_t x, coord_t y) noexcept
	{
		pixel(x, y, color_, mode_);
	}

	/** Draw a pixel with a specific color and mode.
	 *
	 * Draw a pixel at position (x,y) with a specific color setting and draw mode.
	 *
	 * @param x X-coordinate of the pixel.
	 * @param y Y-coordinate of the pixel.
	 * @param c Pixel color.
	 * @param m Mode used to update the screen buffer.
	 */
	virtual void pixel(coord_t x, coord_t y, color c, mode m) noexcept = 0;

	/** Draw a line.
	 *
	 * Draw a line using the configured color setting and draw mode from (x0,y0) to (x1,y1) of the
	 * screen buffer.
	 *
	 * @param x0 X-coordinate where the line originates.
	 * @param y0 Y-coordinate where the line originates.
	 * @param x1 X-coordinate where the line terminates.
	 * @param y1 Y-coordinate where the line terminates.
	 */
	void line(coord_t x0, coord_t y0, coord_t x1, coord_t y1) noexcept
	{
		line(x0, y0, x1, y1, color_, mode_);
	}

	/** Draw a line with a specific color and mode.
	 *
	 * Draw line with a specific color setting and draw mode from (x0,y0) to (x1,y1) of the screen
	 * buffer.
	 *
	 * @param x0 X-coordinate where the line originates.
	 * @param y0 Y-coordinate where the line originates.
	 * @param x1 X-coordinate where the line terminates.
	 * @param y1 Y-coordinate where the line terminates.
	 * @param c Pixel color.
	 * @param m Mode used to update the screen buffer.
	 */
	virtual void line(coord_t x0, coord_t y0, coord_t x1, coord_t y1, color c, mode m) noexcept = 0;

	/** Draw a horizontal line.
	 *
	 * Draw a horizontal line using the configured color setting and draw mode from (x,y) to
	 * (x+width,y) in the screen buffer.
	 *
	 * @param x X-coordinate where the line originates.
	 * @param y Y-coordinate where the line originates.
	 * @param width Length of the line, in pixels. The line will terminate at coordinate
	 *	(x+width, y).
	 */
	void lineH(coord_t x, coord_t y, uint8_t width) noexcept
	{
		lineH(x, y, width, color_, mode_);
	}

	/** Draw a horizontal line with a specific color and mode.
	 *
	 * Draw a horizontal line with a specific color setting and draw mode from (x,y) to
	 * (x+width,y) in the screen buffer.
	 *
	 * @param x X-coordinate where the line originates.
	 * @param y Y-coordinate where the line originates.
	 * @param width Length of the line, in pixels. The line will terminate at coordinate
	 *	(x+width, y).
	 * @param c Pixel color.
	 * @param m Mode used to update the screen buffer.
	 */
	void lineH(coord_t x, coord_t y, uint8_t width, color c, mode m) noexcept
	{
		line(x, y, x + width, y, c, m);
	}

	/** Draw a vertical line.
	 *
	 * Draw a vertical line using the configured color setting and draw mode from (x,y) to
	 * (x,y+height) in the screen buffer.
	 *
	 * @param x X-coordinate where the line originates.
	 * @param y Y-coordinate where the line originates.
	 * @param height Length of the line, in pixels. The line will terminate at coordinate
	 *	(x, y+height).
	 */
	void lineV(coord_t x, coord_t y, uint8_t height) noexcept
	{
		lineV(x, y, height, color_, mode_);
	}

	/** Draw a vertical line with a specific color and mode.
	 *
	 * Draw a vertical line using a specific color setting and draw mode from (x,y) to
	 * (x,y+height) in the screen buffer.
	 *
	 * @param x X-coordinate where the line originates.
	 * @param y Y-coordinate where the line originates.
	 * @param height Length of the line, in pixels. The line will terminate at coordinate
	 *	(x, y+height).
	 * @param c Pixel color.
	 * @param m Mode used to update the screen buffer.
	 */
	void lineV(coord_t x, coord_t y, uint8_t height, color c, mode m) noexcept
	{
		line(x, y, x, y + height, c, m);
	}

	/** Draw a rectangle.
	 *
	 * Draw a rectangle using the configured color setting and draw mode from (x,y)
	 * to (x+width,y+height) in the screen buffer. Only the boundary of the rectangle will be drawn.
	 *
	 * @param x X-coordinate where the rectangle originates.
	 * @param y Y-coordinate where the rectangle originates.
	 * @param width Width of the rectangle, in pixels.
	 *	The rectangle width spans from x to x+width.
	 * @param height Height of the rectangle, in pixels.
	 *	The rectangle height spans from y + y+height.
	 */
	void rect(coord_t x, coord_t y, uint8_t width, uint8_t height) noexcept
	{
		rect(x, y, width, height, color_, mode_);
	}

	/** Draw a rectangle with a specific color and mode.
	 *
	 * Draw a rectangle with a specific color setting and draw mode from (x,y) to
	 * (x+width,y+height) in the screen buffer. Only the boundary of the rectangle will be drawn.
	 *
	 * @param x X-coordinate where the rectangle originates.
	 * @param y Y-coordinate where the rectangle originates.
	 * @param width Width of the rectangle, in pixels.
	 *	The rectangle width spans from x to x+width.
	 * @param height Height of the rectangle, in pixels.
	 *	The rectangle height spans from y + y+height.
	 * @param c Pixel color.
	 * @param m Mode used to update the screen buffer.
	 */
	virtual void rect(coord_t x, coord_t y, uint8_t width, uint8_t height, color c,
					  mode m) noexcept = 0;

	/** Draw a filled rectangle.
	 *
	 * Draw a filled rectangle using the configured color setting and draw mode from (x,y) to
	 * (x+width,y+height) in the screen buffer. The interior of the rectangle will be filled with
	 * the configured color setting.
	 *
	 * @param x X-coordinate where the rectangle originates.
	 * @param y Y-coordinate where the rectangle originates.
	 * @param width Width of the rectangle, in pixels.
	 *	The rectangle width spans from x to x+width.
	 * @param height Height of the rectangle, in pixels.
	 *	The rectangle height spans from y + y+height.
	 */
	void rectFill(coord_t x, coord_t y, uint8_t width, uint8_t height) noexcept
	{
		rectFill(x, y, width, height, color_, mode_);
	}

	/** Draw a filled rectangle with a specific color and mode.
	 *
	 * Draw a filled rectangle using a specific color setting and draw mode from (x,y) to
	 * (x+width,y+height) in the screen buffer. The interior of the rectangle will be filled with
	 * the specified color setting.
	 *
	 * @param x X-coordinate where the rectangle originates.
	 * @param y Y-coordinate where the rectangle originates.
	 * @param width Width of the rectangle, in pixels.
	 *	The rectangle width spans from x to x+width.
	 * @param height Height of the rectangle, in pixels.
	 *	The rectangle height spans from y + y+height.
	 * @param c Pixel color.
	 * @param m Mode used to update the screen buffer.
	 */
	virtual void rectFill(coord_t x, coord_t y, uint8_t width, uint8_t height, color c,
						  mode m) noexcept = 0;

	/** Draw a circle.
	 *
	 * Draw a circle with center (x,y) and the specified radius using the configured color setting
	 * and draw mode in the screen buffer. Only the boundary of the circle will be drawn.
	 *
	 * @param x X-coorindate for the center of the circle.
	 * @param y Y-coordinate for the center of the circle.
	 * @param radius The radius of the circle.
	 */
	void circle(coord_t x, coord_t y, uint8_t radius) noexcept
	{
		circle(x, y, radius, color_, mode_);
	}

	/** Draw a circle with a specific color and mode.
	 *
	 * Draw a circle with center (x,y) and the specified radius using a specific color setting
	 * and draw mode in the screen buffer. Only the boundary of the circle will be drawn.
	 *
	 * @param x X-coorindate for the center of the circle.
	 * @param y Y-coordinate for the center of the circle.
	 * @param radius The radius of the circle.
	 * @param c Pixel color.
	 * @param m Mode used to update the screen buffer.
	 */
	virtual void circle(coord_t x, coord_t y, uint8_t radius, color c, mode m) noexcept = 0;

	/** Draw a filled circle.
	 *
	 * Draw a filled circle with center (x,y) and the specified radius using the configured color
	 * setting and draw mode in the screen buffer. The interior of the circle will be filled with
	 * the specified color setting.
	 *
	 * @param x X-coorindate for the center of the circle.
	 * @param y Y-coordinate for the center of the circle.
	 * @param radius The radius of the circle.
	 */
	void circleFill(coord_t x, coord_t y, uint8_t radius) noexcept
	{
		circleFill(x, y, radius, color_, mode_);
	}

	/** Draw a filled circle with a specific color and mode.
	 *
	 * Draw a filled circle with center (x,y) and the specified radius using a specific color
	 * setting and draw mode in the screen buffer. The interior of the circle will be filled with
	 * the specified color setting.
	 *
	 * @param x X-coorindate for the center of the circle.
	 * @param y Y-coordinate for the center of the circle.
	 * @param radius The radius of the circle.
	 * @param c Pixel color.
	 * @param m Mode used to update the screen buffer.
	 */
	virtual void circleFill(coord_t x, coord_t y, uint8_t radius, color c, mode m) noexcept = 0;

	/** Draw an ASCII character.
	 *
	 * Draw an ASCII character using the configured color and draw mode with the upper left corner
	 * of the character located at (x,y).
	 *
	 * @param x X-coordinate representing the top-left of the character.
	 * @param y Y-coordinate representing the top-left of the character.
	 * @param character the ASCII character to draw. Must be present in the font file. If the
	 *	requested character is not present, nothing will be drawn on the screen buffer.
	 */
	void drawChar(coord_t x, coord_t y, uint8_t character) noexcept
	{
		drawChar(x, y, character, color_, mode_);
	}

	/// Print a character on the display
	/// @param c the character to print to the display
	virtual void putchar(uint8_t c) noexcept = 0;

	/** Draw an ASCII character with a specific color and mode.
	 *
	 * Draw an ASCII character using a specific color and draw mode with the upper left corner
	 * of the character located at (x,y).
	 *
	 * @param x X-coordinate representing the top-left of the character.
	 * @param y Y-coordinate representing the top-left of the character.
	 * @param character the ASCII character to draw. Must be present in the font file. If the
	 *	requested character is not present, nothing will be drawn on the screen buffer.
	 * @param c Pixel color.
	 * @param m Mode used to update the screen buffer.
	 */
	virtual void drawChar(coord_t x, coord_t y, uint8_t character, color c, mode m) noexcept = 0;

	/// Print a string on the display
	/// @param x The x-coordinate to start the string.
	/// @param y The y-coordinate to start the string.
	/// @param str The string to print to the display.
	void printString(coord_t x, coord_t y, const char* str) noexcept
	{
		printString(x, y, str, strlen(str));
	}

	/// Print a string on the display
	/// @param x The x-coordinate to start the string.
	/// @param y The y-coordinate to start the string.
	/// @param str The string to print to the display.
	/// @param length The number of characters in str.
	void printString(coord_t x, coord_t y, const char* str, size_t length) noexcept
	{
		cursor(x, y);

		for(size_t i = 0; i < length; i++)
		{
			putchar(static_cast<uint8_t>(str[i]));
		}
	}

	/** Replace the screen buffer with a bitmap.
	 *
	 * Supply a bitmap to write to the screen buffer. If the size
	 * of the bitmap is less than the size of the screen buffer, the remainder of the screen buffer
	 * will not be modified.
	 *
	 * @param bitmap Bitmap array which will be used to overwrite the screen buffer.
	 */
	virtual void drawBitmap(uint8_t* bitmap) noexcept = 0;

	/** Check the screen width.
	 *
	 * The drived class must implement this function.
	 *
	 * @returns The width of the screen in pixels.
	 */
	virtual uint8_t screenWidth() const noexcept = 0;

	/** Check the screen height.
	 *
	 * The drived class must implement this function.
	 *
	 * @returns The height of the screen in pixels.
	 */
	virtual uint8_t screenHeight() const noexcept = 0;

	/** Set color.
	 *
	 * Set the current draw's color. Only WHITE and BLACK available.
	 *
	 * @param c The desired draw color setting.
	 */
	void drawColor(color c) noexcept
	{
		assert(c == color::white || c == color::black);
		color_ = c;
	}

	/** Set draw mode.
	 *
	 * @param m Desired draw mode: NORM or XOR.
	 */
	virtual void drawMode(mode m) noexcept
	{
		assert(m == mode::normal || m == mode::XOR);
		mode_ = m;
	}

#pragma mark - LCD Rotate Scroll functions -

	/** Enable right scrolling.
	 * Set row start to row stop on the display to scroll right.
	 *
	 * @param start The start row index.
	 * @param stop The stop row index.
	 *
	 * @pre Scrolling is not enabled, start < stop.
	 * @post The display is scrolling to the right.
	 */
	virtual void scrollRight(coord_t start, coord_t stop) noexcept = 0;

	/** Enable left scrolling.
	 * Set row start to row stop on the display to scroll left.
	 *
	 * @param start The start row index.
	 * @param stop The stop row index.
	 *
	 * @pre Scrolling is not enabled, stop < start.
	 * @post The display is scrolling to the left.
	 */
	virtual void scrollLeft(coord_t start, coord_t stop) noexcept = 0;

	// TODO: document
	virtual void scrollVertRight(coord_t start, coord_t stop) noexcept = 0;

	// TODO: document
	virtual void scrollVertLeft(coord_t start, coord_t stop) noexcept = 0;

	/** Stop scrolling.
	 *
	 * Stop the scrolling of graphics on the display.
	 */
	virtual void scrollStop() noexcept = 0;

	/** Vertical flip.
	 *
	 * Flip the graphics on the display vertically.
	 *
	 * @param flip True enables vertical flip, false disables vertical flip.
	 */
	virtual void flipVertical(bool flip) noexcept = 0;

	/** Horizontal flip.
	 *
	 * Flip the graphics on the display horizontally.
	 *
	 * @param flip True enables horizontal flip, false disables horizontal flip.
	 */
	virtual void flipHorizontal(bool flip) noexcept = 0;

  protected:
	// embvm::DriverBase function that derived classes must implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function that derived classes must implement.
	void stop_() noexcept override = 0;

  protected:
	/// Default destructor.
	~basicDisplay() noexcept;

  protected:
	/// The current display mode setting.
	mode mode_ = mode::normal;

	/// The current display color setting.
	color color_ = color::black;
};

} // namespace embvm

#endif // BASIC_DISPLAY_HPP_
