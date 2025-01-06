# 8-bit Games Strike Back: SnakeAI

## Author
Tomáš Hanzlík

## Project Overview
This project implements an intelligent control system for the classic arcade game "Snake." The snake is designed as an isolated, autonomous, and reactive agent operating within a fully observable environment. The objective of the game is for the snake to consume as many apples as possible while avoiding collisions with itself or the game boundaries.

---

## Task Environment
- **Success Metric**: The number of apples consumed by the snake.
- **Game Environment**: The game board is fully observable. New apples are generated stochastically, randomly placed on any available free cell. The snake cannot pass through walls for easier autonomous movement implementation.

---

## Agent Behavior Implementation
The snake operates using an agent function that adjusts its behavior dynamically based on the state of the environment. The following techniques were explored and applied:

### Initial Behavior: A* Algorithm
- **Purpose**: To find the shortest path to the apple.
- **Implementation**: 
  - The A* algorithm is executed at every step, but only the first planned move is used to prevent the snake from taking unnecessarily long detours.
  - This behavior is applied until the snake's size reaches a threshold: `board size / CHANGE_BEHAVIOUR` constant.
  - For a 9x9 game board, the constant value `CHANGE_BEHAVIOUR = 6` was determined to be ideal. Larger boards may require a higher constant.

### Advanced Behavior: Systematic Movement
- **Purpose**: To accommodate the snake's larger size by systematically moving toward the goal.
- **Implementation**:
  - A modified A* algorithm is used, prioritizing the most distant fields in the priority queue instead of the closest.
  - The planned path is fully executed without re-planning at every step.
  - The snake keeps track of visited fields during each journey to avoid cycling into dead ends.
  - Adjustments were made to the A* algorithm to allow revisiting local nodes for a "snake-like" movement pattern (row-by-row or column-by-column traversal).
  - The algorithm checks if the snake's head will have access to at least `70%` of all free fields (`PERCENTAGE` constant) using a Breadth-First Search (BFS) during neighbor evaluations.

### Survival Behavior: "Survive"
- **Purpose**: A fallback strategy if no path to the apple is found (e.g., blocked by the snake's body).
- **Implementation**:
  - The snake selects the farthest free field from the apple for its next move.
  - If a path to the apple becomes available during this behavior, the agent reverts to the previous behavior.

---

## Game Environment Details
- The game board dynamically updates with each move.
- The snake can encounter scenarios where its body temporarily blocks access to the apple, but the environment is designed to allow for dynamic resolution of such situations.
- Apples are generated stochastically and placed on random free cells.

---

## Challenges and Reflections
- **Achievements**: The agent successfully consumes apples and adapts its behavior dynamically. On a 9x9 board, the agent typically achieves about 50% board coverage.
- **Limitations**: The system is not perfect and leaves room for improvement, especially in handling edge cases and maximizing the board's utilization.
- **Future Work**: Additional time could be spent refining the agent's strategies to improve performance and robustness.

---

## Conclusion
This project provided an engaging opportunity to design and implement intelligent agent behavior for a classic arcade game. 
---

## How to Run the Game
1. Ensure you have the necessary dependencies installed (e.g., Python and required libraries).
2. Run the main script to start the game.
3. Observe the snake's intelligent behavior as it navigates the board to consume apples.

---

