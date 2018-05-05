APP_PLATFORM			:= android-21
APP_ABI					:= armeabi-v7a
APP_OPTIM				:= release
APP_STL					:= gnustl_static
NDK_TOOLCHAIN_VERSION	:= clang
APP_CFLAGS				:= -O3 -ffast-math -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -fPIC -march=armv7-a -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -fexceptions -fno-rtti
