#pragma once

#include "opencv2/opencv.hpp"
#include "core.h"
#include "segment.h"
#include "color_histogram.h"

using namespace cv;
using namespace std;

//#define SENSOR_RADIUS 11.0f
#define SENSOR_ANGLE_STEP 6.0f
#define POINTS_IN_CIRCLE (int(360.0f/SENSOR_ANGLE_STEP)) //HACK als variable hinzufügen
#define MAX_SECTORS 80
#define BIG_SECTOR 6

/* Sensor der analysiert vorbestimtes Bereich */
class m_sensor
{

	static uint index[POINTS_IN_CIRCLE]; // Index fuer Versatz in Speicher für Bogenpixel
	static int8_t dx[POINTS_IN_CIRCLE]; // delta x in Koordinaten Bogenpixel vom Sensormitte
	static int8_t dy[POINTS_IN_CIRCLE];		// delta y in Koordinaten Bogenpixel vom Sensormitte
	static Mat out;							// Ausschnitt für Sensor zur Bearbeitung // TODO aktive Sensor nr zu abildung hinterlegen
	static uint sensors_number;				// Anzahl Sensoren für die Klasse
	static short smooth_index[9];			// Glätten Wert für jedees Bogenpixel //TODO notwendig ?

	/* Einfarbige Sektor im Sensor (Bogen) */
	struct Sector
	{
		uint8_t start = 0;	/*Startindex fuer Bogen*/
		uint8_t end = 0;  /*Endindex fuer Bogen*/
		PixelColor color; /*Farbe des Sektors*/	
	};

	Point pos;	// Position in Bild (obere - linke Ecke) 
	uint size;	// Groesse (Höhe) des Sensors ist gleich Breite

	/* Werte fuer entsprechenden Bogenpixel die für die 
	Auswertung des Grenzen notwendig sind*/
	short values[POINTS_IN_CIRCLE];	

	ushort sectors_nmb = 0;	// Anzahl Sektoren für den Sensor
	Sector sectors[MAX_SECTORS]; // Gefundene Sektoren //HACK optimieren

	color_histogram clr_hst; //Histogramm für Sensor (Ring) TODO spaeter soll entfernt werden, zu viel speicher.

	// findet Schlüsselpunkte mit pegel im Sensor und gibt in values zurück
	void search_keypoints(short* values, int pegel);

	// im Sensor prüft next_pixel mit pegel und  baut Sektoren auf
	void search_sectors(PixelColor next_pixel, int pegel);

	// findet Sektoren im Sensor
	void search_sectors(Mat* sensor_mat, int pegel, const color_distance_func dist); 

	// vergleicht Histogramms mit Anderem Sensor und gibt Abstand zurück
	ushort compare_histogramms(m_sensor* ms); 

	/* gibt nächstes Index für Bogenpixel
		index - vorherige Index
		versatz - Steigung
		rückgabe - nächste Index*/
	inline short get_next_ring_value(ushort index, int8_t versatz);

	// glättet Gradientwerte im bereich +- range Pixel für Sensor
	void smooth_values(uint8_t range);

	// zeichnet Gradientwerte im Fenster auf
 	void plot_graph(const String fenster);

	// erstellt segmente im Sensor aus Schlüsselpunkten
 	void add_line_segments();
 
	//void create_sectors_array(const Mat* out, list<Sector>* output_list);
	void create_sectors_array(const Mat* out_ready, Sector* sv);

	// bindet die Sektoren mit nahem wert unter pegel zusammen 
	void connect_sectors(list<Sector>* input_list, list<Sector>* output_list, int pegel);

	// glättet Pixel p über rundum nebenstehende Pixel im Bild
	inline PixelColor smooth_color(PixelColor* p);

	// zeichnet Sensor auf dem Bild input
	//void mark_global(const Mat* input); //HACK check fuer bildgroesse 
	//void soi(const Mat* src, Mat* dst, uint magnify);

 	//list<ushort> nighbors; // nebenstehenden Sensoren //HACK umbauen in private member
	vector<Point> key_points; // keypoints für den Sensor
	vector<segment> line_segments;	// liniensegmente als Ausgabe der Analyse

public:

	const static String sensor_magnifyed_window; // CV Fentername für aktive Sensor
	const static String sensor_result_window;	 // CV Fentername für ergebniss aktiven Sensor

	m_sensor(Point p, uint sz); // Konstruktor
	m_sensor();	 // Konstruktor
	~m_sensor(); //	Destruktor

	// Auswerten den Sensor aus dem Bild.
	// input: Bild das verarbeitet wird.
	// pegel: Schwelle die für die Keypointsfindung notwendig ist. [0...255]	
	void proceed(const Mat* input, int pegel);

	// gibt Position des Sensors zurück
	Point get_position();

	// setzt Position des Sensors
	void set_position(Point p);

	// gibt Grösse des Sensors zurück
	Size get_size(); 

	// gibt Hisogramm des Sensors zurück
	color_histogram get_histogramm();

	// zufügt Sensor's Schlüsselpunkte zu Vector
	void add_points(vector<Point>* global_points);

	// prüft ob Punkt innerhalb Sensor liegt
	bool intersection(Point point);

	// prüft ob Sensor schneidet mit anderem Sensor 
	bool intersection(m_sensor* m);

	// Zeigt Sensor aus dem Bild input im Fenster output
	void draw_magnifyied(const Mat* input, const String output);

	// Abbildet Schlüsselpunkt auf dem Bild
	void draw(const Mat* output_image);

	// gibt abstand vom x,y zu Sensorsmitte  zurück
	int get_distance_to_middle(int x, int y);

	// gibt Farbe des Pixels aus Bild input zurück
	PixelColor get_color(int x, int y, const Mat* input);
};


