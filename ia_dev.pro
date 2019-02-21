#
# Projet d'intelligence artificielle Bio Inspirée
#
# But : implémenter un algorithme génétique permettant de faire naviguer un
# robot aspirateur ne connaissant rien de son environnement.
#
# Structure du projet :
# algorithm : Partie permettant de simuler un agent dans un environnement.
# Genetique : Algorithme génétique permettant de trouver le meilleur agent.
# test : interface graphique permettant de tester les fonctionnalités de la
#   librairie algorithm.

TEMPLATE = subdirs
CONFIG += c++11

CONFIG += ordered

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

QMAKE_LFLAGS_RELEASE -= -O1

SUBDIRS = \
    agent \
    ktbs \
    simulation \
    graphicsview \
    radical_interactionnism \
    view

