#!/bin/bash

# COPY TO EXECUTABLE LOCATION
cp lib/randombg ~/bin/randombg 

# ADD TO CRONTAB
(crontab -l 2>/dev/null; echo "*/5 * * * * $HOME/bin/randombg") | crontab -

# RUN FOR RANDOM IMAGE
$HOME/bin/randombg
