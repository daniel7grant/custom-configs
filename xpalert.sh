#!/bin/bash

# COPY WINDOWS XP DING SOUND TO DEFAULT DIRECTORY
sudo cp xp.ogg /usr/share/sounds/gnome/default/alerts/

# ADD NEW SECTION TO CONFIG FILE IF NOT EXISTS
SOUNDFILE=/usr/share/gnome-control-center/sounds/gnome-sounds-default.xml
if [[ -z $(grep 'xp.ogg' $SOUNDFILE ) ]]  
then
	sudo sed -i 's|</sounds>|  <sound deleted="false">\n    <name>XP</name>\n    <filename>/usr/share/sounds/gnome/default/alerts/xp.ogg</filename>\n  </sound>\n</sounds>|'  \
		$SOUNDFILE
fi

# SET ALERT SOUND FROM MENU
gnome-control-center sound effects 
