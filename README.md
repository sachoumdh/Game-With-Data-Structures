# 🎮 Dynamic Game With Data Structures in C 
A terminal-based multiplayer game developed in C as part of the Algorithms and Data Structures 3 module.
The project demonstrates the use of dynamic data structures such as queues and linked lists to manage players, rankings, priorities, and game progression.

## 📋 Project Overview

The game simulates a tournament between multiple players.

Players compete through a series of matches using two different strategies:

### Strategy 1: Digit Sum

- A random number is generated.
- The sum of its digits is calculated.
- If the sum is a multiple of 5, the player earns a point.
- The match continues until:
  - A score difference of 3 points is reached.
  - Or 12 turns have been played.

### Strategy 2: GCD-Based

- Two random numbers are generated.
- Their Greatest Common Divisor (GCD) is calculated.
- If the GCD contains at least one digit appearing in one of the generated numbers, the player earns a point.
- The match continues until:
  - A score difference of 3 points is reached.
  - Or 16 turns have been played.

 ## 📊 Game Rules

Players are moved between structures according to their performance:

### Strategy 1

- 3 consecutive wins → F1
- 3 losses → F3
- 5 total wins → LG
- 5 total losses → LP

### Strategy 2

- Winner → F1
- Loser → F3
- 2 wins → LG
- 2 losses → LP

## 👥 Development Team 
HAMDI Sarah 
DJENKAL Lyna 

Academic Supervisor : S.Boukhedouma

## 📅 Academic Year 
2025-2026 | University of Science and Technology Houari Boumediene (USTHB)

## 📄 License 
This project was developed for educational purposes as part of an end of semester assignement 
