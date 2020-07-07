#!/bin/bash
# SET USUALS
dconf write /org/gnome/desktop/wm/keybindings/show-desktop "['<Super>d']"
dconf write /org/gnome/desktop/wm/keybindings/toggle-maximized "['<Super>Up']"
dconf write /org/gnome/desktop/wm/keybindings/maximize "@as []"

# ADD CUSTOM
# TERMINAL ON SUPER+T
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal0/binding "'<Super>t'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal0/command "'terminator'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal0/name "'Open terminal'"

# TERMINAL ON CTRL+ALT+T
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal1/binding "'<Primary><Alt>t'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal1/command "'terminator'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal1/name "'Open terminal'"

# KUPFER ON SUPER+K
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/kupfer/binding "'<Super>s'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/kupfer/command "'kupfer'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/kupfer/name "'Show Kupfer'"

# AREA SCREENSHOT ON ALT+X
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenshot/binding "'<Alt>x'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenshot/command "'gnome-screenshot -ac'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenshot/name "'Area Screenshot'"

# PLAY/STOP, PREVIOUS AND NEXT WITH SUPER+F{10..12}
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-play/binding "'<Super>F10'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-play/command "'playerctl play-pause'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-play/name "'Playerctl play/pause'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-prev/binding "'<Super>F11'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-prev/command "'playerctl previous'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-prev/name "'Playerctl previous'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-next/binding "'<Super>F12'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-next/command "'playerctl next'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-next/name "'Playerctl next'"

# APPLY CUSTOM BINDS
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings \
	"['/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal0/', '/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal1/', '/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/kupfer/', '/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenshot/', '/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-play/', '/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-prev/', '/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/playerctl-next/']"
