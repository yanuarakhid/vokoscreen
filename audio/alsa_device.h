#ifndef ALSA_DEVICE_H 
#define ALSA_DEVICE_H

#include <sys/poll.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct AlsaDevice_;

typedef struct AlsaDevice_ AlsaDevice;

AlsaDevice *alsa_device_open(const char *device_name, int channels );
AlsaDevice *alsa_device_busy( const char *device_name );
void alsa_device_close( AlsaDevice *dev );

AlsaDevice *alsa_device_sample( const char *device_name, unsigned int rate );


int rc;
int rcBusy;
int rcSampleRate;

#ifdef __cplusplus
}
#endif

#endif
