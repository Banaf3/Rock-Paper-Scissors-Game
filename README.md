# Rock Paper Scissors Game 🎮

A console-based Rock Paper Scissors game developed in C programming language with persistent user data storage and comprehensive game statistics.

## 📋 Project Details
- **Course**: BCI1023 Programming Techniques
- **Semester**: 2 2023/2024
- **Group**: 25, Section 03A
- **Instructor**: Dr. Zuriani

## ✨ Features
- **User Authentication**: Login/Register system
- **CRUD Operations**: Create, Read, Update, Delete user accounts
- **Persistent Data**: Statistics saved in external file
- **Game Statistics**: Win/Loss/Draw tracking
- **ASCII Art**: Enhanced visual experience
- **Menu-driven Interface**: Easy navigation

## 🛠 Technical Implementation
- **Language**: C Programming
- **Data Structures**: 2D Arrays, Parallel Arrays
- **File I/O**: Reading/Writing to `RPSstats.txt`
- **Memory Management**: Dynamic file operations
- **Random Generation**: Computer move selection
- **Modular Design**: 12 specialized functions

## 🚀 How to Run
```bash
gcc RockPaperScissors.c -o RockPaperScissors
./RockPaperScissors
```

## 📁 File Structure
```
├── RockPaperScissors.c    # Main source code
├── RPSstats.txt          # User statistics database
└── README.md             # Project documentation
```

## 🎯 Game Rules
1. Rock beats Scissors
2. Paper beats Rock  
3. Scissors beats Paper
4. Same choice = Draw

## 📊 Data Storage Format
```
username wins losses draws total_games
```