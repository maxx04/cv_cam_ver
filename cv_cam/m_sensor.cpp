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

	float radius = SENSOR_RADIUS;

	sensors_number++;

	int bytes = sizeof(Pixel);

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
		smooth_index[6] = -size;
		smooth_index[7] = -size + 1;
		smooth_index[8] = 0;

	}


}

m_sensor::~m_sensor()
{

}

Pixel m_sensor::middle_color(Pixel PA, Pixel PB)
{
	Pixel tmp;

	//tmp = PA / 2 + PB / 2;

	tmp.x = (PA.x + PB.x) / 2;
	tmp.y = (PA.y + PB.y) / 2;
	tmp.z = (PA.z + PB.z) / 2;

	return tmp;
}

void m_sensor::check(const Mat * input, int pegel)
{

	//kopieren teil vom bild
	Rect roi(pos.x, pos.y, size, size);

	(*input)(roi).copyTo(out);

	Pixel* pixelPtr = (Pixel*)out.data; 

	Pixel Pixel0 = *(pixelPtr + index[POINTS_IN_CIRCLE - 1]);
	Pixel Pixel1;

	//sectors_nmb = 0; //start fuer find sectors

	//search_sectors(Pixel0, pegel);

	for (int i = 0; i < POINTS_IN_CIRCLE; i += 1)
	{
		Pixel1 = *(pixelPtr + index[i]);

		values[i] = color_distance(Pixel0, Pixel1, RGB_3SUM);

	//	search_sectors(Pixel1, pegel);

		Pixel0 = Pixel1;
	}

	//search_sectors(&out, pegel, RGB_3SUM); 

	//add_segments(); // add line segments

	search_keypoints(values, pegel);

}

void m_sensor::search_keypoints(short* values, int pegel)
{

	// Glaetten
	smooth_values(5);

	key_points.clear();

	short max_value = -3000;
	ushort n_max_value = 0;
	short min_value = 3000;
	ushort n_min_value = 0;
	short current_val;

	bool direction_change = false;

	for (int n = 0; n < POINTS_IN_CIRCLE; n += 1)
	{
		current_val = values[n];

		if (current_val > pegel)
		{			
			if (current_val > max_value)
			{
				direction_change = true;
				max_value = current_val;
				n_max_value = n;
			}
		}
		else
		{
			if (direction_change)
			{
				key_points.push_back(Point(size / 2 + dx[n_max_value], size / 2 + dy[n_max_value]));
				direction_change = false;
			}
		}

		if (current_val < -pegel)
		{
			if (current_val < min_value)
			{
				direction_change = true;
				min_value = current_val;
				n_min_value = n;
			}
		}
		else
		{
			if (direction_change)
			{
				key_points.push_back(Point(size / 2 + dx[n_min_value], size / 2 + dy[n_min_value]));
				direction_change = false;
			}
		}

			
	}
}

void m_sensor::search_sectors(Pixel next_pixel, int pegel)
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

	Pixel p = sectors[sectors_nmb - 1].color;

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
/******/
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

			if (abs(color_distance(s1.color, s2.color, dist)) < pegel)
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

inline short m_sensor::color_distance(Pixel Pixel0, Pixel Pixel1, int8_t function_nr)
{
	int m = 9;
	switch (function_nr)
	{
	case RGB_SQUARE:
		return (Pixel0.x - Pixel1.x) ^ 2 + (Pixel0.y - Pixel1.y) ^ 2 + (Pixel0.z - Pixel1.z) ^ 2;

	case RGB_3SUM:
		return (((Pixel0.x + Pixel0.y + Pixel0.z) - (Pixel1.x + Pixel1.y + Pixel1.z)));

	case RGB_SUM_EACH_COLOR:
		return abs(Pixel0.x - Pixel1.x) + abs(Pixel0.y - Pixel1.y) + abs(Pixel0.z - Pixel1.z);

	case RGB_MAX_EACH_COLOR:
		m = max(abs(Pixel0.y - Pixel1.y), abs(Pixel0.z - Pixel1.z));
		return max(abs(Pixel0.x - Pixel1.x), m);

	default:
		return abs(Pixel0.x - Pixel1.x) + abs(Pixel0.y - Pixel1.y) + abs(Pixel0.z - Pixel1.z);
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


	cout << "keypoints: " << key_points.size() << " - nighbors: " << nighbors.size();
	cout << " sectors: " << (uint)sectors_nmb << endl;
	cout << " >  R   G   B  - start - end" << endl;

	for (int i = 0; i < sectors_nmb; i++)
	{
		cout << format(" > %3d %3d %3d - %2d - %2d"
			, sectors[i].color.x 
			, sectors[i].color.y 
			, sectors[i].color.z 
			, sectors[i].start
			, sectors[i].end) << endl;


//line segments
/*	
		Scalar c;
		for each (segment sg in line_segments)
		{
			c = Scalar(sg.C1.x, sg.C1.y, sg.C1.z);
			line(out, sg.P1, sg.P2, c);
		}
		*/
// sectors
		uint8_t half_sz = size / 2;
		for (int k = sectors[i].start; k < sectors[i].end; k++)
		{
			out.at<Pixel>(dy[k] + half_sz, dx[k] + half_sz) = sectors[i].color;
			//OPTI 
			if( k == sectors[i].start)
				out.at<Pixel>(dy[k] + half_sz, dx[k] + half_sz) = Pixel(255,0,0);
			if (k == sectors[i].end - 1)
				out.at<Pixel>(dy[k] + half_sz, dx[k] + half_sz) = Pixel(0,0,255);
		}

	}

	//keypoint anzeigen
	for each (Point p in key_points) out.at<Pixel>(p.y, p.x) = Pixel(255,255,0);

	//Ringfarbe anzeigen
	//circle(out, Point(get_size() / 2), 8, Scalar(P1.x, P1.y, P1.z), -1);

	//Ringfarbe anzeigen
	//circle(out, Point(get_size() / 2), 4, Scalar(P2.x, P2.y, P2.z), -1);


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

ushort m_sensor::get_distance_to_middle(int x, int y)
{
	return (ushort)sqrt((pos.x + size / 2 - x) ^ 2 + (pos.y + size / 2 - y) ^ 2);
}

Pixel m_sensor::get_color(int x, int y, const Mat * input)
{
	Pixel a = (*input).at<Pixel>(pos.y + y, pos.x + x);

	//circle((*input), Point(pos.x + x, pos.y + y), 6, Scalar(a.x, a.y, a.z), -1);

	return a;
}

void m_sensor::add_segments()
{
	line_segments.clear();

	uint8_t shift = size / 2;

	for (int i = 0; i < sectors_nmb; i++)
	{
		Point p1 = Point(dx[sectors[i].start]+shift, dy[sectors[i].start]+shift);
		Point p2 = Point(dx[sectors[i].end]+shift, dy[sectors[i].end]+shift);
		Pixel c1 = sectors[i].color;
		line_segments.push_back(segment(p1, p2, c1, c1)); //HACK noch zu ende bringen
	}
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
	Pixel* pixelPtr = (Pixel*)(out_ready->data);
	Sector s;
	Pixel p;
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
	Pixel* pixelPtr = (Pixel*)(out_ready->data);

	Pixel p;
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

inline Pixel m_sensor::smooth_color(Pixel* p)
{
	Pixel px = *(p);
	for (uint16_t i = 0; i < 8; i++) //HACK 8 denn selber pixel ist schon berechnet
	{
		px = middle_color(px, *(p + smooth_index[i]));
	}
	return px;
}