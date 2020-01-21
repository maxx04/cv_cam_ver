#pragma once
#include "core.h"

using namespace cv;

/*Trefferanzahl f�r die Referenzfarbe RGB */
struct hst { 
	PixelColor color; // Referentfarbe
	ushort treffer;   // Trefferanzahl
};

/*Trefferanzahl f�r die Referenzfarbe HSV */
struct hst_hsv { 
	HSV color;		// Referentfarbe
	ushort frequency; // Trefferanzahl	 // TODO nicht benutzt
};

#define COLOR_HISTOGRAM_COLORS 32 // Anzahl Referenzfarben
#define COLOR_HISTOGRAM_GRAYS 16  // Azahl Graustufen
#define COLOR_HISTOGRAM_BREITE (1*COLOR_HISTOGRAM_COLORS+COLOR_HISTOGRAM_GRAYS)	// Gesamtzahl Farben
#define COLOR_HISTOGRAM_MAIN 8

class color_histogram
{

protected:

	static hst_hsv base[COLOR_HISTOGRAM_BREITE]; // referenzfarben 

	static bool base_defined; // true == sind die Referenzfarben bef�llt

	uint8_t histogram[COLOR_HISTOGRAM_BREITE]; // Trefferanzahl f�r die Referenzfarben (f�r den Sensor)

public:

	color_histogram(); // Konstruktor

	~color_histogram();	// Destruktor

	ushort compare(color_histogram* h);	// Gibt Trefferanzahl zur�ck

	ushort compare(color_histogram h);	// Vergleich Histogrammen

	inline uint8_t	get_frequency( int i); // Gibt Trefferanzahl zur�ck	 OPTI nicht alle Werte kopieren

	void reset(); // Werte zur�cksetzen 

	void add(PixelColor clr);	// Zuordnen den Pixel zu Histogramm

	void draw(Point start);	  // Draw Histogramm in eigenem Fenster

	Scalar get_mean_color();   // gibt Hauptfarbe zur�ck

	short hsv_distance(HSV color1, HSV color2);	 // HSV abstand zwischen Farben

	short compare_with_base(HSV color, int index);  // HSV abstand zwischen farbe und Referenzfarbe
 
};


