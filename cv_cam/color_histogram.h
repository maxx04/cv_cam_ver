#pragma once

struct hst { PixelColor color; ushort treffer; };
class color_histogram
{

#define COLOR_HISTOGRAM_BREITE (36+16)

public:
	color_histogram();
	~color_histogram();
	ushort compare(color_histogram * h);
	void reset();
	void add(PixelColor clr, ushort distance);
	void draw(Point start);
	void draw_base();

	//static vector <hst> base;
	static hst base[COLOR_HISTOGRAM_BREITE];

protected:

	static bool base_defined;
	//PixelColor histogram[COLOR_HISTOGRAM_BREITE]; //basis zu vergleich
	//ushort treffer[COLOR_HISTOGRAM_BREITE];
	//ushort breite;

	//vector <hst> histogram;
	uint8_t histogram[COLOR_HISTOGRAM_BREITE];

};

