/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "SetHSV.hpp"
#include "Common/Logger.hpp"

// OpenCV writes hue in range 0..180 instead of 0..360
#define H(x) (x>>1)

namespace Processors {
namespace SetHSV {

SetHSV_Processor::SetHSV_Processor(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello SetHSV_Processor\n";
}

SetHSV_Processor::~SetHSV_Processor()
{
	LOG(LTRACE) << "Good bye SetHSV_Processor\n";
}

bool SetHSV_Processor::onInit()
{
	LOG(LTRACE) << "SetHSV_Processor::initialize\n";

  h_onNewImage.setup(this, &SetHSV_Processor::onNewImage);
  registerHandler("onNewImage", &h_onNewImage);

  registerStream("in_img", &in_img);

  newImage = registerEvent("newImage");

  registerStream("out_hue", &out_hue);
  registerStream("out_saturation", &out_saturation);
  registerStream("out_value", &out_value);
  registerStream("out_segments", &out_segments);

	return true;
}

bool SetHSV_Processor::onFinish()
{
	LOG(LTRACE) << "SetHSV_Processor::finish\n";

	return true;
}

bool SetHSV_Processor::onStep()
{
	LOG(LTRACE) << "SetHSV_Processor::step\n";
	return true;
}

bool SetHSV_Processor::onStop()
{
	return true;
}

bool SetHSV_Processor::onStart()
{
	return true;
}

void SetHSV_Processor::onNewImage()
{
  try {

    Mat hsv_img = in_img.read();
    Size hsv_size = hsv_img.size();

    hue_img.create(hsv_size, CV_8UC1);
		saturation_img.create(hsv_size, CV_8UC1);
		value_img.create(hsv_size, CV_8UC1);
		segments_img.create(hsv_size, CV_8UC1);

    int done = 0;

    if (hsv_img.isContinuous() && segments_img.isContinuous() && value_img.isContinuous() 
                               && hue_img.isContinuous() &&     saturation_img.isContinuous()) {
			hsv_size.width *= hsv_size.height;
			hsv_size.height = 1;
		}

		hsv_size.width *= 3;

    for (int i = 0; i < hsv_size.height; i++) {

      //pointers of the rows
			const uchar* hsv_p = hsv_img.ptr <uchar> (i);
			uchar* hue_p = hue_img.ptr <uchar> (i);
			uchar* sat_p = saturation_img.ptr <uchar> (i);
			uchar* val_p = value_img.ptr <uchar> (i);
			uchar* seg_p = segments_img.ptr <uchar> (i);

			int j, k = 0;
			for(j = 0; j < hsv_size.width; j += 3)
			{
				hue_p[k] = hsv_p[j];
				sat_p[k] = hsv_p[j + 1];
				val_p[k] = hsv_p[j + 2];

				seg_p[k] = 0;

				if ((hue_p[k] > H(props.hue_min)) && (hue_p[k] < H(props.hue_max)))
				{
					if((sat_p[k] > props.sat_min) && (sat_p[k] < props.sat_max))
					{
            if((val_p[k] > props.val_min) && (val_p[k] < props.val_max))
            {
						  seg_p[k] = 255;
            }
					}
				}

				++k;
			}
		}

    out_hue.write(hue_img);
    out_saturation.write(saturation_img);
    out_value.write(value_img);
    out_segments.write(segments_img);

    newImage->raise();

  }
	catch (Common::DisCODeException& ex) {
		LOG(LERROR) << ex.what() << "\n";
		ex.printStackTrace();
		exit(EXIT_FAILURE);
	}
	catch (const char * ex) {
		LOG(LERROR) << ex;
	}
	catch (...) {
		LOG(LERROR) << "SetHSV_Processor::onNewImage failed\n";
	}
}

}//: namespace SetHSV
}//: namespace Processors
