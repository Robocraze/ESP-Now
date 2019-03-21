/*
 << This Device Slave >>

     Sample Serial log:
       AP Config Success. Broadcasting with AP: Slave:3C:71:BF:52:D1:14
       AP MAC: 3C:71:BF:52:D1:15
       ESPNow Init Success
       Last Packet Recv from: 24:0a:c4:0a:6f:08
       Last Packet Recv Data: 1

*/

#include <esp_now.h>
#include <WiFi.h>

#define GPIO 15
#define CHANNEL 1
#define SENDCHANNEL 1
#define WIFI_DEFAULT_CHANNEL 3

uint8_t gpioStatus = 0;
esp_now_peer_info_t peer;

void initESPNow();
void configDeviceAP();
void addPeer(uint8_t *peerMacAddress);
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);   

void setup()
{
  Serial.begin(115200);
  
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  
  // configure device AP mode
  configDeviceAP();
  
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); 
  Serial.println(WiFi.softAPmacAddress());
  
  // Init ESPNow with a fallback logic
  initESPNow();
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataRecv);

  pinMode(GPIO, OUTPUT);
}

void loop()
{
  digitalWrite(GPIO, gpioStatus);
  delay(10);
}

// Init ESP Now with fallback
void initESPNow()
{
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else
  {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

// config AP SSID
void configDeviceAP()
{
  String Prefix = "Slave:";
  String Mac = WiFi.macAddress();
  String SSID = Prefix + Mac;
  String Password = "123456789";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  if (!result)
  {
    Serial.println("AP Config failed.");
  }
  else
  {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

// Add Master as a peer 
void addPeer(uint8_t *peerMacAddress)
{
    peer.channel = SENDCHANNEL;
    peer.ifidx = WIFI_IF_AP;
    peer.encrypt = 0;
    
    memcpy(peer.peer_addr, peerMacAddress, 6);
    esp_err_t addStatus = esp_now_add_peer(&peer);
    if (addStatus == ESP_OK)
    {
      // Pair success
      Serial.println("Pair success");
    } 
    else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT)
    {
      // How did we get so far!!
      Serial.println("ESPNOW Not Init");
    } 
    else if (addStatus == ESP_ERR_ESPNOW_ARG)
    {
      Serial.println("Add Peer - Invalid Argument");
    } 
    else if (addStatus == ESP_ERR_ESPNOW_FULL)
    {
      Serial.println("Peer list full");
    } 
    else if (addStatus == ESP_ERR_ESPNOW_NO_MEM)
    {
      Serial.println("Out of memory");
    } 
    else if (addStatus == ESP_ERR_ESPNOW_EXIST)
    {
      Serial.println("Peer Exists");
    } 
    else
    {
      Serial.println("Not sure what WENT WRONG");
    }
}

// callback when data is sent from Master to Slave
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
           
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// callback when data is recv from Master
void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{  
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
           
  Serial.print("Last Packet Recv from: ");
  Serial.println(macStr);
  Serial.print("Last Packet Recv Data: ");
  Serial.println(*data);
  Serial.println("");

  gpioStatus = *data;
}
