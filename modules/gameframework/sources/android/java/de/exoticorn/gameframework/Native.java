package de.exoticorn.gameframework;

class Native
{
	static void loadLib() {}
	static native long createGameFramework();
	static native void surfaceCreated(long gameFramework);
	static native void update(long gameFramework);
	static native void setScreenSize(long gameFramework, int width, int height);
	static native void handleTouchEvent(long gameFramework, int id, boolean down, float x, float y);
	static native void fillAudioBuffer(long gameFramework, short[] buffer);
	
	static
	{
		System.loadLibrary("exogame");
	}
}