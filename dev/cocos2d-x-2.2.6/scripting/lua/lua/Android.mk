LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := lua51_static

LOCAL_MODULE_FILENAME := liblua51

APP_CPPFLAGS := -O2

LOCAL_SRC_FILES := \
lapi.c \
lauxlib.c \
lbaselib.c \
lcode.c \
ldblib.c \
ldebug.c \
ldo.c \
ldump.c \
lfunc.c \
lgc.c \
linit.c \
liolib.c \
llex.c \
lmathlib.c \
lmem.c \
loadlib.c \
lobject.c \
lopcodes.c \
loslib.c \
lparser.c \
lstate.c \
lstring.c \
lstrlib.c \
ltable.c \
ltablib.c \
ltm.c \
lua.c \
lundump.c \
lvm.c \
lzio.c \
print.c \

LOCAL_C_INCLUDES+= $(LOCAL_PATH)/


include $(BUILD_STATIC_LIBRARY)
