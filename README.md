# Installation #
Create a new folder called "iNut" under the folder named "libraries" in your Arduino sketchbook folder.
Create the folder "libraries" in case it does not exist yet. Place all the files in the "iNut" folder.

# Usage #
To use the library in your own sketch, select it from *Sketch > Import Library*.


# Cài đặt #

Tạo một thư mục "iNut" nằm trong thư mục "libraries" trong thư mục Arduino Sketchbook.
Ví dụ:
Trên Windodws: C:\Users\<tên máy tính>\Documents\Arduino\libraries (Cái này trong thư mục Libraries của Arduino á hihi)
Trên MACOS: Documents/Arduino/libraries

# Bản chất của thư viện #

iNut sẽ cho bạn 8 luồn dữ liệu để bạn đồng bộ lên Internet.
- Mỗi luồn dữ liệu gồm 32 bit (float).
- Mỗi luồn có thể được ép kiểu thành:
  - bit_in : Giám sát digital (tối đa 16 bit)
  - bit_out: Điều khiển digital (relay) (tối đa 16 bit)
  - int : Số NGuyên (mặc định)
  - float: Số Thực 
- Mỗi kiểu dữ liệu được điều chỉnh dễ dàng trong app và không bị mất đi khi reset.


# Ứng dụng #
- Điều khiển, giám sát các thiết bị IN/OUT từ xa qua Internet.
- Dùng trong nghiên cứu IoT để ra ứng dụng nhanh chóng mà không cần biết quá nhiều về lĩnh vực IoT.
- Làm SCADA.
- Ứng dụng trong việc học STEAM.
- Làm bộ data logging vì có các tính năng EXCEL, thống kê.
- Làm bộ báo động (cháy, nhiệt, khói,... bất cứ dòng cảm biến nào mà Arduino hỗ trợ).
- Outsourcing Arduino in Fast Protyping.
- Kết hợp với [iNut Node-RED IDE](http://github.com/ngohuynhngocKhanh/iNut-Node-RED-IDE) để lập trình kéo thả tạo dự án đòi hỏi sự phức tạp.
- Bất cứ điều gì làm cho bạn trở nên giàu có về tri thức và vật chất.

# Protocol #

## Khai báo ##

```
#include <iNut.h>
iNut sensor; //Trong đó iNut là tên thư viện (kiểu dữ liệu) còn sensor là tên đại diện (variable / avatar) để mình khởi tạo đối tượng iNut
```

## Cài đặt số luồn cảm biến sử dụng ##

```
sensor.setup(int); 
Ví dụ:
sensor.setup(8); //Mặc định chúng ta có 8 luồn cảm biến nên sẽ để là 8.
```

## Các phương thức gửi dữ liệu về APP iNut hoặc các ứng dụng trên iNut Node-RED IDE 

```
void sensor.setValue(int index, int value); 
void sensor.setValue(int index, int value)
void sensor.turnOn(int index, int bit_index)
void sensor.turnOff(int index, int bit_index)
float sensor.getValue(int index)
```
Trong đó:
```
index = vị trí của luồn cảm biến (từ 0 - 7)
value = giá trị muốn đưa lên
bit_index = vị trí bit thao tác
```

## Các phưong thức bắt sự kiện ##

Dành cho ứng dụng có điều khiển

```
sensor.addCommand(const char * command, (void *)callback);
Ví dụ
sensor.addCommand("ON", onFunction);
sensor.addCommand("OFF", offFunction);
sensor.addCommand("ONLINE", onlineFunction);
sensor.addCommand("OFFLINE", offlineFunction);
sensor.addCommand("TIME", timeFunction);
sensor.setDefaultHandler(defaultHandler);
```

Xem thêm ở ví dụ [Điều khiển 8 relay và gửi 7 luồng cảm biến](/examples/dieu-khien-8-relay-hien-thi-7-cam-bien/dieu-khien-8-relay-hien-thi-7-cam-bien.ino). 
