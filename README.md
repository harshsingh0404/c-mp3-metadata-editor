# c-mp3-metadata-editor
Developed a command-line MP3 Tag Reader/Editor in C that reads and modifies ID3 metadata using binary file handling, structures, and pointer manipulation.
# 🎵 MP3 Tag Reader & Editor (C Project)

## 📌 Overview

This project is a **MP3 Tag Reader and Editor** developed in C.
It allows users to **read and modify ID3 tags** such as:

* Title
* Artist
* Album
* Year
* Genre

## 🚀 Features

* Read MP3 metadata (ID3 tags)
* Edit existing tags
* Supports multiple tag fields
* Simple command-line interface

## 🛠️ Technologies Used

* C Programming
* File Handling
* Structures
* Pointer Manipulation

## 📂 Project Structure

* `main.c` → Entry point
* `read.c` → Reading tags
* `edit.c` → Editing tags
* `header.h` → Structure definitions

## ▶️ How to Run

### Compile:

```bash
gcc main.c read.c edit.c -o mp3tag
```

### Run:

```bash
./mp3tag sample.mp3
```

## 📷 Sample Output

```
Title  : Shape of You
Artist : Ed Sheeran
Album  : Divide
Year   : 2017
Genre  : Pop
```

## 🎯 Learning Outcomes

* Understanding of binary file handling
* Working with real-world file formats
* Memory management in C

## 📌 Future Improvements

* GUI version
* Batch editing support
* Support for more audio formats

## 👨‍💻 Author

Harsh Choudhary
