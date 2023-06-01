//Инициализация регистров
#ifndef TAMC_GT911_H
#define TAMC_GT911_H
 #define GT911_ADDR1 (uint8_t)0x5D
#define GT911_ADDR2 (uint8_t)0x14

#define ROTATION_LEFT      (uint8_t)0
#define ROTATION_INVERTED  (uint8_t)1
#define ROTATION_RIGHT     (uint8_t)2
#define ROTATION_NORMAL    (uint8_t)3


// Real-time command (Write only)
#define GT911_COMMAND       (uint16_t)0x8040
#define GT911_ESD_CHECK     (uint16_t)0x8041
#define GT911_COMMAND_CHECK (uint16_t)0x8046

// Configuration information (R/W)
#define GT911_CONFIG_START             (uint16_t)0x8047
#define GT911_CONFIG_VERSION           (uint16_t)0x8047
#define GT911_X_OUTPUT_MAX_LOW         (uint16_t)0x8048
#define GT911_X_OUTPUT_MAX_HIGH        (uint16_t)0x8049
#define GT911_Y_OUTPUT_MAX_LOW         (uint16_t)0x804A
#define GT911_Y_OUTPUT_MAX_HIGH        (uint16_t)0x804B
#define GT911_TOUCH_NUMBER             (uint16_t)0x804C
#define GT911_MODULE_SWITCH_1          (uint16_t)0x804D
#define GT911_MODULE_SWITCH_2          (uint16_t)0x804E
#define GT911_SHAKE_COUNT              (uint16_t)0x804F
#define GT911_FILTER                   (uint16_t)0x8050
#define GT911_LARGE_TOUCH              (uint16_t)0x8051
#define GT911_NOISE_REDUCTION          (uint16_t)0x8052
#define GT911_SCREEN_TOUCH_LEVEL       (uint16_t)0x8053
#define GT911_SCREEN_RELEASE_LEVEL     (uint16_t)0x8054
#define GT911_LOW_POWER_CONTROL        (uint16_t)0x8055
#define GT911_REFRESH_RATE             (uint16_t)0x8056
#define GT911_X_THRESHOLD              (uint16_t)0x8057
#define GT911_Y_THRESHOLD              (uint16_t)0x8058
#define GT911_X_SPEED_LIMIT            (uint16_t)0x8059 //Reserve
#define GT911_Y_SPEED_LIMIT            (uint16_t)0x805A //Reserve
#define GT911_SPACE_TOP_BOTTOM         (uint16_t)0x805B
#define GT911_SPACE_LEFT_RIGHT         (uint16_t)0x805C
#define GT911_MINI_FILTER              (uint16_t)0x805D
#define GT911_STRETCH_R0               (uint16_t)0x805E
#define GT911_STRETCH_R1               (uint16_t)0x805F
#define GT911_STRETCH_R2               (uint16_t)0x8060
#define GT911_STRETCH_RM               (uint16_t)0x8061
#define GT911_DRV_GROUPA_NUM           (uint16_t)0x8062
#define GT911_DRV_GROUPB_NUM           (uint16_t)0x8063
#define GT911_SENSOR_NUM               (uint16_t)0x8064
#define GT911_FREQ_A_FACTOR            (uint16_t)0x8065
#define GT911_FREQ_B_FACTOR            (uint16_t)0x8066
#define GT911_PANEL_BIT_FREQ_L         (uint16_t)0x8067
#define GT911_PANEL_BIT_FREQ_H         (uint16_t)0x8068
#define GT911_PANEL_SENSOR_TIME_L      (uint16_t)0x8069 //Reserve
#define GT911_PANEL_SENSOR_TIME_H      (uint16_t)0x806A
#define GT911_PANEL_TX_GAIN            (uint16_t)0x806B
#define GT911_PANEL_RX_GAIN            (uint16_t)0x806C
#define GT911_PANEL_DUMP_SHIFT         (uint16_t)0x806D
#define GT911_DRV_FRAME_CONTROL        (uint16_t)0x806E
#define GT911_CHARGING_LEVEL_UP        (uint16_t)0x806F
#define GT911_MODULE_SWITCH3           (uint16_t)0x8070
#define GT911_GESTURE_DIS              (uint16_t)0X8071
#define GT911_GESTURE_LONG_PRESS_TIME  (uint16_t)0x8072
#define GT911_X_Y_SLOPE_ADJUST         (uint16_t)0X8073
#define GT911_GESTURE_CONTROL          (uint16_t)0X8074
#define GT911_GESTURE_SWITCH1          (uint16_t)0X8075
#define GT911_GESTURE_SWITCH2          (uint16_t)0X8076
#define GT911_GESTURE_REFRESH_RATE     (uint16_t)0x8077
#define GT911_GESTURE_TOUCH_LEVEL      (uint16_t)0x8078
#define GT911_NEWGREENWAKEUPLEVEL      (uint16_t)0x8079
#define GT911_FREQ_HOPPING_START       (uint16_t)0x807A
#define GT911_FREQ_HOPPING_END         (uint16_t)0X807B
#define GT911_NOISE_DETECT_TIMES       (uint16_t)0x807C
#define GT911_HOPPING_FLAG             (uint16_t)0X807D
#define GT911_HOPPING_THRESHOLD        (uint16_t)0X807E
#define GT911_NOISE_THRESHOLD          (uint16_t)0X807F //Reserve
#define GT911_NOISE_MIN_THRESHOLD      (uint16_t)0X8080
#define GT911_HOPPING_SENSOR_GROUP     (uint16_t)0X8082
#define GT911_HOPPING_SEG1_NORMALIZE   (uint16_t)0X8083
#define GT911_HOPPING_SEG1_FACTOR      (uint16_t)0X8084
#define GT911_MAIN_CLOCK_AJDUST        (uint16_t)0X8085
#define GT911_HOPPING_SEG2_NORMALIZE   (uint16_t)0X8086
#define GT911_HOPPING_SEG2_FACTOR      (uint16_t)0X8087
#define GT911_HOPPING_SEG3_NORMALIZE   (uint16_t)0X8089
#define GT911_HOPPING_SEG3_FACTOR      (uint16_t)0X808A
#define GT911_HOPPING_SEG4_NORMALIZE   (uint16_t)0X808C
#define GT911_HOPPING_SEG4_FACTOR      (uint16_t)0X808D
#define GT911_HOPPING_SEG5_NORMALIZE   (uint16_t)0X808F
#define GT911_HOPPING_SEG5_FACTOR      (uint16_t)0X8090
#define GT911_HOPPING_SEG6_NORMALIZE   (uint16_t)0X8092
#define GT911_KEY_1                    (uint16_t)0X8093
#define GT911_KEY_2                    (uint16_t)0X8094
#define GT911_KEY_3                    (uint16_t)0X8095
#define GT911_KEY_4                    (uint16_t)0X8096
#define GT911_KEY_AREA                 (uint16_t)0X8097
#define GT911_KEY_TOUCH_LEVEL          (uint16_t)0X8098
#define GT911_KEY_LEAVE_LEVEL          (uint16_t)0X8099
#define GT911_KEY_SENS_1_2             (uint16_t)0X809A
#define GT911_KEY_SENS_3_4             (uint16_t)0X809B
#define GT911_KEY_RESTRAIN             (uint16_t)0X809C
#define GT911_KEY_RESTRAIN_TIME        (uint16_t)0X809D
#define GT911_GESTURE_LARGE_TOUCH      (uint16_t)0X809E
#define GT911_HOTKNOT_NOISE_MAP        (uint16_t)0X80A1
#define GT911_LINK_THRESHOLD           (uint16_t)0X80A2
#define GT911_PXY_THRESHOLD            (uint16_t)0X80A3
#define GT911_GHOT_DUMP_SHIFT          (uint16_t)0X80A4
#define GT911_GHOT_RX_GAIN             (uint16_t)0X80A5
#define GT911_FREQ_GAIN0               (uint16_t)0X80A6
#define GT911_FREQ_GAIN1               (uint16_t)0X80A7
#define GT911_FREQ_GAIN2               (uint16_t)0X80A8
#define GT911_FREQ_GAIN3               (uint16_t)0X80A9
#define GT911_COMBINE_DIS              (uint16_t)0X80B3
#define GT911_SPLIT_SET                (uint16_t)0X80B4
#define GT911_SENSOR_CH0               (uint16_t)0X80B7
#define GT911_DRIVER_CH0               (uint16_t)0X80D5
#define GT911_CONFIG_CHKSUM            (uint16_t)0X80FF
#define GT911_CONFIG_FRESH             (uint16_t)0X8100
#define GT911_CONFIG_SIZE              (uint16_t)0xFF-0x46
// Coordinate information
#define GT911_PRODUCT_ID        (uint16_t)0X8140
#define GT911_FIRMWARE_VERSION  (uint16_t)0X8140
#define GT911_RESOLUTION        (uint16_t)0X8140
#define GT911_VENDOR_ID         (uint16_t)0X8140
#define GT911_IMFORMATION       (uint16_t)0X8140
#define GT911_POINT_INFO        (uint16_t)0X814E
#define GT911_POINT_1           (uint16_t)0X814F
#define GT911_POINT_2           (uint16_t)0X8157
#define GT911_POINT_3           (uint16_t)0X815F
#define GT911_POINT_4           (uint16_t)0X8167
#define GT911_POINT_5           (uint16_t)0X816F
#define GT911_POINTS_REG        {GT911_POINT_1, GT911_POINT_2, GT911_POINT_3, GT911_POINT_4, GT911_POINT_5}

 
 #define TOUCH_GT911
 #define TOUCH_GT911_SCL 32
 #define TOUCH_GT911_SDA 33
 #define TOUCH_GT911_INT 39
 #define TOUCH_GT911_RST 25
 #define TOUCH_GT911_ROTATION ROTATION_LEFT 
 #define TOUCH_MAP_X1 480
 #define TOUCH_MAP_X2 0
 #define TOUCH_MAP_Y1 320
 #define TOUCH_MAP_Y2 0

class TP_Point {
  public:
    TP_Point(void);
    TP_Point(uint8_t id, uint16_t x, uint16_t y, uint16_t size);

    bool operator==(TP_Point);
    bool operator!=(TP_Point);

    uint8_t id;
    uint16_t x;
    uint16_t y;
    uint8_t size;
};

class TAMC_GT911 {
  public:
    TAMC_GT911(uint8_t _sda, uint8_t _scl, uint8_t _int, uint8_t _rst, uint16_t _width, uint16_t _height);
    void begin(uint8_t _addr=GT911_ADDR1);
    void reset();
    void setRotation(uint8_t rot);
    void setResolution(uint16_t _width, uint16_t _height);
    // void setOnRead(void (*isr)());
    uint8_t getGesture(void);
    void read(void);
    uint8_t isLargeDetect;
    uint8_t touches = 0;
    bool isTouched = false;
    // uint8_t gesture = NO_GESTURE;
    TP_Point points[5];

  private:
    void calculateChecksum();
    void reflashConfig();
    // static void ARDUINO_ISR_ATTR onInterrupt();
    TP_Point readPoint(uint8_t *data);
    // void (*onRead)();
    void writeByteData(uint16_t reg, uint8_t val);
    uint8_t readByteData(uint16_t reg);
    void writeBlockData(uint16_t reg, uint8_t *val, uint8_t size);
    void readBlockData(uint8_t *buf, uint16_t reg, uint8_t size);
    uint8_t rotation = ROTATION_NORMAL;
    uint8_t addr;
    uint8_t pinSda;
    uint8_t pinScl;
    uint8_t pinInt;
    uint8_t pinRst;
    uint16_t width;
    uint16_t height;
    uint8_t configBuf[GT911_CONFIG_SIZE];
    // uint8_t *configBuf;
};

#endif // TAMC_GT911_H

int touch_last_x = 0, touch_last_y = 0; //координаты точки касания
TAMC_GT911::TAMC_GT911(uint8_t _sda, uint8_t _scl, uint8_t _int, uint8_t _rst, uint16_t _width, uint16_t _height) :
  pinSda(_sda), pinScl(_scl), pinInt(_int), pinRst(_rst), width(_width), height(_height) {

}

void TAMC_GT911::begin(uint8_t _addr) {
  addr = _addr;
  Wire1.begin(TOUCH_GT911_SDA, TOUCH_GT911_SCL);
  reset();
}
void TAMC_GT911::reset() {
  pinMode(pinInt, OUTPUT);
  pinMode(pinRst, OUTPUT);
  digitalWrite(pinInt, 0);
  digitalWrite(pinRst, 0);
  delay(10);
  digitalWrite(pinInt, addr==GT911_ADDR2);
  delay(1);
  digitalWrite(pinRst, 1);
  delay(5);
  digitalWrite(pinInt, 0);
  delay(50);
  pinMode(pinInt, INPUT);
  // attachInterrupt(pinInt, TAMC_GT911::onInterrupt, RISING);
  delay(50);
  readBlockData(configBuf, GT911_CONFIG_START, GT911_CONFIG_SIZE);
  setResolution(width, height);
}
void TAMC_GT911::calculateChecksum() {
  uint8_t checksum;
  for (uint8_t i=0; i<GT911_CONFIG_SIZE; i++) {
    checksum += configBuf[i];
  }
  checksum = (~checksum) + 1;
  configBuf[GT911_CONFIG_CHKSUM - GT911_CONFIG_START] = checksum;
}
// void ARDUINO_ISR_ATTR TAMC_GT911::onInterrupt() {
//   read();
//   TAMC_GT911::onRead();
// }
void TAMC_GT911::reflashConfig() {
  calculateChecksum();
  writeByteData(GT911_CONFIG_CHKSUM, configBuf[GT911_CONFIG_CHKSUM-GT911_CONFIG_START]);
  writeByteData(GT911_CONFIG_FRESH, 1);
}
void TAMC_GT911::setRotation(uint8_t rot) {
  rotation = rot;
}
void TAMC_GT911::setResolution(uint16_t _width, uint16_t _height) {
  configBuf[GT911_X_OUTPUT_MAX_LOW - GT911_CONFIG_START] = lowByte(_width);
  configBuf[GT911_X_OUTPUT_MAX_HIGH - GT911_CONFIG_START] = highByte(_width);
  configBuf[GT911_Y_OUTPUT_MAX_LOW - GT911_CONFIG_START] = lowByte(_height);
  configBuf[GT911_Y_OUTPUT_MAX_HIGH - GT911_CONFIG_START] = highByte(_height);
  reflashConfig();
}
// void TAMC_GT911::setOnRead(void (*isr)()) {
//   onRead = isr;
// }
void TAMC_GT911::read(void) {
  // Serial.println("TAMC_GT911::read");
  uint8_t data[7];
  uint8_t id;
  uint16_t x, y, size;

  uint8_t pointInfo = readByteData(GT911_POINT_INFO);
  uint8_t bufferStatus = pointInfo >> 7 & 1;
  uint8_t proximityValid = pointInfo >> 5 & 1;
  uint8_t haveKey = pointInfo >> 4 & 1;
  isLargeDetect = pointInfo >> 6 & 1;
  touches = pointInfo & 0xF;
  // Serial.print("bufferStatus: ");Serial.println(bufferStatus);
  // Serial.print("largeDetect: ");Serial.println(isLargeDetect);
  // Serial.print("proximityValid: ");Serial.println(proximityValid);
  // Serial.print("haveKey: ");Serial.println(haveKey);
  // Serial.print("touches: ");Serial.println(touches);
  isTouched = touches > 0;
  if (bufferStatus == 1 && isTouched) {
    for (uint8_t i=0; i<touches; i++) {
      readBlockData(data, GT911_POINT_1 + i * 8, 7);
      points[i] = readPoint(data);
    }
  }
  writeByteData(GT911_POINT_INFO, 0);
}
TP_Point TAMC_GT911::readPoint(uint8_t *data) {
  uint16_t temp;
  uint8_t id = data[0];
  uint16_t x = data[1] + (data[2] << 8);
  uint16_t y = data[3] + (data[4] << 8);
  uint16_t size = data[5] + (data[6] << 8);
  switch (rotation){
    case ROTATION_NORMAL:
      x = width - x;
      y = height - y;
      break;
    case ROTATION_LEFT:
      temp = x;
      x = width - y;
      y = temp;
      break;
    case ROTATION_INVERTED:
      x = x;
      y = y;
      break;
    case ROTATION_RIGHT:
      temp = x;
      x = y;
      y = height - temp;
      break;
    default:
      break;
  }
  return TP_Point(id, x, y, size);
}
void TAMC_GT911::writeByteData(uint16_t reg, uint8_t val) {
  Wire1.beginTransmission(addr);
  Wire1.write(highByte(reg));
  Wire1.write(lowByte(reg));
  Wire1.write(val);
  Wire1.endTransmission();
}
uint8_t TAMC_GT911::readByteData(uint16_t reg) {
  uint8_t x;
  Wire1.beginTransmission(addr);
  Wire1.write(highByte(reg));
  Wire1.write(lowByte(reg));
  Wire1.endTransmission();
  Wire1.requestFrom(addr, (uint8_t)1);
  x = Wire1.read();
  return x;
}
void TAMC_GT911::writeBlockData(uint16_t reg, uint8_t *val, uint8_t size) {
  Wire1.beginTransmission(addr);
  Wire1.write(highByte(reg));
  Wire1.write(lowByte(reg));
  // Wire1.write(val, size);
  for (uint8_t i=0; i<size; i++) {
    Wire1.write(val[i]);
  }
  Wire1.endTransmission();
}
void TAMC_GT911::readBlockData(uint8_t *buf, uint16_t reg, uint8_t size) {
  Wire1.beginTransmission(addr);
  Wire1.write(highByte(reg));
  Wire1.write(lowByte(reg));
  Wire1.endTransmission();
  Wire1.requestFrom(addr, size);
  for (uint8_t i=0; i<size; i++) {
    buf[i] = Wire1.read();
  }
}
TP_Point::TP_Point(void) {
  id = x = y = size = 0;
}
TP_Point::TP_Point(uint8_t _id, uint16_t _x, uint16_t _y, uint16_t _size) {
  id = _id;
  x = _x;
  y = _y;
  size = _size;
}
bool TP_Point::operator==(TP_Point point) {
  return ((point.x == x) && (point.y == y) && (point.size == size));
}
bool TP_Point::operator!=(TP_Point point) {
  return ((point.x != x) || (point.y != y) || (point.size != size));
}

TAMC_GT911 ts = TAMC_GT911(TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT, TOUCH_GT911_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));


//Иницаиализация сенсорного экрана
void touch_init()
{
  ts.begin();
  ts.setRotation(TOUCH_GT911_ROTATION);
}

//Проверка в цикле на срабатывание тача
bool touch_has_signal()
{
  return true;
}
//касание сенсорного экрана
bool touch_touched()
{
  ts.read();
  if (ts.isTouched)
  {
#if defined(TOUCH_SWAP_XY)
    touch_last_x = map(ts.points[0].y, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, TOUCH_MAP_X1-1);
    touch_last_y = map(ts.points[0].x, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, TOUCH_MAP_Y1-1);
#else
    touch_last_x = map(ts.points[0].x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, TOUCH_MAP_X1-1);
    touch_last_y = map(ts.points[0].y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, TOUCH_MAP_Y1-1);
#endif
    return true;
  }
  else
  {
    return false;
  }
}

//касание прекратилось
bool touch_released()
{
  return true;
}
