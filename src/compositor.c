/* compositor.c - compositor backend interface */

#include "wconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "compositor.h"

static const WCompositorBackend backends[] = {
        { WCOMPOSITOR_NONE, "none", NULL },
        { WCOMPOSITOR_PICOM, "picom", "picom" },
        { WCOMPOSITOR_COMPTON, "compton", "compton" },
        { WCOMPOSITOR_XCOMPMGR, "xcompmgr", "xcompmgr" },
        { WCOMPOSITOR_COMPIZ, "compiz", "compiz" },
        { -1, NULL, NULL }
};

const WCompositorBackend *wCompositorGetBackend(int choice)
{
        int i;

        for (i = 0; backends[i].name != NULL; i++) {
                if (backends[i].choice == choice)
                        return &backends[i];
        }

        return NULL;
}

const WCompositorBackend *wCompositorGetFallbackBackend(void)
{
        return &backends[0];
}

Bool wCompositorBackendAvailable(const WCompositorBackend *backend)
{
        const char *path_env;
        char *paths;
        char *token;
        Bool result = False;

        if (!backend)
                return False;

        if (!backend->binary || !backend->binary[0])
                return True;

        if (strchr(backend->binary, '/'))
                return access(backend->binary, X_OK) == 0;

        path_env = getenv("PATH");
        if (!path_env || !path_env[0])
                return False;

        paths = wstrdup(path_env);
        for (token = strtok(paths, ":"); token; token = strtok(NULL, ":")) {
                const char *dir = (*token) ? token : ".";
                size_t len = strlen(dir) + strlen(backend->binary) + 2;
                char *candidate = wmalloc(len);

                snprintf(candidate, len, "%s/%s", dir, backend->binary);
                if (access(candidate, X_OK) == 0) {
                        result = True;
                        wfree(candidate);
                        break;
                }

                wfree(candidate);
        }

        wfree(paths);
        return result;
}

Bool wCompositorBuildLaunchCommand(const WCompositorBackend *backend,
                                   const char *config_argument,
                                   Bool enable_shadows,
                                   char *buffer,
                                   size_t buflen)
{
        if (!buffer || buflen == 0)
                return False;

        buffer[0] = '\0';

        if (!backend || backend->choice == WCOMPOSITOR_NONE)
                return True;

        if (backend->choice == WCOMPOSITOR_PICOM) {
                if (config_argument && config_argument[0]) {
                        snprintf(buffer, buflen,
                                 "%s --backend glx --config %s --experimental-backends --animations%s",
                                 backend->binary, config_argument,
                                 enable_shadows ? " --shadow" : "");
                } else {
                        snprintf(buffer, buflen,
                                 "%s --backend glx --experimental-backends --animations%s",
                                 backend->binary,
                                 enable_shadows ? " --shadow" : "");
                }
                return True;
        }

        if (backend->choice == WCOMPOSITOR_COMPTON) {
                if (config_argument && config_argument[0]) {
                        snprintf(buffer, buflen,
                                 "%s --backend glx --config %s%s",
                                 backend->binary, config_argument,
                                 enable_shadows ? " --shadow" : "");
                } else {
                        snprintf(buffer, buflen,
                                 "%s --backend glx%s",
                                 backend->binary,
                                 enable_shadows ? " --shadow" : "");
                }
                return True;
        }

        if (backend->choice == WCOMPOSITOR_XCOMPMGR) {
                snprintf(buffer, buflen,
                         "%s%s",
                         backend->binary,
                         enable_shadows ? " -c" : "");
                return True;
        }

        if (backend->choice == WCOMPOSITOR_COMPIZ) {
                snprintf(buffer, buflen, "%s --replace ccp", backend->binary);
                return True;
        }

        return False;
}
