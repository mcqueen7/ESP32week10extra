#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "寇格魔鬥凱薩科加斯溫";
const char* password = "87878787";
const char* host = "data.epa.gov.tw/";
const char* resource = "api/v1/aqx_p_02?limit=1000&api_key=9be7b239-557b-4c10-9775-78cadfc555e9&sort=ImportDate%20desc&format=json";
char jsonRead[600]; //讀取response後儲存JSON資料的變數，必須是全域變數
String json;

const char* root_ca = \
                      "-----BEGIN CERTIFICATE-----\n"\
                      "MIIFsDCCA5igAwIBAgIQFci9ZUdcr7iXAF7kBtK8nTANBgkqhkiG9w0BAQUFADBe\n"\
                      "MQswCQYDVQQGEwJUVzEjMCEGA1UECgwaQ2h1bmdod2EgVGVsZWNvbSBDby4sIEx0\n"\
                      "ZC4xKjAoBgNVBAsMIWVQS0kgUm9vdCBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTAe\n"\
                      "Fw0wNDEyMjAwMjMxMjdaFw0zNDEyMjAwMjMxMjdaMF4xCzAJBgNVBAYTAlRXMSMw\n"\
                      "IQYDVQQKDBpDaHVuZ2h3YSBUZWxlY29tIENvLiwgTHRkLjEqMCgGA1UECwwhZVBL\n"\
                      "SSBSb290IENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIICIjANBgkqhkiG9w0BAQEF\n"\
                      "AAOCAg8AMIICCgKCAgEA4SUP7o3biDN1Z82tH306Tm2d0y8U82N0ywEhajfqhFAH\n"\
                      "SyZbCUNsIZ5qyNUD9WBpj8zwIuQf5/dqIjG3LBXy4P4AakP/h2XGtRrBp0xtInAh\n"\
                      "ijHyl3SJCRImHJ7K2RKilTza6We/CKBk49ZCt0Xvl/T29de1ShUCWH2YWEtgvM3X\n"\
                      "DZoTM1PRYfl61dd4s5oz9wCGzh1NlDivqOx4UXCKXBCDUSH3ET00hl7lSM2XgYI1\n"\
                      "TBnsZfZrxQWh7kcT1rMhJ5QQCtkkO7q+RBNGMD+XPNjX12ruOzjjK9SXDrkb5wdJ\n"\
                      "fzcq+Xd4z1TtW0ado4AOkUPB1ltfFLqfpo0kR0BZv3I4sjZsN/+Z0V0OWQqraffA\n"\
                      "sgRFelQArr5T9rXn4fg8ozHSqf4hUmTFpmfwdQcGlBSBVcYn5AGPF8Fqcde+S/uU\n"\
                      "WH1+ETOxQvdibBjWzwloPn9s9h6PYq2lY9sJpx8iQkEeb5mKPtf5P0B6ebClAZLS\n"\
                      "nT0IFaUQAS2zMnaolQ2zepr7BxB4EW/hj8e6DyUadCrlHJhBmd8hh+iVBmoKs2pH\n"\
                      "dmX2Os+PYhcZewoozRrSgx4hxyy/vv9haLdnG7t4TY3OZ+XkwY63I2binZB1NJip\n"\
                      "NiuKmpS5nezMirH4JYlcWrYvjB9teSSnUmjDhDXiZo1jDiVN1Rmy5nk3pyKdVDEC\n"\
                      "AwEAAaNqMGgwHQYDVR0OBBYEFB4M97Zn8uGSJglFwFU5Lnc/QkqiMAwGA1UdEwQF\n"\
                      "MAMBAf8wOQYEZyoHAAQxMC8wLQIBADAJBgUrDgMCGgUAMAcGBWcqAwAABBRFsMLH\n"\
                      "ClZ87lt4DJX5GFPBphzYEDANBgkqhkiG9w0BAQUFAAOCAgEACbODU1kBPpVJufGB\n"\
                      "uvl2ICO1J2B01GqZNF5sAFPZn/KmsSQHRGoqxqWOeBLoR9lYGxMqXnmbnwoqZ6Yl\n"\
                      "PwZpVnPDimZI+ymBV3QGypzqKOg4ZyYr8dW1P2WT+DZdjo2NQCCHGervJ8A9tDkP\n"\
                      "JXtoUHRVnAxZfVo9QZQlUgjgRywVMRnVvwdVxrsStZf0X4OFunHB2WyBEXYKCrC/\n"\
                      "gpf36j36+uwtqSiUO1bd0lEursC9CBWMd1I0ltabrNMdjmEPNXubrjlpC2JgQCA2\n"\
                      "j6/7Nu4tCEoduL+bXPjqpRugc6bY+G7gMwRfaKonh+3ZwZCc7b3jajWvY9+rGNm6\n"\
                      "5ulK6lCKD2GTHuItGeIwlDWSXQ62B68ZgI9HkFFLLk3dheLSClIKF5r8GrBQAuUB\n"\
                      "o2M3IUxExJtRmREOc5wGj1QupyheRDmHVi03vYVElOEMSyycw5KFNGHLD7ibSkNS\n"\
                      "/jQ6fbjpKdx2qcgw+BRxgMYeNkh0IkFch4LoGHGLQYlE535YW6i4jRPpp2zDR+2z\n"\
                      "Gp1iro2C6pSe3VkQw63d4k3jMdXH7OjysP6SHhYKGvzZ8/gntsm+HbRsZJB/9OTE\n"\
                      "W9c3rkIO3aQab3yIVMUWbuF6aC74Or8NpDyJO3inTmODBCEIZ43ygknQW/2xzQ+D\n"\
                      "hNQ+IIX3Sj0rnP0qCglN6oH4EZw=\n"\
                      "-----END CERTIFICATE-----\n";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
}

void loop() {
  int value1 = 25;
  int value2 = 50;
  int value3 = 69;
  //以char*格式儲存回傳的json
     getdata();
    
    Serial.println(json);

    //將字串轉換為JSON，儲存在doc中
    DynamicJsonDocument doc(16384);
    deserializeJson(doc, json);
    
     String location = doc["records"][2]["Site"]; 
     String PM = doc["records"][2]["PM25"];
     double PMD = PM.toDouble();
     String sta,level;
     Serial.print("*** ");
    Serial.print(location);
    Serial.println(" ***");
    Serial.print("PM2.5: ");
    Serial.println(PMD);
    
   if(PMD<=50){
      sta="%e8%89%af%e5%a5%bd";
      level="1";
    }
    else if(PMD<=100){
      sta="%e6%99%ae%e9%80%9a";
      level="2";
    }
    else if(PMD<=150){
      sta="%e5%b0%8d%e6%95%8f%e6%84%9f%e6%97%8f%e7%be%a4%e4%b8%8d%e5%81%a5%e5%ba%b7";
      level="3";
    }
    else if(PMD<=200){
      sta="%e5%b0%8d%e6%89%80%e6%9c%89%e6%97%8f%e7%be%a4%e4%b8%8d%e5%81%a5%e5%ba%b7";
      level="4";
    }
    else if(PMD<=300){
      sta="%e9%9d%9e%e5%b8%b8%e4%b8%8d%e5%81%a5%e5%ba%b7";
      level="5";
    }
    else {
      sta="%e5%8d%b1%e5%ae%b3";
      level="6";
    }
    Serial.print("LEVEL5: ");
    Serial.println(level);
    Serial.println("--------------------"); 

    
  ClientRequest(String("Songshan"), String(PM), String(sta));  //Client傳送資料
  
  delay(3000);
}  
void getdata(){
   HTTPClient hc;  //建立Client物件
  hc.begin("https://data.epa.gov.tw/api/v1/aqx_p_02?limit=1000&api_key=9be7b239-557b-4c10-9775-78cadfc555e9&sort=ImportDate%20desc&format=json"
               , root_ca);
  int httpCode = hc.GET();   //Make the request

  if (httpCode > 0) { //Check for the returning code

    json = hc.getString();
    Serial.println(httpCode);
    
  }
  else {
    Serial.println("Error on HTTP request");
  }
  hc.end(); //Free the resources
  //回傳
}
void ClientRequest(String value1, String value2, String value3){
  const char* host = "maker.ifttt.com"; //IFTTT server網址
  const char* event = "weatherHW";  //IFTTT事件名稱
  const char* apiKey = "pIiy_TYyEb_ML2t523T_OM78MlH6tN2aDLv1oDciHlc";  //IFTTT Applet key
  WiFiClient client;  //建立Client物件
  const int httpPort = 80;  //預設通訊阜80
  String JsonString = "";  //此範例不會用到

  //Client連結Server
  if (client.connect(host, httpPort)) {
    
    //Webhook API
    String url = "/trigger/" + String(event) + "/with/key/" + String(apiKey);
    //Query String
    url += "?value1=" + value1 + "&value2=" + value2+ "&value3=" + value3;
 Serial.println(host+url);
    //Client傳送
    client.println(String("POST ") + url + " HTTP/1.1");
    client.println(String("Host: ") + host); 
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(JsonString.length());
    client.println();
    client.println(JsonString);

    //等待5秒，每0.1秒偵測一次是否有接收到response資料  
    int timeout = 0;
    while(!client.available() && (timeout++ <= 50)){
      delay(100);
    }
  
    //如果無回應
    if(!client.available()) {
      Serial.println("No response...");
    }
    //用while迴圈一字一字讀取Response
    while(client.available()){
      Serial.write(client.read());
    }

    //停止Client
    client.stop(); 
  } 
}
