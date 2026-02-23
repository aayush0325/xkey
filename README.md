# xkey

`xkey` is a Linux utility that maps keyboard events to a virtual Xbox 360 controller using the `uinput` kernel module. It allows you to use your keyboard as a game controller for applications that support X-Input or generic gamepads.

## Features

- **Virtual Controller**: Creates a virtual "Microsoft X-Box 360 pad".
- **Keyboard Grabbing**: Optionally grabs the keyboard device to prevent keys from being sent to other applications while in controller mode.
- **Toggle Mode**: Use the `Esc` key to toggle between normal keyboard usage and controller emulation.
- **Comprehensive Mappings**: Maps common keys to buttons, triggers, and D-pad.

## Default Mappings

| Keyboard Key | Controller Action |
|--------------|-------------------|
| `I`          | North Button (Y) |
| `K`          | South Button (A) |
| `J`          | West Button (X)  |
| `L`          | East Button (B)  |
| `Left Ctrl`  | Left Bumper (LB) |
| `Right Ctrl` | Right Bumper (RB)|
| `Up Arrow`   | D-pad Up        |
| `Down Arrow` | D-pad Down      |
| `Left Arrow` | D-pad Left      |
| `Right Arrow`| D-pad Right     |
| `Left Shift` | Left Trigger    |
| `Right Shift`| Right Trigger   |
| `Enter`      | Start Button    |
| `Backspace`  | Select Button   |
| `Esc`        | Toggle Mode     |

## Prerequisites

- Linux OS
- `uinput` kernel module enabled (`sudo modprobe uinput`)
- Permissions to access `/dev/uinput` and the keyboard device node (usually requires `sudo` or being in the `input` group).

## Installation

1. Clone the repository.
2. Build the project using `make`:
   ```bash
   make
   ```

## Usage

1. Identify your keyboard device path. The default in `main.c` is:
   `/dev/input/by-path/pci-0000:06:00.3-usb-0:1:1.0-event-kbd`
   *Note: You may need to edit `main.c` to match your hardware path.*

2. Run the executable with root privileges:
   ```bash
   sudo ./xkey
   ```

3. Press `Esc` to enable the controller mapping. Press `Esc` again to disable it.

## Configuration

To change the keyboard device path or modify mappings, edit `main.c` and `virtual.c` respectively, then recompile.
