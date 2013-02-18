/*
   Copyright (C) 2004-2006 Jean-Marc Valin
   Copyright (C) 2006 Commonwealth Scientific and Industrial Research
                      Organisation (CSIRO) Australia
   
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:

   1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/

#include "alsa_device.h"
#include <stdlib.h>
#include <alsa/asoundlib.h>

struct AlsaDevice_ {
   char *device_name;
   int channels;
   snd_pcm_t *capture_handle;
   snd_pcm_t *playback_handle;
   int readN, writeN;
   struct pollfd *read_fd, *write_fd;
};


AlsaDevice *alsa_device_open( const char *device_name, int channels )
{
   int err;
   snd_pcm_hw_params_t *hw_params;
   static snd_output_t *jcd_out;
   AlsaDevice *dev = malloc(sizeof(*dev));
   if (!dev)
      return NULL;
   dev->device_name = malloc(1+strlen(device_name));
   if (!dev->device_name)
   {
      free(dev);
      return NULL;
   }
   strcpy(dev->device_name, device_name);
   dev->channels = channels;
   err = snd_output_stdio_attach(&jcd_out, stdout, 0);

   //printf( "11111111111111111111111111111111111111\n" );
   if ( ( err = snd_pcm_open ( &dev->capture_handle, dev->device_name, SND_PCM_STREAM_CAPTURE, 0 ) ) < 0 )
   {
      fprintf (stderr, "cannot open audio device %s (%s)\n", dev->device_name, snd_strerror (err));
      rc = 0;
      return NULL;
   }
   else
      rc = 1;
   
   //printf( "222222222222222222222222222222222222222\n" );
   if ( ( err = snd_pcm_hw_params_malloc ( &hw_params ) ) < 0 )
   {
      fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror (err));
   }

   //printf( "33333333333333333333333333333333333333\n" );
   if ((err = snd_pcm_hw_params_any (dev->capture_handle, hw_params)) < 0)
   {
      fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror (err));
   }

   //printf( "44444444444444444444444444444444444444\n" );
   if ( ( err = snd_pcm_hw_params_set_channels( dev->capture_handle, hw_params, channels ) ) < 0 )
   {
      //fprintf( stderr, "cannot set channel count (%s)\n", snd_strerror( err ) );
      rc = 0;
   }
   else
      rc = 1;
   
   snd_pcm_close(dev->capture_handle);
   free(dev->device_name);
   free(dev);
   
   snd_pcm_hw_params_free (hw_params);
   return dev;
}


AlsaDevice *alsa_device_busy( const char *device_name )
{
   int err;
   snd_pcm_hw_params_t *hw_params;
   static snd_output_t *jcd_out;
   AlsaDevice *dev = malloc(sizeof(*dev));
   if (!dev)
      return NULL;
   dev->device_name = malloc(1+strlen(device_name));
   if (!dev->device_name)
   {
      free(dev);
      return NULL;
   }
   strcpy(dev->device_name, device_name);
   err = snd_output_stdio_attach(&jcd_out, stdout, 0);

   //printf( "11111111111111111111111111111111111111\n" );
   if ( ( err = snd_pcm_open ( &dev->capture_handle, dev->device_name, SND_PCM_STREAM_CAPTURE, 0 ) ) < 0 )
   {
      fprintf (stderr, "cannot open audio device %s (%s)\n", dev->device_name, snd_strerror (err));
      rcBusy = 1;
      return NULL;
   }
   else
      rcBusy = 0;
   
   snd_pcm_close(dev->capture_handle);
   free(dev->device_name);
   free(dev);
   
   //snd_pcm_hw_params_free (hw_params);
   return dev;
   
}



void alsa_device_close(AlsaDevice *dev)
{
   snd_pcm_close(dev->capture_handle);
   snd_pcm_close(dev->playback_handle);
   free(dev->device_name);
   free(dev);
}
