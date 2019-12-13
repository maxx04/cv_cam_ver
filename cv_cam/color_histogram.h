#pragma once
#include "core.h"

using namespace cv;

/*Trefferanzahl für die Referenzfarbe RGB */
struct hst { 
	PixelColor color; // Referentfarbe
	ushort treffer;   // Trefferanzahl
};

/*Trefferanzahl für die Referenzfarbe HSV */
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

	static hst_hsv base[COLOR_HISTOGRAM_BREITE]; // referenzfarben TODO make protected

	static bool base_defined; // Sind die Referenzfarben gefüllt?

	uint8_t histogram[COLOR_HISTOGRAM_BREITE]; // Trefferanzahl für die Referenzfarben (für den Sensor)

public:

	color_histogram(); // Konstruktor

	~color_histogram();	// Destruktor

	ushort compare(color_histogram* h);	// Gibt Trefferanzahl zurück

	ushort compare(color_histogram h);	// Vergleich Histogrammen

	inline uint8_t	get_frequency( int i); // Gibt Trefferanzahl zurück	 OPTI nicht alle Werte kopieren

	void reset(); // Werte zurücksetzen 

	void add(PixelColor clr);	// Zuordnen den Pixel zu Histogramm

	void draw(Point start);	  // Draw Histogramm in eigenem Fenster

	Scalar get_mean_color();   // gibt Hauptfarbe zurück
 
};


