faust2au
========

INSTALLATION:

- replace faust installation Makefiles in the root and tools/faust2appls/ folders and run 'sudo make install' in the root installation folder
- Xcode should also be installed on your system (https://developer.apple.com/xcode/)


========

USAGE: 

- with the default arguments:
faust2au filename (or faust2ausynth filename)
(e.g. faust2au echo.dsp)

- with some extra arguments:
faust2au filename manufacturer STYP MANF

  'manufacturer' is the manufacturer of the AU (The default value is 'Grame')

  'STYP' is a four-character subtype (The default is the first four characters of the 'filename')

  'MANF' is a four-character manufacturer (The default is the first four characters of the 'manufacturer')


========

OUTPUT:
  - ~/Library/Audio/Plug-Ins/Components/'filename'.component
   
  AU Name will be: 'manufacturer: filename'  ('filename' is used as the AU name). 

  The names are case-sensitive, e.g. 'faust2au Echo.dsp' results in 'Echo' as the AU name
