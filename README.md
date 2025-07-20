# SS3RDH7 - Multi-Channel Biometric Data Acquisition System

A custom firmware for modular medical devices designed to recordone channe regular photoplethysmogram (PPG), one channel regular electrocardiogram (ECG) as well as 6-channel ear PPG and single-channel ear ECG from the ear area. This project was developed for PhD thesis research in 2021.


![Main screen view](images/PXL_20250720_051349531.jpg)
*Streaming 7 channels on the screen*

![Connected analog module](images/PXL_20250720_050259142.jpg)
*MAX30003 chip and a custom analog frontend*

![Removed analog module](images/PXL_20250720_054326261.jpg)
*The main PCB view*

![User end sensor - over the ear ECG headphone](images/3b2e473b8eb2.jpeg)
*https://www.researchgate.net/publication/362139775_A_wearable_heart_rate_monitoring_system_based_on_over-ear_headphones*

![User end sensor - in-ear PPG/ECG sensor](images/4ede2fd69765.jpeg)
*Not published yet*




## 📋 Project Overview

This embedded system provides high-precision, real-time acquisition of multiple biometric signals simultaneously, making it ideal for medical research, clinical studies, and wearable device development. The system features a modern touchscreen interface and comprehensive data logging capabilities.

## 🎯 Research Applications

This firmware was primarily developed for PhD thesis research focusing on:
- **Multi-parameter biometric monitoring** from the ear area
- **Simultaneous ECG and PPG data acquisition** for cardiovascular research
- **Real-time signal processing** and data visualization
- **Clinical data collection** for medical research studies

### Research Papers
1. **Effects of Binaural Beat Stimulation in Adults with Stuttering** - Research on audio stimulation effects using biometric monitoring
2. **A Wearable Heart Rate Monitoring System Based on Over-Ear Headphones** - Development of ear-based heart rate monitoring technology
3. **Low-Noise and Cost-Effective Active Electrodes for Dry Contact ECG Applications** - Advanced ECG electrode design for wearable applications
4. **Real-Time R-Peak Detection Algorithm for Low SNR ECG Signal** - Signal processing algorithms for ECG analysis

## 🔧 Hardware Architecture

### Microcontroller
- **STM32H743IITx** - High-performance ARM Cortex-M7 MCU
- **Memory**: 2MB Flash, 1MB RAM (distributed across multiple domains)
- **Clock**: High-speed system with multiple clock domains
- **Peripherals**: Extensive I/O including multiple I2C, SPI, UART, USB, SD card interface

### Sensors & Peripherals

#### ECG Sensors
- **MAX30003** - Single-channel ECG frontend with SPI interface
- **MAX30003LL** - Low-level driver for ECG data acquisition

#### PPG Sensors
- **MAX30102** - Finger PPG sensor (Red/IR wavelengths) with I2C interface
- **MAX86161** - Dual ear PPG sensors (Left/Right) with multiple wavelengths:
  - Green wavelength
  - Red wavelength  
  - Infrared (IR) wavelength
- **MAX30102LL/MAX86161LL** - Low-level drivers for PPG data acquisition

#### User Interface
- **XPT2046** - Resistive touch controller with SPI interface
- **LTDC** - LCD controller for high-resolution display
- **TouchGFX** - Modern GUI framework

#### Data Storage
- **SD Card** - FAT32 file system for data storage
- **USB MSC** - Mass storage device for data transfer

## 🏗️ Software Architecture

### Real-Time Operating System
- **FreeRTOS** with CMSIS-RTOS V2 wrapper
- **Task Configuration**:
  - 56 priority levels
  - 159KB heap size
  - Multiple concurrent tasks for sensor management

### Task Structure

#### Sensor Management Tasks
- `max30003Task` - ECG data acquisition
- `max30102Task` - Finger PPG data acquisition  
- `maxm86161RTask/maxm86161LTask` - Right/Left ear PPG data acquisition
- `touchTask` - Touch interface management

#### System Tasks
- `fatfsTask` - File system operations
- `usbTask` - USB mass storage
- `debugTask` - System debugging and monitoring

### Data Flow Architecture
```
Sensors → Low-Level Drivers → Device Drivers → Use Cases → GUI and Storage
```

## 📊 Data Acquisition Features

### Multi-Channel Biometric Monitoring
- **ECG Channels**:
  - Standard ECG measurement
  - In-ear ECG measurement
- **PPG Channels**:
  - Finger: Red and IR wavelengths
  - Left Ear: Green, Red, and IR wavelengths
  - Right Ear: Green, Red, and IR wavelengths

### Real-Time Data Processing
- **Configurable sampling rates** for each sensor
- **Double-buffer architecture** for continuous recording
- **Real-time data visualization** with 10 simultaneous graphs
- **Data validation** with CRC checking

## 🖥️ User Interface

### TouchGFX Framework
Modern, responsive GUI with multiple screens:

#### Main Screen
- Real-time data visualization with 10 graphs
- Recording controls (Start/Stop)
- Device connection management
- Data export functionality

#### Testing Screen
- System diagnostics and testing
- Performance monitoring
- Hardware validation

#### Settings Screen
- Configuration management
- Sensor calibration
- System parameters

#### Full Graph Screen
- Detailed data analysis
- Zoom and pan capabilities
- Data export options

## 💾 Data Management

### Storage Systems
- **SD Card**: FAT32 file system for data logging
- **USB Mass Storage**: Direct file access from PC
- **Real-time Clock**: Timestamped data records

### Data Format
- Structured binary format with headers
- Multi-channel synchronized data
- Metadata including timestamps and sensor information

### Recording Features
- **Start/Stop Recording** functionality
- **Device Connection Management**: Connect/disconnect sensors
- **Error Handling**: Comprehensive error detection and reporting
- **Data Integrity**: CRC checking and validation

## 🔌 Communication Protocols

- **SPI**: MAX30003 (ECG), XPT2046 (Touch)
- **I2C**: MAX30102, MAX86161 (PPG sensors)
- **UART**: Debug and external communication
- **USB**: Mass storage device interface

## 🚀 Getting Started

### Prerequisites
- STM32CubeIDE
- TouchGFX Designer
- STM32H7 development board
- Required sensors (MAX30003, MAX30102, MAX86161, XPT2046)

### Building the Project
1. Clone the repository:
   ```bash
   git clone git@github.com:voropaiandrii/SS3RDH7.git
   ```

2. Open the project in STM32CubeIDE

3. Configure the project settings according to your hardware setup

4. Build and flash the firmware

### Hardware Setup
1. Connect the sensors according to the pin definitions in `main.h`
2. Connect the display and touch interface
3. Insert an SD card for data storage
4. Power up the system

## 📁 Project Structure

```
SS3RDH7/
├── Core/                    # Main application code
│   ├── Inc/                # Header files
│   │   ├── devices/        # Sensor drivers
│   │   ├── domain/         # Application logic
│   │   └── data/           # Data management
│   └── Src/                # Source files
├── TouchGFX/               # GUI framework
│   ├── gui/                # GUI source code
│   └── generated/          # Auto-generated GUI code
├── Drivers/                # STM32 HAL drivers
├── Middlewares/            # FreeRTOS and other middleware
├── FATFS/                  # File system
├── USB_DEVICE/             # USB mass storage
└── CustomUI/               # Custom UI assets
```

## 🔧 Configuration

### Key Configuration Files
- `main.h` - Pin definitions and system constants
- `FreeRTOSConfig.h` - RTOS configuration
- `SS3RDH7.ioc` - STM32CubeMX configuration
- `STM32H743IITX_FLASH.ld` - Memory layout

### Important Constants
```c
#define ECG_BUFFER_SIZE          2000
#define GRAPH_DOWNSAMPLING_VALUE 19
#define RTOS_DEBUG_BUFFER_SIZE   2000
```

## 📈 Performance Characteristics

- **ECG Sampling**: High-precision single-channel acquisition
- **PPG Sampling**: 6-channel simultaneous acquisition
- **Display**: Real-time graph updates
- **Storage**: Continuous data logging to SD card
- **USB Transfer**: High-speed data export

## 🤝 Contributing

This project was developed for academic research. For contributions or questions, please contact the original author.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

The MIT License is a permissive license that allows for:
- Commercial use
- Modification
- Distribution
- Private use

While requiring only that the license and copyright notice be included in all copies or substantial portions of the software.

## 👨‍🎓 Author

**Andrii Voropai** - PhD Research (2021)

This firmware was developed as part of PhD thesis research focusing on multi-parameter biometric monitoring from the ear area.

## 📚 Related Research

This project supports research in:
- Cardiovascular monitoring
- Wearable medical devices
- Multi-parameter biometric analysis
- Clinical data acquisition systems

---

*This project represents a sophisticated medical device platform with professional-grade data acquisition capabilities, modern user interface, and robust data management systems suitable for clinical and research applications.*

## 📚 References

### Research Papers
1. [Effects of Binaural Beat Stimulation in Adults with Stuttering](https://www.researchgate.net/publication/368487447_Effects_of_Binaural_Beat_Stimulation_in_Adults_with_Stuttering)
2. [A Wearable Heart Rate Monitoring System Based on Over-Ear Headphones](https://www.researchgate.net/publication/362139775_A_wearable_heart_rate_monitoring_system_based_on_over-ear_headphones)
3. [Low-Noise and Cost-Effective Active Electrodes for Dry Contact ECG Applications](https://www.researchgate.net/publication/358703557_Low-Noise_and_Cost-Effective_Active_Electrodes_for_Dry_Contact_ECG_Applications)
4. [Real-Time R-Peak Detection Algorithm for Low SNR ECG Signal](https://www.researchgate.net/publication/354026115_REAL-TIME_R-PEAK_DETECTION_ALGORITHM_FOR_LOW_SNR_ECG_SIGNAL) 