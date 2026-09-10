#include "Wardriver.h"
#include "MeshService.h"

void initWardriver() {
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }
    Serial.println("Standalone Wardriver Active");
}

void logNodeToFS(const meshtastic_NodeInfo* node) {
    if (!node || !node->has_user) return;
    
    File file = LittleFS.open(LOG_FILE_PATH, FILE_APPEND);
    if (!file) return;

    uint32_t now = getRTC(); // Returns 0 if BLE hasn't synced time yet
    
    // Logs: Timestamp, NodeNum, Name, Role, SNR
    file.printf("%u,%08x,%s,%d,%f\n", 
        now,
        node->num,
        node->user.long_name,
        node->user.role,
        node->snr
    );
    file.close();
}

void sendActivePing() {
    meshtastic_MeshPacket pkt = meshtastic_MeshPacket_init_default;
    pkt.decoded.portnum = meshtastic_PortNum_NODEINFO_APP;
    pkt.decoded.payload.size = 0; 
    pkt.want_ack = false;
    
    // Meshtastic's internal router automatically respects Time-On-Air limits
    service->sendToMesh(&pkt);
}
