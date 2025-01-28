# Steganography Application

A command-line application for hiding and retrieving messages in image files using steganography techniques. 
Supports various operations including checking file information, encrypting messages, decrypting messages, and validating file suitability.

**Features**

Supported Formats: .png, .bmp
Commands:
- -i or -info: Display image file details (e.g., dimensions, size, last modified timestamp).
- -e or -encrypt: Embed a message in an image.
- -d or -decrypt: Extract a hidden message from an image.
- -c or -check: Check if a message can be hidden or is present in the image.
- -h or -help: Display help and usage instructions.
- -q or -quit: Exit the application.

**Requirements**

C++20 compiler
fmt library (v11.0.2)
SFML library (v2.6.1)

**Build and Run**

Clone the repository:
git clone <repository-url>
cd <repository-name>

Build using CMake:
cmake -B build
cmake --build build

Run the application:
./build/steganografia

**Usage**

Run the program and input commands as specified:
<flag> <file_path> [message]

Examples:

Display image info: -i path/to/image.png
Hide a message: -e path/to/image.png "your message"
Extract a message: -d path/to/image.png
Validate suitability for hiding a message: -c path/to/image.png "your message"

**Known Issues**

Windows Users: In some cases, the application may fail to run due to missing .dll files. Ensure all required dynamic libraries for the SFML and fmt libraries are properly installed or included in the application's directory.

**Error Handling**

Checks for unsupported file formats.
Ensures valid paths and file permissions.
Provides feedback on invalid or unrecognized flags.

**Author**

Kacper Płachetka