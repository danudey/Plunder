#include "scenemanager.h"
#include "random_gen.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

void SceneManager::scene_02_horde_loot() {
	// reset scene defaults here

	while (scene == HORDE_LOOT) {  	// scene loop
		Graphics_Engine.clear();  //reset screen
		process_mouse_and_keyboard();  //check for any keypresses

		if (update_horde_display) {
			horde_loot_display.resize(treasure_pile.full_gear_list.size());
			for (uint i = 0; i < horde_loot_display.size(); i++) {
				horde_loot_display[i].setRenderer(renderer);
				horde_loot_display[i].load(Bookman, treasure_pile.full_gear_list[i].retrieveItemString(), White);
			}
			update_horde_display = false;
			loot_rolls_this_session++;
			Sound_Engine.playlootsound();
		}

		if (loot_write_out) {
			now = time(0);			//update time to now
			ltm = localtime(&now);  //save time into struct
			fileoutputname = "saved_loot/lootroll" + toString(1900+ltm->tm_year) + toString(1+ltm->tm_mon) + toString(ltm->tm_mday) + "_" + toString(ltm->tm_hour) + toString(ltm->tm_min) + toString(ltm->tm_sec) + "_" + toString(loot_rolls_this_session) + ".txt";
			save_to_file.open(fileoutputname);
			if (!save_to_file) {
				cout << "Could not open to save. Check code or file.\n\n";
				return;
			} else {
				if (treasure_pile.full_gear_list.size() < 1) {
					cout << "Nothing to save, treasure pile full gear list is 0\n.";
					return;
				}
				for (auto & it : treasure_pile.full_gear_list) {  //output to file in 2 lines, name then date+time
					save_to_file << it.retrieveItemString() << "\n";
				}
				save_to_file.close();
			}
			loot_write_out = false;
			notification_text.setRenderer(renderer);
			notification_text.load(Bookman, "Output written to: " + fileoutputname, Green);
			notification_text.setBlendMode(SDL_BLENDMODE_BLEND);
			notification_alpha = 0;
			loot_write_out_notification = true;
		}

		if (loot_results_ready) {
			loot_found_header.draw(80,30);
			for (uint i = 0; i < horde_loot_display.size(); i++) {
				horde_loot_display[i].draw(120, 90+i*40);
			}
			save_loot_button.draw(ScreenWidth - 75 - save_loot_button.getWidth(), ScreenHeight - 75 - save_loot_button.getHeight());
			if (loot_write_out_notification) {
				notification_alpha++;
				notification_text.setAlpha(notification_alpha);
				notification_text.draw(ScreenWidth/2 - notification_text.getWidth()/2, ScreenHeight - notification_text.getHeight() - 10);
				if (notification_alpha > 254) {
					notification_alpha = 0;
					loot_write_out_notification = false;
					notification_text.free();
				}
			}
		} else {
			for (int i = 0; i < 5; i++) {
				horde_menu_display[i].setAlpha(190);
				horde_menu_display[i].draw(((ScreenWidth/2)-(horde_menu_display[i].getWidth()/2)), 110 + i * 95);
			}
		}
		backarrow.draw( 75, ScreenHeight - 75 - backarrow.getHeight());
		Graphics_Engine.render();  //update screen
	}
}
