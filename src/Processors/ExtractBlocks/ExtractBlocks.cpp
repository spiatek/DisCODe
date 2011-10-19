/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "ExtractBlocks.hpp"
#include "Common/Logger.hpp"

#include "Types/Rectangle.hpp"

namespace Processors {
namespace ExtractBlocks {

// OpenCV writes hue in range 0..180 instead of 0..360
#define H(x) (x>>1)

ExtractBlocks_Processor::ExtractBlocks_Processor(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello ExtractBlocks_Processor\n";
	blobs_ready = hue_ready = false;
}

ExtractBlocks_Processor::~ExtractBlocks_Processor()
{
	LOG(LTRACE) << "Good bye ExtractBlocks_Processor\n";
}

bool ExtractBlocks_Processor::onInit()
{
	LOG(LTRACE) << "ExtractBlocks_Processor::initialize\n";

	h_onNewImage.setup(this, &ExtractBlocks_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	h_onNewBlobs.setup(this, &ExtractBlocks_Processor::onNewBlobs);
	registerHandler("onNewBlobs", &h_onNewBlobs);

	registerStream("in_blobs", &in_blobs);
	registerStream("in_hue", &in_hue);

	newImage = registerEvent("newImage");

	registerStream("out_blocks", &out_blocks);

	return true;
}

bool ExtractBlocks_Processor::onFinish()
{
	LOG(LTRACE) << "ExtractBlocks_Processor::finish\n";

	return true;
}

bool ExtractBlocks_Processor::onStep()
{
	LOG(LTRACE) << "ExtractBlocks_Processor::step\n";
  
	blobs_ready = hue_ready = false;

	try {
		int id = 0;
		int i;
		IplImage h = IplImage(hue_img);
		Types::Blobs::Blob *currentBlob;
		Types::DrawableContainer blocks;

		// iterate through all found blobs
		for (i = 0; i < blobs.GetNumBlobs(); i++ )
		{
			currentBlob = blobs.GetBlob(i);

			// get blob bounding rectangle
			CvRect r2 = currentBlob->GetBoundingBox();

			++id;

			blocks.add(new Types::Rectangle(r2.x, r2.y, r2.width, r2.height));

			out_blocks.write(blocks);

			newImage->raise();
		}

		return true;
	}   catch (...) {
		LOG(LERROR) << "ExtractBlocks_Processor::onNewImage failed\n";
		return false;
  }
}

bool ExtractBlocks_Processor::onStop()
{
	return true;
}

bool ExtractBlocks_Processor::onStart()
{
	return true;
}

void ExtractBlocks_Processor::onNewImage()
{
	LOG(LTRACE) << "ExtractBlocks_Processor::onNewImage\n";

	hue_ready = true;
	hue_img = in_hue.read();
	hue_img = hue_img.clone();
	if (blobs_ready && hue_ready)
		onStep();
}

void ExtractBlocks_Processor::onNewBlobs()
{
	LOG(LTRACE) << "ExtractBlocks_Processor::onNewBlobs\n";

	blobs_ready = true;
	blobs = in_blobs.read();
	if (blobs_ready && hue_ready)
		onStep();
}

}//: namespace ExtractBlocks
}//: namespace Processors
