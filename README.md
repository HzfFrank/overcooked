# Game overcooked :rice:

Here is a an amazing couch-coop game overcooked and I simplified it to a flat perspective as follows:

![img](image/demo.gif)

The two blue circle represent the two players, you can collaborate with your partner to complete orders and earn points by selecting, processing, serving, and washing dishes.

You can experience this game by the following two ways:

## Play it by your hand

```
cd QtOvercooked
QtOvercooked.exe -l ..\overcooked\maps\level1\level1-1.txt
```

"-l" parameter is followed by the map path, you can experience different types of maps I provided in ```\overcooked\maps```

Execute the above instructions in Windows terminal and it will show the game interface.

Player1 :boy: can be controlled by ```"WASD"``` (which represent moving up, left, down, right respectively), ```"Space"``` (which represents pick up or put down items) and ```"J"``` (which represents interacting, such as cooking and washing dirty plates).

Player2 :girl: can be controlled by the direction keys on the keyboard :arrow_up: :arrow_left: :arrow_down: :arrow_right: (which represent moving up, left, down, right), ```"Enter"``` (which represents pick up or put down items) and ```"Ctrl"``` (which represents interacting, such as cooking and washing dirty plates).

The top left corner shows the current outstanding orders and scores

## Let it run automatically

You can modify the code in ```\overcooked\source``` and let it run automatically just like the gif above shows.

```
cd QtOvercooked
QtOvercooked.exe -l ..\overcooked\maps\level1\level1-1.txt -p ..\overcooked\cmake-build-debug\main.exe
```
"-l" parameter is followed by the map path, you can experience different types of maps I provided in ```\overcooked\maps```

"-p" parameter is followed by the executable file of your policy code (the default is ```\overcooked\cmake-build-debug\main.exe```)

The logic it automatically runs is to control the player's movement based on the output string in main.cpp, in frame x, you are supposed to output ```"Frame" + x``` in the first line and output ```"Move [LRUD]*n"``` or ```"Interact [LRUD]*n"``` or ```"PutOrPick [LRUD]*n"``` in the second line.

Here also provided two efficient auxiliary tools to help you check your result: 

```
cd QtOvercooked
runner.exe -l ..\overcooked\maps\level1\level1-1.txt -p ..\overcooked\cmake-build-debug\main.exe
```
Execute the above instructions in Windows terminal and it will show your score in a short time.

run ```QtOvercooked.exe -l ..\overcooked\maps\level1\level1-1.txt -p ..\overcooked\cmake-build-debug\main.exe``` and then check the ```clilog.txt``` in the ```QtOvercooked``` it will show all the string that was just output and input during program execution.

Here is the score this original code can get: 

| map | score|
| :-: | :-: |
| 1-1 | 2022 |
| 1-2 | 2377 |
| 1-3 | 2142 |
| 1-4 | 2152 |
| 2-1 | 1590 |
| 2-2 | 1546 |
| 2-3 | 1228 |
| 2-4 | 1064 |
| 3-1 | 1842 |
| 3-2 | 1660 |
| 3-3 | 781 |

Hope you can modify the code and get a higher score!
