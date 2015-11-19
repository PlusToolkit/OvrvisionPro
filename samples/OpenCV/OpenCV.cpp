// OpenCV.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "ovrvision_pro.h"

using namespace cv;
using namespace OVR;

int main(int argc, char* argv[])
{
	OvrvisionPro ovrvision;
	if (ovrvision.Open(0, Camprop::OV_CAMHD_FULL))
	{
		int width = ovrvision.GetCamWidth();
		int height = ovrvision.GetCamHeight();
		Mat left(height, width, CV_8UC4);
		Mat right(height, width, CV_8UC4);

		for (bool loop = true; loop;)
		{
			// Capture frame
			ovrvision.PreStoreCamData(Camqt::OV_CAMQT_DMSRMP);

			// Retrieve frame data
			ovrvision.GetCamImageBGRA(left.data, Cameye::OV_CAMEYE_LEFT);
			ovrvision.GetCamImageBGRA(right.data, Cameye::OV_CAMEYE_RIGHT);

			// Show frame data
			imshow("Left", left);
			//imshow("Right", right);

			switch (waitKey(10))
			{
			case 'q':
				loop = false;
				break;
			}
		}
	}
	else
	{
		puts("FAILED TO OPEN CAMERA");
	}
	return 0;
}


