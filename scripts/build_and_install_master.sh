source $(dirname $0)/config.sh
source $SCRIPT_DIR/config.sh

if ! lace/bin/lace -rb android/master $PROJECT_DIR/project.lace; then exit 1; fi

if ! jarsigner -verbose -sigalg MD5withRSA -digestalg SHA1 -keystore android-release.keystore $PROJECT_DIR/build/android/master/spidr.apk release; then exit 1; fi

if ! $ANDROID_SDK/tools/zipalign -f -v 4 $PROJECT_DIR/build/android/master/spidr.apk $PROJECT_DIR/spidr.apk; then exit 1; fi

adb install -r $PROJECT_DIR/spidr.apk
