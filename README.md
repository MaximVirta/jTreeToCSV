Create root shared library for JBaseTrack.cxx.
In root command line: ".L src/JBaseTrack.cxx+"

To plot \eta and \phi distributions
root -q -l 'draw_dNdeta.C("TShydroJet_jTree.root")'

To create csv events
root -q -l 'jTreeToCSV.C("TShydroJet_jTree.root", "./events")'
