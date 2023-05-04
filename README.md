# Little Elf in the Woods

It's a 2D game that combines the endless runner game and shooter game together
![Watch the video](https://img.youtube.com/vi/AR0ULgA15fc/0.jpg)](https://youtu.be/AR0ULgA15fc)




Full video can also be found in this repository
## How to build

*Windows*

Open git bash to the directory containing this repository.

```
project-template $ mkdir build
project-template $ cd build
project-template/build $ cmake ..
project-template/build $ start project-template.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
project-template/build $ ../bin/Debug/demo.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
project-template $ mkdir build
project-template $ cd build
project-template/build $ cmake ..
project-template/build $ make
```

To run each program from build, you would type

```
project-template/build $ ../bin/demo
```


## Unique features 

Main Character:

![avatar](/arts/Character.GIF)
```
Weapon: bow and arrow
The elf can shoot dark arrows and light arrows to kill bats and gain points
Using the mouse to control the direction of shooting the arrow
However, there are no more than 10 arrows at the same time
```
Enermy 1:
Eyeballs
![avatar](/arts/eye.GIF)
```
Eyeballs walk straightly and cannot be hurt by the arrow
```
Enermy 2:
Bats
![avatar](/arts/bat.GIF)
```
Bats fly toward the elf and can be hurt be the dark arrow.
When bats are killed, they have chances to drop hearts, which can recover elf's HP 
or increase the score when the HP is full(3)
```
Control:
```
Mouse click: shooting arrows
left-click: shoot dark arrows which can hurt enemies
right-click: shoot light arrows which can pick up items

Keyboard: avoid colliding with eyeballs
A: move left
D: move right
```

Infinite Map:
![avatar](/textures/map.png)