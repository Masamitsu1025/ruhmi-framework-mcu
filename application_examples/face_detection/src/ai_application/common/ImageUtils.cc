/*
 * SPDX-FileCopyrightText: Copyright 2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ImageUtils.hpp"
#include <limits>

namespace arm {
namespace app {
namespace image {

    float Calculate1DOverlap(float x1Center, float width1, float x2Center, float width2)
    {
        float left_1 = x1Center - width1/2;
        float left_2 = x2Center - width2/2;
        float leftest = left_1 > left_2 ? left_1 : left_2;

        float right_1 = x1Center + width1/2;
        float right_2 = x2Center + width2/2;
        float rightest = right_1 < right_2 ? right_1 : right_2;

        return rightest - leftest;
    }

    float CalculateBoxIntersect(Box& box1, Box& box2)
    {
        float width = Calculate1DOverlap(box1.x, box1.w, box2.x, box2.w);
        if (width < 0) {
            return 0;
        }
        float height = Calculate1DOverlap(box1.y, box1.h, box2.y, box2.h);
        if (height < 0) {
            return 0;
        }

        float total_area = width*height;
        return total_area;
    }

    float CalculateBoxUnion(Box& box1, Box& box2)
    {
        float boxes_intersection = CalculateBoxIntersect(box1, box2);
        float boxes_union = box1.w * box1.h + box2.w * box2.h - boxes_intersection;
        return boxes_union;
    }

    float CalculateBoxIOU(Box& box1, Box& box2)
    {
        float boxes_intersection = CalculateBoxIntersect(box1, box2);
        if (boxes_intersection == 0) {
            return 0;
        }

        float boxes_union = CalculateBoxUnion(box1, box2);
        if (boxes_union == 0) {
            return 0;
        }

        return boxes_intersection / boxes_union;
    }

    void CalculateNMS(std::forward_list<Detection>& detections, int classes, float iouThreshold)
    {
        int idxClass{0};
        auto CompareProbs = [idxClass](Detection& prob1, Detection& prob2) {
            return prob1.prob[idxClass] > prob2.prob[idxClass];
        };

        for (idxClass = 0; idxClass < classes; ++idxClass) {
            detections.sort(CompareProbs);

            for (auto it=detections.begin(); it != detections.end(); ++it) {
                if (it->prob[idxClass] == 0) continue;
                for (auto itc=std::next(it, 1); itc != detections.end(); ++itc) {
                    if (itc->prob[idxClass] == 0) {
                        continue;
                    }
                    if (CalculateBoxIOU(it->bbox, itc->bbox) > iouThreshold) {
                        itc->prob[idxClass] = 0;
                    }
                }
            }
        }
    }
    /**
   	 * The ConvertImgToInt8 function converts an image represented as unsigned 8-bit integers (ranging from 0 to 255) to
   	 * signed 8-bit integers (ranging from -128 to 127). The function modifies the original data in-place.
   	 * @param[in/out]       void* data		A pointer to the image data, which is expected to be in unsigned 8-bit integer format (values from 0 to 255) input.
   	 * 									    This buffer will be holding the converted INT8 integer after the function finished execution.
   	 * @param[in]       	const size_t kMaxImageSize        		The number of pixels in the image
   	 **/
    void ConvertImgToInt8(void* data, const size_t kMaxImageSize)
    {
        auto* tmp_req_data = static_cast<uint8_t*>(data);
        auto* tmp_signed_req_data = static_cast<int8_t*>(data);

        for (size_t i = 0; i < kMaxImageSize; i++) {
            tmp_signed_req_data[i] = (int8_t) (
                    (int32_t) (tmp_req_data[i]) - 128);
        }
    }

    /*********************************************************************************************************************
	 * This function processes an RGB image by converting each pixel to grayscale using a weighted formula.
	 * The results are stored in the destination buffer, ensuring that the output values are valid 8-bit grayscale values.
	 * @param[in]       const uint8_t* srcPtr		A pointer to the source RGB image data, where each pixel is represented
	 * 												by three consecutive bytes: Red, Green, and Blue (in this order).
	 * @param[in]       uint8_t* dstPtr        		A pointer to the destination buffer that will store the grayscale image data
	 * @param[in]       const size_t dstImgSz      The size of the output (grayscale) image in terms of the number of pixels.
	 * @param[none] 	none
	 ***********************************************************************************************************************/
    void RgbToGrayscale(const uint8_t* srcPtr, uint8_t* dstPtr, const size_t dstImgSz)
    {
        const float R = (float)0.299;
        const float G = (float)0.587;
        const float B = (float)0.114;
        for (size_t i = 0; i < dstImgSz; ++i, srcPtr += 3) {
            uint32_t  int_gray = R * (*srcPtr) +
                                 G * (*(srcPtr + 1)) +
                                 B * (*(srcPtr + 2));
            *dstPtr++ = int_gray <= std::numeric_limits<uint8_t>::max() ?
                        int_gray : std::numeric_limits<uint8_t>::max();
        }
    }

} /* namespace image */
} /* namespace app */
} /* namespace arm */
