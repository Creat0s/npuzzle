# npuzzle

Npuzzle is a small library that allows you to create *NxM puzzles* that operate on the
principles of *15 puzzles*. This repository also contains an implementation and a copy
of the game using the [raylib](https://github.com/raysan5/raylib) library.

## Game

The game uses only *NxN puzzles*, where *3 <= N <= 7*. However, this does not prevent
you from creating puzzles of different dimensions in your projects using this library.
The game was built for Microsoft Windows and Android platforms. To manually build the
provided game example, you will need to use the MAKEFILE from the [raylib](https://github.com/raysan5/raylib) library.

## Lib

The library consists of 3 functions. The function *create_puzzle(array, sizeN, sizeM)*
takes a pointer to a one-dimensional array of integers *int*, where *sizeN* is the
number of horizontal columns, and *sizeM* is the number of vertical columns.
*create_puzzle()* starts filling your array with numbers and then launches the *is_solvable()*
function. If the generated puzzle is solvable, *create_puzzle()* stops its work; otherwise,
it continues generating puzzles. The *count_inversions()* function is called by *is_solvable()*
to obtain the number of inversions in the puzzle. These functions work the same way on any platform.

## Supported platforms (Game)

| Name              |   Version   |
| ----------------- | ----------- |
| Windows           | 7/8.1/10/11 |
| Android           | 7.0+        |

You can easily rebuild this sample game on other platforms supported by [raylib](https://github.com/raysan5/raylib).
You also can download the game from the following link: [google disk](https://drive.google.com/drive/folders/1IHS4GEI9Vd_sIaLYdIvFF8ndfOnoedNP?usp=sharing).
