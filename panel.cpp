#include "panel.h"

/*****************************************************************************/
Panel::Panel(uint8_t c, StripWrapper * w) {
  wrapper_count = c;
  wrappers = w;
  y_size = w[0].num_rows();
  x_size = 0;
  for (uint8_t i=0; i < wrapper_count; i++)
  {
    x_size += wrappers[i].num_columns();
  }
}


/*
 *  UTILITY METHODS
 */
uint32_t Panel::Color(byte r, byte g, byte b) {
  // Take the lowest 7 bits of each value and append them end to end
  // We have the top bit set high (its a 'parity-like' bit in the protocol
  // and must be set!)
  return 0x808080 | ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

uint8_t Panel::column_to_wrapper(uint16_t x) {
  // Takes a cartesian x, y coordinate (eas:???) and returns the strip wrapper which
  // contains that corrdinate
  uint16_t i, column = 0;
  for (i=0; i < wrapper_count; i++)
  {
    column += wrappers[i].num_columns();
    if ( x < column )
    {
      return i;
    }
  }
  // TODO bounds checking
  return i;
}

uint8_t Panel::get_wrapper_column(uint8_t index, uint16_t x) {
  for (uint8_t i=0; i < index; i++)
  {
   x -= wrappers[i].num_columns();
  }
  return x;
}


/*
 *   BEGIN AND SHOW
 */
void Panel::begin(void) {
  for(int i=0; i < wrapper_count; i++)
  {
    wrappers[i].begin();
  }
}

void Panel::show(void) {
  for(int i=0; i < wrapper_count; i++)
  {
    wrappers[i].show();
  }
}

/*
 *  INFO METHODS
 */
int Panel::columns(void) {
  return x_size;
}

/*
 *  BOX SETTER
 */
void Panel::setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint8_t r, uint8_t g, uint8_t b) {
  setBoxColor(left, right, top, bottom, Color(r, g, b));
}

void Panel::setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint32_t c) {
  ///Colors a box bounded by coordinates left, right, top, bottom (inclusive)
  for (uint16_t x=left; x <= right; x++)
  {
    for (uint8_t y=bottom; y <= top; y++)
    {
      setPixelColor(x, y, c);
    }
  }
}

/*
 *   COLUMN AND ROW SETTERS
 */
void Panel::setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b) {
  setColumnColor(x, Color(r, g, b));
}

void Panel::setColumnColor(uint16_t x, uint32_t c) {
  uint8_t index = column_to_wrapper(x);            // First, figure out which wrapper 
  uint8_t strip_x = get_wrapper_column(index, x);  // Next, figure out index into that wrapper
  wrappers[index].setColumnColor(strip_x, c);
}

void Panel::setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setRowColor(y, Color(r, g, b));
}

void Panel::setRowColor(uint8_t y, uint32_t c) {
  for(uint8_t i=0; i < wrapper_count; i++)
  {
    wrappers[i].setRowColor(y, c);
  }
}

/*
 *   PIXEL SETTERS
 */
void Panel::setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelColor(x, y, Color(r, g, b));
}

void Panel::setPixelColor(uint16_t x, uint8_t y, uint32_t c) {
  uint8_t index = column_to_wrapper(x);
  uint8_t strip_x = get_wrapper_column(index, x);
  wrappers[index].setPixelColor(strip_x, y , c);
}

void Panel::setPixelAverage(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelAverage(x, y, Color(r, g, b));
}

void Panel::setPixelAverage(uint16_t x, uint8_t y, uint32_t c) {
  uint8_t index = column_to_wrapper(x);
  uint8_t strip_x = get_wrapper_column(index, x);
  wrappers[index].setPixelAverage(strip_x, y , c);
}
