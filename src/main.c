#include <android/log.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../AoQ-ModLoader-For-Quest/shared/aoqcore/aoq.h"
#include "../../AoQ-ModLoader-For-Quest/shared/inline-hook/inlineHook.h"
#include "../../AoQ-ModLoader-For-Quest/shared/utils/utils.h"
#include "../../AoQ-ModLoader-For-Quest/shared/modapi/modapi.h"

#define ADDR_NetworkFlareGun_Update       0x47B53C
#define ADDR_NetworkFlareGun_ReloadFlare  0x47CCA0

#define ADDR_FireFlare_Update             0x4E36F0
#define ADDR_FireFlare_ReloadFlare        0x4E4174


#define LOG_TAG "FlareMod"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

MAKE_HOOK(NetworkFlareGun_Update, ADDR_NetworkFlareGun_Update, void, void *self)
{ 
	NetworkFlareGun_Update(self);
	if (!aoq_is_mine(self)) return;
	if (*(unsigned char *)((char *)self + 0x50) == 1) return;
	*(unsigned char *)((char *)self + 0x50) = 1;
}

MAKE_HOOK(NetworkFlareGun_ReloadFlare, ADDR_NetworkFlareGun_ReloadFlare, void, void *self)
{ 
}

MAKE_HOOK(FireFlare_Update, ADDR_FireFlare_Update , void, void *self)
{ 
	FireFlare_Update(self);
	if (*(unsigned char *)((char *)self + 0x60) == 1) return;
	*(unsigned char *)((char *)self + 0x60) = 1;
}

MAKE_HOOK(FireFlare_ReloadFlare, ADDR_FireFlare_ReloadFlare, void, void *self)
{ 
}	
	
__attribute__((constructor)) void lib_main()
{
    LOGI("FlareMod-plugin loaded!");
	aoq_init();
	
    aoqmm_register("libflaremod.so", "FlareMod", "1.1.0", "{x}Jester",
                   "sets can fire to true every frame.");

    aoqmm_ensure_config("libflaremod.so",
        "{\n"
        "  \"entries\": [\n" 
        "  ]\n"
        "}\n"
    );
	INSTALL_HOOK(NetworkFlareGun_ReloadFlare);
	INSTALL_HOOK(FireFlare_ReloadFlare);
	INSTALL_HOOK(NetworkFlareGun_Update);
	INSTALL_HOOK(FireFlare_Update);
	LOGI("hooks installed!");
}
