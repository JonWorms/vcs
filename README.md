#Voice Control System

This project will a voice control system that enables uses to privately control various aspects of their computers and network enabled home appliances.

## Why?

Technologies such as Siri and Alexa are great, but after a hotword has been detected, audio is streamed from the users home to data centers outside of their local network.
The provides of these services are free to do what they will with that audio data. 

This software will attempt to handle many of the same tasks that these services provide without sending the users audio data to a third party. 

## Goals

Set up a 'node' application that can be run on single board computers, these sbc's can be distributed around the house to provide an interface in each room.
Create a 'server' application that handles the task of parsing audio into text, and running software associated with parsed commands.

### The node

This software should consume audio data from a local interface such as a usb microphone and parse that data for a small set of hotwords. After a hotword is detected, the node should stream audio data from
the local interface to the server application for command processing and execution of those commands. After a command has been parsed, it should provide some feedback to the user and return to hotword listening

### The server

The server should listen for connections from nodes, and then parse the incoming audio streams for a large set of command words.
It should also save recordings and parsed text to a database so that the user can review the data and make corrections, after a correction is made, the server should update the users voice model.


### How?

voice processing and learning can be handled with CMUSphinx.
Audio input can be reasonably handled with port audio. 


## Phases

Step 1:
	Implement hotword listening on the node software, for now it needs to consume audio, and after a hotword is detected, play the audio that was detected back to the user.
		what I have so far:
			audio can be captured to a ring buffer
Step 2:
	Implement the basics for the server, it should listen for connections from nodes and then process 15 or so seconds of data from the node, displaying the parsed text in a log of some sort
	Modify the node to stream audio data to the server and the necessary protocols to return to hotword listening
	
Step 3:
	Not sure yet


Things that should (probably) be done:
	sample rate conversion, while testing I found that my usb speakers do not seem to support sample rates below 32000, when audio is played below that sample rate it sounds like the chipmunks
	to resolve this, perhaps selection of plugins configured in alsa to do the sample rate conversion would work
