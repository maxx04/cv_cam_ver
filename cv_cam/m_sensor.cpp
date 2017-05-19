#include "stdafx.h"

#include "m_sensor.h"


uint m_sensor::index[POINTS_IN_CIRCLE];
int8_t m_sensor::dx[POINTS_IN_CIRCLE];
int8_t m_sensor::dy[POINTS_IN_CIRCLE];
short m_sensor::smooth_index[9];

uint m_sensor::sensors_number = 0;

Mat m_sensor::out;


m_sensor::m_sensor(Point p, uint sz)
{
	assert(sz > 12 && sz < 120); // muss groesser sein um linie zu finden

	pos = p;
	size = sz;

	sectors_nmb = 0;

	key_points = vector<Point>(10);

	float radius = SENSOR_RADIUS;

	sensors_number++;

	int bytes = sizeof(PixelColor);

	if (sensors_number < 2) //nur bei erstem sensor alles berechnen
	{

		float da = 0.0f;

		for (ushort number = 0; number < POINTS_IN_CIRCLE; number++)
		{
			dx[number] = (int8_t)(radius * cos(3.1415f * da / 180.0f)); //HACK genauigkeit erhoehen pi double
			dy[number] = (int8_t)(radius * sin(3.1415f * da / 180.0f));

			int offset = (size + 1) * (size / 2); //sensor mitte

			index[number] = (dy[number] * size + dx[number] + offset); //HACK von Pixellaenge abhaengig

			da += SENSOR_ANGLE_STEP;
		}


		smooth_index[0] = 1;
		smooth_index[1] = 1 + size;
		smooth_index[2] = size;
		smooth_index[3] = size - 1;
		smooth_index[4] = - 1;
		smooth_index[5] = -1 - size;
		smooth_index[6] = -size; //sehe warnungen am compiler
		smooth_index[7] = -size + 1;
		smooth_index[8] = 0;

	}


}

m_sensor::~m_sensor()
{

}



void m_sensor::query(const Mat * input, int pegel)
{

	//kopieren teil vom bild
	Rect roi(pos.x, pos.y, size, size);
	(*input)(roi).copyTo(out);

	PixelColor* pixelPtr = (PixelColor*)out.data; 

	PixelColor Pixel0 = *(pixelPtr + index[POINTS_IN_CIRCLE - 1]); //letzte pixel
	PixelColor Pixel1, color;

	//sectors_nmb = 0; //start fuer find sectors

	//search_sectors(Pixel0, pegel);

	color = PixelColor(0, 0, 0);

	// reset histogramm
	clr_hst.reset();

	for (int i = 0; i < POINTS_IN_CIRCLE; i += 1)
	{
		Pixel1 = *(pixelPtr + index[i]);

		//aufbauen histogramm
		clr_hst.add(Pixel1, 20);

		values[i] = color_distance(Pixel0, Pixel1, RGB_3SUM); //HACK Berechnungsmethode beachten!
		//color = middle_color(color,Pixel1); //für HSV soll anders sein?

	//	search_sectors(Pixel1, pegel);

		Pixel0 = Pixel1;
	}

	//search_sectors(&out, pegel, RGB_3SUM); //nicht produktiv

	search_keypoints(values, pegel);

	// add_line_segments(); // add line segments

}

void m_sensor::search_keypoints(short* values, int pegel)
{
	//HACK values sollen schon vorbereitet sein

	// Glaetten 2 mal!
	smooth_values(3);
	smooth_values(3);

	key_points.clear();

	short max_value = -3000;
	ushort n_max_value = 0;
	short min_value = 3000;
	ushort n_min_value = 0;
	short current_val, last_value, direction, last_direction;
	short first_keypoint_number = 3000;

	bool direction_change = false;
	bool first_keypoint = true;
	ushort half_size = size / 2;
	ushort vp;
	ushort direction_pegel = 0;

	int n = 0;
	last_value = values[0];
	last_direction = last_value - values[POINTS_IN_CIRCLE - 1];
	n++;

	do
	{
		if (n > POINTS_IN_CIRCLE - 1) n = 0;
		current_val = values[n];
		direction = current_val - last_value;
		// Auswertung, Steigungwechsel und Prüfung auf maximum/minimum
		// und Gereuschzone.
		if ((current_val > pegel && direction < -direction_pegel && last_direction >= 0) ||
			(current_val < -pegel && direction > direction_pegel && last_direction <= 0))
		{
			// Vorherige punkt nehmen
			vp = (n == 0) ? POINTS_IN_CIRCLE - 1 : n - 1;
			// locale maximum
			key_points.push_back(Point(half_size + dx[vp], half_size + dy[vp]));

			if (first_keypoint) 
			{
				first_keypoint_number = n;
				first_keypoint = false;
			}

		}

		// Vorbereiten nachste zyclus
		last_value = current_val;
		last_direction = direction;
		n++;

	} while (first_keypoint_number != n && 
		!(first_keypoint_number == 3000 && n == POINTS_IN_CIRCLE));

	 //key_points.shrink_to_fit();
		
}

void m_sensor::search_sectors(PixelColor next_pixel, int pegel)
{
	//HACK bei auswertung vergleich end ring - startring beachten

	//assert(sectors_nmb < MAX_SECTORS);

	if (sectors_nmb >= MAX_SECTORS)	return;

	//if (sectors_nmb = MAX_SECTORS)
	//{
	//	//distance berechnen
	//	return;
	//}

	// start wenn:
	if (sectors_nmb == 0)
	{
		
		sectors[sectors_nmb].color = next_pixel;
		sectors[sectors_nmb].start = 0;
		sectors[sectors_nmb].end = 0;
		//sectors[sectors_nmb].distance = 0;
		sectors_nmb++;

		return;
	}

	PixelColor p = sectors[sectors_nmb - 1].color;

	if (color_distance(p, next_pixel, RGB_SUM_EACH_COLOR) < pegel) 
	{
		//sectors[sectors_nmb - 1].color = middle_color(p, next_pixel);
		sectors[sectors_nmb - 1].end++;
	}
	else
	{
		//Wenn Laenge vom Sektor kleiner als 3 verwerfen.
		if (sectors[sectors_nmb - 1].end - sectors[sectors_nmb - 1].start < BIG_SECTOR)
		{
			//setzen aktuelle farbe 
			sectors[sectors_nmb - 1].color = next_pixel;
			//setzen start und end
			//in gleiches segment
			sectors[sectors_nmb - 1].start = sectors[sectors_nmb - 1].end;
			//sectors[sectors_nmb - 1].end = sectors[sectors_nmb - 1].end;

		}
		else //anfangen naechstes sector
		{
			sectors[sectors_nmb].color = next_pixel;
			sectors[sectors_nmb].start = sectors[sectors_nmb - 1].end;
			sectors[sectors_nmb].end = sectors[sectors_nmb - 1].end;
			sectors_nmb++;
		}
	}

}

void m_sensor::search_sectors(Mat* sensor_mat, int pegel, const color_distance_enum dist)
{
	static Sector sv[POINTS_IN_CIRCLE];

	Sector  s1, s2;

	create_sectors_array(sensor_mat, sv);

	/*---------------*/
	int up_size_old = 0;
	int i_put = 0;
	int i_get = 1;
	int up_size = POINTS_IN_CIRCLE;

	do
	{
		up_size_old = up_size;

		i_put = 0;
		i_get = 1;

		while (i_get <= up_size - 1)
		{

			s1 = sv[i_put];
			s2 = sv[i_get];

			if (abs( color_distance(s1.color, s2.color, dist)) < pegel)
			{
				sv[i_put].start = s1.start;
				sv[i_put].end = s2.end;
				sv[i_put].color = middle_color(s1.color, s2.color);

		//		i_put += 0; //der sector bleibt naechste mit aktuellem vergleichen
				i_get += 1; //naechste nehmen

			}
			else
			{
				sv[++i_put] = s2; // in naechsten legen
				i_get += 1;
			}
		} /*--------------------------------*/

		up_size = i_put + 1; //menge 

	} while (up_size != up_size_old);
	/*----------------*/
	// letzte und erste sector zusammenlegen




	// nur lange sectors uebernehmen
	int m = 0;

	for (int i = 0; i < up_size; i++)
	{
		if (sv[i].end - sv[i].start > BIG_SECTOR)
		{
			sectors[m++] = sv[i];
		}
	}

	sectors_nmb = m;

	if (m == 0) return;
	
	if (abs(color_distance(sectors[0].color, sectors[sectors_nmb-1].color, dist)) < 2 * pegel)
	{
		sectors_nmb--;
		sectors[0].start = sectors[sectors_nmb].start;
		sectors[0].color = middle_color(sectors[0].color, sectors[sectors_nmb].color);
	}
}

Point m_sensor::get_position()
{
	return pos;
}

Size m_sensor::get_size()
{
	return Size(size, size);
}

inline short m_sensor::get_next_ring_value(ushort index, int8_t versatz) //HACK optimierung notwendig
{
	CV_Assert(index <= 256 && abs(versatz) < 127);

	int16_t n = index + versatz;

	if (n < 0) return values[n + POINTS_IN_CIRCLE - 1 + 1];
	if (n > POINTS_IN_CIRCLE - 1) return values[n - POINTS_IN_CIRCLE - 1];

	return values[n];
}

void m_sensor::smooth_values(uint8_t range)
{
	static short new_values[POINTS_IN_CIRCLE - 1];

	for (ushort k = 0; k < POINTS_IN_CIRCLE; k++)
	{
		int sum = 0;

		for (int8_t i = -range / 2; i <= range / 2; i++)
			sum += get_next_ring_value(k, i); //HACK need optimisation

		new_values[k] = sum / range;
	}

	for (ushort m = 0; m < POINTS_IN_CIRCLE; m++)	values[m] = new_values[m];

	return;

}

void m_sensor::show(const Mat * input, uint magnify)
{

	CV_Assert((*input).type() == out.type());

	Rect roi(pos.x, pos.y, size, size);

	(*input)(roi).copyTo(out);

	Mat out10x(size * magnify, size * magnify, out.type());

	soi(&out, &out10x, magnify);

	cout << key_points.size() << endl;

	for each (Point p in key_points)
		rectangle(out10x, Rect(p.x*magnify, p.y*magnify, magnify, magnify), Scalar(255));

	imshow("magnify", out);
}

void m_sensor::show(const Mat * input, const String fenster)
{

	//CV_Assert((*input).type() == out.type());

	Rect roi(pos.x, pos.y, size, size);

	(*input)(roi).copyTo(out);

	plot_graph("plot");

	clr_hst.draw(pos);

	//clr_hst.draw_base();

	cout << "keypoints: " << key_points.size() << " - nighbors: " << nighbors.size();
	cout << " sectors: " << (uint)sectors_nmb << endl;
	cout << " >  R   G   B  - start - end" << endl;

	//line segments

	Scalar c;
	for each (segment sg in line_segments)
	{
		c = Scalar(sg.C1.x, sg.C1.y, sg.C1.z);
		line(out, sg.P1, sg.P2, c);
	}
	/*
	for (int i = 0; i < sectors_nmb; i++)
	{
		cout << format(" > %3d %3d %3d - %2d - %2d"
			, sectors[i].color.x 
			, sectors[i].color.y 
			, sectors[i].color.z 
			, sectors[i].start
			, sectors[i].end) << endl;



		
// sectors
		uint8_t half_sz = size / 2;
		for (int k = sectors[i].start; k < sectors[i].end; k++)
		{
			out.at<PixelColor>(dy[k] + half_sz, dx[k] + half_sz) = sectors[i].color;
			//OPTI 
			if( k == sectors[i].start)
				out.at<PixelColor>(dy[k] + half_sz, dx[k] + half_sz) = PixelColor(255,0,0);
			if (k == sectors[i].end - 1)
				out.at<PixelColor>(dy[k] + half_sz, dx[k] + half_sz) = PixelColor(0,0,255);
		}

	}
	*/

	//keypoint anzeigen
	for each (Point p in key_points) out.at<PixelColor>(p.y, p.x) = PixelColor(255,255,0);

	//Ringfarbe anzeigen
	//circle(out, Point(get_size() / 2), 8, Scalar(P1.x, P1.y, P1.z), -1);

	//Ringfarbe anzeigen
	//circle(out, Point(get_size() / 2), 4, Scalar(P2.x, P2.y, P2.z), -1);

	//cvtColor(out, out, COLOR_HSV2BGR);
	imshow(fenster, out);

}

void m_sensor::plot_graph(const String fenster)
{
	Mat xData, yData, display;
	Ptr<plot::Plot2d> plot,plot1;
	xData.create(1, POINTS_IN_CIRCLE - 1, CV_64F);//1 Row, 100 columns, Real
	yData.create(1, POINTS_IN_CIRCLE - 1, CV_64F);

	for (int i = 0; i < POINTS_IN_CIRCLE - 1; i++)
	{
		xData.at<double>(i) = i;
		yData.at<double>(i) = values[i];
	}
	plot = plot::createPlot2d(xData, yData);
	plot->setPlotSize(100, 160);
	plot->setMaxX(POINTS_IN_CIRCLE);
	plot->setMinX(0);
	plot->setMaxY(-80);
	plot->setMinY(80);
	plot->setPlotGridColor(Scalar(80, 80, 80));
	plot->render(display);

	imshow(fenster, display);
}

void m_sensor::mark_global(const Mat * input)
{

	for each (Point p in key_points)
		circle((*input), pos + p, 0, Scalar(255, 0, 0));
}

void m_sensor::add_points(vector<Point> * global_points)
{
	for each (Point p in key_points)
	{
		global_points->push_back(Point(pos + p));
	}
}

void m_sensor::soi(const Mat* src, Mat* dst, uint magnify)
{
	CV_Assert((*src).depth() == CV_8U && (*dst).depth() == CV_8U);  // accept only uchar images

	for (uint16_t i = 0; i < src->cols; i++)
		for (uint16_t j = 0; j < src->rows; j++)
			for (uint16_t ik = 0; ik < magnify; ik++)
				for (uint16_t jk = 0; jk < magnify; jk++)
				{
					dst->at<uint8_t>(i * magnify + ik, j * magnify + jk) = src->at<uint8_t>(i, j);
					//cout << i * 10 + ik << "," << j * 10 + jk << endl;
				}


}

bool m_sensor::cross(Point mp) //HACK Size beruecksichtigen, auf Rect aendern
{
	int s = (int)size;

	if (mp.x < pos.x) return false;
	if (mp.x > pos.x + s) return false;
	if (mp.y < pos.y) return false;
	if (mp.y > pos.y + s) return false;

	return true;
}

bool m_sensor::cross(m_sensor* m) 
{
	int s = (int)size;
	int sm = (int) m->size;

	if (m-> pos.x + sm < pos.x) return false;
	if (m-> pos.x > pos.x + s) return false;
	if (m-> pos.y + sm < pos.y) return false;
	if (m-> pos.y > pos.y + s) return false;

	return true;
}

ushort m_sensor::get_distance_to_middle(int x, int y)
{
	return (ushort)sqrt((pos.x + size / 2 - x) ^ 2 + (pos.y + size / 2 - y) ^ 2);
}

PixelColor m_sensor::get_color(int x, int y, const Mat * input)
{
	PixelColor a = (*input).at<PixelColor>(pos.y + y, pos.x + x);

	//circle((*input), Point(pos.x + x, pos.y + y), 6, Scalar(a.x, a.y, a.z), -1);

	return a;
}

void m_sensor::add_line_segments()
{
	//TODO in Arbeit
	line_segments.clear();

	uint8_t shift = size / 2;

	for each (Point pnt in key_points)
	{
		//berechnen linie und anliegende farben
		//segmentieren anliegende umgebung
		//dann trennen farbe und blur (wechsel den farben) dann entschtehen 
		//zwei liniensegmente mit grundfarbe und angrenzende farbe ueber blur
		//auch alle moegliche variante entstehen da (ecken usw.)
 
		//
		if (key_points.size() == 2)
		{
			Point p1 = key_points[0];
			Point p2 = key_points[1];
			line_segments.push_back(segment(p1, p2, PixelColor(100, 89, 40), PixelColor(100, 89, 40)));
		}
		
	}

	//for (int i = 0; i < sectors_nmb; i++)
	//{
	//	Point p1 = Point(dx[sectors[i].start]+shift, dy[sectors[i].start]+shift);
	//	Point p2 = Point(dx[sectors[i].end]+shift, dy[sectors[i].end]+shift);
	//	PixelColor c1 = sectors[i].color;
	//	
	//}
}

bool m_sensor::connect_sectors(Sector S1, Sector S2, list<Sector>* ls)
{
	Sector S;

	if (color_distance(S1.color, S2.color, RGB_SUM_EACH_COLOR) < 6)
	{
		S.start = S1.start;
		S.end = S2.end;
		S.color = middle_color(S1.color, S2.color);
		ls->push_back(S);
		return true;
	}

	ls->push_back(S1);
	ls->push_back(S2);
	return false;
}

void m_sensor::create_sectors_array(const Mat* out_ready, list<Sector>* output_list)
{
	//HACK out soll vorbereited sein
	PixelColor* pixelPtr = (PixelColor*)(out_ready->data);
	Sector s;
	PixelColor p;
	for (int i = 0; i < POINTS_IN_CIRCLE; i += 1)
	{
		p = *(pixelPtr + index[i]);
		s.start = i;
		s.end = i;
		s.color = p;
		output_list->push_back(s);
	}
}

void m_sensor::create_sectors_array(const Mat* out_ready, Sector* sv)
{
	//HACK out soll vorbereited sein
	PixelColor* pixelPtr = (PixelColor*)(out_ready->data);

	PixelColor p;
	for (int i = 0; i < POINTS_IN_CIRCLE; i += 1)
	{
		p = smooth_color(pixelPtr + index[i]);
		sv[i].start = i;
		sv[i].end = i;
		sv[i].color = p;
	}
}

void m_sensor::connect_sectors(list<Sector>* input_list, list<Sector>* output_list, int pegel)
{
	Sector s, s1, s2;

	//assert(input_list->size() > 1);

	while (input_list -> size() > 0)
	{

		s1 = input_list->front(); 
		input_list-> pop_front();

		if (input_list->size() == 0)
		{
			output_list->push_back(s1);
			return;
		}

		s2 = input_list->front();
		input_list->pop_front();

		if (color_distance(s1.color, s2.color, RGB_SUM_EACH_COLOR) < pegel)
		{
			s.start = s1.start;
			s.end = s2.end;
			s.color = middle_color(s1.color, s2.color);
			output_list->push_back(s);
			continue;
		}

		output_list->push_back(s1);
		output_list->push_back(s2);

	}


}

inline PixelColor m_sensor::smooth_color(PixelColor* p)
{
	PixelColor px = *(p);
	for (uint16_t i = 0; i < 8; i++) //HACK 8 denn selber pixel ist schon berechnet
	{
		px = middle_color(px, *(p + smooth_index[i]));
	}
	return px;
}