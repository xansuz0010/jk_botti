#ifndef XAIBOT_GROQ_KEY_H
#define XAIBOT_GROQ_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    XAIBOT_GROQ_MISSING = 0,
    XAIBOT_GROQ_INVALID,
    XAIBOT_GROQ_VALID,
    XAIBOT_GROQ_OFFLINE
} xaibot_groq_status_t;

void XaibotGroq_Init(void);
xaibot_groq_status_t XaibotGroq_GetStatus(void);
const char *XaibotGroq_StatusText(void); // localized via language module

#ifdef __cplusplus
}
#endif

#endif // XAIBOT_GROQ_KEY_H
