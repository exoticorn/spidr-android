package de.exoticorn.gameframework;

import android.opengl.GLSurfaceView;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.content.Context;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GameView extends GLSurfaceView
{
	long m_gameFramework = 0;
	boolean m_reHideSystemUi = false;
	
	public GameView(Context context, long gameFramework)
	{
		super(context);
		
		setFocusable(true);
		requestFocus();
		
		m_gameFramework = gameFramework;
		
		setEGLContextClientVersion(2);
		
		setRenderer(new Renderer());

		hideSystemUi();
		
		if(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.HONEYCOMB)
		{
			setOnSystemUiVisibilityChangeListener(new VisibilityChangeListener());
		}
	}
	
	class VisibilityChangeListener implements View.OnSystemUiVisibilityChangeListener
	{
		@Override public void onSystemUiVisibilityChange(int visibility)
		{
			if((visibility & SYSTEM_UI_FLAG_LOW_PROFILE) == 0)
			{
				m_reHideSystemUi = true;
			}
		}
	}	
	
	private void hideSystemUi()
	{
		if(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.HONEYCOMB)
		{
			setSystemUiVisibility(0);
			setSystemUiVisibility(SYSTEM_UI_FLAG_LOW_PROFILE);
		}
		m_reHideSystemUi = false;
	}
	
	@Override public boolean onTouchEvent(MotionEvent event)
	{
		switch(event.getActionMasked())
		{
		case MotionEvent.ACTION_DOWN:
			sendTouchEvent(0, true, event.getX(), event.getY());
			if(m_reHideSystemUi)
			{
				hideSystemUi();
			}
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
	
	@Override public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if(keyCode >= KeyEvent.KEYCODE_BUTTON_A && keyCode < KeyEvent.KEYCODE_BUTTON_A + 16)
		{
			sendGamepadButtonEvent(keyCode - KeyEvent.KEYCODE_BUTTON_A, true);
			return true;
		}
		return false;
	}
	
	@Override public boolean onKeyUp(int keyCode, KeyEvent event)
	{
		if(keyCode >= KeyEvent.KEYCODE_BUTTON_A && keyCode < KeyEvent.KEYCODE_BUTTON_A + 16)
		{
			sendGamepadButtonEvent(keyCode - KeyEvent.KEYCODE_BUTTON_A, false);
			return true;
		}
		return false;
	}
	
	private void sendGamepadButtonEvent(final int button, final boolean down)
	{
		queueEvent(new Runnable() {
			public void run() {
				Native.handleGamepadButtonEvent(m_gameFramework, button, down);
			}
		});
	}
	
	@Override public boolean onGenericMotionEvent(final MotionEvent event)
	{
		if((event.getDevice().getSources() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0)
		{
			queueEvent(new Runnable() {
				public void run() {
					Native.handleGamepadStickEvent(m_gameFramework, event.getAxisValue(MotionEvent.AXIS_X), event.getAxisValue(MotionEvent.AXIS_Y));
				}
			});
			return true;
		}
		return false;
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