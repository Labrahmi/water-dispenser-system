# Arduino Smart Water Dispenser System

The Arduino Smart Water Dispenser System is a project designed to create an intelligent water dispensing system using various modules and sensors. This repository contains the code and documentation for building and implementing the system.

## Features

- **RFID User Identification:** Utilizes the MFRC522 RFID reader for secure user identification.
- **User Interaction:** Incorporates TTP223B touch buttons for user-friendly interaction.
- **Water Level Monitoring:** Uses an HC-SR04 ultrasonic sensor to measure water levels in the reservoir.
- **Pump Control:** A relay is employed for efficient control of the water pump.

## Usage

- **Pin Configuration:** Refer to the provided table for quick reference on connecting modules to different Arduino boards.
- **User Initialization:** The program defines constants for pin assignments and initializes user data, including UID, description, and balance.
- **User Interaction:** Users can interact with the system by presenting RFID cards and selecting desired water quantities using touch buttons.

## System Operation

- **Continuous Monitoring:** The system continuously monitors the water level in the reservoir, displaying it on an LCD screen.
- **Balance Check:** Checks user balance before dispensing water, updating the balance accordingly.
- **Safety Checks:** Includes safety checks to ensure sufficient water in the reservoir and adequate user balance.

## Project Goal

The project aims to provide a convenient and automated way for users to access water while maintaining a secure payment system through RFID cards. The intelligent system enhances user experience and encourages efficient water usage.

