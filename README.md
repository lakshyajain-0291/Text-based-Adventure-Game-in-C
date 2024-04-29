# The Vindication: Customizable Open-Ended Text-Based Adventure Game World in C

## Overview
"The Vindication" is a modifiable historical fiction open-ended adventure game world developed in C. It offers a captivating text-based gaming experience enriched with various features like mini-games, NPC interactions, quests, terminal ASCII graphics, in-game audio, and more. The game leverages JSON files for storing and parsing game content, ensuring easy modification and expansion.

## Disclaimer
"The Vindication" contains elements of gore and violence. It incorporates literary and music elements without obtaining copyrights. Distribution of this game is not officially supported by the developers. Users are responsible for ensuring compliance with all relevant copyright laws and regulations when using or distributing this software.

## Features
- **Open-World Design:** Explore various locations and interact with NPCs in a vast, open-ended game world.
- **Modifiable Quests:** Easily expand or modify the 27 quests available using JSON format storage.
- **Mini-Games:** Engage in several mini-games integrated into the gameplay, including:
  - Minesweeper
  - Prison Escape Game
  - Hangman
  - Memory Card Game
  - Lock Picking (Number Code Breaker)
  - Combat Game (incomplete implementation)
  - Falconry Game
- **JSON Data Structure:** All game content, including NPC interactions, quest details, and mini-game parameters, are stored and parsed using JSON files, enabling seamless customization and expansion.
- **Unique Game ID and Save Functionality:** Initiate the game with a unique ID and load saved games seamlessly using individual JSON files for each session.
- **Three-State Game Loop:** Navigate through the game world, interact with NPCs, and engage in quests using a three-state game loop system.
- **Item System:** Equip items to receive buffs, enhancing gameplay experience and strategy.
- **Sound Integration:** Enjoy in-game audio using system commands for .mp3 playback (available for MacOS devices).
- **Code Logic and Game Mechanics:** Detailed description of the game's states, mechanics, and mini-games provided in the project report.

## Usage
To compile "The Vindication," use the following command:
```
gcc -o Vindication vindication.c main.c cJSON.c minigames.c
```
then run the executable 
```
./Vindication.exe
```
for Windows OS
or
```
./Vindication.out
```
for Ubuntu or MacOS

Follow the on-screen instructions to navigate through the game world, interact with NPCs, complete quests, and enjoy the mini-games. Ensure that the necessary JSON files are present in the game directory for proper functioning.

## Requirements
- C compiler compatible with the provided codebase.
- cJSON library for JSON parsing and manipulation.
- Terminal environment supporting ASCII graphics and audio playback.

## Contributors
- Aradhya Mahajan (B23CH1008)
- Danie George John (B23CI1012)
- Harsh Nandan Shukla (B23MT1019)
- Lakshya Jain (B23CS1039)
