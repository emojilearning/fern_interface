#ifndef _FernPlanarDetector_H_
#define _FernPlanarDetector_H_
#include <iostream>
#include <highgui.h>
#include <opencv2/features2d.hpp>
#include "planar_pattern_detector_builder.h"
#include <memory>
#include <opencv2\opencv.hpp>
using namespace cv;
class FernPlanarDetector
{
public:
	struct Result {
		int id;
		Matx33d H;
	};

	FernPlanarDetector();
	static Mat convertToGray(Mat& img);
	bool check(Matx33d H);
	void drawon(Mat out3, Scalar color = Scalar(0, 255, 0));
	Result queryMarker(Mat frame, int mark_id);
	static const int nmark = 3;

private:
	shared_ptr<planar_pattern_detector> mdetectors[nmark];
	std::vector<Point2f> obj_corners;
	std::vector<Point2f> scene_corners;
};
#endif // !FernPlanarDetector


