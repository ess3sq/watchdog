# watchdog
_linux cli utility tot watch files or directories for changes and execute tasks when they do_

## Example Usage

I use this while writing latex documents:
```
$ watchdog thesis.tex -c "pdflatex thesis.tex" &
$ zathura thesis.tex
```

Zathura is a pdf reader that will auto-update the pdf it is rendering, and is really fast.
Watchdog, on the other hand, will work with zathura by re-compiling the latex document as `thesis.pdf` every time `thesis.tex` is saved.
This makes it possible to have an automatic pdf preview.

Of course, the applications are most certainly not limited to this, feel free to get creative!

## Compiling and Installing

Just ran `make` to compile:
```
$ make
```

or clean install:
```
$ sudo make clean install
```

## Licensing
Any files in this repository are released in the public domain. No warranty of any form is given, express or implied.
