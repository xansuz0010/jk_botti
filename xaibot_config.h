#ifndef XAIBOT_CONFIG_H
#define XAIBOT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// Simple XAIBot config API for Phase 1

void XaibotConfig_Init(void);
void XaibotConfig_Reload(void);
int  XaibotConfig_IsEnabled(void); // AI chat overall
const char *XaibotConfig_GetLanguage(void);

#ifdef __cplusplus
}
#endif

#endif // XAIBOT_CONFIG_H
