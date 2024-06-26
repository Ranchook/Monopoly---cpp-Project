# Monopoly---cpp-Project

This project involves implementing a Monopoly game in C++ as part of an Object-Oriented Programming course.

# Key Components:
### Game Board (pictured below): Consists of N slots, each being either a property or an instruction slot.
### Properties: Belong to specific city groups, have purchase prices and rent values.
Instruction Slots: Include Jail, Start, and Get Ticket types.
Deck: Contains cards with positive or negative monetary values.
Players: Have names, balance, position on board, and owned properties.

# Game Engine
Manages the game board, player turns, and the deck.
Reads initial game setup from a file.
Handles player actions and game flow.

# Game Flow
Players take turns rolling dice and moving on the board.
Different actions occur based on the slot type landed on (buy property, pay rent, draw card, etc.).
Players can mortgage properties if unable to pay debts.
Game continues until only one player remains.

# Game board:
![Monopoly Board](https://github.com/Ranchook/Monopoly---cpp-Project/assets/102590409/7440f7c3-55e9-46d1-8eef-42b9666f625d)
