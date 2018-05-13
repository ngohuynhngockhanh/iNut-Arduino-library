#include <iNut.h>
iNut sensor;
void setup() {
  Serial.begin(9600); //bật baudrate ở mức 9600

  Serial.println("San sang nhan lenh");
  
  //Khai báo số lượng luồn cảm biến
  sensor.setup(8); //Sẽ có 08 luồn cảm biến

}

void loop() {


  for (byte i = 0; i < 8; i++) {
    unsigned long value = random() % 10000; //trả về giá trị random - "hên xui" - tự do
    sensor.setValue(i, value); //gán giá trị trên vào luồn
  }
  sensor.loop();
}


