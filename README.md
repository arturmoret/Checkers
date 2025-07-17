# Checkers – C++ Game with SDL2

A fully-functional Checkers (Draughts) game built in modern **C++17** using **SDL2** for graphical rendering.  
Includes complete game logic, turn-based interaction, mandatory captures, and a replay system from saved files.

---

## Features

- **Graphical board with real-time interaction**  
  An 8×8 checkers board rendered with SDL2, supporting mouse input and live visual feedback.

- **Two gameplay modes**  
  - *Normal*: Two-player game with full rules and move validation  
  - *Replay*: Load a previous match and replay every move from a recorded file  

- **Complete rule enforcement**  
  - Piece movement and diagonal restrictions  
  - King promotion when reaching the final row  
  - Mandatory captures with recursive multi-jump detection  

- **Save & load support**  
  - Board states stored in `.txt` files  
  - Move history saved as a sequence of positions for replays  

- **Visual cues and indicators**  
  - Valid moves highlighted during player interaction  
  - Display of current mode, active player, and winner status  
  - Mouse coordinates shown in real time  

---

## Key Design Choices

- **Recursive capture resolution**  
  Multi-jump scenarios handled recursively to follow official rules for forced captures.

- **Dynamic memory and smart pointers**  
  Move history is stored in a linked list structure, and all piece objects are managed through smart pointers for safe allocation and deallocation.

- **Separation of concerns**  
  Clear modularity between logic and rendering. The UI layer handles input and visuals, while the core logic remains independent and testable.

- **Modern C++ practices**  
  RAII, `const`-correctness, and minimal use of raw pointers throughout the codebase.

---

## Build & Run

### Dependencies

- C++17  
- SDL2  

### Build Instructions

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make

