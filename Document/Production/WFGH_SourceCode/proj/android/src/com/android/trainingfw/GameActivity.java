/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.trainingfw;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.view.KeyEvent;
import android.app.AlertDialog;
import android.widget.Toast;
import android.content.DialogInterface;
import android.content.Context;
import java.io.File;
import com.android.trainingfw.MusicSer;

public class GameActivity extends Activity {

	static GameActivity		mInstance = null;
	
    static GameSurfaceView 	mView = null;

    @Override protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        mInstance = this;
        
        mView = new GameSurfaceView(getApplication());
        setContentView(mView);  
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		MusicSer.Load(this,R.raw.bgmusic);
		MusicSer.Play();
    }

    @Override protected void onPause() {
        super.onPause();
		MusicSer.Pause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
		MusicSer.Play();
        mView.onResume();
    }
	
	public void exitGame(String strexit,String exit,Context context){
		AlertDialog.Builder alert=new AlertDialog.Builder(context);
		alert.setTitle(exit+"...");
		alert.setMessage(strexit);
		alert.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface arg0, int arg1) {
				Toast.makeText(getApplicationContext(), "Quit", Toast.LENGTH_SHORT).show();					
				/*finish();*/
				MusicSer.Stop();
				System.exit(0);
			}
		});
		alert.setNegativeButton("No", new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				Toast.makeText(getApplicationContext(), "Cancel", Toast.LENGTH_SHORT).show();
				dialog.cancel();
			}
		});
		alert.show();
	}
    
    public static void quitGame(){
    	System.exit(0);
    }
    
	@Override
	public boolean onKeyDown(int keyCode,KeyEvent event){
		switch(keyCode){
			case KeyEvent.KEYCODE_BACK:
				exitGame("Do you want to exit game?", "Exit", this);
				break;
			case KeyEvent.KEYCODE_HOME:
				exitGame("Do you want to exit game?","Exit",this);
				break;
		}
		return super.onKeyDown(keyCode, event);	
	}
	
	@Override
	protected void onUserLeaveHint(){
		System.exit(0);
	}
	
    static {
        System.loadLibrary("trainingfw");
    }
}
