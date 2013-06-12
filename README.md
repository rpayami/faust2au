faust2au
========

INSTALLATION:

- download and extract faust installation archive (http://sourceforge.net/projects/faudiostream/files/faust-0.9.58.zip/download)
- merge the files in this repository with the extracted files (replace Makefiles) 
- install faust
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
