#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "ovrvision_v4l.h"
#include "OvrvisionProCUDA.h"

using namespace OVR;
using namespace cv;

#define WIDTH 320	//640
#define HEIGHT	240	//480

int main(int argc, char *argv[])
{
	Mat image(HEIGHT, WIDTH, CV_16UC1);
	Mat left(HEIGHT, WIDTH, CV_8UC3);
	Mat right(HEIGHT, WIDTH, CV_8UC3);
	OvrvisionProCUDA cuda(WIDTH, HEIGHT);

	OvrvisionVideo4Linux v4l;
	v4l.OpenDevice(0, WIDTH, HEIGHT, 0);
	v4l.QueryCapability();
	v4l.EnumFormats();
	v4l.StartTransfer();
#ifdef JETSON_TK1
	printf("%d %d %d\n", cuda._srcMat.cols, cuda._srcMat.rows, cuda._srcMat.step1());
#endif
	for (bool loop = true; loop;)
	{
#ifdef JETSON_TK1
		unsigned char *buffer = cuda.GetBufferPtr();
		if (0 == v4l.GetBayer16Image(buffer, cuda._srcMat.step))
		{
			cuda.Demosaic();
			//imshow("Bayer", cuda._srcMat);
			imshow("Left", cuda._left);
			imshow("Right", cuda._right);
		}
#else
		if (0 == v4l.GetBayer16Image(image.data))
		{
			//cuda.Demosaic(image, left, right);
			cuda.Demosaic(image);
			imshow("Bayer", image);
			//imshow("Left", left);
			//imshow("Right", right);
		}
#endif
		switch (waitKey(10))
		{case 'q':
			loop = false;
			break;
		case ' ':
			imwrite("frame.png", image);
			break;
		}
	}
	v4l.StopTransfer();
	v4l.DeleteDevice();
}

/* Result
Driver name     : uvcvideo
Driver Version  : 3.10.40
Device name     : OvrvisionPro
Bus information : usb-tegra-ehci.0-1
Capabilities    : 84000001h
V4L2_CAP_VIDEO_CAPTURE (Video Capture)
V4L2_CAP_STREAMING (Streaming I/O method)
*/

