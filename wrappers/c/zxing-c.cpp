/*
* Copyright 2023 siiky
* Copyright 2023 Axel Waggershauser
*/
// SPDX-License-Identifier: Apache-2.0

#include "zxing-c.h"

#include "ReadBarcode.h"

using namespace ZXing;

char* copy(std::string_view sv)
{
	auto ret = (char*)malloc(sv.size() + 1);
	if (ret) {
		strncpy(ret, sv.data(), sv.size());
		ret[sv.size()] = '\0';
	}
	return ret;
}

extern "C"
{
	/*
	 * ZXing/ImageView.h
	 */

	zxing_ImageView* zxing_ImageView_new(const uint8_t* data, int width, int height, zxing_ImageFormat format, int rowStride,
										 int pixStride)
	{
		ImageFormat cppformat = static_cast<ImageFormat>(format);
		return new ImageView(data, width, height, cppformat, rowStride, pixStride);
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

	zxing_BarcodeFormats zxing_BarcodeFormatsFromString(const char* str)
	{
		if (!str)
			return {};
		try {
			auto format = BarcodeFormatsFromString(str);
			return static_cast<zxing_BarcodeFormats>(*reinterpret_cast<BarcodeFormat*>(&format));
		} catch (...) {
			return zxing_BarcodeFormat_Invalid;
		}
	}

	char* zxing_BarcodeFormatToString(zxing_BarcodeFormat format)
	{
		return copy(ToString(static_cast<BarcodeFormat>(format)));
	}

	/*
	 * ZXing/DecodeHints.h
	 */

	zxing_DecodeHints* zxing_DecodeHints_new()
	{
		return new DecodeHints();
	}

	void zxing_DecodeHints_delete(zxing_DecodeHints* hints)
	{
		delete hints;
	}

	void zxing_DecodeHints_setTryHarder(zxing_DecodeHints* hints, bool tryHarder)
	{
		hints->setTryHarder(tryHarder);
	}

	void zxing_DecodeHints_setTryRotate(zxing_DecodeHints* hints, bool tryRotate)
	{
		hints->setTryRotate(tryRotate);
	}

	void zxing_DecodeHints_setTryInvert(zxing_DecodeHints* hints, bool tryInvert)
	{
		hints->setTryInvert(tryInvert);
	}

	void zxing_DecodeHints_setTryDownscale(zxing_DecodeHints* hints, bool tryDownscale)
	{
		hints->setTryDownscale(tryDownscale);
	}

	void zxing_DecodeHints_setIsPure(zxing_DecodeHints* hints, bool isPure)
	{
		hints->setIsPure(isPure);
	}

	void zxing_DecodeHints_setReturnErrors(zxing_DecodeHints* hints, bool returnErrors)
	{
		hints->setReturnErrors(returnErrors);
	}

	void zxing_DecodeHints_setFormats(zxing_DecodeHints* hints, zxing_BarcodeFormats formats)
	{
		hints->setFormats(static_cast<BarcodeFormat>(formats));
	}

	void zxing_DecodeHints_setBinarizer(zxing_DecodeHints* hints, zxing_Binarizer binarizer)
	{
		hints->setBinarizer(static_cast<Binarizer>(binarizer));
	}

	void zxing_DecodeHints_setEanAddOnSymbol(zxing_DecodeHints* hints, zxing_EanAddOnSymbol eanAddOnSymbol)
	{
		hints->setEanAddOnSymbol(static_cast<EanAddOnSymbol>(eanAddOnSymbol));
	}

	void zxing_DecodeHints_setTextMode(zxing_DecodeHints* hints, zxing_TextMode textMode)
	{
		hints->setTextMode(static_cast<TextMode>(textMode));
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

	char* zxing_Result_errorMsg(const zxing_Result* result)
	{
		return copy(ToString(result->error()));
	}

	zxing_BarcodeFormat zxing_Result_format(const zxing_Result* result)
	{
		return static_cast<zxing_BarcodeFormat>(result->format());
	}

	uint8_t* zxing_Result_bytes(const zxing_Result* result, int* len)
	{
		*len = Size(result->bytes());

		auto ret = (uint8_t*)malloc(*len + 1);
		if (ret)
			memcpy(ret, result->bytes().data(), *len);
		else
			*len = 0;

		return ret;
	}

	char* zxing_Result_text(const zxing_Result* result)
	{
		return copy(result->text());
	}

	char* zxing_Result_ecLevel(const zxing_Result* result)
	{
		return copy(result->ecLevel());
	}

	char* zxing_Result_symbologyIdentifier(const zxing_Result* result)
	{
		return copy(result->symbologyIdentifier());
	}

	int zxing_Result_orientation(const zxing_Result* result)
	{
		return result->orientation();
	}

	bool zxing_Result_isInverted(const zxing_Result* result)
	{
		return result->isInverted();
	}

	bool zxing_Result_isMirrored(const zxing_Result* result)
	{
		return result->isMirrored();
	}

	/*
	 * ...
	 */

	/*
	 * ZXing/ReadBarcode.h
	 */

	zxing_Result* zxing_ReadBarcode(const zxing_ImageView* iv, const zxing_DecodeHints* hints)
	{
		auto res = ReadBarcode(*iv, *hints);
		return res.format() != BarcodeFormat::None ? new Result(std::move(res)) : NULL;
	}

	zxing_Results* zxing_ReadBarcodes(const zxing_ImageView* iv, const zxing_DecodeHints* hints)
	{
		return new Results(ReadBarcodes(*iv, *hints));
	}
}
