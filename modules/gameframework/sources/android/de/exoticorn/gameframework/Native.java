package de.exoticorn.gameframework;

class Native
{
	static void loadLib() {}
	static native long createGameFramework();
	static native void surfaceCreated(long gameFramework);
	static native void update(long gameFramework);
	static native void setScreenSize(long gameFramework, int width, int height);
	
	static
	{
		System.loadLibrary("exogame");
	}
}