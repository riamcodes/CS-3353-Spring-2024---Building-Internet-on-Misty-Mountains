# CS-3353-Spring-2024---Building-Internet-on-Misty-Mountains
An MST algorithm to find the least-cost solution to connect houses in the Misty Mountains to the internet using a satellite and cable system.
This repository contains the implementation of an algorithm designed to connect houses in the Misty Mountains to the internet using the least-cost approach. The problem involves determining the minimum cost to connect multiple houses either by installing a satellite dish or by laying cables between houses.

Features
Minimum Spanning Tree (MST): The algorithm constructs an MST to determine the most efficient way to connect the houses while minimizing costs.
Two Connection Options:
Install a satellite dish to connect a house directly to the internet (one house only).
Lay a cable between houses to share the internet connection.
Path and Cable Cost Calculation: The program calculates and outputs the necessary cables and the path connecting the satellite house to House 1.
Algorithm Highlights
Graph Representation: The problem is modeled as a graph where houses are vertices, and the edges represent the possible cable connections with their respective costs.
Dynamic Decision Making: The program uses graph algorithms to determine which house should install the satellite and which cables should be laid for optimal connectivity.
Getting Started
The implementation is in MyGraph.cpp and MyGraph.h.
The test driver is available in Prog2test.cpp.
Sample test data is provided to illustrate how the algorithm works on a fictional setup of the Misty Mountains.
