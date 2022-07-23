# Implementation of Lightweight Cryptographic Algorithms

## Introduction

The resource-constrained Devices that are part of the Internet of Things (IoT) have limited
processing power, limited battery life, and limited primary and secondary memory capacity.
Examples of these devices are sensors, actuators, RFID tags, and smart cards . Although these devices typically perform a single straightforward 
duty, it is crucial to ensure a safe connection between them. For instance, a power plant's 
actuator and controller connection line might sustain severe damage from an attack.

There are numerous cryptographic algorithms that have been shown to be safe. However, a 
high percentage of them need a lot of processing capability, power, and memory, which is too 
much for devices with limited resources. Although it is possible to run some algorithms on 
them, they are not suitable for situations where it is critical to have near real-time 
communication (E.g.: sensors). 

The National Institute of Standards and Technology (NIST) created the Lightweight 
Cryptography Competition (LWC) to address this 
issue. 56 algorithms were submitted to the contest and met the LWC standards. After two rounds, NIST has announced ten algorithms  as the finalists in 2021, namely ASCON, Elephant, GIFT-COFB, 
Grain-128AEAD, ISAP, PHOTON-Beetle, Romulus, SPARKLE, TinyJambu and Xoodyak. 

These algorithms will be implemented in this study, and many elements of their performance 
will be examined.

## Goals of the project
* Implement LWC finalist algorithms.
* Execute them on resource-constrained hardware. (low end microcontroller and/or 
FPGA). 
* Record, compare, and analyse the performance-related measurements including 
running time, memory usage, and power usage.
* Seek potential improvements for the algorithms.


## Links
* Page: [https://cepdnaclk.github.io/e16-4yp-implementation-of-lightweight-cryptographic-algorithms/](https://cepdnaclk.github.io/e16-4yp-implementation-of-lightweight-cryptographic-algorithms/)
* NIST LWC competion page: [https://csrc.nist.gov/Projects/lightweight-cryptography](https://csrc.nist.gov/Projects/lightweight-cryptography)
