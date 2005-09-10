#ifndef MWAVPLAYER_H
#define MWAVPLAYER_H

#include "../../modules/module.h"

#include <sndfile.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

class WavFile {
	public:
		WavFile();
		~WavFile();
		bool Open( const char* filename );
		long Read( float* dest, long numFrames );
		int GetChannelsCount();
        void PrintInfo();
        
	protected:
		SNDFILE* sf;
    	SF_INFO sfInfo;
		long frames, framesLeft;
		long currentFrame;
		int channels;
};

/**
 No description
*/
class MWavPlayer : public Module
{
    typedef boost::mutex::scoped_lock lock;
    
	public:
		MWavPlayer();
		~MWavPlayer();
		virtual void Process();
		virtual void Init();
		REGISTER_MODULE( "Wav Player", MWavPlayer )
		bool Open();
		static void ThreadProcStatic( void* data );
		void ThreadProc();

	protected:
		void Read( float* destBuff, long numFrames );
		void WriteBuffers( );

		ParameterString pFileName;
		Output oOutLeft, oOutRight;
        WavFile wav;
		bool running, isFileOpened;
	    boost::thread* pThread;
        boost::condition buffer_is_empty, buffer_is_ready;
	    boost::mutex monitor, monitorRead;
	    int buffersFree, buffersCount, writeBufferId, readBufferId;
		long currentFrame, bufferSize, framesLeft;
		int channels;
		float** buffers, * readBuffer;
};



#endif // MWAVPLAYER_H
