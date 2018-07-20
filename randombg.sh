#!/bin/bash

# COPY TO EXECUTABLE LOCATION
cp lib/randombg ~/bin/randombg 

# ADD TO CRONTAB IF NOT EXISTS
if [[ -z $(crontab -l | grep $HOME/bin/randombg) ]] 
then
	( crontab -l 2>/dev/null; echo "*/5 * * * * $HOME/bin/randombg" ) | crontab -
fi

# RUN FOR RANDOM IMAGE
$HOME/bin/randombg
