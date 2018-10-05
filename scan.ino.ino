#include <LGATT.h>
#include <LGATTClient.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
//#include <HttpClient.h>
#define WIFI_AP "X9009"
#define WIFI_PASSWORD "123456789"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.(wifi authentication method)
#define SITE_URL "www.google.com"
LWiFiClient content;

  int num=0;
  long double uuid;
  long double buff[25];
  static LGATTUUID test_uuid("B4B4B4B4-B4B4-B4B4-B4B4-B4B4B4B4B4B5");
  LGATTClient c;// object created is  c
  LGATTDeviceInfo info={0};
 // String buffer;
  
boolean wifi_status(LWifiStatus ws){
    switch(ws){
      case LWIFI_STATUS_DISABLED:
        return false;
      break;
      case LWIFI_STATUS_DISCONNECTED:
        return false;
      break;
      case LWIFI_STATUS_CONNECTED:
        return true;
      break;
    }
    return false;
  }

  
/*void scanningeir()
 {
  upload_to_MCS(10);//info.eir
 }
  
  
void upload_to_MCS(int value)
  {
   while(!content.connect(SITE_URL,80))
   {
    Serial.printf(".");
    delay(500);
   }
   content.println("POST /mcs/v2/devices/Dn8l7QFb/datapoints.csv HTTP/1.1");
   String data = "BLE!,,AA33FF ";//+String value";
   int dataLength = data.length();
   content.println("Host: api.mediatek.com");
   content.println("deviceKey: 8XaVYLGYaHgeaNkt");
   content.print("Content-Length: ");
   content.println(dataLength);
   content.println("Content-Type: text/csv");
   content.println("Connection: close");
   content.println();
   content.println(data);
  }

*/
  void setup() 
  {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  Serial.println("LGATTC setup.");
  if (c.begin(test_uuid))
    {
        Serial.println("success to register.");
    }
    else
    {
        Serial.println("failed to register.");
    }
   LWiFi.begin();
   int numOfAP = LWiFi.scanNetworks();
   Serial.print("number of WiFi AP found:");
   Serial.println(numOfAP);
   for(int i=0;i<numOfAP;i++)
   {
     Serial.print(i);
     Serial.print(") ");
     Serial.print(LWiFi.SSID(i));
   }
   // keep retrying until connected to AP
   LWifiStatus ws = LWiFi.status();
   boolean status = wifi_status(ws);
   if(!status)
   {
     Serial.println("Connecting to AP");
     while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)));
     Serial.print("Connected to ");
     Serial.print(LWiFi.SSID());
   }
   // keep retrying until connected to website
  /* Serial.println("Connecting to WebSite");
   while (0 == content.connect(SITE_URL, 80))
   {
     Serial.println("Re-Connecting to WebSite");
     delay(100);
   }*/
 
  }

void loop()
{
  // put your main code here, to run repeatedly:
      LGATTDeviceInfo info = { 0 };
   char buffer [sizeof(int)*8+1];
      num = c.scan(5); // timeout is set to 10 seconds.The LGATTClient scan function scans for Bluetooth 4.0 devices. 
      Serial.printf("scan num [%d] a", num);
      for (int i = 0; i < num; i++)
      {
       boolean res=c.getScanResult(i, info);
       if(res)
       {
        // Serial.printf("[LGATTAdress ino]rssi= %d\n",info.rssi);
         if(info.bd_addr.addr[5]==85)
          {Serial.printf("RemoteRSSI address : [%x:%x:%x:%x:%x:%x]",info.bd_addr.addr[5], info.bd_addr.addr[4], info.bd_addr.addr[3], info.bd_addr.addr[2], info.bd_addr.addr[1], info.bd_addr.addr[0]);
         Serial.println();
         String payload = "{";
         for(int j=0;j<info.eir_len;j++)
         {
         sprintf(buffer,"%02x",info.eir[j]);
         //& 0xFF
         payload += buffer;
         //Serial.printf("hexadecimal: %s\n",buffer);
         }
          payload += "}";
         Serial.print(payload);
       }
       }
      }
}
