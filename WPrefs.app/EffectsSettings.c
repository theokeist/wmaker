#include "EffectsInternal.h"

#include <strings.h>

static Bool compositor_index_supports_config(int index)
{
        if (index < 0 || !wPrefsCompositorOptions[index].db_value)
                return False;

        return (strcasecmp(wPrefsCompositorOptions[index].db_value, "Picom") == 0
                || strcasecmp(wPrefsCompositorOptions[index].db_value, "Compton") == 0);
}

int wPrefsClampTransitionEffectIndex(int index)
{
        int max = 0;

        while (wPrefsTransitionEffects[max].db_value)
                max++;

        if (index < 0 || index >= max)
                return 0;

        return index;
}

int wPrefsGetTransitionEffectIndex(const char *str)
{
        int i;

        if (!str)
                return 0;

        for (i = 0; wPrefsTransitionEffects[i].db_value; i++) {
                if (strcasecmp(str, wPrefsTransitionEffects[i].db_value) == 0)
                        return i;
        }

        wwarning(_("bad option value %s in Window animation. Using default value"), str);
        return 0;
}

int wPrefsGetCompositorIndex(const char *value)
{
        int i;

        if (!value)
                return 0;

        for (i = 0; wPrefsCompositorOptions[i].db_value; i++) {
                if (strcasecmp(value, wPrefsCompositorOptions[i].db_value) == 0)
                        return i;
        }

        return 0;
}

const char *wPrefsDefaultCompositorPath(int index)
{
        if (index < 0 || !wPrefsCompositorOptions[index].db_value)
                return "";

        return wPrefsCompositorOptions[index].default_path;
}

void wPrefsEffectsShowData(_Panel *panel)
{
        const char *str;
        int index;
        int opacity;

        str = GetStringForKey("WindowMovementEffect");
        WMSetPopUpButtonSelectedItem(panel->moveEffectP,
                                     wPrefsClampTransitionEffectIndex(wPrefsGetTransitionEffectIndex(str)));

        str = GetStringForKey("LaunchEffect");
        WMSetPopUpButtonSelectedItem(panel->launchEffectP,
                                     wPrefsClampTransitionEffectIndex(wPrefsGetTransitionEffectIndex(str)));

        WMSetButtonSelected(panel->showContentB,
                GetBoolForKey("ShowWindowContentsDuringAnimations"));

        str = GetStringForKey("PreferredCompositor");
        index = wPrefsGetCompositorIndex(str);
        panel->compositorIndex = index;
        WMSetPopUpButtonSelectedItem(panel->compositorP, index);

        WMSetButtonSelected(panel->autostartB, GetBoolForKey("AutostartCompositor"));
        WMSetButtonSelected(panel->shadowB, GetBoolForKey("EnableWindowShadows"));

        opacity = GetIntegerForKey("DockOpacity");
        if (opacity < 0)
                opacity = 0;
        if (opacity > 100)
                opacity = 100;
        WMSetSliderValue(panel->dockOpacityS, opacity);
        wPrefsEffectsUpdateDockOpacityLabel(panel);

        str = GetStringForKey("CompositorConfigPath");
        if (!str || !str[0])
                str = wPrefsDefaultCompositorPath(index);
        if (compositor_index_supports_config(index))
                wPrefsEffectsSetConfigPath(panel, str);
        else
                wPrefsEffectsSetConfigPath(panel, NULL);

        wPrefsEffectsUpdateConfigPathLabel(panel);
        wPrefsEffectsEnsureTemplateForSelection(panel);
        wPrefsEffectsUpdateCapabilityStatus(panel);
        wPrefsEffectsLayoutPanel(panel);
}

void wPrefsEffectsStoreData(_Panel *panel)
{
        int move_index = wPrefsClampTransitionEffectIndex(WMGetPopUpButtonSelectedItem(panel->moveEffectP));
        int launch_index = wPrefsClampTransitionEffectIndex(WMGetPopUpButtonSelectedItem(panel->launchEffectP));
        int compositor_index = WMGetPopUpButtonSelectedItem(panel->compositorP);

        SetStringForKey(wPrefsTransitionEffects[move_index].db_value, "WindowMovementEffect");
        SetStringForKey(wPrefsTransitionEffects[launch_index].db_value, "LaunchEffect");
        SetBoolForKey(WMGetButtonSelected(panel->showContentB),
                      "ShowWindowContentsDuringAnimations");

        SetStringForKey(wPrefsCompositorOptions[compositor_index].db_value, "PreferredCompositor");
        SetBoolForKey(WMGetButtonSelected(panel->autostartB), "AutostartCompositor");
        if (panel->configPath && compositor_index_supports_config(panel->compositorIndex))
                SetStringForKey(panel->configPath, "CompositorConfigPath");
        else
                RemoveObjectForKey("CompositorConfigPath");

        SetIntegerForKey(WMGetSliderValue(panel->dockOpacityS), "DockOpacity");
        SetBoolForKey(WMGetButtonSelected(panel->shadowB), "EnableWindowShadows");
}

void wPrefsEffectsUndo(_Panel *panel)
{
        wPrefsEffectsShowData(panel);
}

void wPrefsEffectsPrepareForClose(_Panel *panel)
{
        wPrefsEffectsSetConfigPath(panel, NULL);
}
