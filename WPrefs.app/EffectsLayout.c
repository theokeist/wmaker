#include "EffectsInternal.h"

static void effects_panel_resized(void *self, WMNotification *notif)
{
        _Panel *panel = (_Panel *)self;

        if (!panel || WMGetNotificationName(notif) != WMViewSizeDidChangeNotification)
                return;

        if (WMGetNotificationObject(notif) != WMWidgetView(panel->box))
                return;

        wPrefsEffectsLayoutPanel(panel);
}

void wPrefsEffectsLayoutPanel(_Panel *panel)
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
        int sliderWidth;
        int valueWidth;
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

        WMResizeWidget(panel->dockOpacityL, labelWidth, rowHeight);
        WMMoveWidget(panel->dockOpacityL, innerMargin, compY);
        valueWidth = 48;
        sliderWidth = controlWidth - valueWidth - 6;
        if (sliderWidth < 120)
                sliderWidth = 120;
        WMResizeWidget(panel->dockOpacityS, sliderWidth, rowHeight);
        WMMoveWidget(panel->dockOpacityS, controlX, compY);
        WMResizeWidget(panel->dockOpacityValueL, valueWidth, rowHeight);
        WMMoveWidget(panel->dockOpacityValueL, controlX + sliderWidth + 6, compY);

        compY += rowHeight + spacing;

        WMResizeWidget(panel->statusL, labelWidth, rowHeight);
        WMMoveWidget(panel->statusL, innerMargin, compY);
        WMResizeWidget(panel->statusValueL, controlWidth, rowHeight);
        WMMoveWidget(panel->statusValueL, controlX, compY);

        compY += rowHeight + spacing;

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

void wPrefsEffectsShowPanel(Panel *p)
{
        _Panel *panel = (_Panel *)p;

        wPrefsEffectsUpdateCapabilityStatus(panel);
        wPrefsEffectsLayoutPanel(panel);
}

void wPrefsEffectsCreatePanel(Panel *p)
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
        WMSetFrameTitle(panel->transitionF, _("Window animations"));

        panel->moveEffectL = WMCreateLabel(panel->transitionF);
        WMSetLabelText(panel->moveEffectL, _("Window movement curve:"));

        panel->moveEffectP = WMCreatePopUpButton(panel->transitionF);

        panel->launchEffectL = WMCreateLabel(panel->transitionF);
        WMSetLabelText(panel->launchEffectL, _("Launch animation curve:"));

        panel->launchEffectP = WMCreatePopUpButton(panel->transitionF);

        for (i = 0; wPrefsTransitionEffects[i].db_value; i++) {
                WMAddPopUpButtonItem(panel->moveEffectP, _(wPrefsTransitionEffects[i].label));
                WMAddPopUpButtonItem(panel->launchEffectP, _(wPrefsTransitionEffects[i].label));
        }

        panel->showContentB = WMCreateSwitchButton(panel->transitionF);
        WMSetButtonText(panel->showContentB, _("Show window contents during animations"));

        panel->compositorF = WMCreateFrame(panel->contentB);
        WMSetFrameTitle(panel->compositorF, _("Compositor integration"));

        panel->compositorL = WMCreateLabel(panel->compositorF);
        WMSetLabelText(panel->compositorL, _("Preferred compositor:"));

        panel->compositorP = WMCreatePopUpButton(panel->compositorF);
        for (i = 0; wPrefsCompositorOptions[i].db_value; i++)
                WMAddPopUpButtonItem(panel->compositorP, _(wPrefsCompositorOptions[i].label));
        WMSetPopUpButtonAction(panel->compositorP, wPrefsEffectsCompositorChanged, panel);

        panel->configPathL = WMCreateLabel(panel->compositorF);
        WMSetLabelWraps(panel->configPathL, True);

        panel->autostartB = WMCreateSwitchButton(panel->compositorF);
        WMSetButtonText(panel->autostartB,
                        _("Start compositor automatically when Window Maker launches"));

        panel->shadowB = WMCreateSwitchButton(panel->compositorF);
        WMSetButtonText(panel->shadowB, _("Enable window shadows (requires compositor)"));

        panel->openConfigB = WMCreateCommandButton(panel->compositorF);
        WMSetButtonText(panel->openConfigB, _("Edit configuration"));
        WMSetButtonAction(panel->openConfigB, wPrefsEffectsOpenConfig, panel);

        panel->dockOpacityL = WMCreateLabel(panel->compositorF);
        WMSetLabelText(panel->dockOpacityL, _("Dock transparency:"));

        panel->dockOpacityS = WMCreateSlider(panel->compositorF);
        WMSetSliderMinValue(panel->dockOpacityS, 0);
        WMSetSliderMaxValue(panel->dockOpacityS, 100);
        WMSetSliderContinuous(panel->dockOpacityS, True);
        WMSetSliderAction(panel->dockOpacityS, wPrefsEffectsDockOpacityChanged, panel);

        panel->dockOpacityValueL = WMCreateLabel(panel->compositorF);
        WMSetLabelTextAlignment(panel->dockOpacityValueL, WARight);

        panel->statusL = WMCreateLabel(panel->compositorF);
        WMSetLabelText(panel->statusL, _("Compositor status:"));

        panel->statusValueL = WMCreateLabel(panel->compositorF);
        WMSetLabelTextAlignment(panel->statusValueL, WALeft);

        panel->hintL = WMCreateLabel(panel->compositorF);
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

        wPrefsEffectsShowData(panel);
        wPrefsEffectsUpdateConfigPathLabel(panel);
        wPrefsEffectsLayoutPanel(panel);

        WMAddNotificationObserver(effects_panel_resized, panel,
                                  WMViewSizeDidChangeNotification,
                                  WMWidgetView(panel->box));
}
