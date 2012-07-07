package de.exoticorn.gameframework;

import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.content.Context;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GameView extends GLSurfaceView
{
	long m_gameFramework = 0;
	
	public GameView(Context context, long gameFramework)
	{
		super(context);
		
		m_gameFramework = gameFramework;
		
		setEGLContextClientVersion(2);
		
		setRenderer(new Renderer());
	}
	
	@Override public boolean onTouchEvent(MotionEvent event)
	{
		switch(event.getActionMasked())
		{
		case MotionEvent.ACTION_DOWN:
			sendTouchEvent(0, true, event.getX(), event.getY());
			break;
		case MotionEvent.ACTION_MOVE:
			sendTouchEvent(0, true, event.getX(), event.getY());
			break;
		case MotionEvent.ACTION_UP:
			sendTouchEvent(0, false, event.getX(), event.getY());
			break;
		}
		return true;
	}
	
	private void sendTouchEvent(final int id, final boolean down, final float x, final float y)
	{
		queueEvent(new Runnable() {
			public void run() {
				Native.handleTouchEvent(m_gameFramework, id, down, x, y);
			}
		});
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