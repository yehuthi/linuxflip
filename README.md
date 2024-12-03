# Linux Flip

Detects and executes commands when the computer switches to and from tablet mode.

## Setup

1. Download the [latest release](https://github.com/yehuthi/linuxflip/releases/latest), or if you're on Arch: `yay -S linuxflip-git`
2. Add your user to the `input` group (unless you want to run it as root -- not recommended).

> [!TIP]
> <details>
> <summary>⌨ Add yourself to <code>input</code> group</summary>
>
> Open the terminal (press <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>T</kbd> or find it as you would any other program), and enter:
>    
> ```sh
> sudo usermod -aG input $USER
> ```
> </details>

Then you can simply run it or [hook](#appendix-run-automatically) it to a desktop session startup script.


## Usage

Run `linuxflip --help` for usage options. The one you're probably interested in is:
```
linuxflip <tablet-cmd> <laptop-cmd>
    Runs <tablet-cmd> in a shell when the mode changes to tablet, and <laptop-cmd> when the mode changes to laptop.
```

- [x] The commands run in a shell, so shell features are supported (such as [tilde expansion](https://www.gnu.org/software/bash/manual/html_node/Tilde-Expansion.html)).
- [x] The commands run asynchronously.

### Examples

#### Scripts
A typical example is running script files. You can place them wherever you want and to run them, the command is simply their path. Set up those scripts however you want and make sure they're executable (`chmod +x`), then run:

```bash
linuxflip "~/.config/linuxflip/tablet" "~/.config/linuxflip/laptop"
```

#### Commands
You can pass commands, which can be very handy for trivial setups. For example, if you _just_ want to display an [on-screen keyboard](https://wiki.archlinux.org/title/List_of_applications/Utilities#On-screen_keyboards):
```bash
linuxflip corekeyboard "killall corekeyboard"
```

## Appendix: Disable/Enable Devices

To disable or enable a device, such as a keyboard, the trackpad, etc. use `xinput`:
```sh
xinput disable "device name"
xinput enable "device name"
```

To find out the device name,  you can run `xinput list` and see if you can definitely spot the device there. If it's not obvious:
1. Run `libinput debug-events`. You should see some lines for the connected devices.
2. Send input from the device (press a button on a keyboard, move the cursor on a mouse, etc.). The terminal should react with new lines.
3. Note the event number on the first column for the new lines.
4. Scroll up to the first lines about the connected devices and find the event number there. The third column is the device name.

Now we can disable/enable a device called "device name" on switch by putting the disable/enable commands in the scripts, or directly like this:
```
linuxflip 'xinput disable "device name"' 'xinput enable "device name"'
```

## Appendix: Run Automatically

1. Create a file `~/.xsession` if doesn't exist
2. Append your `linuxflip` command to it, and remember not to block, e.g.
   ```sh
   linuxflip "~/.config/linuxflip/tablet" "~/.config/linuxflip/laptop" &
   ```
   (note the `&` at the end)
4. Make sure it's executable (`chmod +x ~/.xsession`)
