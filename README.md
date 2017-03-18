# myonsh

A tiny linux shell demo (as a school assignment

Only experimental, like a pile of sh!t

No architecture there. A refactoring must be done if someone want to  continue this...

~~Why not use bash~~

## Install

	$ git clone https://github.com/MyonKeminta/myonsh.git
	$ cd myonsh
	$ cmake .
	$ make
	$ sudo make install

There's a myonshrc example in 'example' directory. If you want to use this, just:

	$ cp example/.myonshrc ~/

## Uninstall

Go to the project directory and:
	
	$ sudo xargs rm < install_manifest.txt

