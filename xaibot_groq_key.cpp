#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xaibot_groq_key.h"
#include "xaibot_config.h"
#include "xaibot_language.h"
#include "util.h"

// Phase 1: groq-api_key.txt loader (Base64 text only). No network calls.

static xaibot_groq_status_t g_status = XAIBOT_GROQ_MISSING;

static void trim_whitespace(char *s)
{
    if (!s) return;
    // trim BOM (UTF-8)
    if ((unsigned char)s[0] == 0xEF && (unsigned char)s[1] == 0xBB && (unsigned char)s[2] == 0xBF)
        memmove(s, s+3, strlen(s+3)+1);
    // leading
    char *p = s;
    while (*p && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')) p++;
    if (p != s) memmove(s, p, strlen(p)+1);
    // trailing
    int len = strlen(s);
    while (len > 0 && (s[len-1] == ' ' || s[len-1] == '\t' || s[len-1] == '\r' || s[len-1] == '\n')) s[--len] = '\0';
}

// small base64 decode: returns decoded length or -1
static int b64_val(char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

static int base64_decode(const char *in, unsigned char *out, int outlen)
{
    int len = 0;
    int val = 0, valb = -8;
    for (const char *p = in; *p; ++p) {
        int c = b64_val(*p);
        if (c == -1) {
            if (*p == '=') break;
            if (*p == '\r' || *p == '\n' || *p == ' ' || *p == '\t') continue;
            return -1;
        }
        val = (val<<6) + c;
        valb += 6;
        if (valb >= 0) {
            if (len >= outlen) return -1;
            out[len++] = (unsigned char)((val>>valb)&0xFF);
            valb -= 8;
        }
    }
    return len;
}

static int try_load_path(const char *path, unsigned char *decoded, int decoded_size)
{
    FILE *fp = fopen(path, "r");
    if (!fp) return 0;
    char buf[4096];
    size_t r = fread(buf, 1, sizeof(buf)-1, fp);
    fclose(fp);
    buf[r] = 0;
    trim_whitespace(buf);
    if (!buf[0]) return 0;
    int dlen = base64_decode(buf, decoded, decoded_size);
    if (dlen <= 0) return -1;
    if (dlen < 8) return -1;
    // success; do NOT log the decoded key
    return 1;
}

void XaibotGroq_Init(void)
{
    unsigned char decoded[2048];
    g_status = XAIBOT_GROQ_MISSING;

    // 1) current dir
    if (try_load_path("groq-api_key.txt", decoded, sizeof(decoded)) == 1) { g_status = XAIBOT_GROQ_VALID; return; }

    // 2) addons/jk_botti/
    char path[512];
    UTIL_BuildFileName_N(path, sizeof(path), "addons/jk_botti/groq-api_key.txt", NULL);
    {
        int r = try_load_path(path, decoded, sizeof(decoded));
        if (r == 1) { g_status = XAIBOT_GROQ_VALID; return; }
        if (r == -1) { g_status = XAIBOT_GROQ_INVALID; return; }
    }

    // 3) config override
    const char *cfg = XaibotConfig_GroqKeyPath();
    if (cfg && cfg[0]) {
        int r = try_load_path(cfg, decoded, sizeof(decoded));
        if (r == 1) { g_status = XAIBOT_GROQ_VALID; return; }
        if (r == -1) { g_status = XAIBOT_GROQ_INVALID; return; }
    }

    g_status = XAIBOT_GROQ_MISSING;
}

xaibot_groq_status_t XaibotGroq_GetStatus(void) { return g_status; }

const char *XaibotGroq_StatusText(void)
{
    if (g_status == XAIBOT_GROQ_VALID) return XaibotLanguage_Get("groq.status.connected");
    if (g_status == XAIBOT_GROQ_INVALID) return XaibotLanguage_Get("groq.status.invalid");
    if (g_status == XAIBOT_GROQ_OFFLINE) return XaibotLanguage_Get("groq.status.offline");
    return XaibotLanguage_Get("groq.status.missing");
}
