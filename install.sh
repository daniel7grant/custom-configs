#!/bin/bash
# COLORS YAAY
sed -i 's/#Color/Color/g' /etc/pacman.conf

# UPDATE ALL PACKAGES
pacman -Syu

# INSTALL BASIC PACKAGES
pacman -S git vim bash-completion htop	# BASIC 
pacman -S base-devel yay				# AUR
pacman -S remmina freerdp				# RDP
pacman -S nodejs npm					# JS
pacman -S php composer					# PHP
pacman -S budgie-extras					# BUDGIE

# REMOVE UNNECESSARY BLOAT
pacman -R brasero empathy gedit hexchat lollypop manjaro-hello manjaro-documentation-en transmission-gtk uget totem thunderbird tlp

# INSTALL PACKAGES FROM AUR
yay -S google-chrome
yay -S visual-studio-code-bin
yay -S discord
yay -S spotify
yay -S slack-desktop
yay -S jetbrains-toolbox
yay -S budgie-screenshot-applet
yay -S peek
