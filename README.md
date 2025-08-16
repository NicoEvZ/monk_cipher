# Monk Cipher Clock
## What is it?
This project sets out to create a digital clock using an unconventional numbering system, The "Monk Cipher".

This system can represent any 4 digit number, up to 9999, in one combined cipher.
## Inspiration
Many years ago, I came across an image of the cipher on the internet, originally hosted on an author's personal website:
https://web.archive.org/web/20160304051616/http://www.davidaking.org/Ciphers.htm
![Ciphers_clip_image002](https://github.com/user-attachments/assets/f338e3f2-74d1-4d38-98e8-4bbcf10ae430)

The modular nature of the cipher intriuged me, and something about having a modern digital being able to show numbers, using such an ancient notaion scheme, really tickled something in my brain.

I had ideas bouncing around in my head for years. Ideas like using some kind of segmented display, like a 14-segment display. Other ideas included just having a small OLED screen and drawing out the ciphers.
None of these really struck a cord, or didnt fully work for the concept. 

I had all sorts of ideas, but never managed to make anything, software or hardware wise... until now!

A recent video I saw on YouTube is what really got me back into the headspace for this project.

https://youtu.be/dBEupkQBFis?si=4U0O5Yceylcod2Og

This video popped up on my recommended, and I instantly knew this display would be perfect.

## Simulation
First step of this project is to have a simulation of the display running in software. This way I can test ideas, and figure out the easiest ways of doing the actual cipher generations, before thinking about hardware.

I'm using C, and printing coloured characters to the commandline, as seems to be becoming my style...

A lot of things I have learned in my CLI renderer project have come in handy, and allowed me to very quickly get a working software simulation working.

## Physical Clock
### Display
The component that will be critical in determining if this project becomes a real thing is the HCMS-29XX series displays.

These have 4 distinct character displays, featuring a 5x7 LED matrix.
This is *just* enough pixels to render a pretty faithful and legible rendition of the ciphers, without taking too many creative liberties.

These are a bit pricy (That guy in the video is wearing nearly NZD$1000 on his wrist, in displays alone!), but I think just a single 4 character display is all I need. 

The warm yellow glow is just tantilising enough, to fork over the cash for.

### Controller
I have a STM NUCLEO-F030R8 bouncing around that I have wanted to learn how to program for a few years.
It will be used for initial breadboarding, and is very overkill for what this project needs.

I am yet to consider what controller I will use to controll the final clock.

