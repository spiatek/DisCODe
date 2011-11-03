/*!
 * \file
 * \brief
 */

#ifndef SETHSV_PROCESSOR_HPP_
#define SETHSV_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
using namespace cv;

namespace Processors {
namespace SetHSV {

/*!
 * \brief SetHSV properties
 */
struct SetHSV_Props: public Base::Props
{

  int hue_min;
  int hue_max;

  int sat_min;
  int sat_max;

  int val_min;
  int val_max;
 
  /*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
    hue_min = pt.get("hue_min",0);
    hue_max = pt.get("hue_max",255);
    sat_min = pt.get("sat_min",0);
    sat_max = pt.get("sat_max",255);
    val_min = pt.get("val_min",0);
    val_max = pt.get("val_max",255);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
    pt.put("hue_min", hue_min);
    pt.put("hue_max", hue_max);
    pt.put("sat_min", sat_min);
    pt.put("sat_max", sat_max);
    pt.put("val_min", val_min);
    pt.put("val_max", val_max);
	}

};

/*!
 * \class SetHSV_Processor
 * \brief SetHSV processor class.
 */
class SetHSV_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	SetHSV_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~SetHSV_Processor();

	/*!
	 * Return window properties
	 */
	Base::Props * getProperties()
	{
		return &props;
	}

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();

  void onNewImage();

  Base::EventHandler <SetHSV_Processor> h_onNewImage;

  Base::DataStreamIn <Mat> in_img;

  Base::Event * newImage;

  Base::DataStreamOut <Mat> out_hue;

  Base::DataStreamOut <Mat> out_saturation;

  Base::DataStreamOut <Mat> out_value;

  Base::DataStreamOut <Mat> out_segments;

	/// Properties
	SetHSV_Props props;

private:

  Mat hue_img;
  Mat saturation_img;
  Mat value_img;
  Mat segments_img;

};

}//: namespace SetHSV
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("SetHSV", Processors::SetHSV::SetHSV_Processor, Common::Panel_Empty)

#endif /* SETHSV_PROCESSOR_HPP_ */
