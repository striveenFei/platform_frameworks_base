/*
 * Copyright (C) 2009 The Android Open Source Project
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

#ifndef CEDARXNATIVE_RENDERER_H_

#define CEDARXNATIVE_RENDERER_H_


#include <utils/RefBase.h>
#include <ui/android_native_buffer.h>
#include <OMX_IVCommon.h>

namespace android {

enum VIDEORENDER_CMD
{
	VIDEORENDER_CMD_UNKOWN = 0,
	VIDEORENDER_CMD_GETFRAMEID,
};

struct MetaData;

class CedarXNativeRenderer {
public:
    CedarXNativeRenderer(
            const sp<ANativeWindow> &nativeWindow, const sp<MetaData> &meta);

    ~CedarXNativeRenderer();

    void render(
            const void *data, size_t size, void *platformPrivate);

    int control(int cmd, int para);
private:
    OMX_COLOR_FORMATTYPE mColorFormat;
    sp<ANativeWindow> mNativeWindow;
    int32_t mWidth, mHeight;
    int32_t mCropLeft, mCropTop, mCropRight, mCropBottom;
    int32_t mCropWidth, mCropHeight;

    CedarXNativeRenderer(const CedarXNativeRenderer &);
    CedarXNativeRenderer &operator=(const CedarXNativeRenderer &);
};

}  // namespace android

#endif  // SOFTWARE_RENDERER_H_
