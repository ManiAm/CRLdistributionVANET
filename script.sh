# start a screen session
screen -S session1 -f ./scriptKTHServer.sh

# At the end press Ctrl-A then Ctrl-D.
# This will "detach" your screen session but leave your processes running. 
# You can now log out of the remote box.

# If you want to come back later, log on again and type the following:

# screen -r session1

# This will "resume" your screen session, and you can see the output of your process.

# how to scroll!
# http://superuser.com/questions/138748/how-to-scroll-up-and-look-at-data-in-gnu-screen

# list all screens
# screen -list

# to delete a session
# Once connected to the session which might or might not respond, do the following. 
# press "Ctrl + a" (there wont be any changes in your window now) 
# type ":quit" ( its a colon[:] followed quit)

