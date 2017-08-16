#使用方式如下
#sh release_build.sh versionCode(100000000) versionName(1.0.0)

#判断传入参数
T=$1
V=$2
if [ -z $T ]; then
echo "versionCode参数为空"
echo "请指定versionCode和versionName参数,如sh release_build.sh 100000000 1.0.0"
exit 1
fi
if [ -z $V ]; then
echo "versionName参数为空"
echo "请指定versionCode和versionName参数,如sh release_build.sh 100000000 1.0.0"
exit 1
fi

#执行脚本
./build_native.sh
android update project -p ./ --subprojects --target android-10
android update project -p ../../../../CocosEasy/Plugin/android/PluginLibs/ --subprojects --target android-10
android update project -p ../../../../CocosEasy/SDKEasy/android/SDKEasyLibs/ --subprojects --target android-10
android update project -p ../../../../cocos2dx/platform/android/java/ --subprojects --target android-10
ant clean
ant release -DversionCode=$1 -DversionName=$2
cd bin/
#adb install -r client-release.apk
