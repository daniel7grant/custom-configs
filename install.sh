#!/bin/bash
# COLORS YAAY
sudo sed -i 's/#Color/Color/g;s/#ILoveCandy/ILoveCandy/g' /etc/pacman.conf

# UPDATE ALL PACKAGES
sudo pacman -Syu

# INSTALL BASIC PACKAGES
sudo pacman --needed -S git vim bash-completion htop ncdu bat                              # BASIC 
sudo pacman --needed -S base-devel yay                                                     # AUR
sudo pacman --needed -S kupfer terminator playerctl nemo nemo-fileroller ttf-inconsolata   # OS USAGE
sudo pacman --needed -S budgie-extras gnome-tweaks adapta-maia-theme                       # BUDGIE
sudo pacman --needed -S remmina freerdp                                                    # RDP
sudo pacman --needed -S nodejs npm                                                         # JS
sudo pacman --needed -S php composer                                                       # PHP
sudo pacman --needed -S discord                                                            # DISCORD
sudo pacman --needed -S code                                                               # VS CODE
sudo pacman --needed -S nextcloud-client                                                   # NEXTCLOUD

# REMOVE UNNECESSARY BLOAT
sudo pacman -R brasero gedit hexchat hplip lollypop manjaro-hello manjaro-documentation-en manjaro-printer pidgin transmission-gtk uget totem thunderbird tlp yelp

# INSTALL PACKAGES FROM AUR
yay --needed -S \
	# google-chrome \
    spotify \
    slack-desktop \
    # jetbrains-toolbox \
    flameshot \
    peek
