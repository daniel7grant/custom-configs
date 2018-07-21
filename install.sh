#!/bin/bash
# COLORS YAAY
sudo sed -i 's/#Color/Color/g' /etc/pacman.conf

# UPDATE ALL PACKAGES
sudo pacman -Syu

# INSTALL BASIC PACKAGES
sudo pacman -S git vim bash-completion htop	# BASIC 
sudo pacman -S base-devel yay				# AUR
sudo pacman -S remmina freerdp				# RDP
sudo pacman -S nodejs npm					# JS
sudo pacman -S php composer					# PHP
sudo pacman -S budgie-extras					# BUDGIE

# REMOVE UNNECESSARY BLOAT
sudo pacman -R brasero empathy gedit hexchat lollypop manjaro-hello manjaro-documentation-en transmission-gtk uget totem thunderbird tlp

# INSTALL PACKAGES FROM AUR
sudo yay -S google-chrome
sudo yay -S visual-studio-code-bin
sudo yay -S discord
sudo yay -S spotify
sudo yay -S slack-desktop
sudo yay -S jetbrains-toolbox
sudo yay -S budgie-screenshot-applet
sudo yay -S peek
