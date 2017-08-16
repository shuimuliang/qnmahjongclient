sh build_native.sh NDK_DEBUG=1 V=1
android update project -p ./ --subprojects --target android-10
android update project -p ../../../../CocosEasy/Plugin/android/PluginLibs/ --subprojects --target android-10
android update project -p ../../../../CocosEasy/SDKEasy/android/SDKEasyLibs/ --subprojects --target android-10
android update project -p ../../../../cocos2dx/platform/android/java/ --subprojects --target android-10
ant clean
ant debug
cd bin/
adb install -r client-debug-unaligned.apk
