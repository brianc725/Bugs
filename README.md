# Bugs
A game simulation coded in C++ where Bugs compete against each other to fight for resources and survive the longest.

To run Bugs:

1. You'll need to create the executable using the code and have the executable in the same folder as the assets, field.txt, and .bug file.

2. Using terminal, cd to the right directory. 

3. Ensure the Bug executable and all of the other files are available via ls. 

4. Type
	./Bugs field.txt USCAnt.bug USCAnt.bug USCAnt.bug USCAnt.bug 
This will start a game using the field.txt as the file and 4 bugs, all of USCAnt's. 

# The field can be modified by using: 

‘*’ *characters designate pebbles which block movement of all insects1. Every field must have its perimeter consist entirely of pebbles.
<br>
‘g’ characters specify the starting locations of baby grasshoppers. 
<br>
‘0’, ‘1’, ‘2’ and ‘3’ specify the location of the four ant colonies’ anthills.
<br>
‘w’ characters specify pools of water.
<br>
‘f’ characters specify piles of food.
<br>
‘p’ characters specify poison.
Space characters represent empty locations.

The field.txt is a 64x64 grid.

# Program your own .bug

Use the pdf titled AntInstructions.pdf to create your own .bug colony to fight against your friends!
