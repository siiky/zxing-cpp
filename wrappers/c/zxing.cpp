#include "zxing.h"

#include <ZXing/BarcodeFormat.h>
#include <ZXing/DecodeHints.h>
#include <ZXing/ImageView.h>
#include <ZXing/ReadBarcode.h>
#include <ZXing/Result.h>

extern "C"
{
	/*
	 * ZXing/ImageView.h
	 */

	zxing_ImageView* zxing_ImageView_new(const uint8_t* data, int width, int height, zxing_ImageFormat format, int rowStride,
										 int pixStride)
	{
		ZXing::ImageFormat cppformat = static_cast<ZXing::ImageFormat>(format);
		return new ZXing::ImageView(data, width, height, cppformat, rowStride, pixStride);
	}

	void zxing_ImageView_delete(zxing_ImageView* iv)
	{
		delete iv;
	}

	int zxing_ImageView_width(const zxing_ImageView* iv)
	{
		return iv->width();
	}

	int zxing_ImageView_height(const zxing_ImageView* iv)
	{
		return iv->height();
	}

	int zxing_ImageView_pixStride(const zxing_ImageView* iv)
	{
		return iv->pixStride();
	}

	int zxing_ImageView_rowStride(const zxing_ImageView* iv)
	{
		return iv->rowStride();
	}

	zxing_ImageFormat zxing_ImageView_format(const zxing_ImageView* iv)
	{
		return static_cast<zxing_ImageFormat>(iv->format());
	}

	/*
	 * ...
	 */

	/*
	 * ZXing/BarcodeFormat.h
	 */

	zxing_BarcodeFormat zxing_BarcodeFormatFromString(const char* format)
	{
		return static_cast<zxing_BarcodeFormat>(ZXing::BarcodeFormatFromString(std::string(format)));
	}

	// TODO:
	// zxing_BarcodeFormat zxing_BarcodeFormatsFromString (const char * formats)
	//{
	//    return (zxing_BarcodeFormat) ZXing::BarcodeFormatsFromString(std::string(formats));
	//}

	/*
	 * ZXing/DecodeHints.h
	 */

	zxing_DecodeHints* zxing_DecodeHints_new()
	{
		return new ZXing::DecodeHints();
	}

	void zxing_DecodeHints_delete(zxing_DecodeHints* hints)
	{
		delete hints;
	}

	void zxing_DecodeHints_setTryHarder(zxing_DecodeHints* hints, bool tryHarder)
	{
		hints->setTryHarder(tryHarder);
	}

	void zxing_DecodeHints_setTryDownscale(zxing_DecodeHints* hints, bool tryDownscale)
	{
		hints->setTryDownscale(tryDownscale);
	}

	void zxing_DecodeHints_setFormats(zxing_DecodeHints* hints, zxing_BarcodeFormat formats)
	{
		hints->setFormats(static_cast<ZXing::BarcodeFormat>(formats));
	}

	void zxing_DecodeHints_setBinarizer(zxing_DecodeHints* hints, zxing_Binarizer binarizer)
	{
		hints->setBinarizer(static_cast<ZXing::Binarizer>(binarizer));
	}

	void zxing_DecodeHints_setEanAddOnSymbol(zxing_DecodeHints* hints, zxing_EanAddOnSymbol eanAddOnSymbol)
	{
		hints->setEanAddOnSymbol(static_cast<ZXing::EanAddOnSymbol>(eanAddOnSymbol));
	}

	void zxing_DecodeHints_setTextMode(zxing_DecodeHints* hints, zxing_TextMode textMode)
	{
		hints->setTextMode(static_cast<ZXing::TextMode>(textMode));
	}

	/*
	 * ...
	 */

	/*
	 * ZXing/Result.h
	 */

	void zxing_Result_delete(zxing_Result* result)
	{
		delete result;
	}

	bool zxing_Result_isValid(const zxing_Result* result)
	{
		return result->isValid();
	}

	zxing_BarcodeFormat zxing_Result_format(const zxing_Result* result)
	{
		return static_cast<zxing_BarcodeFormat>(result->format());
	}

	char* zxing_Result_bytes(const zxing_Result* result, int* len)
	{
		const ZXing::ByteArray& bytes = result->bytes();
		size_t nbytes = bytes.size();

		char* ret = (char*) malloc(nbytes + 1);
		if (!ret) {
			*len = 0;
			return ret;
		}

		const char* data = (const char*) bytes.data();
		strncpy(ret, data, nbytes);
		*len = nbytes;

		return ret;
	}

	/*
	 * ...
	 */

	/*
	 * ZXing/ReadBarcode.h
	 */

	zxing_Result* zxing_ReadBarcode(const zxing_ImageView* iv, const zxing_DecodeHints* hints)
	{
		return new ZXing::Result(ZXing::ReadBarcode(*iv, *hints));
	}

	zxing_Results* zxing_ReadBarcodes(const zxing_ImageView* iv, const zxing_DecodeHints* hints)
	{
		return new ZXing::Results(ZXing::ReadBarcodes(*iv, *hints));
	}
}
