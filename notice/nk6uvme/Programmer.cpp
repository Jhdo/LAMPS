/*
 * Programmer.cpp
 *
 *  Created on: Oct 2, 2012
 *      Author: evgeniy
 */

#include <iostream>
#include <vector>
#include <iterator>
#include <cstring>
#include <fstream>

#include "pvmex.h"
#include "Device.h"
#include "Actions.h"
#include "Option.h"

using namespace std;

static vector<Option *>* options = new vector<Option *>;

static Option * findOption(char* name) {
	int size = options->size();
	for (int i = 0; i < size; i++) {
		if (!strcmp(name+1, options->at(i)->name)) {
			return options->at(i);
		}
	}
	return NULL;
}

int main(int argv, char** argc) {

	if(argv < 2){
		help_action(NULL,NULL);
	}

	Option help = Option("help", 0, NULL, help_action);
	Option list = Option("list", 0, NULL, list_action);
	Option dev = Option("dev", 1, NULL, dev_action);
	Option set_v = Option("set-version", 1, &dev, set_version_action);
	Option info = Option("info", 0, &dev, devinfo_action);
	Option set_a = Option("set-address", 1, &dev, set_address_action);
	Option donw_fpga = Option("download-fpga", 1, &dev, downloadFPGA_action);
	Option up_fpga = Option("fpga", 1, &dev, uploadFPGA_action);
	Option up_fx3 = Option("fx3", 1, &dev, uploadFX3_action);
	Option reset = Option("reset", 0, &dev, reset_action);

	Option* children[] = { &up_fx3, &up_fpga, &donw_fpga, &info, &set_a, &set_v, &reset };
	dev.chidrensize = 6;
	dev.children = children;

	options->push_back(&help);
	options->push_back(&list);
	options->push_back(&dev);
	options->push_back(&set_v);
	options->push_back(&set_a);
	options->push_back(&donw_fpga);
	options->push_back(&up_fpga);
	options->push_back(&up_fx3);
	options->push_back(&info);
	options->push_back(&reset);


	for (int i = 1; i < argv; i++) {
		if (argc[i][0] == '-') {
			Option* option = findOption(argc[i]);
			if (option == NULL || option->argument_size + i >= argv) {
				help_action(&help, NULL);
				return 0;
			}
			if(option->argument_size == 0){
				option->enabled = true;
			}else if (argc[i + 1][0] != '-') {
				option->args = new char*[option->argument_size];
				option->args[0] = argc[++i];
				option->enabled = true;
			} else {
				help_action(&help, NULL);
				return 0;
			}
		}
	}
	int size = options->size();
	for (int i = 0; i < size; i++) {
		Option* option = options->at(i);
		if (option->enabled) {
			if (option->parent == NULL) {
				option->runAction(option, NULL);
			} else if (!option->parent->enabled) {
				help_action(&help, NULL);
			}
		}
	}
	return 0;
}
