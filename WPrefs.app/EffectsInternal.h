/* internal helpers for Effects panel split implementation */
#ifndef WPREFS_EFFECTS_INTERNAL_H
#define WPREFS_EFFECTS_INTERNAL_H

#include "WPrefs.h"

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
        WMLabel *dockOpacityL;
        WMSlider *dockOpacityS;
        WMLabel *dockOpacityValueL;
        WMLabel *statusL;
        WMLabel *statusValueL;

        int compositorIndex;
        char *configPath;
} _Panel;

extern const struct WPrefsTransitionEffect {
        const char *db_value;
        const char *label;
} wPrefsTransitionEffects[];

extern const struct WPrefsCompositorOption {
        const char *db_value;
        const char *label;
        const char *default_path;
} wPrefsCompositorOptions[];

int wPrefsClampTransitionEffectIndex(int index);
int wPrefsGetTransitionEffectIndex(const char *str);
int wPrefsGetCompositorIndex(const char *value);
const char *wPrefsDefaultCompositorPath(int index);

void wPrefsEffectsLayoutPanel(_Panel *panel);
void wPrefsEffectsShowPanel(Panel *panel);
void wPrefsEffectsCreatePanel(Panel *panel);

void wPrefsEffectsShowData(_Panel *panel);
void wPrefsEffectsStoreData(_Panel *panel);
void wPrefsEffectsUndo(_Panel *panel);
void wPrefsEffectsPrepareForClose(_Panel *panel);

void wPrefsEffectsCompositorChanged(WMWidget *w, void *data);
void wPrefsEffectsOpenConfig(WMWidget *w, void *data);
void wPrefsEffectsDockOpacityChanged(WMWidget *w, void *data);
void wPrefsEffectsSyncCompositorControls(_Panel *panel);
void wPrefsEffectsSetConfigPath(_Panel *panel, const char *path);
void wPrefsEffectsUpdateConfigPathLabel(_Panel *panel);
void wPrefsEffectsUpdateDockOpacityLabel(_Panel *panel);
void wPrefsEffectsUpdateCapabilityStatus(_Panel *panel);
void wPrefsEffectsEnsureTemplateForSelection(_Panel *panel);

#endif
