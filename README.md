# Linux Flip

Detects and executes commands when the computer switches to and from tablet mode.

## Setup

Download the [latest release](https://github.com/yehuthi/linuxflip/releases/latest), and add your user to the `input` group (unless you want to run it as root -- not recommended).
Then you can simply run it or hook it to a desktop session startup script.

> [!TIP]
> <details>
> <summary>⌨ Add yourself to <code>input</code> group</summary>
>
> Open the terminal (press <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>T</kbd> or find it as you would any other program), and enter:
>    
> ```sh
> sudo usermod -a input $USER
> ```
> </details>

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
