#include <string.h>
#include <stdlib.h>

#include "xaibot_language.h"
#include "xaibot_config.h"
#include "util.h"

// Minimal localization implementation for Phase 1
// Turkish default, English optional

#define MAX_KEY_LEN 128

static char g_language_code[8] = "tr";

typedef struct {
    const char *key;
    const char *tr;
    const char *en;
} lang_entry_t;

static const lang_entry_t lang_table[] = {
    {"menu.title", "[XANSUZ AI BOT PANEL]", "[XANSUZ AI BOT PANEL]"},
    {"menu.category.ai_chat", "Yapay Zeka Sohbeti", "AI Chat"},
    {"menu.category.groq", "Groq API", "Groq API"},
    {"menu.category.personality", "Bot Kişilikleri", "Personality"},
    {"menu.category.emotion", "Duygu ve Atmosfer", "Emotion & Atmosphere"},
    {"menu.category.banter", "Bot Dedikodusu", "Bot Banter"},
    {"menu.category.dynamic", "Dinamik Zorluk", "Dynamic Difficulty"},
    {"menu.category.gameplay", "Oynanış Profili", "Gameplay Profile"},
    {"menu.category.spam", "Sohbet Spam Koruması", "Spam Protection"},
    {"menu.category.language", "Dil Ayarları", "Language Settings"},
    {"menu.category.debug", "Debug ve Tanılama", "Debug & Diagnostics"},
    {"menu.category.status", "Durum / Hakkında", "Status / About"},

    {"menu.option.ai_chat_on", "Yapay Zeka Sohbeti: %s", "AI Chat: %s"},
    {"menu.option.local_fallback_on", "Yerel Sohbet Yedeği: %s", "Local Fallback Chat: %s"},
    {"menu.option.groq_enabled", "Groq: %s", "Groq: %s"},
    {"menu.option.reload_groq", "Groq anahtarını yeniden yükle", "Reload Groq key"},
    {"menu.hint.bind_p", "AI bot menüsünü P tuşuna bağlamak için: bind p \"xaibot_menu\"", "To bind the AI bot menu to P: bind p \"xaibot_menu\""},

    {"groq.status.connected", "Groq: Bağlı", "Groq: Connected"},
    {"groq.status.missing", "Groq: Anahtar Yok", "Groq: Missing Key"},
    {"groq.status.invalid", "Groq: Hatalı Anahtar", "Groq: Invalid Key"},
    {"groq.status.offline", "Groq: Çevrimdışı", "Groq: Offline"},
    {"groq.status.waiting", "Groq: Köprü Bekleniyor", "Groq: Waiting for Bridge"},

    // fallback messages (short examples)
    {"fallback.kill.1", "tamam abi şimdi kişisel oldu", "okay, I\'m hunting you now"},
    {"fallback.kill.2", "crossbow ile duvara astın ya ayıp", "nah that crossbow pin was illegal"},
    {"fallback.death.1", "gauss beni başka evrene yolladı resmen", "bro that gauss shot sent me to another dimension"},
    {"fallback.lowhealth.1", "canım az, biri cover atsın", "cover me, I\'m low"},
    {"fallback.idle.1", "planım yok ama özgüvenim var", "don\'t worry boys, I have absolutely no plan"},
    {"fallback.groq_offline.1", "Groq anahtarı bulunamadı, yerel mesajlar kullanılacak", "Groq key missing, local messages will be used"},

    {"status.bots_count", "Bot sayısı: %d", "Bots: %d"},
    {"status.language", "Dil: %s", "Language: %s"},

    {NULL, NULL, NULL}
};

void XaibotLanguage_Init(const char *language_code)
{
    if (!language_code || !language_code[0])
        strncpy(g_language_code, "tr", sizeof(g_language_code));
    else
    {
        if (!stricmp(language_code, "en"))
            strncpy(g_language_code, "en", sizeof(g_language_code));
        else
            strncpy(g_language_code, "tr", sizeof(g_language_code));
    }
}

const char *XaibotLanguage_Get(const char *key)
{
    static char buf[512];
    if (!key) return "";

    for (int i = 0; lang_table[i].key != NULL; i++)
    {
        if (!strcmp(lang_table[i].key, key))
        {
            if (!stricmp(g_language_code, "en") && lang_table[i].en)
                return lang_table[i].en;
            if (lang_table[i].tr)
                return lang_table[i].tr;
            break;
        }
    }

    // fallback: return key itself if not found to help debugging
    safevoid_snprintf(buf, sizeof(buf), "%s", key);
    return buf;
}
