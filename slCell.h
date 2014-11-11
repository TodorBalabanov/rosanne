/*
 * slCell.h
 *
 *  Created on: Nov 11, 2014
 *      Author: student
 */

#ifndef SLCELL_H_
#define SLCELL_H_

// Each cell will have
// a) min - the minimum number of cards which must be allocated
// b) max - the maximum number ofa cards that can be callocated
// c) suit_length - number of cards allocated. If it is vacant, the value is slVACANT
class slCell {
public:
	int min;
	int max;
	int suit_length;
};

#endif /* SLCELL_H_ */
