faust2au
========

INSTALLATION:

- copy the contents of the 'architecture' and 'tools' folders into the similar faust folders
- if needed, change FAUST_DIR variable in the 'tools/faust2appls/faust2au' file
- in the terminal window, run  'install FAUST_DIR/faust2au /usr/local/bin' (FAUST_DIR is your faust folder)


========

USAGE: 

- simple case, with default arguments:
faust2au filename
(e.g. faust2au echo.dsp)

- in general, with some extra arguments:
faust2au filename manufacturer STYP MANF

  'manufacturer' is the manufacturer of the AU (The default value is 'Grame')

  'STYP' is a four-charachter subtype (The default is the first four characters of the 'filename')

  'MANF' is a four-charachter manufacturer (The default is the first four characters of the 'manufacturer')


========

OUTPUT:
  - ~/Library/Audio/Plug-Ins/Components/'filename'.components
   
  AU Name will be: 'manufacturer: filename'  ('filename' is used as the AU name). 

  The names are case-sensitive, e.g. 'faust2au Echo.dsp' results in 'Echo' as the AU name
