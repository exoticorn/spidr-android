#ifndef EXO_GAMEFRAMEWORKJNI_ANDROID_HPP
#define EXO_GAMEFRAMEWORKJNI_ANDROID_HPP

#include <jni.h>

namespace exo
{
	extern "C" {
		JNIEXPORT jlong JNICALL Java_de_exoticorn_gameframework_Native_createGameFramework(JNIEnv* pEnv, jobject self);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_surfaceCreated(JNIEnv* pEnv, jobject self, jlong gameFramework);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_update(JNIEnv* pEnv, jobject self, jlong gameFramework);
		JNIEXPORT void JNICALL Java_de_exoticorn_gameframework_Native_setScreenSize(JNIEnv* pEnv, jobject self, jlong gameFramework, jint width, jint height);
	}
}

#endif
