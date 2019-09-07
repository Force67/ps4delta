
// Copyright (C) 2019 Force67

#include <utl/File.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "json.hpp"

#include <ctime>

namespace fs = std::filesystem;
using namespace nlohmann;

/*
	a world of suffering lies ahead
*/

int main(int argc, char** argv)
{
	std::puts("imputil - Copyright (C) Force67 2019");

#if 1
	std::puts("Copying deps");
	std::ifstream dep("deps.txt");

	std::vector<std::string> changeset;
	std::string line;
	while (std::getline(dep, line))
	{
		changeset.push_back(line);
	}

	std::filesystem::create_directory("changeset");

	for (auto& e : changeset) {
		std::filesystem::copy(R"(output\)" + e, R"(changeset\)" + e);
	}

	return 0;


	/*if (argc > 2) {

		for (int i = 0; i < argc; i++) {
			if (std::strcmp(argv[1], "-deps") == 0) {

				std::puts("Copying deps");
				std::ifstream dep("deps.txt");

				std::vector<std::string> changeset;
				std::string line;
				while (std::getline(dep, line))
				{
					changeset.push_back(line);
				}

				std::filesystem::create_directory("changeset");

				for (auto& e : changeset) {
					std::filesystem::copy(R"(output\)" + e, R"(changeset\)" + e);
				}
			}

			return 0;
		}
	}*/
#endif

	// discover system libs
	std::vector<std::string> files;
	for (auto& it : fs::recursive_directory_iterator(R"(ps4libdoc\system\common\lib)")) {
		const auto& p = it.path();

		if (fs::is_regular_file(it) && p.extension().string() == ".json"){
			auto str = p.string();
	
			auto pos = str.find_last_of('\\');
			if (pos != std::string::npos) {
				if (!str.compare(pos + 1, 3, "lib")) {
					files.push_back(p.string());
				}
			}
		}
	}

	fs::create_directory("output");

	// step1: make dirs
	for (auto& it : files) {
		auto pos = it.find_last_of('\\') + 1;
		if (pos != std::string::npos) {
			auto dot = it.find_first_of('.', pos);
			std::string folder = "output\\" + it.substr(pos, dot - pos);
			fs::create_directory(folder);
		}
	}
	
	// step2: parse json
	for (auto& it : files) {

		// load the json
		std::ifstream file(it);
		json j;
		file >> j;

		// modern cpp they say...
		auto pos = it.find_last_of('\\') + 1;
		auto dot = it.find_first_of('.', pos);
		std::string folder = "output\\" + it.substr(pos, dot - pos);

		std::string tname;
		try {
			tname = j["shared_object_name"].get<std::string>();
		}
		catch (...) {
			continue;
		}
		std::string fxxxx = tname.substr(0, tname.find_first_of('.'));

		auto gen_header = [](std::ofstream& ofs)
		{
			ofs << "\n// Copyright (C) 2019 Force67\n\n// This file was generated on ";

			std::time_t time = std::time(nullptr);
			ofs << std::asctime(std::localtime(&time)) << std::endl;
		};

		std::ofstream hpp(folder + "\\" + fxxxx + ".h");
		gen_header(hpp);

		std::ofstream api(folder + "\\" + fxxxx + "_api.cpp");
		gen_header(api);
		api << R"(#include "../../ModuleLinker.h")" "\n\n";
		api << "#include " << R"(")" << fxxxx << R"(.h")""\n\n";
		api << "static const mlink::FunctionInfo functions[] = {\n";
		
		try {
			for (auto& mod : j["modules"]) {

				for (auto& that : mod["libraries"]) {
					bool exp = that["is_export"];
					if (exp) {
						auto libname = that["name"].get<std::string>();
						auto ts = mod["name"].get<std::string>();
						std::printf("gen %s, %s\n", libname.c_str(), ts.c_str());

						std::ofstream cpp(folder + "\\" + libname + ".cpp");
						gen_header(cpp);
						cpp << R"(#include "../../ModuleLinker.h")" "\n\n";

						static std::unordered_map<std::string,int> symstore;

						static int uniq = 0;
						for (auto& sym : that["symbols"]) {
							std::string symn;
							try {
								symn = sym["name"].get<std::string>();
								// not found
								if (symn.compare(0, 3, "sce")) {
									symn = "scec_" + symn;
								}
							}
							catch (...) {
								symn = "unk_" + sym["encoded_id"].get<std::string>();
							}

							// sanitize
							std::replace(symn.begin(), symn.end(), '-', '_');
							std::replace(symn.begin(), symn.end(), '+', '_');

							// ensure no global collisions happen
							bool hit = false;
							for (auto& bad : symstore) {
								if (bad.first == symn) {
									symn = symn + std::to_string(bad.second);

									// bump reference count
									bad.second++;
									hit = true;
									break;
								}
							}
							if (!hit) {
								// start with a reference count of 0
								symstore.insert(std::make_pair(symn, 0));
							}

							hpp << "int " << symn << "();\n";
							cpp << "int " << symn << "()\n{\n	UNIMPLEMENTED_FUNC;\n	return 0;\n}\n\n";
							api << "	{0x" << sym["hex_id"].get<std::string>() << ", &" << symn << "},\n";
						}
					}
				}
			}
		}
		catch (std::exception& e) {
			std::printf("Parse Error: %s\n", e.what());
			continue;
		}

		api << "};\n\nMODULE_INIT(" << fxxxx << ");\n";
	}
	

	return 0;
}