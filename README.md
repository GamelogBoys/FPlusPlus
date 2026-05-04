<div align="center"><img width="101" height="101" alt="logo" src="https://github.com/user-attachments/assets/362699cd-4df7-4fb1-a6ef-7d7a7f85d962" />
F++ (FPlusPlus) 🚀
A lightweight, high-performance programming language designed for child programmers, animators, and web developers.

F++ is a custom-built language featuring a professional C++ interpreter and a dedicated installer for worldwide distribution. Whether you're building logic for VFX or simple automation, F++ provides a minimalist and powerful syntax.

<img width="1366" height="728" alt="test fpp - F++ - Visual Studio Code 04-05-2026 01_22_56" src="https://github.com/user-attachments/assets/d385baba-d90b-4649-b676-90dfd83ee269" />

✨ Key Features
Fast Interpreter: Built with C++ for maximum execution speed.

Dynamic Math: Handles both int and float math perfectly using std::fmod.

Professional Installer: 1280x720 GUI installer with path selection and terms of service.

VS Code Integration: Official syntax highlighting available on the Marketplace.

Clean Syntax: Uses intuitive keywords like is, oris, when, and define.

🚀 Getting Started
1. Installation
Download the latest FPlusPlus_Setup.exe from the Releases page.

Run the installer as Administrator.

Accept the Terms and Conditions.

Select your installation path (Default: C:\FPlusPlus).

The installer will automatically download frun.exe and set up your system path.

2. VS Code Setup
To get syntax highlighting (Method A):

Open VS Code.

Go to the Extensions tab (Ctrl+Shift+X).

Search for "F++ Language Support" and click Install.

Code Example: Simple Calculator
Code snippet
// Function Definitions
define add() {
    var a = read{"Number 1: "}
    var b = read{"Number 2: "}
    var res = a + b
    show = {"Sum: ", res}
}

// Main Program Loop
var status = "run"
when status == "run" {
    show = {"1. Add"}
    show = {"2. Exit"}
    
    var choice = read{"Select option: "}

    is choice == "1" {
        add()
    }
    oris choice == "2" {
        var status = "stop"
    }
}

🛠 Built With
Interpreter: C++ (GCC/TDM-GCC)

GUI Installer: Win32 API / C++

Networking: WinINet Library

UI/UX: Custom Light-Mode Minimalist Design

📄 License & Terms
F++ is distributed under the F++ Official License.

Free for personal and commercial creative projects.

The core engine (frun.exe) is protected and may not be reverse-engineered.

Developed with ❤️ by SS Studios.</div>
