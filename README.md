# gio-remote-location-demo
this demo intends to show how to connect to a remote server and get files info from server.

# description
this demo provide a login dialog to connect a remote location, and then show the files' names at a new list box window.
<br></br>
for using it connect a server, for example, ususally a sftp server, we need complete the dialog like this:
<br></br>
- url: like "sftp://xxx.xxx.xxx.xxx"
- user: like "root" or another user in remote server
- pwd: the user's password this is not visible at pwd entry

then click login button, the files list window will show just waiting a moment.

# build-dep
- libgtk+-3.0-dev
- libglib2.0-dev

# build and run
- git clone https://github.com/Yue-Lan/gio-remote-location-demo.git
- cd  gio-remote-location-demo
- make
- ./test
