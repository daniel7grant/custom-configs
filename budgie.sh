#!/bin/bash
# SET THEME
dconf write /com/solus-project/budgie-panel/dark-theme true
dconf write /com/solus-project/budgie-panel/builtin-theme false

# SET PANEL
PANELID=$( dconf list /com/solus-project/budgie-panel/panels/ )
dconf write /com/solus-project/budgie-panel/panels/${PANELID}location "'bottom'"
dconf write /com/solus-project/budgie-panel/panels/${PANELID}size 45
dconf write /com/solus-project/budgie-panel/panels/${PANELID}transparency "'always'"

# SET PINNED APPLICATIONS
INSTANCEID=$( dconf list /com/solus-project/budgie-panel/instance/icon-tasklist/ )
dconf write /com/solus-project/budgie-panel/instance/icon-tasklist/${INSTANCEID}pinned-launchers "['org.gnome.Terminal.desktop', 'org.gnome.Nautilus.desktop', 'google-chrome.desktop']"

# COPY .CONFIG FILES FROM /ETC
cp -ar etc/.config/* ~/.config

# SETUP TERMINATOR
dconf write /org/gnome/desktop/applications/terminal/exec "'terminator'"

# SET TERMINAL PREFERENCES
TERMINALPREFID=$( dconf list /org/gnome/terminal/legacy/profiles:/ )
dconf write /org/gnome/terminal/legacy/new-terminal-mode "'tab'"
dconf write /org/gnome/terminal/legacy/theme-variant "'dark'"
dconf write /org/gnome/terminal/legacy/profiles:/${TERMINALPREFID}use-theme-colors true
dconf write /org/gnome/settings-daemon/peripherals/keyboard/bell-custom-file "''"    #TODO XP

# RELOAD PANEL
nohup budgie-panel --replace &

