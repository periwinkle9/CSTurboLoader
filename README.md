# Cave Story Turbo Mod
This is a DLL file that adds turbo functionality to freeware Cave Story.
It works similarly to existing "mod loader" projects, by spoofing a system DLL
(in this case, version.dll) to inject extra functionality into the game's code
at runtime.

## Installation instructions
1. Download a release (link TBD)
2. Rename the .dll file to "version.dll" (without quotes) and place it in the same
   folder as Doukutsu.exe.  
   **DO NOT OVERWRITE THE VERSION.DLL IN YOUR SYSTEM FOLDER!!!**
3. That's it!

## Controls
This mod turns the Shift key into a turbo button. There are three main modes of operation:
- Text mashing mode: Hold Shift during a cutscene and the game will automatically advance
  through all of the text. Additionally:
  * Yes/No prompts can be answered automatically by holding left/right (respectively)
    during the prompt
  * You can hold down to automatically interact with whatever you are standing on when the
    cutscene ends.
	(This is useful for entering a door immediately after an event, etc.)
- Standard turbo mode: Hold Shift to automatically fire your current weapon at the maximum
  rate allowed by the game (once per 4 frames, or 12.5 Hz).
- Smart turbo mode: Hold Shift+X to activate. In this mode, the following will occur in
  this order of precedence:
  * Curly's Nemesis will attempt to fire every other frame (25 Hz)
  * Spur level 2 and 3 will automatically perform the "infinite Spur" technique by
    pause-buffering to fire lasers without expending its charge
  * Autofire weapons (Machine Gun, Bubbler level 2/3) will fire at their maximum possible
    (non-pause-buffered) rate (2 shots every 5 frames, or 20 Hz average)
  * Non-autofire weapons that are one swap away from an autofire weapon will automatically
    twiddle between those two weapons to take advantage of autofire shots
	(this allows for shooting 2 shots every 4 frames, effectively 25 Hz)
  * All other weapons will fire every 4 frames (12.5 Hz).

## Build instructions
Project files for Visual Studio 2019 are included in the repo. Because of some MSVC-specific
macro definitions, the code is unlikely to compile in GCC.
