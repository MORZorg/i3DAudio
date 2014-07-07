/*
 * Main test file for i3DAudio
 * Loads a file and applies an arbitrary variating hrtf
 * Author: Mattia Rizzini
 * Date: 2014-07-07
 */
#include "hrtf.h"
#include "SDL_config.h"

#include <stdio.h>
#include <stdlib.h>

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "SDL.h"
#include "SDL_audio.h"

struct
{
    SDL_AudioSpec spec;
    Uint8 *sound;               /* Pointer to wave data */
    Uint32 soundlen;            /* Length of wave data */
    int soundpos;               /* Current play position */
} wave;

static void quit( int rc )
{
    SDL_Quit();
    exit(rc);
}

void SDLCALL fillerup( void *unused, Uint8 * stream, int len )
{
    Uint8 *waveptr;
    int waveleft;

    // Set up the pointers
    waveptr = wave.sound + wave.soundpos;
    waveleft = wave.soundlen - wave.soundpos;

    // Go!
    while( waveleft <= len ) {
        SDL_memcpy(stream, waveptr, waveleft);
        stream += waveleft;
        len -= waveleft;
        waveptr = wave.sound;
        waveleft = wave.soundlen;
        wave.soundpos = 0;
    }

    SDL_memcpy( stream, waveptr, len );
    wave.soundpos += len;
}

static int done = 0;

void poked( int sig )
{
    done = 1;
}

int main( int argc, char** argv )
{
    int i;
    char filename[4096];

	// Enable standard application logging
	SDL_LogSetPriority( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO );

    // Load the SDL library
    if( SDL_Init(SDL_INIT_AUDIO) < 0 )
	{
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError() );
        return 1;
    }

    if( argc > 1 )
        SDL_strlcpy( filename, argv[ 1 ], sizeof( filename ) );
    else
        SDL_strlcpy( filename, "../samples/the_lord_has_spoken.wav", sizeof( filename ) );

    // Load the wave file into memory
    if( SDL_LoadWAV( filename, &wave.spec, &wave.sound, &wave.soundlen ) == NULL )
	{
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't load %s: %s\n", filename, SDL_GetError() );
        quit( 1 );
    }

    wave.spec.callback = fillerup;
#if HAVE_SIGNAL_H
    /* Set the signals */
#ifdef SIGHUP
    signal(SIGHUP, poked);
#endif
    signal(SIGINT, poked);
#ifdef SIGQUIT
    signal(SIGQUIT, poked);
#endif
    signal(SIGTERM, poked);
#endif

    SDL_Log( "Available audio drivers:" );

    for( i = 0; i < SDL_GetNumAudioDrivers(); ++i )
		SDL_Log( "%i: %s", i, SDL_GetAudioDriver( i ) );

    // Initialize fillerup() variables
    if( SDL_OpenAudio(&wave.spec, NULL) < 0 )
	{
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't open audio: %s\n", SDL_GetError() );
        SDL_FreeWAV( wave.sound );
        quit( 2 );
    }

    SDL_Log( "Using audio driver: %s\n", SDL_GetCurrentAudioDriver() );

    // Let the audio run
    SDL_PauseAudio( 0 );

    while( !done && ( SDL_GetAudioStatus() == SDL_AUDIO_PLAYING ) )
        SDL_Delay( 1000 );

    // Clean up on signal
    SDL_CloseAudio();
    SDL_FreeWAV( wave.sound );
    SDL_Quit();

    return 0;
}
