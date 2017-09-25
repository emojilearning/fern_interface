#include "FernPlanarDetector.h"
FernPlanarDetector::FernPlanarDetector()
{
	for (int i = 0; i < 4; i++)
	{
		obj_corners.push_back(cvPoint(0, 0));
		scene_corners.push_back(cvPoint(0, 0));
	}

	affine_transformation_range range;
	for (int i = 0; i < nmark; i++)
	{
		char model_image_url[50];
		sprintf(model_image_url, "../../marker%d.jpg.detector_data", i + 1);
		shared_ptr<planar_pattern_detector> mdetector(planar_pattern_detector_builder::just_load(model_image_url));
		mdetector->set_maximum_number_of_points_to_detect(1000);
		mdetectors[i] = mdetector;
	}
}

Mat FernPlanarDetector::convertToGray(Mat& img)
{
	Mat out;
	if (img.channels() == 4)
		cv::cvtColor(img, out, CV_RGBA2GRAY);
	else if (img.channels() == 3)
		cv::cvtColor(img, out, CV_BGR2GRAY);
	else if (img.channels() == 1)
		out = img.clone();
	return  out;
}

bool FernPlanarDetector::check(Matx33d H)
{
	perspectiveTransform(obj_corners, scene_corners, H);
	auto flag = true;
	for (size_t i = 0; i < 2; i++)
	{
		auto edge = scene_corners[(i + 2) % 4] - scene_corners[i];
		auto edge1 = scene_corners[(i + 1) % 4] - scene_corners[i];
		auto edge2 = scene_corners[(i + 3) % 4] - scene_corners[i];
		auto r1 = edge.cross(edge1);
		auto r2 = edge.cross(edge2);
		if (r1*r2 > 0)
			flag = false;
	}
	return flag;
}

void FernPlanarDetector::drawon(Mat out3, Scalar color)
{
	line(out3, scene_corners[0], scene_corners[1], color, 4);
	line(out3, scene_corners[1], scene_corners[2], color, 4);
	line(out3, scene_corners[2], scene_corners[3], color, 4);
	line(out3, scene_corners[3], scene_corners[0], color, 4);
}

FernPlanarDetector::Result FernPlanarDetector::queryMarker(Mat frame, int mark_id)
{
	Mat gray = convertToGray(frame);
	Mat detimg = gray;
	Matx33d H;
	bool isdetected = false;
	auto detector = mdetectors[mark_id];
	
	IplImage ipl_img(detimg);
	detector->detect(&ipl_img);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(detimg.cols, 0);
	obj_corners[2] = cvPoint(detimg.cols, detimg.rows); obj_corners[3] = cvPoint(0, detimg.rows);
	if (detector->pattern_is_detected)
	{
		for (int i = 0; i < 9; ++i) {
			H.val[i] = detector->H.data.fl[i];
		}
		Matx33d H1;
		isdetected = check(H);
		if (isdetected)
		{
			Mat t;
			warpPerspective(detimg, t, H.inv(), detimg.size());
			IplImage ipl_img1(t);
			detector->detect(&ipl_img1);
			isdetected = detector->pattern_is_detected;
			if (isdetected)
			{
				for (int i = 0; i < 9; ++i) {
					H1.val[i] = detector->H.data.fl[i];
				}
				H = H1*H;
				isdetected = check(H);
			}//if
		}//isleagal
	}//detected
	return { isdetected ? mark_id : -1 ,H };
};//quearymarker