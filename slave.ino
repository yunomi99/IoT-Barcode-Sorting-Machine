//--------------Library yang dipakai-------------//
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

//Nama Wifi
#define WIFI_SSID "KONTRAKAN R8"
#define WIFI_PASSWORD "sapidermansultan"  

// Isikan sesuai pada Firebase
#define FIREBASE_HOST "https://konveyor-automation-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "SeQ6iAoAEnN653oMuBwfUAArxDvnMMt46oOifNYS"

// mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;

char dataMasuk = '0';
int updateDataA, updateDataB;

void setup(){
  Serial.begin(9600);
  //memulai Koneksi ke Wifi sesuai SSID dan Pass
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Memulai Komunikasi dengan firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //Membaca data Terakhir
  for(int i = 0; i<=5; i++){
      readLastData();
      delay(200);
    }
}

void loop(){
    if(Serial.available()) {
      int intBarcode = Serial.read();
      dataMasuk = (char)intBarcode;
      Serial.println(dataMasuk);
      if(dataMasuk == 'A'){updateDataA = updateDataA +1;}
      else if(dataMasuk == 'B'){updateDataB = updateDataB +1;}
    }

  if(Firebase.setFloat(firebaseData, "/barang_a", updateDataA)){
      Serial.println("Data Berat terkirim");
    } 
  else{
    Serial.println("Data berat tidak terkirim");
    Serial.println("Karena: " + firebaseData.errorReason());
    } 

  if (Firebase.setFloat(firebaseData, "/barang_b", updateDataB)){
      Serial.println("Data tetesan terkirim");
    } 
  else{
    Serial.println("Data tetesan tidak terkirim");
    Serial.println("Karena: " + firebaseData.errorReason());
    } 
}


void readLastData(){
   if(Firebase.getString(firebaseData, "/barang_a")){
      if(firebaseData.dataType() == "string"){
        String lastDataA = firebaseData.stringData();
        updateDataA = lastDataA.toInt();
        }
        Serial.println("Data A = " + updateDataA);
      }

   if(Firebase.getString(firebaseData, "/barang_b")){
      if(firebaseData.dataType() == "string"){
        String lastDataB = firebaseData.stringData();
        updateDataB = lastDataB.toInt();
        }
        Serial.println("Data B = " + updateDataB);
      }
  }
