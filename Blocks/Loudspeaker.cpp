#include "Loudspeaker.h"

//// struktura przekazywana do rtsoundCallback()
//typedef struct
//{
// Generator *gen;
//   Amplifier *amp;
//   CConnector *conn;
//}
//rtsoundData;
//
////-----------------------------------------------------------------------------
//int rtsoundCallback(char *outputBuffer, int bufferSize, void *userData)
//{
//  rtsoundData *data = (rtsoundData*)userData;
//  
//  float *out = (float*)outputBuffer;
//    float *connOut;
//    int i;
//    
//     // przetwarzanie 
//    data->gen->Process();
//  data->amp->Process();
// 
//  // zapis do bufora karty
//    connOut = data->conn->Out();
//    for( i = 0; i < bufferSize; i++ )
//    {
//      *out++ = *connOut;      /* lewy  */
//        *out++ = *connOut++;   /* prawy */
//    }
//    return 0;
//}

Loudspeaker::Loudspeaker()
{
//#if defined(__WINDOWS_DS__)
//   int channels = 2;
// int sampleRate = 44100;
//   int bufferSize = FRAMES_PER_BUFFER;
//   int nBuffers = 24;      // number of internal buffers used by device
//  int device = 0;         // 0 indicates the default or first available device
//  char input;
//   RtAudio *audio;     
//    rtsoundData data;
//    
//    // struktura przekazywana do rtsoundCallback()
//    data.gen = gen;
//    data.amp = amp;
//    data.conn = conn2; // gdzie wyjscie calego systemu
//    
// // Open a stream during RtAudio instantiation
// try
//   {
//     audio = new RtAudio(device, channels, 0, 0, RTAUDIO_FLOAT32,
//                        sampleRate, &bufferSize, nBuffers);
// }
// catch (RtError &error)
//    {
//     error.printMessage();
//     exit(EXIT_FAILURE);
//   }
//
//   try
//   {
//     // Set the stream callback function
//       audio->setStreamCallback(&rtsoundCallback, (void *)&data);
//
//      // Start the stream
//       audio->startStream();
// }
// catch (RtError &error)
//    {
//     error.printMessage();
//     goto cleanup;
// }
//
//   cout << "\nPlaying ... press <enter> to quit.\n";
// cin.get(input);
//
// try
//   {
//     // Stop and close the stream
//      audio->stopStream();
//      audio->closeStream();
// }
// catch (RtError &error)
//    {
//     error.printMessage();
// }
//
//   cleanup:
//      delete audio;
//#endif
//
//   return 0;
}
Loudspeaker::~Loudspeaker()
{}
