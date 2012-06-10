source $(dirname $0)/config.sh

echo "set solib-search-path /tmp/androiddebug" > /tmp/androiddebug/gdbinit
echo "file /tmp/androiddebug/app_process" >> /tmp/androiddebug/gdbinit
echo "target remote :1234" >> /tmp/androiddebug/gdbinit
echo "break keen::Debug::triggerBreakPoint" >> /tmp/androiddebug/gdbinit
echo "c" >> /tmp/androiddebug/gdbinit
$ANDROID_NDK/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-gdb -x /tmp/androiddebug/gdbinit
if [ $? != 0 ] ; then
	echo "Failed to start gdb."
	exit 1
fi
