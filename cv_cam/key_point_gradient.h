#pragma once
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;

#define KEYPOINT_RADIUS 6.0f
#define KEYPOINT_ANGLE_STEP 6.0f
#define KP_POINTS_IN_CIRCLE (int(360.0f/KEYPOINT_ANGLE_STEP)) //HACK variable hinzufügen

/* Klass Beschreibt Gradientauslegung um Schlüsselpunkt */
class key_point_gradient
{

private:
	static uint index[KP_POINTS_IN_CIRCLE];	 // Index in Speicher für den Kreis um Schlüsselpunkt
 	static int8_t dx[KP_POINTS_IN_CIRCLE]; // x Koordinaten vom Kreis um Schlüsselpunkt
 	static int8_t dy[KP_POINTS_IN_CIRCLE];	// y Koordinaten vom Kreis um Schlüsselpunkt

	static Mat kp_roi;	 // Umgebungsbild vom Schlüsselpunkt
	static bool index_ready; // true == Werte für den Index sind initialisiert
	static short size;	 // Seitenlänge vom Quadrat um den Schlüsselpunkt
	static Mat keypoint_mat;  // Umgebungsbild vom Schlüsselpunkt für Vergrösserrte Abbildung

protected:
	Point position;
	short direction;
	//Scalar c1;
	//Scalar c2;

public:

	// zeichne Gradient um Position im Bild input  
	void draw_magnifyied(const Mat* input, const String magnifiyed_fenster_name);

	key_point_gradient(Point p); // Konstruktor

	key_point_gradient(); // Konstruktor

	~key_point_gradient(); // Destruktor

	void proceed(const Mat* input, int pegel);	// berechnet Gradient für den Schllüsselpunkt

};

