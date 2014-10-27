// Rosanne : Trump card game popularly known as Twenty Eight (28)
// Copyright (C) 2006-2010 Vipin Cherian
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _AI_SUITLENGTHSOLVER_H_
#define _AI_SUITLENGTHSOLVER_H_

#include "wx/wx.h"

#define slLENGTH_MAX 8
#define slTOTAL_HANDS 4
#define slTOTAL_SUITS 4
#define slVACANT -1

// Super type for holding suit length for hands

typedef int  slMatrix[slTOTAL_HANDS][slTOTAL_SUITS];

// The data which represents played cards, which is provided as one of the inputs to the
// suit length solver. The format is identical to that of slProblem

typedef slMatrix slPlayed;

// Solution to the problem

typedef slMatrix slSolution;

// The data which represents the problem which is provided as one of the inputs to the
// suit length solver.

class slProblem {
public:

	slMatrix suit_length;
	int suit_total_length[slTOTAL_SUITS];
	int hand_total_length[slTOTAL_HANDS];

	slProblem& operator=(const slProblem& value) {
		for(int i=0; i<slTOTAL_HANDS; i++) {
			for(int j=0; j<slTOTAL_SUITS; j++) {
				suit_length[i][j] = value.suit_length[i][j];
			}
		}

		for(int j=0; j<slTOTAL_SUITS; j++) {
			suit_total_length[j] = value.suit_total_length[j];
		}

		for(int i=0; i<slTOTAL_HANDS; i++) {
			hand_total_length[i] = value.hand_total_length[i];
		}

		return(*this);
	}

};

// Working data for computing the solution.

// Each cell will have
// a) min - the minimum number of cards which must be allocated
// b) max - the maximum number ofa cards that can be callocated
// c) suit_length - number of cards allocated. If it is vacant, the value is slVACANT

typedef struct slCELL {
	int min;
	int max;
	int suit_length;
} slCell;

class slData {

public:

	slCell cells[slTOTAL_HANDS][slTOTAL_SUITS];

	int suit_total_length[slTOTAL_SUITS];
	int hand_total_length[slTOTAL_HANDS];
	int suit_allocated[slTOTAL_SUITS];
	int hand_allocated[slTOTAL_HANDS];

	int suit_sum_of_maxs[slTOTAL_SUITS];
	int hand_sum_of_maxs[slTOTAL_HANDS];

	// This is not really the sum of min values.
	// It is the sum of min values of vacant cells
	int suit_sum_of_vacant_mins[slTOTAL_SUITS];
	int hand_sum_of_vacant_mins[slTOTAL_HANDS];

	slData& operator=(const slData& value) {
		for(int i=0; i<slTOTAL_HANDS; i++) {
			for(int j=0; j<slTOTAL_SUITS; j++) {
				cells[i][j] = value.cells[i][j];
			}
		}

		for(int j=0; j<slTOTAL_SUITS; j++) {
			suit_total_length[j] = value.suit_total_length[j];
			suit_allocated[j] = value.suit_allocated[j];
			suit_sum_of_maxs[j] = value.suit_sum_of_maxs[j];
			suit_sum_of_vacant_mins[j] = value.suit_sum_of_vacant_mins[j];
		}

		for(int i=0; i<slTOTAL_HANDS; i++) {
			hand_total_length[i] = value.hand_total_length[i];
			hand_allocated[i] = value.hand_allocated[i];
			hand_sum_of_maxs[i] = value.hand_sum_of_maxs[i];
			hand_sum_of_vacant_mins[i] = value.hand_sum_of_vacant_mins[i];
		}

		return(*this);
	}

};

class aiSuitLengthSolver {
private:
	slProblem m_problem;
	slData m_saved, m_working;
	slPlayed m_played;
	int m_suit_sum_of_min[slTOTAL_SUITS];
	int m_hand_sum_of_min[slTOTAL_HANDS];
	// Disallow copy constructor/assignment operators
	aiSuitLengthSolver(const aiSuitLengthSolver &);
	aiSuitLengthSolver & operator=(const aiSuitLengthSolver &);
	inline void InitializeWorkingData(slData *data);
	inline bool RecalcCellMax(slData *data, int hand, int suit);
	inline bool SetCell(slData *data, int hand, int suit, int suit_length);
	inline bool RecalcMaxForImpactedCells(slData *data, int hand, int suit);
	inline bool RecalcMinForImpactedCells(slData *data, int hand, int suit);
	//inline bool RecalcSumOfMaxForAllCells()
	inline bool RecalcCellMin(slData *data, int hand, int suit);
	inline bool RecalcMinForAllCells(slData *data, bool * changed = NULL);
	inline bool RecalcMaxForAllCells(slData *data);
	inline int GenerateRandomFill(int min, int max);

public:
	aiSuitLengthSolver();
	~aiSuitLengthSolver();
	static void InitializeProblem(slProblem *problem);
	static void InitializePlayed(slPlayed played);
	bool SetProblem(slProblem *problem, slPlayed played);
	bool GenerateRandomSolution(slSolution solution);
	//static wxString PrintProblem(slProblem *problem);
	static wxString PrintData(slData *data);
	static wxString PrintMatrix(slMatrix matrix);
	wxString PrintSavedData();
	wxString PrintWorkingData();
	//static wxString PrintSolution(slSolution *solution);
};



#endif
