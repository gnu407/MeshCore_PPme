#include "Wardriver.h"
#include "MeshService.h"

void initWardriver() {
    // Meshtastic already initialized SPIFFS at boot.
    // DO NOT call SPIFFS.begin() here, or it will clash and crash.
    Serial.println("Standalone Wardriver Active - Using Native SPIFFS");
}

void logNodeToFS(const meshtastic_NodeInfo* node) {
    if (!node || !node->has_user) return;
    
    // FILE_APPEND will create the file if it doesn't exist
    File file = SPIFFS.open(LOG_FILE_PATH, FILE_APPEND);
    if (!file) {
        Serial.println("FS Error: Could not open wardrive.csv");
        return;
    }

    uint32_t now = getRTC(); 
    
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
    
    service->sendToMesh(&pkt);
}
