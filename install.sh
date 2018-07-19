#!/bin/bash

## UPDATE ALL PACKAGES
pacman -Syu

## INSTALL BASIC PACKAGES
pacman -S git vim bash-completion htop	# BASIC 
pacman -S base-devel cower				# AUR
pacman -S remmina freerdp				# RDP
pacman -S nodejs npm					# JS
pacman -S php composer					# PHP

## REMOVE UNNECESSARY BLOAT
pacman -R brasero empathy gedit hexchat lollypop manjaro-hello manjaro-documentation-en transmission-gtk uget totem thunderbird tlp

## INSTALL PACKAGES FROM AUR
function cowerinstall(){ cd ; cower -d $1 ; cd $1 ; makepkg -si ; }
cowerinstall google-chrome
cowerinstall visual-studio-code-bin
cowerinstall spotify
cowerinstall slack
cowerinstall jetbrains-toolbox
