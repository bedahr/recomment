Recomment
=========

Voice controlled recommender system (runs on Linux (ALSA) and QNX (BlackBerry PlayBook)).

Depends on BBSimoneCommon from the simon-tools repository on the KDE infrastructure:
http://quickgit.kde.org/?p=simon-tools.git&a=tree&f=Simone%2FBlackBerry%2FBBSimoneShared
Checkout: git://anongit.kde.org/simon-tools.git/
Compile BBSimoneShared and adjust the BBSimoneCommon path in recomment.pro before compiling.

Internally, Recomment uses the Simon speech recognition system and connects to a Simond server.
Please refer to http://simon.kde.org for more information, support  and installation instructions.

The database of available products is to be found in db/ and currently contains a set of 
currently available compact cameras (April, 2013).

At the moment, Recomments NLP processing (and interface) is configured for (Austrian) German.
When adjusting to another language don't forget to update the NLP rules in db/nlp.xml
