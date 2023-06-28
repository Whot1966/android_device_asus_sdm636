/*
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (c) 2019, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

void set_dalvik_vm_config() {
    struct sysinfo sys;
    sysinfo(&sys);

    if (sys.totalram > 5072ull * 1024 * 1024) {
        // from - phone-xhdpi-6144-dalvik-heap.mk
        property_override("dalvik.vm.heapstartsize","16m");
        property_override("dalvik.vm.heapgrowthlimit", "256m");
        property_override("dalvik.vm.heapsize", "512m");
        property_override("dalvik.vm.heaptargetutilization", "0.5");
        property_override("dalvik.vm.heapminfree", "8m");
        property_override("dalvik.vm.heapmaxfree", "32m");
    } else if (sys.totalram > 3072ull * 1024 * 1024) {
        // from - phone-xxhdpi-4096-dalvik-heap.mk
        property_override("dalvik.vm.heapstartsize","8m");
        property_override("dalvik.vm.heapgrowthlimit", "256m");
        property_override("dalvik.vm.heapsize", "512m");
        property_override("dalvik.vm.heaptargetutilization", "0.6");
        property_override("dalvik.vm.heapminfree", "8m");
        property_override("dalvik.vm.heapmaxfree", "16m");
    } else {
        // from - phone-xhdpi-2048-dalvik-heap.mk
        property_override("dalvik.vm.heapstartsize","8m");
        property_override("dalvik.vm.heapgrowthlimit", "192m");
        property_override("dalvik.vm.heapsize", "512m");
        property_override("dalvik.vm.heaptargetutilization", "0.75");
        property_override("dalvik.vm.heapminfree", "512k");
        property_override("dalvik.vm.heapmaxfree", "8m");
    }

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
}

void vendor_load_properties() {
    if (access("/system/bin/recovery", F_OK) != 0) {
        load_vendor_props();
    }
    set_avoid_gfxaccel_config();
    set_dalvik_vm_config();
    NFC_check();
}
