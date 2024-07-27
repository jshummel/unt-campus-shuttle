# CSCE 3444.503 - UNT Campus Shuttle

This project involves parsing UNT shuttle timetables to create a GUI application that allows users to search for bus route information and build a "smart route" based on user-supplied starting location, destination, and current or future time.

## Program Functionality
The program parses the `.csv` files and creates a graph data structure for the database, which is used for route processing. Users can add or remove routes based on their availability for the current UNT semester. The "smart route" functionality calculates the most efficient path between two points considering real-time or future schedules.

## Running the Program
To run the compiled program, follow these steps:
1. Ensure the _IMAGES and _CSV folders are available in the root directory.
2. Navigate to the _RELEASE folder.
3. Execute the program with:
```sh
BusRouteApplication.exe
```

### Prerequisites
- Ensure all required `.csv` files are in the _CSV folder.
- The _IMAGES folder should contain any necessary graphical assets.
