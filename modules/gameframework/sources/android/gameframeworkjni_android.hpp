#ifndef EXO_GAMEFRAMEWORKJNI_ANDROID_HPP
#define EXO_GAMEFRAMEWORKJNI_ANDROID_HPP

#include <jni.h>

namespace exo
{
	extern "C" {
		JNIEXPORT jlong JNICALL Java_de_exoticorn_gameframework_Native_createGameFramework(JNIEnv* pEnv, jobject self, jstring storagePath);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_surfaceCreated(JNIEnv* pEnv, jobject self, jlong gameFramework);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_update(JNIEnv* pEnv, jobject self, jlong gameFramework);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_onPause(JNIEnv* pEnv, jobject self, jlong gameFramework);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_setScreenSize(JNIEnv* pEnv, jobject self, jlong gameFramework, jint width, jint height);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_handleTouchEvent(JNIEnv* pEnv, jobject self, jlong gameFramework, jint id, jboolean down, jfloat x, jfloat y);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_handleGamepadStickEvent(JNIEnv* pEnv, jobject self, jlong gameFramework, jfloat x, jfloat y);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_handleGamepadButtonEvent(JNIEnv* pEnv, jobject self, jlong gameFramework, jint button, jboolean down);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_fillAudioBuffer(JNIEnv* pEnv, jobject self, jlong gameFramework, jshortArray buffer);
		JNIEXPORT jboolean JNICALL Java_de_exoticorn_gameframework_Native_onBackPressed(JNIEnv* pEnv, jobject self, jlong gameFramework);
	}
}

#endif
