#include <iNut.h>
iNut sensor;

#define LED 13 //chân 13 là chân LED 
void setup() {
  Serial.begin(9600); //bật baudrate ở mức 9600

  Serial.println("San sang nhan lenh");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); //Bật đèn LED sáng
  
  //Khai báo số lượng luồn cảm biến
  sensor.setup(8); //Sẽ có 08 luồn cảm biến

  sensor.addCommand("LED", ledFunction);
  sensor.setDefaultHandler(defaultHandler);
}

void loop() {

  sensor.setValue(0, digitalRead(LED)); //đọc giá trị của đèn LED

 
  //giá trị cho các luồn từ 1-7 là giá trị random
  ///Tại sao lại sử dụng random? Vì đây là thử nghiệm, bạn có thể thay thế giá trị random bằng một giá trị cảm biến hoặc trị số mà bạn mong muốn đồng bộ lên Internet!
  for (byte i = 1; i <= 7; i++) {
    unsigned long value = random() % 10000; //trả về giá trị random - "hên xui" - tự do
    sensor.setValue(i, value); //gán giá trị trên vào luồn
  }
  sensor.loop();
}


//thực hiện khi nhận lệnh LED
void ledFunction() {
  char *arg = sensor.next();
  if (strcmp(arg, "ON") == 0) { //nếu tham số 1 là ON. LED ON. Thì đèn sáng
    digitalWrite(LED, HIGH);
    Serial.println("Bat den");
  } else if (strcmp(arg, "OFF") == 0) { // LED OFF. Đèn tắt
    digitalWrite(LED, LOW);
    Serial.println("Tat den");
  } else if (strcmp(arg, "TOGGLE") == 0) { //Đảo trạng thái của LED
    digitalWrite(LED, !digitalRead(LED)); 
    Serial.println("Doi trang thai den");
  } else {
    Serial.println("Khong nam trong tap hop lenh");
  }
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

