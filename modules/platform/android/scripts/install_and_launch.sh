#!/bin/bash

source $(dirname $0)/config.sh

echo "Installing apk"
adb install -r $BUILD_DIR/$PROJECT_NAME.apk
if [ $? != 0 ] ; then
	echo "Failed to install app."
	exit 1
fi

echo "Starting app"
adb shell am start $@ -n $PROJECT_PACKAGE/.$PROJECT_ACTIVITY
if [ $? != 0 ] ; then
	echo "Failed to start app."
	exit 1
fi

adb shell sleep 4
