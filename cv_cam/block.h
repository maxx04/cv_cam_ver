#pragma once
#include "contours.h"
#include "m_sensor.h"

#define BLOCK_ZAHL 9

struct side_contour
{
	ushort contour_nr;
	ushort point_nr;
};

template <class T>
class block
{
	///	die adressen für Konturpunkten auf den seiten.
	/// entsteht wenn konturen werden aufgebaut
	/// soll gelöscht werden wenn nächste blocklevel konturzusammensetzung abgeschlossen ist.
	vector <side_contour> side_points[4];
	T blocks[BLOCK_ZAHL]; // selber blocks. erste mitte.

public:

	static ushort size; // block breite
	contours cnt; //Konturen die werden erstellt aus vorhandenen bloecken.

	block()
	{
		//TODO aktuell grenzsensoren werden doppelt berechnet.
		// liegen aufeinander. Alternativ, loesung mit adresspeicherung vom sensor
		size = blocks[0].get_size().width * 2; 
	}	
	
	~block()
	{
	
	}

	Size get_size()
	{
		return Size(size, size);
	} 

	vector<Point> get_side1()
	{
		vector<Point> vp;

		vp.push_back(blocks[8].get_side1());
		vp.push_back(blocks[1].get_side1());
		vp.push_back(blocks[2].get_side1());

		return vp;
	}

	// Geht von oben nach unten
	void set_position(Point p)
	{

		blocks[0].set_position(p);
		blocks[1].set_position(Point(p.x - size, p.y));
		blocks[2].set_position(Point(p.x - size, p.y - size));
		blocks[3].set_position(Point(p.x, p.y - size));
		blocks[4].set_position(Point(p.x + size, p.y - size));
		blocks[5].set_position(Point(p.x + size, p.y));
		blocks[6].set_position(Point(p.x + size, p.y + size));
		blocks[7].set_position(Point(p.x, p.y + size));
		blocks[8].set_position(Point(p.x - size, p.y + size));

	}

	//Konturen zusammenstellen von 1 bis 9 in cnt 
	//dann bereinigen einzelne konturen ?? oder die werden gebraucht.
	void connect_contours(void)
	{
		// jedes grenzbereich wird zusammengeführt.
		// was ist das grenzbereich? Zusammenkomen begrenzung und konturgrenze.
		// die stuecke zwischen den punkten mit gleiches Histogamm werden geloescht 
		// und die zwei enden vom kontur werden zusammengebunden.

		// bloecke teilen gleiche sensoren oder nicht?
		// Anzahl den Grenzpunkten ist gleich

		// --> gehen durch die grenze
		// --> finden zugehoerigkeit 
		// --> gehen bis zu zugehoerigkeitswechsel
		// --> finden erste verbindungspunkt zu kontur 1
		// --> finden erste verbindungspunkt zu kontur 2
		// --> finden zweite verbindungspunkt zu kontur 1
		// --> finden zweite verbindungspunkt zu kontur 2
		// --> loeschen zwischenstueck in kontur 1
		// --> loeschen zwischenstueck in kontur 2
		// --> anbinden kontur 2 zu kontur 1 in punkt 1
		// --> anbinden kontur 2 zu kontur 1 in punkt 2
		// --> dann naechstes kontur

		cnt._contours.clear();

		for (ushort i = 0; i < BLOCK_ZAHL; i++)
		{
			for each (contour_hist ch in blocks[i].cnt._contours)
			{
				cnt.add_contour(ch);
			}
			
		}



	}

	void draw_contours(const Mat* frame)
	{
		cnt.draw_contour(frame);
	}

	void quiery(const Mat * input, int pegel)
	{
		for (ushort i = 0; i < BLOCK_ZAHL; i++)
		{
			blocks[i].query(input, pegel);
		}

		connect_contours();
	}

	void query_all(const Mat * input, int pegel)
	{
		for (ushort i = 0; i < BLOCK_ZAHL; i++)
		{
			blocks[i].query_all(input, pegel);
		}

		connect_contours();
	}
};

template <class T>
ushort block<T>::size = 0;

template <>
class block <m_sensor> {

	// seitliche punkte, links, oben, rechts, unten.
	vector <side_contour> side_points[4];
	// unten-links, oben links, oben rechts, unten rechts.
	side_contour corner_points[4];
	m_sensor blocks[BLOCK_ZAHL]; // selber blocks. erste in mitte.

public:

	ushort size; // block breite
	contours cnt; //Konturen die werden erstellt aus vorhandenen bloecken.

	block()
	{
		size = blocks[0].get_size().width;
	}

	block(Point p) // p - mitte vom block
	{
		set_position(p);
	}


	~block()
	{

	}

	Size get_size()
	{
		return Size(size, size);
	}

	void set_position(Point p)
	{

		blocks[0].set_position(p);
		blocks[1].set_position(Point(p.x - size, p.y));
		blocks[2].set_position(Point(p.x - size, p.y - size));
		blocks[3].set_position(Point(p.x, p.y - size));
		blocks[4].set_position(Point(p.x + size, p.y - size));
		blocks[5].set_position(Point(p.x + size, p.y));
		blocks[6].set_position(Point(p.x + size, p.y + size));
		blocks[7].set_position(Point(p.x, p.y + size));
		blocks[8].set_position(Point(p.x - size, p.y + size));

	}

	void connect_contours(void)
	{
		ushort dist = 0;
		const ushort hist_pegel = 100;

		ushort current_contour = 0;
		ushort current_point = 0;

		//TODO Kann man mit vorherigem zustand vergleichen
		cnt._contours.clear(); 
		side_points[0].clear();
		side_points[1].clear();
		side_points[2].clear();
		side_points[3].clear();
		
		// Anfang mit dem block 1 links.
		color_histogram ch = blocks[1].get_histogramm();

		cnt.new_contour(blocks[1].get_position(), ch);
		// grenzpunkte aufbauen mit aktuellem kontour und aktuellem punkt
		// grenze geht ueber sensor selber
		side_points[0].push_back(side_contour{ current_contour, current_point });
		
		// vergleiche histogrammen von sensoren
		// anfang fuer weiteren sensor
		ushort i = 2;

		//nach urzeigesinn
		while (i < BLOCK_ZAHL)
		{
			ch = blocks[i-1].get_histogramm();

			// mit naechstem sensor vergleichen
			dist = ch.compare(blocks[i].get_histogramm());

			if (dist < hist_pegel)
			{
				// wenn aenliche histogram
				cnt.add_point(blocks[i].get_position());
				corner_points[0] = side_contour{ current_contour, ++current_point };
				//TODO mitteln histogram?
			}
			else
			{	
				// neues kontur anfangen
				cnt.new_contour(blocks[i].get_position(), blocks[i].get_histogramm());
				corner_points[0] = side_contour{ ++current_contour, current_point = 0 };
			}

			i++;
		}

		// letztes mit erstem vergleichen
		ch = blocks[BLOCK_ZAHL - 1].get_histogramm();

		// mit naechstem sensor vergleichen
		dist = ch.compare(blocks[1].get_histogramm()); //HACK reihenfolge den Punkten beachten

		if (dist < hist_pegel)
		{
			//wenn gleiches histogramm, punkt zuweisen
			cnt.add_point(blocks[1].get_position());
			corner_points[0] = side_contour{ current_contour, ++current_point };
			//TODO mitteln histogram?
		}
		else
		{
			cnt.new_contour(blocks[BLOCK_ZAHL - 1].get_position(), blocks[BLOCK_ZAHL - 1].get_histogramm());
			corner_points[0] = side_contour{ ++current_contour, current_point = 0 };
		}

		// mittleres punkt mit allen konturen vergleichen

		ushort k = 0;
		ushort min_dist = 20000;
		ushort beste_contour = 0;

		ch = blocks[0].get_histogramm();

		for each (contour_hist h1 in cnt._contours)
		{
			dist = ch.compare(h1.histogram);

			if (dist < min_dist)
			{
				min_dist = dist;
				beste_contour = k;
			}
			k++;
		}

		// wenn alle gleich --> den punkt ausschliessen
		if (min_dist < 50) // TODO Zahl ersetzen
		{
			if(cnt._contours.size() > 1)
			cnt._contours[beste_contour]._cnt.push_back(blocks[0].get_position());
		}
		else
		{
			cnt.new_contour(blocks[0].get_position(), ch);
		}

		cout  << "block contours: " << cnt._contours.size() << endl;
		
	}

	void draw_contours(const Mat* frame)
	{
		cnt.draw_contour(frame);
	}

	void query_all(const Mat * input, int pegel)
	{
		for (ushort i = 0; i < BLOCK_ZAHL; i++)
		{
			blocks[i].query(input, pegel);
		}

		connect_contours();
	}
};


