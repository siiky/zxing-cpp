/*
* Copyright 2022 gitlost
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "ThresholdBinarizer.h"
#include "oned/ODReader.h"

#include "gtest/gtest.h"

using namespace ZXing;

// Helper to parse a 0/1 string into a BitMatrix
static BitMatrix ParseBitMatrix(const std::string& str, const int width)
{
	const int height = static_cast<int>(str.length() / width);

	BitMatrix mat(width, height);
	for (int y = 0; y < height; ++y) {
		size_t offset = y * width;
		for (int x = 0; x < width; ++x, offset++) {
			if (str[offset] == '1') {
				mat.set(x, y);
			}
		}
	}
	return mat;
}

// Helper to convert a BitMatrix into a black/white ImageView
static ImageView getImageView(std::vector<uint8_t> &buf, const BitMatrix &bits)
{
	buf.reserve(bits.width() * bits.height());
	for (int r = 0; r < bits.height(); r++) {
		const int k = r * bits.width();
		for (int c = 0; c < bits.width(); c++) {
			buf[k + c] = bits.get(c, r) ? 0x00 : 0xFF;
		}
	}
	return ImageView(buf.data(), bits.width(), bits.height(), ImageFormat::Lum);
}

TEST(ThresholdBinarizerTest, PatternRowClear)
{
	std::string bitstream;
	BitMatrix bits;
	DecodeHints hints;
	std::vector<uint8_t> buf;
	Result result(DecodeStatus::NotFound);

	// Test that ThresholdBinarizer::getPatternRow() clears row first (same as GlobalHistogramBinarizer)
	// Following was failing due to OneD::DoDecode() accumulating bars in loop when using ThresholdBinarizer

	// DataBarExpanded Stacked with 11 rows (11 + 10 * 3 separators), 1 column (2 data segments) wide
	bitstream = "01010101111011111110111111110000101100100000010100010"
				"00001010000100000001000000001010010011011111101010000"
				"00000101010101010101010101010101010101010101010100000"
				"00001000001110100010100001010101001110000101100110000"
				"10100111110001011101011110000000010001111010011000101"
				"00001000001110100010100001010101001110000101100110000"
				"00000101010101010101010101010101010101010101010100000"
				"00000001110100100001010000001010010101100111000110000"
				"01011110001011011110001111110000101010011000111000010"
				"00000001110100100001010000001010010101100111000110000"
				"00000101010101010101010101010101010101010101010100000"
				"00000110001111101110100001010100001110001110111010000"
				"10101001110000010001011110000001110001110001000100101"
				"00000110001111101110100001010100001110001110111010000"
				"00000101010101010101010101010101010101010101010100000"
				"00001000011011000101010000101010010011100010001100000"
				"01000111100100111010001111000000101100011101110010010"
				"00001000011011000101010000101010010011100010001100000"
				"00000101010101010101010101010101010101010101010100000"
				"00001000110001110110100000000100001011110100001000000"
				"10100111001110001001011111111001110100001011110111101"
				"00001000110001110110100000000100001011110100001000000"
				"00000101010101010101010101010101010101010101010100000"
				"00000010011101111101010010101010010100110000100000000"
				"01011101100010000010001100000000101011001111011110010"
				"00000010011101111101010010101010010100110000100000000"
				"00000101010101010101010101010101010101010101010100000"
				"00000100010111101110100000101010001100000110011010000"
				"10111011101000010001011111000000110011111001100101101"
				"00000100010111101110100000101010001100000110011010000"
				"00000101010101010101010101010101010101010101010100000"
				"00000011000110001001000000010101010000011110011010000"
				"01011100111001110110011111100000101111100001100101010"
				"00000011000110001001000000010101010000011110011010000"
				"00000101010101010101010101010101010101010101010100000"
				"00001011100010001110100000000010001011111010001100000"
				"10100100011101110001011111111100110100000101110011101"
				"00001011100010001110100000000010001011111010001100000"
				"00000101010101010101010101010101010101010101010100000"
				"00001000111010000101000101010101010100001011000110000"
				"01000111000101111010011000000000101011110100111000010";

	bits = ParseBitMatrix(bitstream, 53 /*width*/);
	hints.setFormats(BarcodeFormat::DataBarExpanded);
	OneD::Reader reader(hints);

	result = reader.decode(ThresholdBinarizer(getImageView(buf, bits), 0x7F));
	EXPECT_TRUE(result.isValid());
	EXPECT_EQ(result.text(), L"(91)12345678901234567890123456789012345678901234567890123456789012345678");
}
