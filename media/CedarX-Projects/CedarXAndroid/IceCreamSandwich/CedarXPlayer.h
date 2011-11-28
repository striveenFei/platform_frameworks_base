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

#ifndef CEDARX_PLAYER_H_

#define CEDARX_PLAYER_H_

#include <media/MediaPlayerInterface.h>
#include <media/stagefright/DataSource.h>
//#include <media/stagefright/OMXClient.h>
#include <media/stagefright/TimeSource.h>
#include <utils/threads.h>

#include <media/stagefright/CedarXAudioPlayer.h>
#include <media/stagefright/MediaBuffer.h>
//#include <media/stagefright/MediaClock.h>

#include <media/mediaplayerinfo.h>

#include <CDX_PlayerAPI.h>

namespace android {

struct CedarXAudioPlayer;
struct DataSource;
struct MediaBuffer;
struct MediaExtractor;
struct MediaSource;
struct NuCachedSource2;
struct ISurfaceTexture;

struct ALooper;
//struct ARTSPController;
//struct ARTPSession;
//struct UDPPusher;

//struct CedarXRenderer : public RefBase {
//    CedarXRenderer() {}
//
//    //virtual void render(MediaBuffer *buffer) = 0;
//    virtual void render(const void *data, size_t size) = 0;
//#ifndef __CHIP_VERSION_F20
//    virtual int control(int cmd, int para) = 0;
//#else
//    virtual int getframeid() = 0;
//#endif
//
//private:
//    CedarXRenderer(const CedarXRenderer &);
//    CedarXRenderer &operator=(const CedarXRenderer &);
//};

struct CedarXRenderer : public RefBase {
    CedarXRenderer() {}

    virtual void render(const void *data, size_t size) = 0;
    //virtual void render(MediaBuffer *buffer) = 0;
    virtual int control(int cmd, int para) = 0;
private:
    CedarXRenderer(const CedarXRenderer &);
    CedarXRenderer &operator=(const CedarXRenderer &);
};

struct CedarXPlayer {
    CedarXPlayer();
    ~CedarXPlayer();

    void setListener(const wp<MediaPlayerBase> &listener);
    void setUID(uid_t uid);

    status_t setDataSource(
            const char *uri,
            const KeyedVector<String8, String8> *headers = NULL);

    status_t setDataSource(int fd, int64_t offset, int64_t length);
    status_t setDataSource(const sp<IStreamSource> &source);

    void reset();

    status_t prepare();
    status_t prepare_l();
    status_t prepareAsync();
    status_t prepareAsync_l();

    status_t play();
    status_t pause();
    status_t stop_l();
    status_t stop();

    bool isPlaying() const;

    status_t setSurface(const sp<Surface> &surface);
    status_t setSurfaceTexture(const sp<ISurfaceTexture> &surfaceTexture);
    void setAudioSink(const sp<MediaPlayerBase::AudioSink> &audioSink);
    status_t setLooping(bool shouldLoop);

    status_t getDuration(int64_t *durationUs);
    status_t getPosition(int64_t *positionUs);

    status_t setParameter(int key, const Parcel &request);
    status_t getParameter(int key, Parcel *reply);
    status_t setCacheStatCollectFreq(const Parcel &request);
    status_t seekTo(int64_t timeUs);

    status_t getVideoDimensions(int32_t *width, int32_t *height) const;

    status_t suspend();
    status_t resume();
#ifndef __CHIP_VERSION_F20
    status_t setScreen(int screen);
    status_t set3DMode(int mode);
    int    	 getMeidaPlayerState();
    int      getSubCount();
    int      getSubList(MediaPlayer_SubInfo *infoList, int count);
    int      getCurSub();
    status_t switchSub(int index);
    status_t setSubGate(bool showSub);
    bool     getSubGate();
    status_t setSubColor(int color);
    int      getSubColor();
    status_t setSubFrameColor(int color);
    int      getSubFrameColor();
    status_t setSubFontSize(int size);
    int      getSubFontSize();
    status_t setSubCharset(const char *charset);
    status_t getSubCharset(char *charset);
    status_t setSubPosition(int percent);
    int      getSubPosition();
    status_t setSubDelay(int time);
    int      getSubDelay();
    int      getTrackCount();
    int      getTrackList(MediaPlayer_TrackInfo *infoList, int count);
    int      getCurTrack();
    status_t switchTrack(int index);
    status_t setInputDimensionType(int type);
    int 	 getInputDimensionType();
    status_t setOutputDimensionType(int type);
    int 	 getOutputDimensionType();
    status_t setAnaglaghType(int type);
    int 	 getAnaglaghType();
    status_t getVideoEncode(char *encode);
    int 	 getVideoFrameRate();
    status_t getAudioEncode(char *encode);
    int 	 getAudioBitRate();
    int 	 getAudioSampleRate();
#endif
    // This is a mask of MediaExtractor::Flags.
    uint32_t flags() const;

    void postAudioEOS();
    void postAudioSeekComplete();

    int CedarXPlayerCallback(int event, void *info);
private:
    friend struct CedarXEvent;

    enum {
        PLAYING             = 0x01,
        LOOPING             = 0x02,
        FIRST_FRAME         = 0x04,
        PREPARING           = 0x08,
        PREPARED            = 0x10,
        AT_EOS              = 0x20,
        PREPARE_CANCELLED   = 0x40,
        CACHE_UNDERRUN      = 0x80,
        AUDIO_AT_EOS        = 0x0100,
        VIDEO_AT_EOS        = 0x0200,
        AUTO_LOOPING        = 0x0400,
        WAIT_TO_PLAY         = 2048,
        WAIT_VO_EXIT         = 4096,
        CEDARX_LIB_INIT      = 8192,
        SUSPENDING           = 16384,
        PAUSING				 = 32768,
        RESTORE_CONTROL_PARA = 65536,
    };

    mutable Mutex mLock;
    Mutex mMiscStateLock;

    CDXPlayer *mPlayer;
    int mStreamType;

    bool mQueueStarted;
    wp<MediaPlayerBase> mListener;
    bool mUIDValid;
    uid_t mUID;

    sp<Surface> mSurface;
    sp<ANativeWindow> mNativeWindow;
    sp<MediaPlayerBase::AudioSink> mAudioSink;

    String8 mUri;
    bool mIsUri;
    KeyedVector<String8, String8> mUriHeaders;

    sp<CedarXRenderer> mVideoRenderer;
    bool mVideoRendererIsPreview;

    CedarXMediaInformations mMediaInfo;
    CedarXAudioPlayer *mAudioPlayer;
    int64_t mDurationUs;

    uint32_t mFlags;
    uint32_t mExtractorFlags;
    bool isCedarXInitialized;

    int32_t input_3d_type;
    int32_t output_3d_type;
    int32_t anaglagh_type;

    int32_t mVideoWidth, mVideoHeight, mFirstFrame;
    int32_t mCanSeek;
    int32_t mDisplayWidth, mDisplayHeight, mDisplayFormat;
    int64_t mTimeSourceDeltaUs;
    int64_t mVideoTimeUs;

    bool mTagPlay;
    bool mSeeking;
    bool mSeekNotificationSent;
    int64_t mSeekTimeUs;

    int64_t mBitrate;  // total bitrate of the file (in bps) or -1 if unknown.

    int64_t mLastValidPosition;

    bool mIsAsyncPrepare;
    status_t mPrepareResult;
    status_t mStreamDoneStatus;

    int     mPlayerState;

    void postVideoEvent_l(int64_t delayUs = -1);
    void postBufferingEvent_l();
    void postStreamDoneEvent_l(status_t status);
    void postCheckAudioStatusEvent_l();
    status_t play_l(int command);

    bool mWaitAudioPlayback;

    sp<ALooper> mLooper;

    int64_t mSuspensionPositionUs;

    struct SuspensionState {
        String8 mUri;
        KeyedVector<String8, String8> mUriHeaders;
        sp<DataSource> mFileSource;

        uint32_t mFlags;
        int64_t mPositionUs;
    } mSuspensionState;

    /*user defined parameters*/
    int32_t mScreenID;
    int32_t mAudioTrackIndex;

    struct SubtitleParameter {
		int32_t mSubtitleFontSize;
		int32_t mSubtitleColor;
		int32_t mSubtitleFrameColor;
		int32_t mSubtitleCharset;
		int32_t mSubtitleGate;
		int32_t mSubtitleDelay;
		int32_t mSubtitleIndex;
		int32_t mSubtitlePosition;
    }mSubtitleParameter;



//    status_t setDataSource_l(
//            const char *uri,
//            const KeyedVector<String8, String8> *headers = NULL);
//
//    status_t setDataSource_l(const sp<DataSource> &dataSource);
//    status_t setDataSource_l(const sp<MediaExtractor> &extractor);
    void reset_l();
    void partial_reset_l();
    status_t seekTo_l(int64_t timeUs);
    status_t pause_l(bool at_eos = false);
    void initRenderer_l();
    void seekAudioIfNecessary_l();

    void cancelPlayerEvents(bool keepBufferingGoing = false);

    void setAudioSource(sp<MediaSource> source);
    status_t initAudioDecoder();

    void setVideoSource(sp<MediaSource> source);
    status_t initVideoDecoder(uint32_t flags = 0);

    void onStreamDone();

    void notifyListener_l(int msg, int ext1 = 0, int ext2 = 0);

    void onBufferingUpdate();
    void onCheckAudioStatus();
    void onPrepareAsyncEvent();
    void abortPrepare(status_t err);
    void finishAsyncPrepare_l(int err);
    void finishSeek_l(int err);
    bool getCachedDuration_l(int64_t *durationUs, bool *eos);

    status_t finishSetDataSource_l();

    static bool ContinuePreparation(void *cookie);

    bool getBitrate(int64_t *bitrate);

    status_t setNativeWindow_l(const sp<ANativeWindow> &native);

    int StagefrightVideoRenderInit(int width, int height, int format, void *frame_info);
    int StagefrightAudioRenderInit(int samplerate, int channels, int format);
    void StagefrightVideoRenderData(void *frame_info, int frame_id);
    int StagefrightVideoRenderGetFrameID();

    void StagefrightAudioRenderExit(int immed);
    int StagefrightAudioRenderData(void* data, int len);
    int StagefrightAudioRenderGetSpace(void);
    int StagefrightAudioRenderGetDelay(void);

    CedarXPlayer(const CedarXPlayer &);
    CedarXPlayer &operator=(const CedarXPlayer &);
};

}  // namespace android

#endif  // AWESOME_PLAYER_H_

