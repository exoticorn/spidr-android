#include "gameframeworkjni_android.hpp"
#include "exo/gameframework/gameframework.hpp"

namespace exo
{
	jlong Java_de_exoticorn_gameframework_Native_createGameFramework(JNIEnv* pEnv, jobject self)
	{
		EXO_USE_PARAMETER(pEnv);
		EXO_USE_PARAMETER(self);
		return reinterpret_cast<jlong>(new GameFrameworkAndroid());
	}

	void Java_de_exoticorn_gameframework_Native_surfaceCreated(JNIEnv* pEnv, jobject self, jlong gameFramework)
	{
		EXO_USE_PARAMETER(pEnv);
		EXO_USE_PARAMETER(self);
		reinterpret_cast<GameFrameworkAndroid*>(gameFramework)->surfaceCreated();
	}

	void Java_de_exoticorn_gameframework_Native_update(JNIEnv* pEnv, jobject self, jlong gameFramework)
	{
		EXO_USE_PARAMETER(pEnv);
		EXO_USE_PARAMETER(self);
		reinterpret_cast<GameFrameworkAndroid*>(gameFramework)->update();
	}

	void Java_de_exoticorn_gameframework_Native_setScreenSize(JNIEnv* pEnv, jobject self, jlong gameFramework, jint width, jint height)
	{
		EXO_USE_PARAMETER(pEnv);
		EXO_USE_PARAMETER(self);
		reinterpret_cast<GameFrameworkAndroid*>(gameFramework)->setScreenSize((uint)width, (uint)height);
	}

	void Java_de_exoticorn_gameframework_Native_handleTouchEvent(JNIEnv* pEnv, jobject self, jlong gameFramework, jint id, jboolean down, jfloat x, jfloat y)
	{
		EXO_USE_PARAMETER(pEnv);
		EXO_USE_PARAMETER(self);
		reinterpret_cast<GameFrameworkAndroid*>(gameFramework)->handleTouchInput(id, down, x, y);
	}
}
