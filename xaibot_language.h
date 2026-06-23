#ifndef XAIBOT_LANGUAGE_H
#define XAIBOT_LANGUAGE_H

#ifdef __cplusplus
extern "C" {
#endif

// Simple localization API

// Initialize language system (called after config init)
void XaibotLanguage_Init(const char *language_code);

// Get localized string by key. Returns pointer to internal static buffer.
// Safe fallback to Turkish if key missing.
const char *XaibotLanguage_Get(const char *key);

#ifdef __cplusplus
}
#endif

#endif // XAIBOT_LANGUAGE_H
