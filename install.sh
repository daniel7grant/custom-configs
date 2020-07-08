#!/bin/bash
# COLORS YAAY
sudo sed -i 's/#Color/Color/g;s/#ILoveCandy/ILoveCandy/g' /etc/pacman.conf

# UPDATE ALL PACKAGES
sudo pacman -Syu

# INSTALL BASIC PACKAGES
sudo pacman -S git vim bash-completion htop ncdu bat                # BASIC 
sudo pacman -S base-devel yay                                       # AUR
sudo pacman -S kupfer terminator playerctl nemo nemo-fileroller     # OS USAGE
sudo pacman -S budgie-extras gnome-tweaks adapta-maia-theme         # BUDGIE
sudo pacman -S remmina freerdp                                      # RDP
sudo pacman -S nodejs npm                                           # JS
sudo pacman -S php composer                                         # PHP
sudo pacman -S discord                                              # DISCORD
sudo pacman -S nextcloud-client                                     # NEXTCLOUD

# REMOVE UNNECESSARY BLOAT
sudo pacman -R brasero gedit hexchat hplip lollypop manjaro-hello manjaro-documentation-en manjaro-printer pidgin transmission-gtk uget totem thunderbird tlp yelp

# INSTALL PACKAGES FROM AUR
yay -S google-chrome \
       visual-studio-code-bin \
       spotify \
       slack-desktop \
       jetbrains-toolbox \
       flameshot \
       peek
