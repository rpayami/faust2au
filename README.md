faust2au
========

INSTALLATION:

- copy the contents of the 'architecture' and 'tools' folders into the similar faust folders
- if needed, change FAUST_DIR variable in 'faust2au' (means faust2aueffect) and 'faust2ausynth' files in the 'tools/faust2appls/' folder
- in the terminal window, run  'install faust2au /usr/local/bin' and 'install faust2ausynth /usr/local/bin' (the files you just copied to 'faust2appls' in the previous step)
- Xcode should also be installed on your system (https://developer.apple.com/xcode/)


========

USAGE: 

- with the default arguments:
faust2au filename (or faust2ausynth filename)
(e.g. faust2au echo.dsp)

- with some extra arguments:
faust2au filename manufacturer STYP MANF

  'manufacturer' is the manufacturer of the AU (The default value is 'Grame')

  'STYP' is a four-charachter subtype (The default is the first four characters of the 'filename')

  'MANF' is a four-charachter manufacturer (The default is the first four characters of the 'manufacturer')


========

OUTPUT:
  - ~/Library/Audio/Plug-Ins/Components/'filename'.component
   
  AU Name will be: 'manufacturer: filename'  ('filename' is used as the AU name). 

  The names are case-sensitive, e.g. 'faust2au Echo.dsp' results in 'Echo' as the AU name
