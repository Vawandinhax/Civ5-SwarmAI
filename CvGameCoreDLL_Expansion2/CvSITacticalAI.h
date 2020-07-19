/*	-------------------------------------------------------------------------------------------------------
	© 2017-2018 Joao Varandas
	------------------------------------------------------------------------------------------------------- */

#pragma once

// SITacticalAI.h

#ifndef CIV5_SI_AI_H
#define CIV5_SI_AI_H

#include "CvTacticalAI.h"

// Intent types
/*enum TacticalIntentTypes
{
	eNO_INTENT = -1,
	eATTACK,
	eMOVE,
	eCAPTURE,
};*/

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvSITacticalIntent
//!  \brief		A units intention for the turn
//
//!  Key Attributes:
//!  - Created on CvSISelfishPhase()
//!  - Updated during CvSINegotiationPhase()
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*class CvSITacticalIntent
{
public:
	CvSITacticalIntent()
	{
		m_pTargetPlot = NULL;
		m_eIntent = eNO_INTENT;
		m_iUnitID = 0;
	};
	CvPlot* GetPlot() const
	{
		return m_pTargetPlot;
	};
	void SetPlot(CvPlot* pPlot)
	{
		m_pTargetPlot = pPlot;
	};
	int GetIntent() const
	{
		return m_eIntent;
	};
	void SetIntent(int eIntent) 
	{
		m_eIntent = eIntent;
	};
	int GetUnitID() const
	{
		return m_iUnitID;
	};
	void SetUnitID(int iID)
	{
		m_iUnitID = iID;
	};

private:
	CvPlot* m_pTargetPlot;
	int m_eIntent;
	int m_iUnitID;
};
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvSITacticalAI
//!  \brief		An alternative to tactical AI based on Swarm Intelligence
//
//!  Key Attributes:
//!  - To be defined
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvSITacticalAI : public CvTacticalAI
{
	public:
		CvSITacticalAI(void);
		virtual ~CvSITacticalAI(void);

		void LogTacticalMessage(CvString& strMsg, bool bSkipLogDominanceZone);

		// Class data
		CvPlayer* m_pPlayer;
		CvTacticalAnalysisMap* m_pMap;
		list<int> m_CurrentTurnUnits;
		std::vector<CvTacticalUnit> m_CurrentMoveUnits;
		std::vector<CvTacticalUnit> m_CurrentMoveHighPriorityUnits;
		std::vector<CvTacticalCity> m_CurrentMoveCities;
		FStaticVector<CvTacticalMove, 256, true, c_eCiv5GameplayDLL > m_MovePriorityList;
		int m_MovePriorityTurn;

		// Data for multi-unit attacks - not serialized, cleared out for each turn
		std::list<CvQueuedAttack> m_QueuedAttacks;
		int m_iCurrentSeriesID;

		// Lists of targets for the turn
		TacticalList m_AllTargets;
		TacticalList m_ZoneTargets;
		TacticalList m_NavalResourceBlockadePoints;
		FStaticVector<CvTacticalTarget, NUM_CITY_PLOTS, true, c_eCiv5GameplayDLL, 0> m_TempTargets;

		// Targeting ranges (pulled in from GlobalAIDefines.XML)
		int m_iRecruitRange;
		int m_iLandBarbarianRange;
		int m_iSeaBarbarianRange;
		int m_iRepositionRange;
		int m_iDeployRadius;
		int m_iRandomRange;
		double m_fFlavorDampening;

		// Dominance zone info
		int m_iCurrentZoneIndex;
		int m_eCurrentTargetType;
		int m_iCurrentTargetIndex;
		int m_iCurrentUnitTargetIndex;
		FStaticVector<CvTacticalPosture, SAFE_ESTIMATE_NUM_CITIES, true, c_eCiv5GameplayDLL, 0> m_Postures;
		FStaticVector<CvTacticalPosture, SAFE_ESTIMATE_NUM_CITIES, true, c_eCiv5GameplayDLL, 0> m_NewPostures;
		int m_iCurrentTempZoneIndex;
		FStaticVector<CvTemporaryZone, SAFE_ESTIMATE_NUM_TEMP_ZONES, true, c_eCiv5GameplayDLL, 0> m_TempZones;

		// Blocking (and flanking) position data
		FStaticVector<CvBlockingUnit, SAFE_ESTIMATE_NUM_BLOCKING_UNITS, true, c_eCiv5GameplayDLL, 0> m_PotentialBlocks;
		FStaticVector<CvBlockingUnit, SAFE_ESTIMATE_NUM_BLOCKING_UNITS, true, c_eCiv5GameplayDLL, 0> m_TemporaryBlocks;
		FStaticVector<CvBlockingUnit, NUM_CITY_PLOTS, true, c_eCiv5GameplayDLL, 0> m_ChosenBlocks;
		FStaticVector<CvBlockingUnit, NUM_CITY_PLOTS, true, c_eCiv5GameplayDLL, 0> m_NewlyChosen;

		// Operational AI support data
		FStaticVector<CvOperationUnit, SAFE_ESTIMATE_NUM_MULTIUNITFORMATION_ENTRIES, true, c_eCiv5GameplayDLL, 0> m_OperationUnits;
		FStaticVector<CvOperationUnit, SAFE_ESTIMATE_NUM_MULTIUNITFORMATION_ENTRIES, true, c_eCiv5GameplayDLL, 0> m_GeneralsToMove;
		FStaticVector<CvOperationUnit, SAFE_ESTIMATE_NUM_MULTIUNITFORMATION_ENTRIES, true, c_eCiv5GameplayDLL, 0> m_ParatroopersToMove;

		int m_CachedInfoTypes[eNUM_TACTICAL_INFOTYPES];
};

#endif