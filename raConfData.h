#ifndef RACONFDATA_H_
#define RACONFDATA_H_

#include "raConfAppData.h"
#include "raConfGameData.h"
#include "raConfPrefsData.h"

class raConfData {
public:

	raConfAppData app_data;
	raConfGameData game_data;
	raConfPrefsData prefs_data;

	raConfData& operator=(const raConfData& value) {
		app_data.x = value.app_data.x;
		app_data.y = value.app_data.y;
		app_data.width = value.app_data.width;
		app_data.height = value.app_data.height;
		app_data.maximized = value.app_data.maximized;
		game_data.clockwise = value.game_data.clockwise;
		game_data.min_bid3 = value.game_data.min_bid3;
		game_data.waive_rule4 = value.game_data.waive_rule4;
		game_data.sluff_jacks = value.game_data.sluff_jacks;
		prefs_data.play_card_on = value.prefs_data.play_card_on;
		prefs_data.card_back = value.prefs_data.card_back;
		prefs_data.auto_play_single = value.prefs_data.auto_play_single;
		prefs_data.show_bid_bubbles = value.prefs_data.show_bid_bubbles;

		return(*this);
	}
};

#endif
