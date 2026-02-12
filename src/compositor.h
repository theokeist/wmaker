/* compositor.h - compositor backend interface */
#ifndef WMCOMPOSITOR_H_
#define WMCOMPOSITOR_H_

#include "WindowMaker.h"

typedef struct {
        int choice;
        const char *name;
        const char *binary;
} WCompositorBackend;

const WCompositorBackend *wCompositorGetBackend(int choice);
const WCompositorBackend *wCompositorGetFallbackBackend(void);
Bool wCompositorBackendAvailable(const WCompositorBackend *backend);
Bool wCompositorBuildLaunchCommand(const WCompositorBackend *backend,
                                   const char *config_argument,
                                   Bool enable_shadows,
                                   char *buffer,
                                   size_t buflen);

#endif
