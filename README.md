# 🧠🚗 Smart-Parking-System-with-RFID-Display
**Course:** ISE-102: Intro to Intelligent Systems
**Timeline**: February 2024-May 2024

## 📖 Abstract
This project presents the design and implementation of a smart parking system using an Arduino Uno microcontroller. It integrates infrared (IR) sensors for vehicle detection, an RFID reader for secure access control, a servo motor to operate the gate, and a 20x4 LCD display to provide real-time feedback to users. The system efficiently monitors parking availability and automates entry and exit, enhancing security and user experience.

## 🚀 Introduction
With urban populations rapidly increasing, parking demand has surged, often leading to congestion and inefficiency in traditional parking systems. Our smart parking system addresses these issues by automating parking space management and access control, reducing traffic jams, and improving user convenience through RFID authentication and sensor driven automation. 

## 🔨 Systems Components & Methods

### Sensors
- **Infrared (IR) Sensors:** Detect vehicle presence by measuring reflected infrared light.
  *Range:* 2-30 cm
  *Output:* Digital HIGH when vehicle detected, LOW otherwise.
  
- **RFID Reader:** Scans RFID tags for secure access authorization.
  *Range:* 0-60 cm

### Actuators
- **MSG90 Servo Motor:** Controls the gate's open/close mechanism with precise angular positioning (0-180 degrees).

### Computational Devices
- **Arduino Uno:** The central controller coordinates sensor data, actuator commands, and display updates.
- **20x4 LCD Display:** Provides real-time parking and system status feedback using I2C communication protocol.

## 📊 Results
We tested the system by triggering sensors and actuators 100 times. Each component performed reliably and as expected. The integrated system accurately detected vehicle presence, controlled gate operations, and displayed status updates without failure.

## ⚙️ Installation and Usage
For these details, look into my report, code, and mechanical/electrical detail files.

## 📦 Discussion and Conclusion
Our smart parking system enhances space optimization, improves traffic flow, and boosts security with automated controls and RFID access. While setup complexity and sensor reliability pose challenges, this project demonstrates a viable, scalable solution for urban parking management.

## ®️ References
- [Smart Parking System Using Arduino - GeeksforGeeks](https://www.geeksforgeeks.org/smart-parking-system-using-arduino/)
- [Arduino RFID Tutorial - Arduino Getting Started](https://arduinogetstarted.com/tutorials/arduino-rfid-nfc)
- [IR Sensor Working and Applications - WatElectronics](https://www.watelectronics.com/ir-sensor/)
- And others as cited in my project report!

## 📚 License
