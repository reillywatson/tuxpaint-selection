About Tux Paint's src/po/ folder

Bill Kendrick
bill@newbreedsoftware.com

2007-May-09

Run "update-po.sh" whenever new gettext() strings are added, or existing
ones (the original English text) are changed, so the changes are made
visible in the POT and PO files.

Run "update-po.sh" prior to a release, so any translations of the ".desktop"
file's strings get put into the ".desktop" file.

Thanks to Karl Ove Hufthammer for explaining these steps.

