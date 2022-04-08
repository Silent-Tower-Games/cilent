#pragma once

typedef struct Cilent_Config {
    char language[6];
} Cilent_Config;

Cilent_Config Cilent_Config_Create(Cilent_Config configDefault);
