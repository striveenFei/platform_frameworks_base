#this file is used for Linux compile configuration

CEDARX_DEBUG_LEVEL := L0

CEDARX_EXT_CFLAGS := -D__CDX_ENABLE_NETWORK -D__CHIP_VERSION_$(CEDARX_CHIP_VERSION) -D_FILE_OFFSET_BITS=64 
#CEDARX_EXT_CFLAGS += -D__CDX_PRODUCTOR_WISTON
#CEDARX_EXT_CFLAGS += -D__CDX_ENABLE_SUBTITLE