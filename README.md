# 🛠️ Assembler for Simple RISC
Welcome to the **Simple RISC Assembler**, an open-source assembler written in **C** for a custom Reduced Instruction Set Computing (RISC) architecture. This project enables **tokenization, validation, and encoding of assembly instructions** into machine-readable formats.

## 🚀 Project Overview
This assembler **translates Simple RISC assembly code** into integer-based binary representations, ensuring accurate instruction processing and validation.

### 🔍 Features

✅ **Instruction Parsing & Tokenization**

✅ **Hexadecimal & Operand Validation**

✅ **Register & Label Handling**

✅ **Binary equivalent integer file generation**

✅ **Binary File Generation**

✅ **GUI Assembler (made with php and stored in /dev/GUI_ASSEMBLER)**

## How To USE?
the main fail is in dev, if you want to use it in windoes compile the code in windoes using the following
```
gcc -o main.exe main.c
```
If you are on other devices like mac or linux

```
gcc -o main main.c
```
And the GUI ASSEMBLER is made with php as server side logic

With in the dev folder there is a folder called GUI_Assembler, now within this there is file_processing.php

Now, at the lines 11 and 12 i have commented them, uncommet them if using in windows and comment 13 and 15 lines as they are linux-based system

To start the localhost of the web project

Naviage to GUI_Assembler Project, and type the following
```
php -S localhost:8000
```

The above command will localhost the website, now you type localhost:8000 in brower and you will see the assembler

## Basic look on how the website looks like

<img width="732" alt="Screenshot 2025-03-11 at 12 19 49 PM" src="https://github.com/user-attachments/assets/2a2c80c0-c778-40e8-885a-a11954cf4be3" />




---

## 📂 Project Structure
----------------------------------------

    ├── README.md             # 📖 Project Documentation

    ├── index.html            # 🌐 Web-based documentation (Hosted on GitHub Pages)

    ├── main.css              # 🎨 Styling for the documentation

    └── dev/

        ├── main.c            # 🏗️ Contains the main() function
    
        ├── token_parse.c     # 🔍 Handles tokenization & input validation
    
        ├── tokens.h          # 🏷️ Header file for tokens
    
        ├── instruction_encoder.c  # 🛠️ Converts tokens to integer representations
        
        ├── out.int           # 📜 Integer-encoded instructions
    
        ├── asm.txt           # 📄 Sample assembly code
    
        ├── asm1.txt          # 📄 Additional sample code
    
        ├── main              # 🔧 Compiled binary
        
        └── GUI Assembler/

            ├── index.html            # 🏗️ Contains the index to the assembler GUI
    
            ├── file_processing.php     # 🔍 Handles the user input and compiling
    
            ├── main.css         # 🏷️ Styling for the code
        
        


----------------------------------------

# 🔢 Instruction Encoding Format

The out.int file contains integer-encoded instructions based on the following format:

```
Assembly Instruction:   add r15, r2, 0x12
Integer Encoding:       0|1|15|2|0|18
```


- **Opcode Mapping:** Each instruction is assigned a unique integer opcode.
  
- **Register Encoding:** Registers (`rX`) are mapped to their respective
integer representations and X is in between 0-15.

- **Immediate Values:** Hex values (e.g., `0x12`) are converted to their decimal equivalents.

---

## 🛠️ Assembler Workflow

1️⃣ **Tokenization & Validation** (`token_parse.c`)
- Extracts tokens from assembly code
- Validates registers, operands, and syntax
- 

2️⃣ **Instruction Encoding** (`instruction_encoder.c`)
- Maps tokens to corresponding integer values
- Converts labels and immediate values into their respective formats
- 

3️⃣ **Binary File Generation** (`out.int`)
- Stores machine-readable instruction sequences

---

## 🔗 Live Documentation
📖 **[View Online Documentation](https://sowhatnowgithub.github.io/Assembler_simple_risc/)**

If you haven't visited our hosted documentation, please **check the main rules and implementation details** at the bottom of this README.

---

## 📜 Assembler Rules & Implementation Details


### 🏗 Development Guidelines

🔹 **Relative Addressing & ORG Handling**

🔹 **Opcode & Operand Validation**

🔹 **Register Addressing Modes**

🔹 **Label Mapping with Instruction Addresses**

🔹 **Immediate Value Formatting (e.g., `0x24h` format required)**

### ⚠️ Key Constraints

⚡ **The immediate value has to written in lowercase alphabets. ex:`0x12baf`** 

⚡ **The immediate value has to start with in `0x`, only `0x` is given then error will come** 

⚡ **Maximum Instructions:** `999`

⚡ **Character Limit (Excluding Whitespace):** `30,000`

⚡ **Label Length Limit:** `9 characters`

---

## 💡 How to Contribute

We **welcome** all contributions—whether it's fixing bugs, adding new features, or improving documentation.


🔧 **Ways to Contribute:**

✅ **Report Issues** – Found a bug? Open an issue!

✅ **Submit a PR** – Have a fix or enhancement? Send a pull request!

✅ **Improve Documentation** – Help keep our docs clear & concise!


Stay tuned for updates & new features! 🚀


---

⭐ **If you like this project, consider giving it a star!** 🌟

📌 **Maintained by:** *sowhatnow/sowhatnowgithub*
