# Custom-configs - bash

Home-baked configuration files for a bash terminal - by Daniel Grant.

## Installation

If you want to install all configurations, run
```
make
```

If you want to install only the bash or the vim configurations, run
```bash
make bash-install
# OR
make vim-install
```
respectively.

### Private files

Private files are bundled into a gzipped tarball and symmetrycally encrypted with gpg. Run
```
make decrypt
```
and enter the password to decrypt them, and run the installation as usual.
