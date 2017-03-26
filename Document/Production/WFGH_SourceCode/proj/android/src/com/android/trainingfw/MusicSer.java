package com.android.trainingfw;

import android.app.Activity;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.IBinder;

public class MusicSer extends Service{
	
	public static MediaPlayer media,player;
	public static boolean Isplaying=false;
	private static SoundPool sndPool;
	private static float rate = 1.0f;
	private static float leftVolume = 1.0f;
	private static float rightVolume = 1.0f;
	private static int shipexplode, explodeinwater;
	
	public static void Load(Context context,int uri){
		media=MediaPlayer.create(context, uri);
		media.setLooping(true);
		media.setVolume(0.5f, 0.5f);
	}
	
	public static void loadSound(Context context,int sound_id){
		 sndPool = new SoundPool(5, AudioManager.STREAM_MUSIC, 0);

		 /** soundId for Later handling of sound pool **/
		 int soundId = sndPool.load(context, sound_id, 1); // in 2nd param u have to pass your desire ringtone

		 sndPool.play(soundId, 1.0f, 1.0f, 0, 0, 1);

		 player = MediaPlayer.create(context, sound_id); // in 2nd param u have to pass your desire ringtone
		 //mPlayer.prepare();
		 player.start();
	}
	
	public static void Play()
	{
		media.start();
	}
	public static void Pause()
	{
		media.pause();
	}
	public static void Release()
	{
		player.release();
	}
	
	public static void Stop()
	{
		media.stop();
	}
	
	public static void AutoPlayPause()
	{
		if(Isplaying){
		
			if(media.isPlaying())
				Pause();
			else
				Play();
		}
	}
	
	@SuppressWarnings("deprecation")
	public static void LoadAll(Context context)
	{
		sndPool = new SoundPool(16, AudioManager.STREAM_MUSIC, 100);
		/*Activity activity=(Activity)context;
		activity.setVolumeControlStream(AudioManager.STREAM_MUSIC);*/
		//shipexplode = load(R.raw.expoldeship,context);
		//explodeinwater = load(R.raw.explodeinwater,context);
		// clickitem = load(R.raw.jump,appContext);
		// gameover = load(R.raw.thua,appContext);
		// win = load(R.raw.votay,appContext);
		// win = load(R.raw.menuchange,appContext);
		// coin = load(R.raw.coin,appContext);
	}
	public static int load(int sound_id,Context appContext)
	{
		return sndPool.load(appContext, sound_id, 1);
	}
	public static void play(int sound_id,Context context)
	{		
		sndPool.play(sound_id, leftVolume, rightVolume, 1, 0, rate);		
	}
	public static void unloadAll()
	{
		sndPool.release();
	}
	public static void PlaySound(int type,Context context)
	{
		switch (type)
		{
			case 1:
				play(shipexplode,context);
				break;
			case 2:
				play(explodeinwater,context);
				break;
		}
	}

	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		return null;
	}
}
