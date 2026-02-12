#include "EffectsInternal.h"
#include "config.h"

#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

#include <WINGs/WUtil.h>

#ifndef PATH_MAX
#define PATH_MAX DEFAULT_PATH_MAX
#endif

static Bool command_exists(const char *binary)
{
        const char *path_env;
        char *paths;
        char *token;
        Bool result = False;

        if (!binary || !*binary)
                return False;

        if (strchr(binary, '/'))
                return access(binary, X_OK) == 0;

        path_env = getenv("PATH");
        if (!path_env || !*path_env)
                return False;

        paths = wstrdup(path_env);
        for (token = strtok(paths, ":"); token; token = strtok(NULL, ":")) {
                const char *dir = (*token) ? token : ".";
                size_t len = strlen(dir) + strlen(binary) + 2;
                char *candidate = wmalloc(len);

                snprintf(candidate, len, "%s/%s", dir, binary);
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

static Bool process_running(const char *name)
{
        char cmd[64];

        if (!name || !*name)
                return False;

        snprintf(cmd, sizeof(cmd), "pgrep -x '%s' >/dev/null 2>&1", name);
        return system(cmd) == 0;
}

static Bool compositor_index_is_none(int index)
{
        return (index < 0 || !wPrefsCompositorOptions[index].db_value
                || strcasecmp(wPrefsCompositorOptions[index].db_value, "None") == 0);
}

static Bool compositor_index_is_picom(int index)
{
        return (index >= 0 && wPrefsCompositorOptions[index].db_value
                && strcasecmp(wPrefsCompositorOptions[index].db_value, "Picom") == 0);
}

static Bool compositor_index_supports_config(int index)
{
        return compositor_index_is_picom(index)
                || (index >= 0 && wPrefsCompositorOptions[index].db_value
                    && strcasecmp(wPrefsCompositorOptions[index].db_value, "Compton") == 0);
}


static const char *compositor_template_name_for_index(int index)
{
        if (compositor_index_is_picom(index))
                return "picom.conf";
        if (index >= 0 && wPrefsCompositorOptions[index].db_value
            && strcasecmp(wPrefsCompositorOptions[index].db_value, "Compton") == 0)
                return "compton.conf";

        return NULL;
}

static const char *compositor_binary_for_index(int index)
{
        if (compositor_index_is_picom(index))
                return "picom";
        if (index >= 0 && wPrefsCompositorOptions[index].db_value
            && strcasecmp(wPrefsCompositorOptions[index].db_value, "Compton") == 0)
                return "compton";
        if (index >= 0 && wPrefsCompositorOptions[index].db_value
            && strcasecmp(wPrefsCompositorOptions[index].db_value, "Xcompmgr") == 0)
                return "xcompmgr";
        if (index >= 0 && wPrefsCompositorOptions[index].db_value
            && strcasecmp(wPrefsCompositorOptions[index].db_value, "Compiz") == 0)
                return "compiz";

        return NULL;
}

static char *quote_for_shell(const char *path)
{
        const char *src;
        size_t len = 2;
        char *buffer;
        char *dst;

        if (!path)
                return NULL;

        for (src = path; *src; src++) {
                if (*src == '\'')
                        len += 4;
                else
                        len++;
        }

        buffer = wmalloc(len + 1);
        dst = buffer;
        *dst++ = '\'';
        for (src = path; *src; src++) {
                if (*src == '\'') {
                        memcpy(dst, "'\\''", 4);
                        dst += 4;
                } else {
                        *dst++ = *src;
                }
        }
        *dst++ = '\'';
        *dst = '\0';

        return buffer;
}

static int ensure_config_template(int index, const char *path)
{
        struct stat st;
        char *dircopy;
        char *dirpath;
        char template_path[PATH_MAX];
        FILE *in, *out;
        int c;

        if (!path || !compositor_index_supports_config(index))
                return -1;

        if (stat(path, &st) == 0)
                return 0;

        dircopy = wstrdup(path);
        dirpath = dirname(dircopy);
        if (dirpath && dirpath[0])
                wmkdirhier(dirpath);
        wfree(dircopy);

        {
                const char *template_name = compositor_template_name_for_index(index);

                if (!template_name)
                        return -1;

                snprintf(template_path, sizeof(template_path), "%s/Compositors/%s",
                         WMAKER_RESOURCE_PATH, template_name);
        }

        in = fopen(template_path, "r");
        if (!in) {
                wwarning(_("could not open compositor template %s: %s"), template_path, strerror(errno));
                out = fopen(path, "w");
                if (!out)
                        return -1;
                fprintf(out, "# Window Maker compositor configuration placeholder\n");
                fclose(out);
                return 0;
        }

        out = fopen(path, "w");
        if (!out) {
                int saved = errno;
                fclose(in);
                wwarning(_("could not write %s: %s"), path, strerror(saved));
                return -1;
        }

        while ((c = fgetc(in)) != EOF)
                fputc(c, out);

        fclose(in);
        fclose(out);
        return 0;
}

static void launch_editor_for_config(_Panel *panel)
{
        char *expanded;
        char *quoted;
        const char *editor;
        char command[PATH_MAX * 2];
        pid_t pid;

        if (!compositor_index_supports_config(panel->compositorIndex) || !panel->configPath)
                return;

        expanded = wexpandpath(panel->configPath);
        if (!expanded)
                return;

        if (ensure_config_template(panel->compositorIndex, expanded) != 0) {
                wfree(expanded);
                return;
        }

        quoted = quote_for_shell(expanded);
        editor = getenv("VISUAL");
        if (!editor || !editor[0])
                editor = getenv("EDITOR");
#ifdef __APPLE__
        if ((!editor || !editor[0]) && command_exists("open"))
                editor = "open -e";
#endif
        if (!editor || !editor[0]) {
                if (command_exists("xdg-open"))
                        editor = "xdg-open";
                else if (command_exists("sensible-editor"))
                        editor = "sensible-editor";
                else
                        editor = "xterm -e vi";
        }

        snprintf(command, sizeof(command), "%s %s", editor, quoted ? quoted : expanded);

        pid = fork();
        if (pid == 0) {
                execl("/bin/sh", "sh", "-c", command, NULL);
                _exit(111);
        }

        if (quoted)
                wfree(quoted);
        wfree(expanded);
}

void wPrefsEffectsSetConfigPath(_Panel *panel, const char *path)
{
        if (panel->configPath) {
                wfree(panel->configPath);
                panel->configPath = NULL;
        }

        if (path)
                panel->configPath = wstrdup(path);
}

void wPrefsEffectsSyncCompositorControls(_Panel *panel)
{
        Bool hasCompositor = !compositor_index_is_none(panel->compositorIndex);

        if (panel->shadowB)
                WMSetButtonEnabled(panel->shadowB, hasCompositor);

        if (panel->autostartB)
                WMSetButtonEnabled(panel->autostartB, hasCompositor);
}

void wPrefsEffectsUpdateConfigPathLabel(_Panel *panel)
{
        Bool hasConfigBackend = compositor_index_supports_config(panel->compositorIndex);
        Bool hasConfig = (hasConfigBackend && panel->configPath && panel->configPath[0] != '\0');

        if (!hasConfigBackend) {
                WMSetLabelText(panel->configPathL, _("No compositor configuration in use"));
        } else if (!hasConfig) {
                WMSetLabelText(panel->configPathL, _("Compositor configuration will be created on demand"));
        } else {
                WMSetLabelText(panel->configPathL, panel->configPath);
        }

        WMSetButtonEnabled(panel->openConfigB, hasConfig);
        wPrefsEffectsSyncCompositorControls(panel);
}

void wPrefsEffectsUpdateDockOpacityLabel(_Panel *panel)
{
        char buffer[16];
        int value;

        if (!panel || !panel->dockOpacityS || !panel->dockOpacityValueL)
                return;

        value = WMGetSliderValue(panel->dockOpacityS);
        snprintf(buffer, sizeof(buffer), "%d%%", value);
        WMSetLabelText(panel->dockOpacityValueL, buffer);
}

void wPrefsEffectsEnsureTemplateForSelection(_Panel *panel)
{
        char *expanded;

        if (!panel || !compositor_index_supports_config(panel->compositorIndex) || !panel->configPath)
                return;

        expanded = wexpandpath(panel->configPath);
        if (!expanded)
                return;

        ensure_config_template(panel->compositorIndex, expanded);
        wfree(expanded);
}

void wPrefsEffectsUpdateCapabilityStatus(_Panel *panel)
{
        const char *backend;
        const char *binary;
        const char *running;
        const char *fallback;
        Bool available;
        Bool is_running;
        char text[160];

        if (!panel || !panel->statusValueL)
                return;

        if (compositor_index_is_none(panel->compositorIndex)) {
                backend = "none";
                available = True;
                is_running = False;
                fallback = "inactive";
        } else {
                backend = wPrefsCompositorOptions[panel->compositorIndex].db_value;
                binary = compositor_binary_for_index(panel->compositorIndex);
                available = (binary != NULL) && command_exists(binary);
                is_running = available && process_running(binary);
                fallback = available ? "inactive" : "active";
        }

        running = is_running ? "yes" : "no";
        if (!available && !compositor_index_is_none(panel->compositorIndex))
                running = "no";

        snprintf(text, sizeof(text), "running: %s | backend: %s | fallback: %s",
                 running, backend, fallback);
        WMSetLabelText(panel->statusValueL, text);
}

void wPrefsEffectsCompositorChanged(WMWidget *w, void *data)
{
        _Panel *panel = (_Panel *)data;
        int index = WMGetPopUpButtonSelectedItem(panel->compositorP);
        const char *default_path;

        (void)w;

        if (index < 0 || !wPrefsCompositorOptions[index].db_value)
                index = 0;

        if (index != panel->compositorIndex) {
                default_path = wPrefsDefaultCompositorPath(index);
                if (compositor_index_supports_config(index))
                        wPrefsEffectsSetConfigPath(panel, default_path);
                else
                        wPrefsEffectsSetConfigPath(panel, NULL);
                panel->compositorIndex = index;
        }

        wPrefsEffectsEnsureTemplateForSelection(panel);
        wPrefsEffectsUpdateConfigPathLabel(panel);
        wPrefsEffectsUpdateCapabilityStatus(panel);
}

void wPrefsEffectsOpenConfig(WMWidget *w, void *data)
{
        (void)w;
        launch_editor_for_config((_Panel *)data);
}

void wPrefsEffectsDockOpacityChanged(WMWidget *w, void *data)
{
        (void)w;
        wPrefsEffectsUpdateDockOpacityLabel((_Panel *)data);
}
