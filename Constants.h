/*
 * Constants.h
 *
 *  Created on: Nov 11, 2014
 *      Author: student
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define slLENGTH_MAX 8
#define slTOTAL_HANDS 4
#define slTOTAL_SUITS 4
#define slVACANT -1

#define raBID_ROUND_3 2

#define raTOTAL_CARD_BACKS 2
#define raMAX_CARDS_PER_HAND 8
#define raCARD_VERT_RELIEF (12)
#define raCARD_HORZ_RELIEF (GG_CARD_WIDTH / 4)
#define raCARD_PANEL_RELIEF 20

#define raGAME_CARD_BACK_SEL 0

#define raGAME_ARROW_RELIEF 8

#define raGAME_FOUR_JACKS (0x80808080)
#define raGAME_ALL_LOW_CARDS (0x0F0F0F0F)
#define raGAME_ALL_HIGH_CARDS (0xF0F0F0F0)

#define	gmRULE_1 1
#define	gmRULE_2 2
#define	gmRULE_3 4
#define	gmRULE_4 8
// Sluffing of jacks
#define	gmRULE_5 16

#define gmDEAL_ROUND_1 0
#define gmDEAL_ROUND_2 1

#define gmFOUR_JACKS 0x80808080
#define gmJACK 0x80
#define gmALL_CARDS 0xFFFFFFFF

#define gmTOTAL_CARDS 32
#define gmTOTAL_PLAYERS 4
#define gmTOTAL_BID_ROUNDS 3
#define gmPLAYER_INVALID -1
#define gmSUIT_INVALID -1
#define gmCARD_INVALID -1
#define gmPartner(X) ((X + 2) % gmTOTAL_PLAYERS)

#define gmBID_PASS 0
#define gmBID_ALL 1000
#define gmTOTAL_SUITS 4
#define gmTOTAL_VALUES 8
#define gmTOTAL_TRICKS 8
#define gmTOTAL_TEAMS 2

#define SPACES20 _("                    ")

#define MEXP 19937
#define N (MEXP / 128 + 1)
#define N32 (N * 4)

// Super type for holding suit length for hands
typedef int slMatrix[slTOTAL_HANDS][slTOTAL_SUITS];

// The data which represents played cards, which is provided as one of the inputs to the
// suit length solver. The format is identical to that of slProblem
typedef slMatrix slPlayed;

// Solution to the problem
typedef slMatrix slSolution;

#endif /* CONSTANTS_H_ */
