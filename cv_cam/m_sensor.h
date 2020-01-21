#pragma once

#include "opencv2/opencv.hpp"
#include "core.h"
#include "segment.h"
#include "color_histogram.h"

using namespace cv;
using namespace std;

//#define SENSOR_RADIUS 11.0f
#define SENSOR_ANGLE_STEP 6.0f
#define POINTS_IN_CIRCLE (int(360.0f/SENSOR_ANGLE_STEP)) //HACK als variable hinzuf�gen
#define MAX_SECTORS 80
#define BIG_SECTOR 6

/* Sensor der analysiert vorbestimtes Bereich */
class m_sensor
{

	static uint index[POINTS_IN_CIRCLE]; // Index fuer Versatz in Speicher f�r Bogenpixel
	static int8_t dx[POINTS_IN_CIRCLE]; // delta x in Koordinaten Bogenpixel vom Sensormitte
	static int8_t dy[POINTS_IN_CIRCLE];		// delta y in Koordinaten Bogenpixel vom Sensormitte
	static Mat out;							// Ausschnitt f�r Sensor zur Bearbeitung // TODO aktive Sensor nr zu abildung hinterlegen
	static uint sensors_number;				// Anzahl Sensoren f�r die Klasse
	static short smooth_index[9];			// Gl�tten Wert f�r jedees Bogenpixel //TODO notwendig ?

	/* Einfarbige Sektor im Sensor (Bogen) */
	struct Sector
	{
		uint8_t start = 0;	/*Startindex fuer Bogen*/
		uint8_t end = 0;  /*Endindex fuer Bogen*/
		PixelColor color; /*Farbe des Sektors*/	
	};

	Point pos;	// Position in Bild (obere - linke Ecke) 
	uint size;	// Groesse (H�he) des Sensors ist gleich Breite

	/* Werte fuer entsprechenden Bogenpixel die f�r die 
	Auswertung des Grenzen notwendig sind*/
	short values[POINTS_IN_CIRCLE];	

	ushort sectors_nmb = 0;	// Anzahl Sektoren f�r den Sensor
	Sector sectors[MAX_SECTORS]; // Gefundene Sektoren //HACK optimieren

	color_histogram clr_hst; //Histogramm f�r Sensor (Ring) TODO spaeter soll entfernt werden, zu viel speicher.

	// findet Schl�sselpunkte mit pegel im Sensor und gibt in values zur�ck
	void search_keypoints(short* values, int pegel);

	// im Sensor pr�ft next_pixel mit pegel und  baut Sektoren auf
	void search_sectors(PixelColor next_pixel, int pegel);

	// findet Sektoren im Sensor
	void search_sectors(Mat* sensor_mat, int pegel, const color_distance_func dist); 

	// vergleicht Histogramms mit Anderem Sensor und gibt Abstand zur�ck
	ushort compare_histogramms(m_sensor* ms); 

	/* gibt n�chstes Index f�r Bogenpixel
		index - vorherige Index
		versatz - Steigung
		r�ckgabe - n�chste Index*/
	inline short get_next_ring_value(ushort index, int8_t versatz);

	// gl�ttet Gradientwerte im bereich +- range Pixel f�r Sensor
	void smooth_values(uint8_t range);

	// zeichnet Gradientwerte im Fenster auf
 	void plot_graph(const String fenster);

	// erstellt segmente im Sensor aus Schl�sselpunkten
 	void add_line_segments();
 
	//void create_sectors_array(const Mat* out, list<Sector>* output_list);
	void create_sectors_array(const Mat* out_ready, Sector* sv);

	// bindet die Sektoren mit nahem wert unter pegel zusammen 
	void connect_sectors(list<Sector>* input_list, list<Sector>* output_list, int pegel);

	// gl�ttet Pixel p �ber rundum nebenstehende Pixel im Bild
	inline PixelColor smooth_color(PixelColor* p);

	// zeichnet Sensor auf dem Bild input
	//void mark_global(const Mat* input); //HACK check fuer bildgroesse 
	//void soi(const Mat* src, Mat* dst, uint magnify);

 	//list<ushort> nighbors; // nebenstehenden Sensoren //HACK umbauen in private member
	vector<Point> key_points; // keypoints f�r den Sensor
	vector<segment> line_segments;	// liniensegmente als Ausgabe der Analyse

public:

	const static String sensor_magnifyed_window; // CV Fentername f�r aktive Sensor
	const static String sensor_result_window;	 // CV Fentername f�r ergebniss aktiven Sensor

	m_sensor(Point p, uint sz); // Konstruktor
	m_sensor();	 // Konstruktor
	~m_sensor(); //	Destruktor

	// Auswerten den Sensor aus dem Bild.
	// input: Bild das verarbeitet wird.
	// pegel: Schwelle die f�r die Keypointsfindung notwendig ist. [0...255]	
	void proceed(const Mat* input, int pegel);

	// gibt Position des Sensors zur�ck
	Point get_position();

	// setzt Position des Sensors
	void set_position(Point p);

	// gibt Gr�sse des Sensors zur�ck
	Size get_size(); 

	// gibt Hisogramm des Sensors zur�ck
	color_histogram get_histogramm();

	// zuf�gt Sensor's Schl�sselpunkte zu Vector
	void add_points(vector<Point>* global_points);

	// pr�ft ob Punkt innerhalb Sensor liegt
	bool intersection(Point point);

	// pr�ft ob Sensor schneidet mit anderem Sensor 
	bool intersection(m_sensor* m);

	// Zeigt Sensor aus dem Bild input im Fenster output
	void draw_magnifyied(const Mat* input, const String output);

	// Abbildet Schl�sselpunkt auf dem Bild
	void draw(const Mat* output_image);

	// gibt abstand vom x,y zu Sensorsmitte  zur�ck
	int get_distance_to_middle(int x, int y);

	// gibt Farbe des Pixels aus Bild input zur�ck
	PixelColor get_color(int x, int y, const Mat* input);
};


