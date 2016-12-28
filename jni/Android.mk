LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := mpfr-prebuilt
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libmpfr.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include/$(TARGET_ARCH_ABI)
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := mpfrjni
LOCAL_SRC_FILES := org_gmplib_gmpjni_MPFR.cpp org_gmplib_gmpjni_MPFR_mpfr_t.cpp
LOCAL_CFLAGS := -O2
LOCAL_SHARED_LIBRARIES := mpfr-prebuilt
include $(BUILD_SHARED_LIBRARY)
