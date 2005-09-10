//#include <boost/utility.hpp>
//#include <boost/thread/condition.hpp>


#include <boost/thread/xtime.hpp>
#include <boost/bind.hpp>

#include "mwavplayer.h"

MWavPlayer::MWavPlayer() :
   	Module( "New WAV Player" ),
	pFileName( "Nazwa pliku" ),
 	oOutLeft( "output left" ),
 	oOutRight( "output right" )
{
	AddParameter( pFileName );
	AddOutput( oOutLeft );
	AddOutput( oOutRight );

	pFileName.SetText( "" );

	buffersCount = 2;
	running = true;
	currentFrame = 0;
	buffersFree = 0;
	readBufferId = writeBufferId = 0;
	channels = 0;
	isFileOpened = false;
	
	buffers = new float*[ buffersCount ];
	
	for( int i = 0; i < buffersCount; i++ ) {
		buffers[i] = NULL;
	}
	
	readBuffer = NULL;
}

MWavPlayer::~MWavPlayer() {
	// insert your code here

	running = false;
	buffersFree++;

	buffer_is_empty.notify_one();

	// TODO: jak dobrze zamknac watek czytania???
	//pThread->join();
	boost::xtime stop;
	boost::xtime_get( &stop, boost::TIME_UTC );
	stop.nsec += 100000000;
	boost::thread::sleep( stop );

	delete pThread;
	
	for( int i = 0; i < buffersCount; i++ ) {
		delete []buffers[i];
	}
	delete []buffers;
	delete []readBuffer;
	
	TRACE( "MWavPlayer::~MWavPlayer() - Sukces\n" );
}

void MWavPlayer::Init() {
	cout << "MWavPlayer::Init" << endl;
 	Open();
	pThread = new boost::thread ( boost::bind( &MWavPlayer::ThreadProcStatic, this ) );
}

bool MWavPlayer::Open() {
	TRACE( "MWavPlayer::Open - Openig '%s'\n", pFileName.GetText().c_str() );

	isFileOpened = false;

	if( pFileName.GetText() == "" ) {
		TRACE( "MWavPlayer::Open - Nie wybrano pliku\n" );
		return false;
	}

	if( !wav.Open( pFileName.GetText().c_str() ) )
	    return false;
	    
	wav.PrintInfo();
	channels = wav.GetChannelsCount();
	
	bufferSize = (long) Module::sampleRate * 2; // 2 sekundy
	
	TRACE( "MWavPlayer::Open - channels = %i\n", channels );
	TRACE( "MWavPlayer::Open - bufferSize = %i\n", bufferSize );

	// sprzatanie
	for( int i = 0; i < buffersCount; i++ ) {
		delete []buffers[i];
		buffers[i] = NULL;
	}
	delete []buffers;
	buffers = NULL;

	// prefetch
	buffers = new float*[ buffersCount ];
	for( int i = 0; i < buffersCount; i++ ) {
		buffers[i] = new float[ channels * bufferSize ];
		wav.Read( buffers[i], bufferSize );
	}
	
	delete []readBuffer;
	readBuffer = new float[ channels * Module::framesPerBlock ];

	framesLeft = bufferSize;
	currentFrame = buffersFree = 0;
	writeBufferId = readBufferId = 0;
	isFileOpened = true;
	
	return true;
}

void MWavPlayer::Process() {
	if( isFileOpened ) {
  		float* outL = oOutLeft.GetSignal();
		float* outR = oOutRight.GetSignal();

		// czytanie z bufora
		Read( readBuffer, Module::framesPerBlock );

		if( channels == 1 ) {
			for (unsigned long i = 0; i < Module::framesPerBlock; i++) {
				*outL++ = readBuffer[ i * channels ];
				//*outR++ = readBuffer[ i * channels + channels - 1 ];
			}
		}

   		if( channels == 2 ) {
			for (unsigned long i = 0; i < Module::framesPerBlock; i++) {
				*outL++ = readBuffer[ i * channels ];
				*outR++ = readBuffer[ i * channels + 1 ];
			}
		}
	}
}

void MWavPlayer::Read( float* destBuff, long numFrames ) {
	lock lk( monitor );

	while( buffersFree == buffersCount ) {
		cout << "!!! MWavPlayer::Read - nie moge czytac - brak pelnych buforow, czekam..." << endl;
	    buffer_is_ready.wait( lk );
	}
	
	if( numFrames > framesLeft ) {
		//TRACE( "nie zmiesci sie     numFrames = %i, frmesLeft = %i\n", numFrames, framesLeft )  ;

		long fristPartFrames = bufferSize - currentFrame;
		
		memcpy( destBuff, buffers[ readBufferId ] + channels * currentFrame,
			sizeof(float) * channels * framesLeft );
			
		framesLeft -= fristPartFrames;
		++buffersFree;

		buffer_is_empty.notify_one(); // obudzic watek czytania

		while( buffersFree == buffersCount ) {
			cout << "!!! MWavPlayer::Read - nie moge czytac - brak pelnych bufforow, czekam...." << endl;
		    buffer_is_ready.wait( lk );
		}

		// przlaczmy bufor bo jakis jest dostepny
 		++readBufferId;
		readBufferId = (readBufferId) % buffersCount;
		framesLeft = bufferSize;

  		long toRead = numFrames - fristPartFrames;
		destBuff = destBuff + ( channels * ( fristPartFrames ) );

		memcpy( destBuff, buffers[ readBufferId ], sizeof(float) * channels * toRead );
		framesLeft -= toRead;
		currentFrame = toRead;
	}
	else {
		memcpy( destBuff, buffers[ readBufferId ] + currentFrame * channels,
			sizeof(float) * channels * numFrames );
		framesLeft -= numFrames;
		currentFrame = currentFrame + numFrames;
	}
}

void MWavPlayer::ThreadProcStatic( void* data ) {
    MWavPlayer* pThis = (MWavPlayer*) data;
	pThis->ThreadProc();
	cout << "MWavPlayer::ThreadProcStatic ended" << endl;
	return;
}

void MWavPlayer::ThreadProc() {
	while( running ) {
		lock lk( monitor );

        while ( buffersFree == 0 ) {
            buffer_is_empty.wait( lk );
		}

		if( running ) {
			long read = wav.Read( buffers[ writeBufferId ], bufferSize );

			if( read != bufferSize )
				TRACE( "!!!! MWavPlayer::ThreadProc - Wczytalem tylko %i\n", read );
				
			--buffersFree;
			++writeBufferId;
			writeBufferId = writeBufferId % buffersCount;
			buffer_is_ready.notify_one();
		}
	}

	cout << "MWavPlayer::ThreadProc - Koniec" << endl;
	
	return;
}

//----------------------------------------------------------------------
WavFile::WavFile () {
	frames = 0;
	currentFrame = 0;
	framesLeft = 0;
	channels = 0;
}

WavFile::~WavFile () {
	sf_close(sf);
	TRACE( "WavFile::~WavFile - Sukces\n" );
}

bool WavFile::Open( const char* filename ) {

	sfInfo.format = 0;
	sf = sf_open( filename, SFM_READ, &sfInfo );
	
	if( sf == 0 )
	    return false;
	
	channels = sfInfo.channels;
	frames = sfInfo.frames;
	framesLeft = frames;
	
	return true;
}

long WavFile::Read( float* dest, long numFrames ) {
	long toRead = numFrames;
	long framesRead = 0;

	while( toRead != 0 ) {
        // chcemy wiecej ramek niz zostalo do konca pliku
		if ( toRead > framesLeft ) {
   			long read = sf_readf_float( sf, dest, framesLeft ); // wczytujemy to co jest
			dest = dest + channels * read; // przesuwamy sie w buforze docelowym
			toRead = toRead - read;
			framesRead += read;

			sf_seek(sf, 0, SEEK_SET); // na poczatek WAV'a
			framesLeft = frames;

			cout << "Warping file..." << endl;
		}
		else {
			//currentFrame += toRead;
			long read = sf_readf_float( sf, dest, toRead );
			framesLeft = framesLeft - read;
			toRead = toRead - read;
			dest = dest + channels * read;
			framesRead += read;
		}
	}

	if ( framesRead != numFrames ) cout << "WavFile::Read - wczytano tylko " << framesRead << endl;

	return framesRead;
}

int WavFile::GetChannelsCount() {
	return sfInfo.channels;
}

void WavFile::PrintInfo() {
	cout << "frames: " << sfInfo.frames << endl
    << "samplerate: " << sfInfo.samplerate << endl
    << "channels: " << sfInfo.channels << endl
    << "format: " << hex << sfInfo.format << dec << endl
    << "sections: " << sfInfo.sections << endl
    << "seekable: " << sfInfo.seekable << endl;
}
