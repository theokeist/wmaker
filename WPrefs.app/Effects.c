/* Effects.c - window animation and compositor preferences */
/*
 *  WPrefs - Window Maker Preferences Program
 *
 *  Copyright (c) 2025 Window Maker Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#include "WPrefs.h"
#include "config.h"

#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <WINGs/WUtil.h>

#define ICON_FILE "animations"

typedef struct _Panel {
        WMBox *box;

        char *sectionName;
        char *description;

        CallbackRec callbacks;

        WMWidget *parent;

        WMScrollView *scrollV;
        WMBox *contentB;

        WMFrame *transitionF;
        WMLabel *moveEffectL;
        WMPopUpButton *moveEffectP;
        WMLabel *launchEffectL;
        WMPopUpButton *launchEffectP;
        WMButton *showContentB;

        WMFrame *compositorF;
        WMLabel *compositorL;
        WMPopUpButton *compositorP;
        WMLabel *configPathL;
        WMButton *openConfigB;
        WMLabel *hintL;
        WMButton *shadowB;
        WMButton *autostartB;

        int compositorIndex;
        char *configPath;
} _Panel;

static void syncCompositorControls(_Panel *panel);
static void layout_effects_panel(_Panel *panel);
static void effects_panel_resized(void *self, WMNotification *notif);
static void showEffectsPanel(Panel *panel);

static const struct {
        const char *db_value;
        const char *label;
} transition_effects[] = {
        { "Classic", N_("Classic (legacy acceleration)") },
        { "Smooth",  N_("Smooth ease-in/out") },
        { "Gentle",  N_("Gentle smoothstep") }
};

static const struct {
        const char *db_value;
        const char *label;
        const char *default_path;
} compositor_options[] = {
        { "None",  N_("No compositor (classic look)"), "" },
        { "Picom", N_("Picom (lightweight effects)"), "~/.config/picom/picom.conf" }
};

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

static int clamp_transition_effect_index(int index)
{
        if (index < 0 || index >= (int)wlengthof(transition_effects))
                return 0;

        return index;
}

static int getTransitionEffect(const char *str)
{
        int i;

        if (!str)
                return 0;

        for (i = 0; i < (int)wlengthof(transition_effects); i++) {
                if (strcasecmp(str, transition_effects[i].db_value) == 0)
                        return i;
        }

        wwarning(_("bad option value %s in Window animation. Using default value"), str);
        return 0;
}

static int getCompositorIndex(const char *value)
{
        int i;

        if (!value)
                return 0;

        for (i = 0; i < (int)wlengthof(compositor_options); i++) {
                if (strcasecmp(value, compositor_options[i].db_value) == 0)
                        return i;
        }

        return 0;
}

static const char *default_path_for_index(int index)
{
        if (index < 0 || index >= (int)wlengthof(compositor_options))
                return "";

        return compositor_options[index].default_path;
}

static void free_config_path(_Panel *panel)
{
        if (panel->configPath) {
                wfree(panel->configPath);
                panel->configPath = NULL;
        }
}

static void set_config_path(_Panel *panel, const char *path)
{
        free_config_path(panel);
        if (path)
                panel->configPath = wstrdup(path);
}

static void syncCompositorControls(_Panel *panel)
{
        Bool hasCompositor = (panel->compositorIndex == 1);

        if (panel->shadowB)
                WMSetButtonEnabled(panel->shadowB, hasCompositor);

        if (panel->autostartB)
                WMSetButtonEnabled(panel->autostartB, hasCompositor);
}

static void updateConfigPathLabel(_Panel *panel)
{
        Bool hasPicom = (panel->compositorIndex == 1);
        Bool hasConfig = (hasPicom && panel->configPath && panel->configPath[0] != '\0');

        if (!hasPicom) {
                WMSetLabelText(panel->configPathL, _("No compositor configuration in use"));
        } else if (!hasConfig) {
                WMSetLabelText(panel->configPathL, _("Picom configuration will be created on demand"));
        } else {
                WMSetLabelText(panel->configPathL, panel->configPath);
        }

        WMSetButtonEnabled(panel->openConfigB, hasConfig);
        syncCompositorControls(panel);
}

static char *quote_for_shell(const char *path)
{
        const char *src;
        size_t len = 2; /* quotes */
        char *buffer;
        char *dst;

        if (!path)
                return NULL;

        for (src = path; *src; src++) {
                if (*src == '\'' )
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
        const char *template_name;
        FILE *in, *out;
        int c;

        if (!path || index != 1)
                return -1;

        if (stat(path, &st) == 0)
                return 0;

        dircopy = wstrdup(path);
        dirpath = dirname(dircopy);
        if (dirpath && dirpath[0])
                wmkdirhier(dirpath);
        wfree(dircopy);

        template_name = "picom.conf";
        snprintf(template_path, sizeof(template_path), "%s/Compositors/%s", WMAKER_RESOURCE_PATH, template_name);

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

static void ensure_template_for_selection(_Panel *panel)
{
        char *expanded;

        if (!panel || panel->compositorIndex != 1 || !panel->configPath)
                return;

        expanded = wexpandpath(panel->configPath);
        if (!expanded)
                return;

        ensure_config_template(panel->compositorIndex, expanded);
        wfree(expanded);
}

static void launch_editor_for_config(_Panel *panel)
{
        char *expanded;
        char *quoted;
        const char *editor;
        char command[PATH_MAX * 2];
        pid_t pid;

        if (panel->compositorIndex != 1 || !panel->configPath)
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

static void showData(_Panel *panel)
{
        const char *str;
        int index;

        str = GetStringForKey("WindowMovementEffect");
        WMSetPopUpButtonSelectedItem(panel->moveEffectP,
                                     clamp_transition_effect_index(getTransitionEffect(str)));

        str = GetStringForKey("LaunchEffect");
        WMSetPopUpButtonSelectedItem(panel->launchEffectP,
                                     clamp_transition_effect_index(getTransitionEffect(str)));

        WMSetButtonSelected(panel->showContentB,
                GetBoolForKey("ShowWindowContentsDuringAnimations"));

        str = GetStringForKey("PreferredCompositor");
        index = getCompositorIndex(str);
        panel->compositorIndex = index;
        WMSetPopUpButtonSelectedItem(panel->compositorP, index);

        WMSetButtonSelected(panel->autostartB, GetBoolForKey("AutostartCompositor"));
        WMSetButtonSelected(panel->shadowB, GetBoolForKey("EnableWindowShadows"));

        str = GetStringForKey("CompositorConfigPath");
        if (!str || !str[0])
                str = default_path_for_index(index);
        if (index == 1)
                set_config_path(panel, str);
        else
                set_config_path(panel, NULL);
        updateConfigPathLabel(panel);

        ensure_template_for_selection(panel);

        layout_effects_panel(panel);
}

static void storeData(_Panel *panel)
{
        int move_index = clamp_transition_effect_index(WMGetPopUpButtonSelectedItem(panel->moveEffectP));
        int launch_index = clamp_transition_effect_index(WMGetPopUpButtonSelectedItem(panel->launchEffectP));
        int compositor_index = WMGetPopUpButtonSelectedItem(panel->compositorP);

        SetStringForKey(transition_effects[move_index].db_value, "WindowMovementEffect");
        SetStringForKey(transition_effects[launch_index].db_value, "LaunchEffect");
        SetBoolForKey(WMGetButtonSelected(panel->showContentB),
                      "ShowWindowContentsDuringAnimations");

        SetStringForKey(compositor_options[compositor_index].db_value, "PreferredCompositor");
        SetBoolForKey(WMGetButtonSelected(panel->autostartB), "AutostartCompositor");
        if (panel->configPath && panel->compositorIndex == 1)
                SetStringForKey(panel->configPath, "CompositorConfigPath");
        else
                RemoveObjectForKey("CompositorConfigPath");

        SetBoolForKey(WMGetButtonSelected(panel->shadowB), "EnableWindowShadows");
}

static void undo(_Panel *panel)
{
        showData(panel);
}

static void prepareForClose(_Panel *panel)
{
        free_config_path(panel);
}

static void compositorChanged(WMWidget *w, void *data)
{
        _Panel *panel = (_Panel *)data;
        int index = WMGetPopUpButtonSelectedItem(panel->compositorP);
        const char *default_path;

        (void)w;

        if (index < 0 || index >= (int)wlengthof(compositor_options))
                index = 0;

        if (index != panel->compositorIndex) {
                default_path = default_path_for_index(index);
                if (index == 1)
                        set_config_path(panel, default_path);
                else
                        set_config_path(panel, NULL);
                panel->compositorIndex = index;
        }

        ensure_template_for_selection(panel);
        updateConfigPathLabel(panel);
}

static void openConfig(WMWidget *w, void *data)
{
        (void)w;
        launch_editor_for_config((_Panel *)data);
}

static void layout_effects_panel(_Panel *panel)
{
        const int outerMargin = 8;
        const int innerMargin = 12;
        const int rowHeight = 24;
        const int spacing = 12;
        int boxWidth;
        int boxHeight;
        int scrollWidth;
        int scrollHeight;
        int contentWidth;
        int frameWidth;
        int controlWidth;
        int controlX;
        int labelWidth;
        int availableWidth;
        int visibleHeight;
        int y;
        int compY;
        int totalHeight;
        int toggleWidth;
        int buttonWidth;
        int compositorTop;

        if (!panel || !panel->box || !panel->scrollV || !panel->contentB)
                return;

        boxWidth = WMWidgetWidth(panel->box);
        boxHeight = WMWidgetHeight(panel->box);

        scrollWidth = boxWidth - (outerMargin * 2);
        scrollHeight = boxHeight - (outerMargin * 2);
        if (scrollWidth < 240)
                scrollWidth = 240;
        if (scrollHeight < 240)
                scrollHeight = 240;

        WMResizeWidget(panel->scrollV, scrollWidth, scrollHeight);
        WMMoveWidget(panel->scrollV, outerMargin, outerMargin);

        contentWidth = scrollWidth - (outerMargin * 2);
        if (contentWidth < 320)
                contentWidth = 320;

        frameWidth = contentWidth - (outerMargin * 2);
        if (frameWidth < 260) {
                frameWidth = 260;
                if (contentWidth < frameWidth + (outerMargin * 2))
                        contentWidth = frameWidth + (outerMargin * 2);
        }

        controlWidth = frameWidth / 2;
        if (controlWidth < 220)
                controlWidth = 220;
        if (controlWidth > frameWidth - (innerMargin * 2))
                controlWidth = frameWidth - (innerMargin * 2);

        controlX = frameWidth - controlWidth - innerMargin;
        labelWidth = controlX - innerMargin;
        if (labelWidth < 100)
                labelWidth = 100;

        availableWidth = frameWidth - (innerMargin * 2);
        if (availableWidth < 120)
                availableWidth = 120;

        y = innerMargin;

        WMResizeWidget(panel->moveEffectL, labelWidth, rowHeight);
        WMMoveWidget(panel->moveEffectL, innerMargin, y);
        WMResizeWidget(panel->moveEffectP, controlWidth, rowHeight);
        WMMoveWidget(panel->moveEffectP, controlX, y - 2);

        y += rowHeight + spacing;

        WMResizeWidget(panel->launchEffectL, labelWidth, rowHeight);
        WMMoveWidget(panel->launchEffectL, innerMargin, y);
        WMResizeWidget(panel->launchEffectP, controlWidth, rowHeight);
        WMMoveWidget(panel->launchEffectP, controlX, y - 2);

        y += rowHeight + spacing;

        toggleWidth = availableWidth;
        WMResizeWidget(panel->showContentB, toggleWidth, 28);
        WMMoveWidget(panel->showContentB, innerMargin, y);

        y += 28 + spacing;

        WMResizeWidget(panel->transitionF, frameWidth, y + innerMargin);
        WMMoveWidget(panel->transitionF, outerMargin, outerMargin);

        compY = innerMargin;

        WMResizeWidget(panel->compositorL, labelWidth, rowHeight);
        WMMoveWidget(panel->compositorL, innerMargin, compY);
        WMResizeWidget(panel->compositorP, controlWidth, rowHeight);
        WMMoveWidget(panel->compositorP, controlX, compY - 2);

        compY += rowHeight + spacing;

        WMResizeWidget(panel->autostartB, availableWidth, 28);
        WMMoveWidget(panel->autostartB, innerMargin, compY);

        compY += 28 + spacing;

        WMResizeWidget(panel->shadowB, availableWidth, 28);
        WMMoveWidget(panel->shadowB, innerMargin, compY);

        compY += 28 + spacing;

        WMResizeWidget(panel->configPathL, availableWidth, 48);
        WMMoveWidget(panel->configPathL, innerMargin, compY);

        compY += 48 + spacing;

        buttonWidth = controlWidth;
        if (buttonWidth > availableWidth)
                buttonWidth = availableWidth;
        WMResizeWidget(panel->openConfigB, buttonWidth, 28);
        WMMoveWidget(panel->openConfigB, innerMargin, compY);

        compY += 28 + spacing;

        WMResizeWidget(panel->hintL, availableWidth, 60);
        WMMoveWidget(panel->hintL, innerMargin, compY);

        compY += 60 + innerMargin;

        compositorTop = outerMargin + WMWidgetHeight(panel->transitionF) + spacing;
        WMMoveWidget(panel->compositorF, outerMargin, compositorTop);
        WMResizeWidget(panel->compositorF, frameWidth, compY);

        totalHeight = compositorTop + compY + outerMargin;
        visibleHeight = scrollHeight - (outerMargin * 2);
        if (visibleHeight < 0)
                visibleHeight = 0;
        if (totalHeight < visibleHeight)
                totalHeight = visibleHeight;

        WMResizeWidget(panel->contentB, contentWidth, totalHeight);
}

static void effects_panel_resized(void *self, WMNotification *notif)
{
        _Panel *panel = (_Panel *)self;

        if (!panel || WMGetNotificationName(notif) != WMViewSizeDidChangeNotification)
                return;

        if (WMGetNotificationObject(notif) != WMWidgetView(panel->box))
                return;

        layout_effects_panel(panel);
}

static void showEffectsPanel(Panel *p)
{
        _Panel *panel = (_Panel *)p;

        layout_effects_panel(panel);
}

static void createPanel(Panel *p)
{
        _Panel *panel = (_Panel *)p;
        int i;

        panel->box = WMCreateBox(panel->parent);
        WMSetViewExpandsToParent(WMWidgetView(panel->box), 2, 2, 2, 2);
        WMSetBoxHorizontal(panel->box, False);
        WMSetBoxBorderWidth(panel->box, 0);

        panel->scrollV = WMCreateScrollView(panel->box);
        WMResizeWidget(panel->scrollV, FRAME_WIDTH - 16, FRAME_HEIGHT - 16);
        WMMoveWidget(panel->scrollV, 8, 8);
        WMSetViewExpandsToParent(WMWidgetView(panel->scrollV), 8, 8, 8, 8);
        WMSetScrollViewRelief(panel->scrollV, WRSunken);
        WMSetScrollViewHasVerticalScroller(panel->scrollV, True);
        WMSetScrollViewHasHorizontalScroller(panel->scrollV, True);

        panel->contentB = WMCreateBox(panel->box);
        WMSetBoxHorizontal(panel->contentB, False);
        WMSetBoxBorderWidth(panel->contentB, 8);
        WMResizeWidget(panel->contentB, FRAME_WIDTH - 32, 300);
        WMSetViewExpandsToParent(WMWidgetView(panel->contentB), 0, 0, 0, 0);

        WMSetScrollViewContentView(panel->scrollV, WMWidgetView(panel->contentB));

        panel->transitionF = WMCreateFrame(panel->contentB);
        WMResizeWidget(panel->transitionF, FRAME_WIDTH - 48, 120);
        WMMoveWidget(panel->transitionF, 8, 8);
        WMSetFrameTitle(panel->transitionF, _("Window animations"));

        panel->moveEffectL = WMCreateLabel(panel->transitionF);
        WMSetLabelText(panel->moveEffectL, _("Window movement curve:"));

        panel->moveEffectP = WMCreatePopUpButton(panel->transitionF);
        WMResizeWidget(panel->moveEffectP, 210, 20);
        WMMoveWidget(panel->moveEffectP, 150, 20);

        panel->launchEffectL = WMCreateLabel(panel->transitionF);
        WMSetLabelText(panel->launchEffectL, _("Launch animation curve:"));

        panel->launchEffectP = WMCreatePopUpButton(panel->transitionF);
        WMResizeWidget(panel->launchEffectP, 210, 20);
        WMMoveWidget(panel->launchEffectP, 150, 50);

        for (i = 0; i < (int)wlengthof(transition_effects); i++) {
                WMAddPopUpButtonItem(panel->moveEffectP, _(transition_effects[i].label));
                WMAddPopUpButtonItem(panel->launchEffectP, _(transition_effects[i].label));
        }

        panel->showContentB = WMCreateSwitchButton(panel->transitionF);
        WMResizeWidget(panel->showContentB, FRAME_WIDTH - 72, 30);
        WMMoveWidget(panel->showContentB, 10, 78);
        WMSetButtonText(panel->showContentB, _("Show window contents during animations"));

        panel->compositorF = WMCreateFrame(panel->contentB);
        WMResizeWidget(panel->compositorF, FRAME_WIDTH - 48, 140);
        WMMoveWidget(panel->compositorF, 8, 150);
        WMSetFrameTitle(panel->compositorF, _("Compositor integration"));

        panel->compositorL = WMCreateLabel(panel->compositorF);
        WMSetLabelText(panel->compositorL, _("Preferred compositor:"));

        panel->compositorP = WMCreatePopUpButton(panel->compositorF);
        WMResizeWidget(panel->compositorP, 210, 20);
        WMMoveWidget(panel->compositorP, 150, 20);
        for (i = 0; i < (int)wlengthof(compositor_options); i++)
                WMAddPopUpButtonItem(panel->compositorP, _(compositor_options[i].label));
        WMSetPopUpButtonAction(panel->compositorP, compositorChanged, panel);

        panel->configPathL = WMCreateLabel(panel->compositorF);
        WMResizeWidget(panel->configPathL, FRAME_WIDTH - 70, 30);
        WMMoveWidget(panel->configPathL, 10, 50);
        WMSetLabelWraps(panel->configPathL, True);

        panel->autostartB = WMCreateSwitchButton(panel->compositorF);
        WMSetButtonText(panel->autostartB,
                        _("Start compositor automatically when Window Maker launches"));

        panel->shadowB = WMCreateSwitchButton(panel->compositorF);
        WMSetButtonText(panel->shadowB, _("Enable window shadows (requires compositor)"));

        panel->openConfigB = WMCreateCommandButton(panel->compositorF);
        WMResizeWidget(panel->openConfigB, 160, 26);
        WMMoveWidget(panel->openConfigB, 10, 90);
        WMSetButtonText(panel->openConfigB, _("Edit configuration"));
        WMSetButtonAction(panel->openConfigB, openConfig, panel);

        panel->hintL = WMCreateLabel(panel->compositorF);
        WMResizeWidget(panel->hintL, FRAME_WIDTH - 210, 60);
        WMMoveWidget(panel->hintL, 180, 80);
        WMSetLabelWraps(panel->hintL, True);
        WMSetLabelText(panel->hintL,
                       _("Picom pairs well with the Glide minimize effect and provides soft shadows when enabled. "
                         "Window Maker falls back to classic animations if no compositor is active."));

        WMRealizeWidget(panel->box);
        WMRealizeWidget(panel->contentB);
        WMMapWidget(panel->scrollV);
        WMMapWidget(panel->contentB);
        WMMapSubwidgets(panel->box);
        WMMapSubwidgets(panel->contentB);
        WMMapSubwidgets(panel->transitionF);
        WMMapSubwidgets(panel->compositorF);

        panel->configPath = NULL;
        panel->compositorIndex = 0;

        showData(panel);
        updateConfigPathLabel(panel);
        layout_effects_panel(panel);

        WMAddNotificationObserver(effects_panel_resized, panel,
                                  WMViewSizeDidChangeNotification,
                                  WMWidgetView(panel->box));
}

Panel *InitEffects(WMWidget *parent)
{
        _Panel *panel;

        panel = wmalloc(sizeof(_Panel));
        memset(panel, 0, sizeof(_Panel));

        panel->sectionName = _("Animations & Effects");
        panel->description = _("Choose animation curves and configure an external compositor\n"
                               "for glass and Glide effects.");
        panel->parent = parent;

        panel->callbacks.createWidgets = createPanel;
        panel->callbacks.updateDomain = storeData;
        panel->callbacks.undoChanges = undo;
        panel->callbacks.prepareForClose = prepareForClose;
        panel->callbacks.showPanel = showEffectsPanel;

        AddSection((Panel *)panel, ICON_FILE);

        return panel;
}
