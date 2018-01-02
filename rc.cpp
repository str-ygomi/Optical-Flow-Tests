#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <ctype.h>

using namespace std;
using namespace cv;

Size subPixWinSize(10,10), winSize(31,31);					// For corner refinement, for calcopticalflow
TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);		// Dont understand this properly
vector<uchar> status;
vector<float> err;

//	cout<<"** Number of corners detected: "<<points[0].size()<<endl;

//	for(int i=0; i < points[0].size(); ++i){
//		cout << "point #" << points[0][i] << endl;
//	}



int main(int argc, char** argv){

	VideoCapture inputvideo("myvideo.mp4");

	namedWindow("Repeat OpticalFlow", 1);

	Mat frame, frame_grey;
	inputvideo >> frame;

	cvtColor(frame, frame_grey, CV_BGR2GRAY);

	vector<Point2f> points[3];
	goodFeaturesToTrack(frame_grey, points[0], 500, 0.01, 10, Mat(), 3, 0, 0.04);



	Mat next_frame, next_frame_grey;
for(;;){
	inputvideo >> next_frame;



	cvtColor(next_frame, next_frame_grey, CV_BGR2GRAY);

	calcOpticalFlowPyrLK(frame_grey, next_frame_grey, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.1);


// POINTS AND FRAME POTENTIALLY DONT MATCH FOR SHOWING

	for(int i = 0; i < points[1].size(); ++i){
		if(status[i]!=0){
		circle(next_frame, points[1][i], 4, Scalar(0,0,255), -1, 8, 0 );
		}
	}

	imshow("Repeat OpticalFlow", next_frame);
	if(waitKey(30) >= 0) break;

	next_frame_grey.copyTo(frame_grey);
	
	points[2] = points[0];
	points[0] = points[1];
	points[1] = points[2];
	
//	cout << "First tracked point coords are " << points[1][0] << endl;
//	cout << "Status of first point is " << status[0] << endl;

	//waitKey(0);




	}
}
