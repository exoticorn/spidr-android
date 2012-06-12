package de.exoticorn.gameframework;

import android.opengl.GLSurfaceView;
import android.content.Context;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GameView extends GLSurfaceView
{
	long m_gameFramework = 0;
	
	public GameView(Context context)
	{
		super(context);
		
		Native.loadLib();
		
		m_gameFramework = Native.createGameFramework();
		
		setEGLContextClientVersion(2);
		
		setRenderer(new Renderer());
	}
	
	class Renderer implements GLSurfaceView.Renderer
	{
		int m_width = 0;
		int m_height = 0;
		
		public void onSurfaceCreated(GL10 unused, EGLConfig config)
		{
			Native.surfaceCreated(m_gameFramework);
		}
		
		public void onSurfaceChanged(GL10 unused, int width, int height)
		{
			m_width = width;
			m_height = height;
			Native.setScreenSize(m_gameFramework, width, height);
		}
		
		public void onDrawFrame(GL10 unused)
		{
			Native.update(m_gameFramework);
		}
	}
}