# Monk Cipher Clock
## What is it?
This project sets out to create a digital clock using an unconventional numbering system, the "Monk Cipher".

The monk cipher is a little known number-notation format, from the Middle Ages. 

This system can represent any 4 digit number, up to 9999, in one combined cipher.

![Ciphers_clip_image002](https://github.com/user-attachments/assets/f338e3f2-74d1-4d38-98e8-4bbcf10ae430)

https://web.archive.org/web/20160304051616/http://www.davidaking.org/Ciphers.htm

### A Long Running Idea...
Many years ago, I came across that image of the cipher on the internet, originally hosted on an author's personal website, and fascinated by it.

The modular nature of the cipher intrigued me, as it reminded me about seven-segment and alphanumeric displays; and something about having a modern, digital display using such an ancient notation scheme, really tickled something in my brain.

I had ideas bouncing around in my head for years. Ideas like manufacturing my own segmented display with LEDs or just having a small OLED screen and drawing out the ciphers, but nothing manifested (exept for a few doodles), and so it stayed a cool concept.

### Inspiration Strikes!
A recent video I saw on YouTube is what really got me back into the headspace for this project.

This video popped up on my recommended feed, and I instantly knew this display would be perfect:

https://youtu.be/dBEupkQBFis?si=4U0O5Yceylcod2Og

## Physical Clock
### Display
The component that is driving the motivation for this whole project is the HCMS-29XX series displays.

<img width="440" height="440" alt="image" src="https://github.com/user-attachments/assets/391bc03e-e904-436c-a2d7-6f0baec13949" />

These have 4 distinct character displays, featuring a 5x7 LED matrix.
This is *just* enough pixels to render a pretty faithful and legible rendition of the ciphers, without taking too many creative liberties.

These are a bit pricey (That guy in the video is wearing nearly NZD$1000 on his wrist, in displays alone!), but I think just a single 4 character display is all I need. 

The warm yellow glow is just tantalising enough, to fork over the cash for.

### Controller
I have a STM NUCLEO-F446RE board lying around that I have wanted to learn how to program for a few years.
It will be used for initial breadboarding, and is very overkill for what this project needs.

I am yet to consider what controller I will use to control the final clock.

## The Project so far
### Simulation
First step of this project is to have a simulation of the display running in software. This way I can test ideas, and figure out the easiest ways of doing the actual cipher generations, before thinking about hardware.

I'm using C, and printing coloured characters to the command line, as seems to be becoming my style...

A lot of things I have learned in my CLI renderer project have come in handy, and allowed me to very quickly get a working software simulation.

GIF of simulation in action! 🕚

![monk_cipher_demo](https://github.com/user-attachments/assets/be240ae9-60c9-4d60-ad5b-26dfa432a1c0)
