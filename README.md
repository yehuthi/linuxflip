# Linux Flip

Detects and executes commands when the computer switches to and from tablet mode.

## Usage

Download the [latest release](https://github.com/yehuthi/linuxflip/releases/latest), and add your user to the `input` group (unless you want to run it as root).
Then you can simply run it or hook it to your system (e.g. in `~/.xinitrc`).

Run `linuxflip --help` for usage options. The one you're probably interested in is:
```
linuxflip <tablet-cmd> <laptop-cmd>
    Runs <tablet-cmd> in a shell when the mode changes to tablet, and <laptop-cmd> when the mode changes to laptop.
```

The commands run in a shell, so shell features are supported. If you'll run as root even through `sudo`, remember to not assume `~` will be substituted for your user's home directory; it's probably going to be `/root`. The commands run asynchronously.

> [!CAUTION]
> Your commands will run with elevated privileges inherited from the program.

### Example

#### Scripts
A typical example is running scripts from files. You can place them wherever you want and to run them, the command is simply their path (and as mentioned above, be careful with `~`). Set up those scripts however you want and make sure they're executable (`chmod +x`), then run:

```bash
linuxflip "/home/myuser/linuxflip_tablet" "/home/myuser/linuxflip_laptop"
```

#### Commands
You can pass commands, which can be very handy for trivial setups. For example, if you _just_ want to display an [on-screen keyboard](https://wiki.archlinux.org/title/List_of_applications/Utilities#On-screen_keyboards):
```bash
linuxflip corekeyboard "killall corekeyboard"
```
