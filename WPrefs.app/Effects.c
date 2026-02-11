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

#include "EffectsInternal.h"

#define ICON_FILE "animations"

const struct WPrefsTransitionEffect wPrefsTransitionEffects[] = {
        { "Classic", N_("Classic (legacy acceleration)") },
        { "Smooth",  N_("Smooth ease-in/out") },
        { "Gentle",  N_("Gentle smoothstep") },
        { NULL, NULL }
};

const struct WPrefsCompositorOption wPrefsCompositorOptions[] = {
        { "None",  N_("No compositor (classic look)"), "" },
        { "Picom", N_("Picom (lightweight effects)"), "~/.config/picom/picom.conf" },
        { "Compton", N_("Compton (legacy fork)"), "~/.config/compton.conf" },
        { "Xcompmgr", N_("xcompmgr (basic compositor)"), "" },
        { "Compiz", N_("Compiz (plugin compositor)"), "" },
        { NULL, NULL, NULL }
};

Panel *InitEffects(WMWidget *parent)
{
        _Panel *panel;

        panel = wmalloc(sizeof(_Panel));
        memset(panel, 0, sizeof(_Panel));

        panel->sectionName = _("Animations & Effects");
        panel->description = _("Choose animation curves and configure an external compositor\n"
                               "for glass and Glide effects.");
        panel->parent = parent;

        panel->callbacks.createWidgets = wPrefsEffectsCreatePanel;
        panel->callbacks.updateDomain = (void (*)(Panel *))wPrefsEffectsStoreData;
        panel->callbacks.undoChanges = (void (*)(Panel *))wPrefsEffectsUndo;
        panel->callbacks.prepareForClose = (void (*)(Panel *))wPrefsEffectsPrepareForClose;
        panel->callbacks.showPanel = wPrefsEffectsShowPanel;

        AddSection((Panel *)panel, ICON_FILE);

        return panel;
}
