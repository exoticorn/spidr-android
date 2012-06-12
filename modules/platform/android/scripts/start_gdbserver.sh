source $(dirname $0)/config.sh

get_pid_of ()
{
    adb shell ps | awk -f $HOME/projects/android/android-ndk-r8/build/awk/extract-pid.awk -v PACKAGE="$1"
}

APPPID=$(get_pid_of $PROJECT_PACKAGE)
if [ $? != 0 -o "$APPPID" = "0" ] ; then
	echo "Failed to get PID of app."
	exit 1
fi

echo "PID: $APPPID"

$ANDROID_SDK/platform-tools/adb shell run-as $PROJECT_PACKAGE lib/gdbserver +debug-socket --attach $APPPID &
if [ $? != 0 ] ; then
	echo "Failed to start dbgserver."
	exit 1
fi

adb forward tcp:1234 localfilesystem:/data/data/$PROJECT_PACKAGE/debug-socket
if [ $? != 0 ] ; then
	echo "Failed to start forwarding."
	exit 1
fi

mkdir -p /tmp/androiddebug

adb pull /system/bin/app_process /tmp/androiddebug/app_process
if [ $? != 0 ] ; then
	echo "Failed to pull app_process."
	exit 1
fi

adb pull /system/lib/libc.so /tmp/androiddebug/libc.so
if [ $? != 0 ] ; then
	echo "Failed to pull app_process."
	exit 1
fi

cp $BUILD_DIR/libexogame.so /tmp/androiddebug/

echo "done. You can now attach gdb."

