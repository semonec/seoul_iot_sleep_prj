#ifdef TARGET_K64F
#include "mbed.h"
#include "ESP8266Interface.h"
#include "TCPSocket.h"

Serial pc2(USBTX, USBRX);
ESP8266Interface wifi(D1, D0);

const char *sec2str(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}
 
void scan_demo(WiFiInterface *wifi)
{
    WiFiAccessPoint *ap;
 
    pc2.printf("Scan:\r\n");
 
    int count = wifi->scan(NULL,0);
 
    /* Limit number of network arbitrary to 15 */
    count = count < 15 ? count : 15;
 
    ap = new WiFiAccessPoint[count];
    count = wifi->scan(ap, count);
    for (int i = 0; i < count; i++)
    {
      pc2.printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\r\n", ap[i].get_ssid(),
               sec2str(ap[i].get_security()), ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
               ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5], ap[i].get_rssi(), ap[i].get_channel());
    }
    pc2.printf("%d networks available.\r\n", count);
 
    delete[] ap;
}
 
void http_demo(NetworkInterface *net)
{
    TCPSocket socket;
 
    pc2.printf("Sending HTTP request to www.arm.com...\r\n");
 
    // Open a socket on the network interface, and create a TCP connection to www.arm.com
    socket.open(net);
    socket.connect("www.arm.com", 80);
 
    // Send a simple http request
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: www.arm.com\r\n\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    pc2.printf("sent %d [%.*s]\r\n", scount, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);
 
    // Recieve a simple http response and print out the response line
    char rbuffer[64];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    pc2.printf("recv %d [%.*s]\r\n", rcount, strstr(rbuffer, "\r\n")-rbuffer, rbuffer);
 
    // Close the socket to return its memory and bring down the network interface
    socket.close();
}

void connect_wifi() {
  pc2.printf("WiFi example\r\n\r\n");
  
  // Scan for available access points 
  //scan_demo(&wifi);

  printf("\r\nConnecting...\r\n");
  pc2.printf("\r\nConnecting222222...\r\n");
  
  int ret = wifi.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
  if (ret != 0) {
      pc2.printf("\r\nConnection error\r\n");
      return;
  }

  printf("Success\r\n\r\n");
  printf("MAC: %s\r\n", wifi.get_mac_address());
  printf("IP: %s\r\n", wifi.get_ip_address());
  printf("Netmask: %s\r\n", wifi.get_netmask());
  pc2.printf("Gateway: %s\r\n", wifi.get_gateway());
  pc2.printf("RSSI: %d\r\n\r\n", wifi.get_rssi());

  //http_demo(&wifi);

  //wifi.disconnect();

  pc2.printf("\r\nDone\r\n");
  return;
}
#endif