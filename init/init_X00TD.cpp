/*
 * Copyright (C) 2022 PixelExperience
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <vector>

#include <android-base/logging.h>
#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

using android::base::GetProperty;

void property_override(char const prop[], char const value[], bool add = true) {
    prop_info* pi;

    pi = (prop_info*)__system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else if (add)
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void set_avoid_gfxaccel_config() {
    struct sysinfo sys;
    sysinfo(&sys);

    if (sys.totalram <= 3072ull * 1024 * 1024) {
        // Reduce memory footprint
        property_override("ro.config.avoid_gfx_accel", "true");
    }
}

void NFC_check()
{
    // Check NFC
    std::ifstream infile("/proc/NFC_CHECK");
    std::string check;

    getline(infile, check);
    if (!check.compare("SUPPORTED")) {
        property_override("ro.hq.support.nfc", "1");
        property_override("ro.boot.product.hardware.sku", "NFC");
    } else
        property_override("ro.hq.support.nfc", "0");
}

void load_vendor_props() {
    property_override("bluetooth.device.default_name", "Zenfone Max Pro M1");
    property_override("ro.product.brand", "asus");
    property_override("ro.product.manufacturer", "asus");
    property_override("ro.product.device", "ASUS_X00T_2");
    property_override("ro.vendor.product.device", "ASUS_X00T_2");
    property_override("ro.product.name", "WW_X00TD");
    property_override("ro.vendor.product.name", "WW_X00TD");
    property_override("ro.build.fingerprint", "asus/WW_X00TD/ASUS_X00T_2:9/PKQ1/16.2017.2009.087-20200826:user/release-keys");
    property_override("ro.vendor.build.fingerprint", "asus/WW_X00TD/ASUS_X00T_2:9/PKQ1/16.2017.2009.087-20200826:user/release-keys");
    property_override("ro.bootimage.build.fingerprint", "asus/WW_X00TD/ASUS_X00T_2:9/PKQ1/16.2017.2009.087-20200826:user/release-keys");
    property_override("ro.build.description", "sdm660_64-user 9 PKQ1 43 release-keys");
    property_override("vendor.usb.product_string", "Zenfone Max Pro M1");
    property_override("ro.boot.verifiedbootstate", "green");
    property_override("ro.boot.verifiedstate", "green");
}

void vendor_load_properties() {
    if (access("/system/bin/recovery", F_OK) != 0) {
        load_vendor_props();
    }
    set_avoid_gfxaccel_config();
    NFC_check();
}
