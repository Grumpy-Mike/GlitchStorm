# glitch-storm

Glitch Storm is an exploratory synth that can be build and soldered even for total beginners. It produces a plethora of blips and blops ranging for chords, techno rhythms to alien soundscapes and FXs.

At its core is a Bytebeat synth but this one is interactive. You play it tuning into frequencies and soundscapes. You have four dimensions in it to explore, three ones via the knobs and a forth one via buttons, changing the equation that is playing.

The unit is able to be played as a standalone or in the context of one studio or modular.

You can get PCBs, Kits and Built units in this Tindie Store
https://www.tindie.com/stores/jc2046/

------------------------------------------------------------------

FROM Mike Cook
This is my take on the project. Here you will find my schematic and software along with some images. 
Video at:- https://youtu.be/A6AD-EArwGI

Changes by Mike Cook Jan 2020
HARDWARE
I made it in a triangular box, cut out on my laser cutter. If you do not have access to one you can still make this by printing out the design files and sticking the printouts to the back of 3mm plywood. Then drill and cut to make the top. The base should be drilled by fixing the top to the base and drilling through the holes in the top into the base. In this way you will assure they line up. The sides should be cut out and the ends angled at 30 degrees using a disc sanding machine. Then position and glue to the top. I used super glue to get them into position and then used a fillet of clear Gorilla glue on the insides. Note if you use a laser cutter sand down the cut side otherwise the super glue will not stick to the cut side.

DESIGN
I corrected the mistake of the transposed capacitor and resistor on the audio output of the original circuit and added more decoupling capacitors and also capacitors on the Reference input of the Nano and the analogue inputs to stabilise the potentiometer readings. I added a two colour LED, red / blue to the blink display. I also added a TDA7052 amplifier and an optional audio output jack. You can connect Pin 11 directly to a speaker providing the impedance of the speaker is 33R and you have a 100R resistor in series with it. You connect the other speaker input to ground. The binary output should have bit 0 as the right hand most LED in the row. I chose a different LED colour for each bit, and I added a battery to power it all. I used 22K pots because I had some, but 10K ones are fine.

SOFTWARE
Cleaned up the code to remove unused variables, unnecessary blank lines, and library #includes.

Added a more interesting blinking LED for panel display using a two colour LED.

The timing clockout easter-egg mode simply messes up the LED number display so I have removed it.

There were only sounds 1 to 15 so I added a sound 0 from a commented out sound algorithm in case 5.

The variables aTop, aBottom, bTop, bBottom, cTop and cBottom only change with program numbers, it is a waste of precious interrupt time to set them on every sample interrupt. So I set them with the program number, from a predefined array, with original values, when you read the pots with analogRead.
For case 14 -  "cTop = 1; cBottom = 6;" which makes no sense, apart from disabling the third pot. So these values were swapped.

The buttonsManager code was full of timing to see if it was a long press for either button. But then nothing was done with this information so I simplified the code by removing it. There was also code in the button manager to set variables to show a button was being held down, this is not necessary, again it was removed.
    
Using the C language there is no advantage in short variable names unlike interpreted languages, but I have kept most of them for compatible reasons.
 
I converted the sample interrupt ISR to use a vectored jump table. This is faster than the case statement originally used so allows more time to compute the next sample.

If the blinking LED stops or slows down then the sound routine is too much for the sample rate or processor. What is happening is that the time spent generating each sample is making the millis() timer miss some calls and so the blinking rate which is controlled by it will slow down or appear to stop. It will still produces sound though.

Take time to listen to what it is producing as sometimes the sequences are long. Less knob twiddling is often more. 