# SoftfonJerzy2019
This project is a software-based IP phone implemented using PJSIP for SIP communication and .NET Framework with WPF for the user interface. It bridges native C++ PJSIP functionality with .NET via C++/CLI, enabling seamless VoIP call handling in a Windows desktop application.

# Software IP Phone using PJSIP and .NET Framework

## Overview
This project is a software-based IP phone implemented using **PJSIP** for SIP communication and **.NET Framework** with **WPF** for the user interface. It bridges native C++ PJSIP functionality with .NET via **C++/CLI**, enabling seamless VoIP call handling in a Windows desktop application.

## Chosen Technologies
To implement this project, the **PJSIP** library was selected due to its extensive documentation and open-source nature. An additional advantage is the **high-level PJSUA2 API**, which integrates SIP session management and audio/video media handling into a user-friendly programming interface. This allows developers to establish and manage multimedia sessions from a single interface without relying on system APIs, avoiding compatibility issues.

The library is compatible with **.NET Framework** and is written in **C and C++**.

The application is designed to run on **Windows 10**, the most popular operating system for personal computers. **.NET Framework** was chosen as the platform, as it supports Windows 10 and other operating systems, potentially allowing for future portability to platforms like **Android** with minimal effort.

## Development Tools
The implementation is done using **Microsoft Visual Studio 2017 Community**, an IDE that facilitates .NET Framework development and provides various tools for programming and UI design.

The graphical user interface (GUI) is built using **Windows Presentation Foundation (WPF)**, which offers a simple and powerful way to design application layouts and interactions. 

- The UI logic is implemented in **C#**.
- The SIP library is written in **C and C++**.
- Managed and unmanaged code interoperability is handled using **C++/CLI**, which requires additional effort from the developer to bridge the two environments.

## Features (Planned)
- SIP account registration
- Voice call initiation and reception

## Installation & Usage
TBD – instructions for building and running the application will be added soon.

## License
This project is released under the MIT License. See [LICENSE](LICENSE) for more details.