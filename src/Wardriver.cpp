#include "Wardriver.h"
#include "MeshService.h"

void initWardriver() {
    Serial.println("Standalone Wardriver Active - Using Native SPIFFS");
}

void logNodeToFS(const meshtastic_NodeInfo* node) {
    if (!node || !node->has_user) return;
    
    File file = SPIFFS.open(LOG_FILE_PATH, FILE_APPEND);
    if (!file) return;

    uint32_t now = getRTC(); 
    file.printf("%u,%08x,%s,%d,%f\n", 
        now, node->num, node->user.long_name, node->user.role, node->snr);
    file.close();
}

void dumpLogToSerial() {
    Serial.println("\n--- WARDRIVE DATA START ---");
    File file = SPIFFS.open(LOG_FILE_PATH, "r");
    if (!file) {
        Serial.println("No data found or file error.");
    } else {
        while (file.available()) {
            Serial.write(file.read());
        }
        file.close();
    }
    Serial.println("\n--- WARDRIVE DATA END ---");
    
    // Optional: Uncomment to delete file after reading
    // SPIFFS.remove(LOG_FILE_PATH); 
}

void loopWardriver() {
    // 1. Handle Active Pinging
    static uint32_t lastPing = 0;
    if (millis() - lastPing > PING_INTERVAL_MS) {
        meshtastic_MeshPacket pkt = meshtastic_MeshPacket_init_default;
        pkt.decoded.portnum = meshtastic_PortNum_NODEINFO_APP;
        pkt.decoded.payload.size = 0; 
        pkt.want_ack = false;
        service->sendToMesh(&pkt);
        lastPing = millis();
    }

    // 2. Handle CLI Dump Command
    static String inputStr = "";
    while (Serial.available()) {
        char c = (char)Serial.read();
        inputStr += c;
        if (c == '\n' || c == '\r') {
            if (inputStr.indexOf("?wardrive") >= 0) {
                dumpLogToSerial();
            }
            inputStr = ""; // Clear buffer
        }
    }
}
