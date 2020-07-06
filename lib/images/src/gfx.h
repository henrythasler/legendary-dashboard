#ifndef GFX_H
#define GFX_H

#include <images.h>
#include <icons.h>

struct Image{
	const uint8_t *color;
	const uint8_t *black;

  Image(const uint8_t *color, const uint8_t *black)
      : color(color), black(black)
  {
  }	
};

struct {
	Image bmw = Image(bmwYellow, bmwBlack);
	Image bruce = Image(bruceYellow, bruceBlack);
	Image yellowScreen = Image(bsodRed, bsodBlack);
	Image beer = Image(beerYellow, beerBlack);
	Image coffin = Image(coffinYellow, coffinBlack);
	Image parking = Image(parkingYellow, parkingBlack);
	Image unittest = Image(unittestYellow, unittestBlack);
	Image fixing = Image(whiteWall, fixingBlack);
    Image employees = Image(whiteWall, employeesBlack);
} images;

struct {
	Image gift = Image(giftRed, giftBlack);
	Image logo = Image(empty, logoBlack);
    Image pressure = Image(empty, pressureBlack);
    Image humidity = Image(empty, humidityBlack);
    Image temperature = Image(empty, temperatureBlack);
} icons;
#endif
