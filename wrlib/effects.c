/*
 *  Window Maker - Window Manager for X11
 *
 *  Copyright (c) 2024 Window Maker Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "wraster.h"

static double clamp_progress(double progress)
{
        if (progress <= 0.0)
                return 0.0;
        if (progress >= 1.0)
                return 1.0;
        return progress;
}

double REffectProgressForCurve(REffectCurve curve, double progress)
{
        double t = clamp_progress(progress);

        switch (curve) {
        case R_EFFECT_CURVE_SMOOTH:
                /* cosine-based ease-in/ease-out */
                return 0.5 - cos(t * M_PI) * 0.5;
        case R_EFFECT_CURVE_GENTLE:
                /* cubic smoothstep */
                return t * t * (3.0 - 2.0 * t);
        case R_EFFECT_CURVE_CLASSIC:
        default:
                break;
        }

        return t;
}

