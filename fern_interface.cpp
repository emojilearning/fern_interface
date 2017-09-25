#include "FernPlanarDetector.h"
using namespace std;
using namespace cv;

//Mat convertToGray(Mat& img)
//{
//    Mat out;
//    if (img.channels() == 4)
//        cv::cvtColor(img, out, CV_RGBA2GRAY);
//    else if (img.channels() == 3)
//        cv::cvtColor(img, out, CV_BGR2GRAY);
//    else if (img.channels() == 1)
//        out = img.clone();
//    return  out;
//}
//
//bool checkAndDraw(Mat out3, Matx33d H, bool isDraw = false,Scalar color = Scalar(0, 255, 0))
//{
//	std::vector<Point2f> obj_corners(4);
//	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(out3.cols, 0);
//	obj_corners[2] = cvPoint(out3.cols, out3.rows); obj_corners[3] = cvPoint(0, out3.rows);
//	std::vector<Point2f> scene_corners(4);
//
//	perspectiveTransform(obj_corners, scene_corners, H);
//	auto flag = true;
//	for (size_t i = 0; i < 2; i++)
//	{
//		auto edge = scene_corners[(i + 2) % 4] - scene_corners[i];
//		auto edge1 = scene_corners[(i + 1) % 4] - scene_corners[i];
//		auto edge2 = scene_corners[(i + 3) % 4] - scene_corners[i];
//		auto r1 = edge.cross(edge1);
//		auto r2 = edge.cross(edge2);
//		if (r1*r2 > 0)
//			flag = false;
//	}
//	if (flag&isDraw)
//	{
//		line(out3, scene_corners[0], scene_corners[1], color, 4);
//		line(out3, scene_corners[1], scene_corners[2], color, 4);
//		line(out3, scene_corners[2], scene_corners[3], color, 4);
//		line(out3, scene_corners[3], scene_corners[0], color, 4);
//	}
//	return flag;
//}





int main(int argc, char** argv)
{
	//planar_pattern_detector *t = planar_pattern_detector_builder::build_with_cache(model_image_url,
	//                                                             &range,
	//                                                             400,
	//                                                             5000,
	//                                                             0.0,
	//                                                             32, 7, 4,
	//                                                             30, 12,
	//                                                             10000, 200);

	shared_ptr<FernPlanarDetector> detector = make_shared<FernPlanarDetector>();
	cv::VideoCapture capture("../../VideoTest.avi");
	//cv::VideoCapture capture(0);
    while(1) {
        Mat frame;
		if (!capture.read(frame))
            break;
		Mat out3 = frame.clone();
		for (int cnt = 0; cnt < FernPlanarDetector::nmark; cnt++)
		{
			auto ret = detector->queryMarker(frame,cnt);
			if(ret.id>=0)
				detector->drawon(out3);
		}
		imshow("detection", out3);
		waitKey(1);
    }
    return 0;
}