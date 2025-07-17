# Checkers – C++ Game with SDL2

A complete, modern implementation of the classic Checkers game in **C++17**, featuring an interactive graphical interface with **SDL2**, rule enforcement, and support for saving and replaying matches from files.

---

## Features

- **Graphical 8×8 board with real-time interaction**  
  Smooth SDL2-powered interface with mouse support and visual feedback.

- **Two game modes**  
  - *Normal*: Turn-based match between two players with full rule validation  
  - *Replay*: Load saved moves and watch a full game unfold automatically  

- **Full rule support**  
  - Diagonal movement and valid piece restrictions  
  - Mandatory capture logic (including multi-jump chaining)  
  - Automatic king promotion when reaching the opposite side  

- **Game state persistence**  
  - Save and load board configurations from plain text  
  - Store entire move history for playback and analysis  

- **Visual feedback**  
  - Highlighted valid moves  
  - On-screen status: current player, mode, winner  
  - Real-time mouse coordinate display  

- **Modern C++ design**  
  Emphasizes modularity, smart pointers, RAII, and `const`-correctness for safe and maintainable code.

---

## Controls

<div align="center">

| Input         | Action                                 |
|---------------|----------------------------------------|
| Left Click    | Select a piece / destination square    |
| Right Click   | Cancel current selection (if any)      |
| `Esc`         | Exit the game                          |

</div>

In **Replay Mode**, moves can be triggered per frame with clicks, depending on your configuration.

---

## Build & Run

### Dependencies

- C++17  
- SDL2  


