

floppy_music: floppy_music.c rsync
	ssh himbeere "cd floppy; clang -o floppy_music floppy_music.c -lwiringPi"

rsync:
	rsync -a * dev@himbeere:floppy/
