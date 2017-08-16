APPNAME="全民娱乐城"

# options

buildexternalsfromsource=

usage(){
cat << EOF
usage: $0 [options]

Build C/C++ code for $APPNAME using Android NDK

OPTIONS:
-s  Build externals from source
-h  this help
EOF
}

while getopts "sh" OPTION; do
case "$OPTION" in
s)
buildexternalsfromsource=1
;;
h)
usage
exit 0
;;
esac
done

# paths

if [ -z "${NDK_ROOT+aaa}" ];then
echo "please define NDK_ROOT"
exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# ... use paths relative to current directory
COCOS2DX_ROOT="$DIR/../../../.."
APP_ROOT="$DIR/../.."
APP_ANDROID_ROOT="$DIR"

echo "NDK_ROOT = $NDK_ROOT"
echo "COCOS2DX_ROOT = $COCOS2DX_ROOT"
echo "APP_ROOT = $APP_ROOT"
echo "APP_ANDROID_ROOT = $APP_ANDROID_ROOT"

# make sure assets is exist
if [ -d "$APP_ANDROID_ROOT"/assets ]; then
    rm -rf "$APP_ANDROID_ROOT"/assets
fi

mkdir "$APP_ANDROID_ROOT"/assets

# copy resources
for file in "$APP_ROOT"/Resources/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_ANDROID_ROOT"/assets
fi

if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

# copy common luaScript
for file in "$APP_ROOT"/../../../scripting/lua/script/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_ANDROID_ROOT"/assets
fi

if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

if [[ "$buildexternalsfromsource" ]]; then
    echo "Building external dependencies from source"
    "$NDK_ROOT"/ndk-build -C "$APP_ANDROID_ROOT" $* \
        "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/source"
else
    echo "Using prebuilt externals"
    "$NDK_ROOT"/ndk-build -C "$APP_ANDROID_ROOT" $* \
        "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt"
fi

# copy assets
for file in "$APP_ANDROID_ROOT"/raw/*
do
if [ -d "$file" ]; then
cp -rf "$file" "$APP_ANDROID_ROOT"/assets
fi

if [ -f "$file" ]; then
cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

# copy sdk res
for file in "$APP_ANDROID_ROOT"/sdk/armeabi-v7a/*
do
if [ -d "$file" ]; then
cp -rf "$file" "$APP_ANDROID_ROOT"/libs/armeabi-v7a
fi

if [ -f "$file" ]; then
cp "$file" "$APP_ANDROID_ROOT"/libs/armeabi-v7a
fi
done


AS_ASSERT="$DIR/../mj_hn_as/zhajinhua/src/main/assets"
AS_LIB="$DIR/../mj_hn_as/zhajinhua/src/main/jniLibs"

rm -rf "$AS_ASSERT"
mkdir "$AS_ASSERT"

# copy resources
for file in "$APP_ROOT"/Resources/*
do
if [ -d "$file" ]; then
cp -rf "$file" "$AS_ASSERT"/
fi

if [ -f "$file" ]; then
cp "$file" "$AS_ASSERT"/
fi
done

####################
cp -rf "$APP_ANDROID_ROOT"/libs/armeabi-v7a "$AS_LIB"
cp "$APP_ROOT"/Resources/config.json "$AS_ASSERT"/config.json
cp -rf "$COCOS2DX_ROOT"/CocosEasy/GCloudVoice/assets/GCloudVoice "$AS_ASSERT"


