#include "gameframeworkjni_android.hpp"
#include "exo/gameframework/gameframework.hpp"
#include "exo/gameframework/applicationbase.hpp"
#include "exo/base/functions.hpp"

namespace exo
{
	jlong Java_de_exoticorn_gameframework_Native_createGameFramework(JNIEnv* pEnv, jobject self, jstring storagePath)
	{
		EXO_USE_PARAMETER(self);

		const char* pStoragePath = pEnv->GetStringUTFChars(storagePath, nullptr);
		jlong gameFramework = reinterpret_cast<jlong>(new GameFrameworkAndroid(pStoragePath));
		pEnv->ReleaseStringUTFChars(storagePath, pStoragePath);

		return gameFramework;
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

	void Java_de_exoticorn_gameframework_Native_onPause(JNIEnv* pEnv, jobject self, jlong gameFramework)
	{
		EXO_USE_PARAMETER(pEnv);
		EXO_USE_PARAMETER(self);
		reinterpret_cast<GameFrameworkAndroid*>(gameFramework)->getApplication()->onPause();
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

	void Java_de_exoticorn_gameframework_Native_handleGamepadStickEvent(JNIEnv* pEnv, jobject self, jlong gameFramework, jfloat x, jfloat y)
	{
		EXO_USE_PARAMETER(pEnv);
		EXO_USE_PARAMETER(self);
		reinterpret_cast<GameFrameworkAndroid*>(gameFramework)->handleGamepadStickEvent(x, y);
	}

	void Java_de_exoticorn_gameframework_Native_handleGamepadButtonEvent(JNIEnv* pEnv, jobject self, jlong gameFramework, jint button, jboolean down)
	{
		EXO_USE_PARAMETER(pEnv);
		EXO_USE_PARAMETER(self);
		reinterpret_cast<GameFrameworkAndroid*>(gameFramework)->handleGamepadButtonEvent(button, down);
	}

	void Java_de_exoticorn_gameframework_Native_fillAudioBuffer(JNIEnv* pEnv, jobject self, jlong gameFramework, jshortArray buffer)
	{
		EXO_USE_PARAMETER(self);
		GameFrameworkAndroid* pGameFramework = reinterpret_cast<GameFrameworkAndroid*>(gameFramework);
		sint16 cBuffer[1024*2];
		int numSamples = pEnv->GetArrayLength(buffer) / 2;
		int pos = 0;
		while(pos < numSamples)
		{
			int batchSize = min(numSamples - pos, 1024);
			pGameFramework->getApplication()->fillAudioBuffer(cBuffer, (uint)batchSize);
			pEnv->SetShortArrayRegion(buffer, pos, batchSize * 2, cBuffer);
			pos += batchSize * 2;
		}
	}

	jboolean Java_de_exoticorn_gameframework_Native_onBackPressed(JNIEnv* pEnv, jobject self, jlong gameFramework)
	{
		EXO_USE_PARAMETER(self);
		EXO_USE_PARAMETER(pEnv);
		return reinterpret_cast<GameFrameworkAndroid*>(gameFramework)->getApplication()->onBackPressed();
	}
}
