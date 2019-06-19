#include <iNut.h>
iNut sensor;

#define RELAY_COUNT 8
#define WRITE_COMMAND 200
int relay[] = {2,3,4,5,6,7,8,9};
void setup() {
  Serial.begin(9600); //bật baudrate ở mức 9600

  Serial.println("San sang nhan lenh");

  for (int i = 0; i < RELAY_COUNT; i++) {
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], HIGH);
  }
  
  //Khai báo số lượng luồn cảm biến
  sensor.setup(8); //Sẽ có 08 luồn cảm biến

  sensor.addCommand("ON", onFunction);
  sensor.addCommand("OFF", offFunction);
  sensor.addCommand("ONLINE", onlineFunction);
  sensor.addCommand("OFFLINE", offlineFunction);
  sensor.addCommand("TIME", timeFunction);
  sensor.setDefaultHandler(defaultHandler);
}
void timeFunction() {
  //Lấy dữ liệu để in ra thời gian không cần module RTC
  int year = atoi(sensor.next());
  int month = atoi(sensor.next());
  int day = atoi(sensor.next());
  int hour = atoi(sensor.next());
  int minute = atoi(sensor.next());
  int second = atoi(sensor.next());
  Serial.print("Bay gio la: ");
  Serial.print(year);
  Serial.print('/');
  Serial.print(month);
  Serial.print('/');
  Serial.print(day);
  Serial.print(' ');
  Serial.print(hour);
  Serial.print(':');
  Serial.print(minute);
  Serial.print(':');
  Serial.print(second);
  Serial.println('!');
}

void onlineFunction() {
  Serial.println("Thiet bi dang ket noi Internet rat ok");
}

void offlineFunction() {
  Serial.println("Thiet bi dang mat ket noi");
}

void onFunction() {
  
  int bit_index = atoi(sensor.next()) - WRITE_COMMAND;
  
  Serial.print("Bat relay thu ");
  Serial.println(bit_index);
  
  digitalWrite(relay[bit_index], LOW);
}

void offFunction() {
  int bit_index = atoi(sensor.next())  - WRITE_COMMAND;
  Serial.print("Tat relay thu ");
  Serial.println(bit_index);
  digitalWrite(relay[bit_index], HIGH);
}

void loop() {

  //sensor.setValue(0, digitalRead(LED)); //đọc giá trị của đèn LED


  //Sử dụng luồn cảm biến số 0 và bật các bit bên trong luồn để điều khiển thiết 
  for (int i = 0; i < RELAY_COUNT; i++) {
    int value = digitalRead(relay[i]);
    //đồng bộ trạng thái iNut
    if (value == LOW) {
      sensor.turnOn(0, i); 
    } else {
      sensor.turnOff(0, i); 
    }
  }

  
  //giá trị cho các lbịuồn từ 1-7 là giá trị random
  ///Tại sao lại sử dụng random? Vì đây là thử nghiệm, bạn có thể thay thế giá trị random bằng một giá trị cảm biến hoặc trị số mà bạn mong muốn đồng bộ lên Internet!
  for (byte i = 1; i <= 7; i++) {
    unsigned long value = random() % 10000; //trả về giá trị random - "hên xui" - tự do
    sensor.setValue(i, value); //gán giá trị trên vào luồn
  }
  sensor.loop();
}

//Nếu nhận được lệnh không phải từ những lệnh được đăng ký trước thì sẽ ghi ra ở đây
void defaultHandler(const char *command) {
  Serial.print(F("---Not found handler: "));
  Serial.println(command);


  //Đoạn chương trình dưới chỉ dùng để debug, để các bạn biết chương trình của mình ở lỗi ở đâu để khắc phục, cũng không cần hiểu nha.
  int idx = 0;
  char *arg;
  while ((arg = sensor.next()) != NULL) { //còn tham số nào thì in ra
    Serial.print("-----> Gia tri thu #");
    Serial.print(idx);
    Serial.print(": ");
    Serial.println(arg);
   
    idx++;
  }
}
