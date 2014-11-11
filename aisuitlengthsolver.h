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

#include "Constants.h"
#include "slProblem.h"
#include "slCell.h"
#include "slData.h"

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
