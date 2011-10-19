/*!
 * \file
 * \brief
 */

#ifndef EXTRACTBLOCKS_PROCESSOR_HPP_
#define EXTRACTBLOCKS_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

#include <vector>

#include "Types/Blobs/BlobResult.hpp"
#include "Types/DrawableContainer.hpp"

namespace Processors {
namespace ExtractBlocks {

using namespace cv;

/*!
 * \brief ExtractBlocks properties
 */
struct ExtractBlocks_Props: public Base::Props
{

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{

	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{

	}

};

/*!
 * \class ExtractBlocks_Processor
 * \brief ExtractBlocks processor class.
 */
class ExtractBlocks_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	ExtractBlocks_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~ExtractBlocks_Processor();

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
  Base::EventHandler <ExtractBlocks_Processor> h_onNewImage;

  void onNewBlobs();
  Base::EventHandler <ExtractBlocks_Processor> h_onNewBlobs;

  Base::DataStreamIn <Types::Blobs::BlobResult> in_blobs;
  Base::DataStreamIn <Mat> in_hue;

  Base::Event * newImage;

  Base::DataStreamOut < Types::DrawableContainer > out_blocks;

	/// Properties
	ExtractBlocks_Props props;

private:
	
  Mat hue_img;
	Mat segments;

	bool blobs_ready;
	bool hue_ready;

	Types::Blobs::BlobResult blobs;

};

}//: namespace ExtractBlocks
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("ExtractBlocks", Processors::ExtractBlocks::ExtractBlocks_Processor, Common::Panel_Empty)

#endif /* EXTRACTBLOCKS_PROCESSOR_HPP_ */
