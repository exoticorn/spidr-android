package de.exoticorn.gameframework;

import android.app.Activity;
import android.os.Bundle;

abstract public class GameActivity extends Activity
{
	GameView m_view;
	
	@Override protected void onCreate(Bundle icicle)
	{
		super.onCreate(icicle);
		
		m_view = new GameView(getApplication());
		setContentView(m_view);
	}

	@Override protected void onPause()
	{
		super.onPause();
		m_view.onPause();
	}
	
	@Override protected void onResume()
	{
		super.onResume();
		m_view.onResume();
	}	
}