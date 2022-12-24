/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

package org.lineageos.settings;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.UserHandle;
import android.provider.Settings;

import androidx.preference.PreferenceManager;

public class DefaultSystemSettings {
    private static final String TAG = "DefaultSystemSettings";
    private static final boolean DEBUG = false;

    private Context mContext;
    private SharedPreferences mSharedPrefs;

    public DefaultSystemSettings(final Context context) {
        mContext = context;
        mSharedPrefs = PreferenceManager.getDefaultSharedPreferences(context);
    }

    private boolean isFirstRun(String key) {
        boolean isFirstRun = mSharedPrefs.getBoolean(key, true);
        if (isFirstRun) {
            mSharedPrefs.edit().putBoolean(key, false).apply();
        }
        return isFirstRun;
    }

    private void saveFirstRun(String key) {
        mSharedPrefs.edit().putBoolean(key, true).apply();
    }

    public void onBootCompleted() {
        if (isFirstRun("enable-dt2w")) {
            writeDt2wOption(true);
        }

        writeAnimationSettings();
    }

    private void writeDt2wOption(final boolean enabled) {
        final boolean isDt2wEnabled = Settings.Secure.getIntForUser(
                mContext.getContentResolver(), Settings.Secure.DOUBLE_TAP_TO_WAKE, 0,
                UserHandle.USER_CURRENT) != 0;
        if (enabled != isDt2wEnabled) {
            Settings.Secure.putIntForUser(mContext.getContentResolver(),
                    Settings.Secure.DOUBLE_TAP_TO_WAKE, enabled ? 1 : 0,
                    UserHandle.USER_CURRENT);
        }
    }

    private void writeAnimationSettings() {
        Settings.Global.putString(mContext.getContentResolver(),
                Settings.Global.WINDOW_ANIMATION_SCALE, "0.5");
        Settings.Global.putString(mContext.getContentResolver(),
                Settings.Global.TRANSITION_ANIMATION_SCALE, "0.5");
    }
}
