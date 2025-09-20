# RFID_Based_attendece_system_with_database_integration

This project implements a smart attendance system using RFID technology, integrated with a Linux-based database to manage user data, attendance logs, and admin operations.

## 🧱 System Overview

The project is divided into two major parts:

- **Microcontroller Application**  
  Developed in Embedded C for LPC2148 using KEIL

- **Linux Application**  
  Developed in C using GCC to receive data over UART and manage a `.csv` database

## ⚙️ Hardware Components

📌 LPC2148 ARM7 Microcontroller

📌 RFID Reader + RFID Cards

📌 LCD Display

📌 4x4 Keypad

📌 AT25LC512 EEPROM (SPI)

📌 MAX232 (Serial Communication Driver)

📌 USB-to-UART Converter

📌 Switches (for external interrupt)

## 💻 Software Tools Used

- **Keil µVision** – Embedded C development and debugging
   
- **Flash Magic** – Programming HEX files to LPC2148
  
- **GCC (GNU Compiler Collection)** – Compiling Linux-side C application
  
- **UART Communication** – With interrupt handling (UART0 and UART1)
  
- **CSV File Handling** – Attendance logs maintained in `.csv` format on Linux

## 🧩 Key System Features

- **RFID-based Identification:** Recognizes and differentiates between Admin and User cards

- **EEPROM Storage:** Admin card number and RTC settings saved persistently

- **Bidirectional UART Communication:** Microcontroller <--> Linux

- **CSV-Based Attendance Logging:** Stores IN/OUT timings, date, and total working hours

- **Real-time LCD Display:** Displays card info, messages, and menu options

- **External Interrupt:** Triggered for Admin updates and RTC settings

## 🛠️ How It Works
- On startup, Admin card number is read from EEPROM  
- System waits for card scan (Admin or User)

- On **User card scan**:  
  - Sends card number + timestamp to Linux via UART  
  - Linux updates `.csv` file with attendance  
  - Sends acknowledgment back

  ## 📊 Example CSV Format

  | S.No | USER ID | USER NAME | DATE | WORKING HOURS | IN/OUT STATUS | IN TIME | OUT TIME |
  |------|---------|-----------|------|----------------|----------------|---------|----------|
  | 1    | 12345678 | John Doe | 2025-08-18 | 3h 15m |IN | 09:00 | 12:15 |

- On **Admin card scan**:  
  - Admin menu is displayed for user database management

   ## 🖥️ Linux Admin Menu

   - Displayed when Admin card is scanned:

       1.ADD USER
      
       2.DELETE USER
      
       3.EDIT USER
      
       4.EXIT

- External interrupt allows updating Admin card or RTC settings.

## 📁 Folder Structure

- **Linux-Side Code**
  
  ├── linux_backend_code.c       # Linux-side UART & CSV handling

- **ARM (Microcontroller) Code**
  
  ├── major_main.c                # Main microcontroller application

  ├── attendance.csv             # Attendance logging database

  ├── userdetails.txt            # User details storage

  ├── lcd.c/h, lcd_defines.h     # LCD display driver

  ├── uart.c/h, uart_defines.h   # UART communication driver

  ├── keypad.c/h, kpm_defines.h  # 4x4 Keypad interface

  ├── spi.c/h, spi_defines.h     # SPI communication driver

  ├── spi_eeprom.c/h             # EEPROM read/write handling

  ├── pincfg.c/h                 # Pin configuration

  ├── interrupt.c/h              # Interrupt service routines

  ├── rtc.c/h, rtc_defines.h     # RTC handling

  ├── delay.c/h                  # Delay functions

## 🧪 Testing & Validation

- Test each peripheral module separately: LCD output,Keypad key detection,EEPROM read/write,UART transmit/receive

- Validate UART communication with Linux

- Simulate real-world usage with: Multiple users scanning IN and OUT, Admin adding/editing/removing users

- Check .csv file updates and accuracy of timing and status

- Trigger external interrupt and verify RTC/admin updates

 ## ✅ Conclusion
 
 This project offers a real-time, RFID-based attendance tracking system that combines embedded systems and Linux programming, providing a scalable & efficient solution for institutions or companies.
