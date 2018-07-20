# SET USUALS
dconf write /org/gnome/desktop/wm/keybindings/show-desktop "['<Super>d']"
dconf write /org/gnome/desktop/wm/keybindings/toggle-maximized "['<Super>Up']"
dconf write /org/gnome/desktop/wm/keybindings/maximize "[]"
#dconf write /org/gnome/desktop/wm/keybindings/panel-run-dialog "['<Super>p']"

# ADD CUSTOM
# TERMINAL ON SUPER+T
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal0/binding "'<Super>t'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal0/command "'gnome-terminal'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal0/name "'Open terminal'"

# TERMINAL ON CTRL+ALT+T
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal1/binding "'<Primary><Alt>t'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal1/command "'gnome-terminal'"
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal1/name "'Open terminal'"

# APPLY CUSTOM BINDS
dconf write /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings \
	"['/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal0/', '/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/terminal1/']"
