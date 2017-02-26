#include "stdafx.h"
#include "key_point_gradient.h"


uint key_point_gradient::index[KP_POINTS_IN_CIRCLE];
int8_t key_point_gradient::dx[KP_POINTS_IN_CIRCLE];
int8_t key_point_gradient::dy[KP_POINTS_IN_CIRCLE];

bool key_point_gradient::index_ready = false;
short key_point_gradient::size = 0;
Mat key_point_gradient::keypoint_mat;
Mat key_point_gradient::kp_roi;


key_point_gradient::key_point_gradient()
{
	direction = 0;

	if (!index_ready) //nur bei erstem sensor alles berechnen
	{
		size = 2 * KEYPOINT_RADIUS + 2;

		float da = 0.0f;

		for (ushort number = 0; number < KP_POINTS_IN_CIRCLE; number++)
		{
			dx[number] = (int8_t)(KEYPOINT_RADIUS * cos(3.1415f * da / 180.0f)); //HACK genauigkeit erhoehen pi double
			dy[number] = (int8_t)(KEYPOINT_RADIUS * sin(3.1415f * da / 180.0f));

			int offset = (size + 1) * (size / 2); //sensor mitte

			index[number] = (dy[number] * size + dx[number] + offset); //HACK von Pixellaenge abhaengig

			da += KEYPOINT_ANGLE_STEP; //HACK Überlappungen möglich
		}

		index_ready = true; //nur für erster keypoint ausfueren
	}

}


void key_point_gradient::show(const Mat * input, const String fenster)
{

	//CV_Assert((*input).type() == out.type());

	Rect roi(position.x-size/2, position.y-size/2, size, size);

	(*input)(roi).copyTo(keypoint_mat);

	imshow(fenster, keypoint_mat);
}

key_point_gradient::key_point_gradient(Point p)
{
	key_point_gradient();
	position = p;
}

key_point_gradient::~key_point_gradient()
{
}

void key_point_gradient::get_direction(Mat frame, Point pos)
{
}

void key_point_gradient::check(const Mat * input, int pegel)
{

	//kopieren teil vom bild
	Rect roi(position.x-size/2, position.y-size/2, size, size); //HACK frame border?

	(*input)(roi).copyTo(kp_roi);

	Pixel* pixelPtr = (Pixel*)kp_roi.data;

	Pixel Pixel0 = *(pixelPtr + index[KP_POINTS_IN_CIRCLE - 1]);
	Pixel Pixel1;

	short values[KP_POINTS_IN_CIRCLE];

	for (int i = 0; i < KP_POINTS_IN_CIRCLE; i += 1)
	{
		Pixel1 = *(pixelPtr + index[i]);

		values[i] = color_distance(Pixel0, Pixel1, RGB_3SUM);

		Pixel0 = Pixel1;
	}

	//search_keypoints(values, pegel);

}
