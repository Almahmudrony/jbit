# Game API Tutorial #

In this tutorial I assume that you have read the Beginner's Tutorial, spent a
bit of time studying the 6502 examples and written some snippets of code
targeting the MicroIO chip.

## Tiles

The MIDP2 IO chip (called simply IO chip from now on) provides essentially
the same level of control that professional game programmers have when
writing 2D action games. This flexibility comes at a price: complexity.

Most of this complexity is usually at the initial stage of the program. How
many objects are there? What is their shape and color? What about the
playfield? How big is it? Does it scroll horizontally? Vertically? Not at
all? All of these questions must be answered, answered of course using
bytes... you can imagine that it is not going to be easy.

The "Empty" demo included in the GameKit provides a set of answers. On page
3, where you would normally write your program, a ready-made block of code
configures the IO chip. Once the IO chip has been configured, the control is
transfered to the code at 4:0.

The answers that Empty provide are designed to setup a playfield that will
feel like the MicroIO display from the inside, but that will look prettier
from the outside. Instead of displaying letters, symbols and numbers, you can
display painted tiles chosen from a set of 56 tiles, thanks to:

	http://www.famfamfam.com/lab/icons/silk/

Here is the description of the first 8 tiles:

	 1 shape_square
	 2 lightbulb_off
	 3 lightbulb
	 4 lock_open
	 5 lock
	 6 emoticon_unhappy
	 7 emoticon_smile
	 8 door

So, is it just a matter of writing 3 into 2:42 to make a bulb appear?
Unfortunately not. A direct link of cells to tiles is fine for a small grid,
but with the IO chip you can define grids of thousands of tiles. Nothing
prevents you from defining a grid of, say, 500x500 tiles (i.e. 25000 tiles)
and show only a portion of them.

Having said that, Empty actually configures as many tiles as possible to fit
on the display of your phone. For example, on a phone with a resolution of
176x220 pixels, 11 columns of 13 rows of tiles are available (each tile as
configured by Empty is 16x16 pixels big).

So, how can you access a potentially large number of tiles with the IO chip?
One at a time. The IO chip has the notion of the Current Tile. You can use
the following cells:

	Reg    Content
	----   -----------------------------------
	2:87   Column of the Current Tile
	       (initially 0, the leftmost column).
	2:89   Row of the Current Tile
	       (initially 0, the topmost row).
	2:91   The tile itself
	       (initially all 0, empty cells).

After all this theory, let's do some practice. Load Empty, go to the Editor,
select Save and enter a new name for it. Go to cell 4:0 and type the
following:

	LDA #2
	STA 2:87
	LDX #3
	STX 2:91
	INC 2:89
	STX 2:91
	INC 2:89
	STA 2:91

Also, go to cell 20:5 and enter:

	100 150 200

These three cells are used by Empty to configure the background color (20:5
is red, 20:6 is green and 20:7 is blue).

Run the program and make sure that you understand it.

## Paint

The standard JBit2P distribution includes a demo called "template" that is
similar to the "Empty" in the GameKit.

If you load it and look at page 4:0, you will see the following code:

	LDA #1
	STA 2:91

If you followed the description above, you would expect a tile to appear, but
if you run the program you will see that the display remains empty. What is
going on? Unlike Empty, template does not come with predefined tiles, but it
expect you to paint them.

Exit the Editor and start the Paint module. You should see a list with a
single item in it:

	21:0 (64x96).

This means that the program includes a single image of 64x96 pixels at
location 21:0. Select the item.

You can move the red square and then press fire to change the pixels of the
image one by one. Tiles in the "template" demo are 8x8 pixels big and the
tile number one is located at the top left corner. On some old phones, moving
the cursor can be really slow. If this is the case, selecting the
"(No)Coords" item from the menu should help.

When you have changed the tile number one to your liking, you can test the
result by pressing *.

Here is a short, partial and terse description of the Paint module. There are
two major modes: image and palette. You can switch between the two by
pressing 0.

There are two sub modes for the image mode: pixel and tile. Tile mode is only
available if the the dimensions of the image are multiple of 8. You can
switch between the two by pressing #.

Image/pixel mode: 2, 4, 6, 8: move the cursor, 5: change the current pixel,
1: cycle foreground color, 3: cycle background color, 7: pick foreground
color, 9: pick background color.

Image/tile mode: 2, 4, 6, 8: move the cursor, 1: cycle foreground color, 3:
cycle background color, 7: copy tile, 5: paste/clear tile. Be careful when
you use tile mode, as no undo is available and it is easy to clear entire
tiles.

To practice painting, an idea could be to paint the first row of tiles using
the descriptions above (e.g. 2 lightbulb_off) as a source of ispiration. When
you have finished, you could save the resulting program as "Empty" and
continue with the rest of the tutorial.

You can then type the program above and check that it works. The "template"
demo also uses the three cells starting at 20:5 to configure the background
color.

## Animation (1)

Animation is essentially about timing and loops.

Programs so far have been "linear", i.e. after a few instructions, the
program would reach a BRK and terminate. To be able to write animations, we
need to be able to repeat a sequence of instructions over and over again. The
easiest way to do it is to use JMP, for JuMP. It allows us to change the PC
of the CPU, thus changing the address of the next instruction to execute.

With this knowledge, you might think of writing the following program
(starting from 4:0; I assume that you have loaded Empty first):

	INC 2:91
	DEC 2:91
	JMP 4:0

Unfortunatelly, this program does not work. Well, it might work, but it is so
unpredictable and inefficient that you really do not want to run it.

It is unpredictable, because JBit tries to run the program as fast as it can,
and the actual speed varies a lot from phone to phone. My tests and the
feedback that I have received suggest that JBit is able to simulate from
around 10,000 instructions per second on slow phones to around 500.000
instructions per second on fast ones.

It is inefficient, because it would try to change the tile too often. Even on
a slow phone, the tile would change thousands of times per second.

Actually, all this work would be wasted, because the IO chip would not even
bother to keep up. The IO chip would update the tile eventually, but you have
no idea how often. It might even be the case that the IO chip would use the
same value more than once. What you would see is effectively a sampling of a
very fast variable. The odds are not even 50%, as the value 0 is more likely.

There is a better way. When you are finished arranging the tiles, you can
tell the IO chip that now is the time to update the display. You do so by
writing any walue into 2:18. This has the nice side effect that JBit will
suspend its CPU for a while and tell your phone that JBit is idle. Some
phones might not care, but some might be able to switch their CPU into
low-power mode in turn, draining less power from the battery.

So, this is a predictable and efficient version of the program above:

	INC 2:91
	STA 2:18
	DEC 2:91
	STA 2:18
	JMP 4:0

## Animation (2)

Usually, we don't want to repeat a sequence of instructions forever, but just
for a little while. We can do this by using BNE, for Branch if Not Equal.
Branch instructions change the PC only if a specific condition is met. In the
case of BNE, Not Equal is a bit misleading, as the condition really is Not
Zero and it refers to the result of the instruction before the branch.

For example, the following sequence:

	...
	LDA #3
	BNE 4:10
	...

will continue to 4:10, while the following:

	...
	LDA #0
	BNE 4:10
	...

will continue to the next instruction after BNE.

There is a slight complication here. If you try to type in BNE, you will
notice that the format is BNE r, not BNE n:n as you might expect.

The reason is that branches usually point to an instruction nearby and two
bytes would often be wasted. The single byte operand is used as the number of
bytes to skip forward (if between 0 and 127) or backward (if between 128 and
255, where 255 means 1, 254 means 2, etc...). Fortunately, there is no need
for you to count them.

We will use the following code as an example:

	LDX #30
	STA 2:18    [1]
	DEX
	BNE <<1>>

We want the BNE instruction to point to STA 2:18, thus writing into 2:18
thirty times and causing the VM to run for about 3 seconds (plus the time
spent for initialization).

Load Empty and type in the code starting from 4:0, leaving the operand of BNE
to 0. Then switch to NAV ASM mode. The listing should look like this:

	LDX #30
	STA 2:18
	DEX
	BNE !!!!

The BNE is pointless as it is (no matter the result of DEX, no branching
takes place) and !!!! is a reminder that you probably meant to change it
later on.

Here are the steps:

1. Make sure that the cursor is on BNE !!!!

2. Press 7. This sets an invisible marker on this address (4:6).

3. Move the cursor to STA 2:18 (i.e., 4:2)

4. Press 0. This swaps the cursor and the marker. The cursor is now on the
BNE !!!! instruction and the invisible marker is now on the STA instruction.

Select PutMark from the menu.

The listing should now look like this:

	LDX #30
	STA 2:18
	DEX
	BNE 4:2

With the cursor still on the BNE instruction, press 9. This moves the cursor
to the address pointed by the operand and can be handy to check if the
operands are correct.

If you run the program, it should last for a bit more than 3 seconds and then
terminate.

As an exercise, type in the following:

	LDY #10
	LDX #8      [1]
	LDA #2
	STA 2:91
	STA 2:18    [2]
	DEX
	BNE <<2>>
	LDX #2
	LDA #3
	STA 2:91
	STA 2:18    [3]
	DEX
	BNE <<3>>
	DEY
	BNE <<1>>

## Key presses

When you are confident in writing animations, you can start making programs
that react to the user (i.e. interactive programs).

Empty includes a handy subroutine. It takes care of updating the display and
saving in the Accumulator the code of the key that the user has pressed.

What is a subroutine? It is a piece of code that can be used again and again.
You don't even have to understand how the piece of code works to be able to
use. You "call" the subroutine (using JSR), and, when the subroutine has
finished to do its job, the control returns to your code.

You can call the subroutine above with:

	JSR 3:3

You can then check if a key has been pressed (BNE) or not (BEQ). For example,
to wait for the user to press two keys, you could write something like this:

	JSR 3:3     [1]
	BEQ <<1>>
	JSR 3:3     [2]
	BEQ <<2>>

If the user has pressed a key, you can inspect the Accumulator to find out
which key it was. The key codes are the same as the ones you use to write
into 2:40 - 2:79. As a reminder, on a standard keypads they are:

	key|value
	---+-----
	 # | 35
	 * | 42
	 0 | 48
	 1 | 49
	 . . ..
	 . . ..
	 9 | 57

On a QWERTY, you might receive other codes too (e.g. 65 or 97 if the user
presses the 'A' key).

Here is a more complex example. Press '1' or '3' to change a tile on the
screen, and '*' to terminate the program.

	JSR 3:3     [1]
	BEQ <<1>>
	CMP #49
	BNE <<2>>
	DEC 2:91
	JMP <<1>>
	CMP #51     [2]
	BNE <<3>>
	INC 2:91
	JMP <<1>>
	CMP #42     [3]
	BNE <<1>>

If you have problem understanding it at first, you can set a break point on
the instruction CMP #49 (i.e. from within the editor, go to 4:5 and select
SetBrkPt before running the program). Once you press a key, the program is
stopped and you can start pressing '1' to see what happens. If you are lost,
you can press '*' to resume the program and try again with another key.
