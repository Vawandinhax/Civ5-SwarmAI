// SI.h
// Used to keep track of modified functions throughout the code

#pragma once

#ifndef SI_MODS_H
#define SI_MODS_H

// SI mod new GUID
#define SI_GUID
#define SI_NO_PEACE

#define SI_VIEW_RANGE 3
#define SI_HELP_FLAG 10
#define SI_MAX_PLAYERS 2

// Debugging
//#define SI_DEBUG
#define SI_LOGS
#define SI_COMPLETE_LOGS


// ALGORTIHM PARTS
// Selfish phase
#define SI_SELFISH_PHASE
// Negotiation phase
#define SI_NEGOTIATION_PHASE
// Heuristic evaluation for every intent
#define SI_HEURISTIC_EVALUATION

// INFLUENCE MAPPING
#define SI_INFLUENCE_UNIT			100
#define SI_INFLUENCE_CITY			150
#define SI_INFLUENCE_ENEMY_UNIT		-100
#define SI_INFLUENCE_ENEMY_CITY		-150
#define SI_INFLUENCE_DECAY			0.25
#define SI_INFLUENCE_RANGE			4

// HEURISTIC EVALUATION
#define SI_AGGRESSION_MODIFIER		1

// SI mod new Tactical AI constructor
// #define SI_NEW_TACTICAL_AI_INIT

// Properly destroy 2D arrays, credit to Delnar_Ersike
#define AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION

#endif