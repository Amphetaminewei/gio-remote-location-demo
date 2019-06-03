all:
	gcc -o test main.c `pkg-config --cflags --libs glib-2.0 gio-2.0 gtk+-3.0`
